#include "PumpControl.h"
#include "PumpStatus.h"

#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 9

// Maximum run time of the pump: 5 minutes 
unsigned long PumpControl::MAX_RUNTIME = 5ul * 60 * 1000;
uint8_t PumpControl::MAX_TEMP_PUMP = 30;
uint8_t PumpControl::MAX_TEMP_SSR = 50;
uint8_t PumpControl::SENSOR_PUMP_ID = 0;
uint8_t PumpControl::SENSOR_SSR_ID = 1;

PumpControl::PumpControl(uint8_t pressure_pin, uint8_t pump_pin) : 
    m_oneWire(ONE_WIRE_BUS), m_sensors(&m_oneWire),
    m_state(PumpControl::PumpState::NOT_INITIALIZED), m_pressure_sensor_pin(pressure_pin),
    m_pump_control_pin(pump_pin), m_lastRuntime(0)
{
}

void PumpControl::Initialize(PumpStatus* pumpStatus, LED& led)
{
    m_pumpStatus = pumpStatus;
    this->led = led;
    pinMode(m_pressure_sensor_pin, INPUT_PULLUP);
    pinMode(m_pump_control_pin, OUTPUT);

    // Start up the library
    m_sensors.begin();
    m_sensors.setResolution(TEMPERATURE_PRECISION);

    // Initial check
    m_state = CheckSensors() ? PumpState::READY : PumpState::MANUAL_STOP;
}

void PumpControl::TriggerBtnPressed()
{
    m_confirm_btn_pressed = true;
}

PumpControl::PumpState PumpControl::CheckStateLoop()
{
    switch (m_state)
    {
        case PumpState::READY:
            led.Dim();
            if (CheckLowPressure() == true)
            {
                if (CheckSensors())
                {
                    StartPump();
                    m_state = PumpState::START;
                }
                else
                {
                    SetManualState();
                }
            }
            else if (CheckButtonPressed())
            {
                CheckSensors();
                m_pumpStatus->ShowStatus(5000);
            }
            
            break;
        case PumpState::START:
            if (CheckLowPressure() == false)
            {
                StopPump();
                m_state = PumpState::READY;
                // Everything went OK, reset message
                m_message = F("OK");
                break;
            }
            if (CheckTemperature() == false || CheckRuntime() == false)
            {
                StopPump();
                SetManualState();
            }
            break;
        case PumpState::MANUAL_STOP:
            led.Blink(500);
            if (CheckButtonPressed())
            {
                m_message = F("Confirm button pressed");
                SetReadyState();
            }
            break;
        case PumpState::NOT_INITIALIZED:
            m_message = F("Not initialized");
            break;
    }
    return m_state;
}

void PumpControl::SetManualState()
{
    m_confirm_btn_pressed = false;
    m_state = PumpState::MANUAL_STOP;
}

void PumpControl::SetReadyState()
{
    m_state = PumpState::READY;
}

void PumpControl::StartPump()
{
    digitalWrite(m_pump_control_pin, HIGH);
    m_startTime = millis();
}

void PumpControl::StopPump()
{
    digitalWrite(m_pump_control_pin, LOW);
}

bool PumpControl::CheckLowPressure()
{
    bool lowPressure = digitalRead(m_pressure_sensor_pin) == LOW ? true : false;
    return lowPressure;
}

bool PumpControl::CheckSensors()
{
    DeviceAddress addr0, addr1;
    if (!m_sensors.getAddress(addr0, SENSOR_PUMP_ID) || !m_sensors.getAddress(addr1, SENSOR_SSR_ID))
    {
        m_message = F("Temp. sensors not present");
        return false;
    }
    if (!m_sensors.isConnected(addr0) || !m_sensors.isConnected(addr1))
    {
        m_message = F("Temp. sensors connection error");
        return false;
    }
    return true;
}

bool PumpControl::CheckTemperature()
{
    bool result = true;
    m_sensors.requestTemperatures();
    if (m_sensors.getTempCByIndex(SENSOR_PUMP_ID) > MAX_TEMP_PUMP)
    {
        m_message = F("Pump temp.: ");
        m_message.concat(m_sensors.getTempCByIndex(SENSOR_PUMP_ID));
        result = false;
    }
    if (m_sensors.getTempCByIndex(SENSOR_SSR_ID) > MAX_TEMP_SSR)
    {
        m_message = F("SSR temp.: ");
        m_message.concat(m_sensors.getTempCByIndex(SENSOR_SSR_ID));
        result = false;
    }
    return result;
}

bool PumpControl::CheckRuntime()
{
    m_lastRuntime = (millis() - m_startTime) / 1000;
    if (millis() - m_startTime > MAX_RUNTIME)
    {
        m_message = F("Max. runtime: ");
        m_message.concat(m_lastRuntime);
        m_message.concat(" s");
        return false;
    }
    return true;
}

bool PumpControl::CheckButtonPressed()
{
    if (m_confirm_btn_pressed)
    {
        m_confirm_btn_pressed = false;
        return true;
    }
    
    return false;
}

PumpControl::PumpState PumpControl::GetState() const
{
    return m_state;
}

String PumpControl::GetStateText() const
{
    switch (m_state)
    {
    case PumpState::READY: 
        return String(F("READY"));
    case PumpState::START:
        return String(F("RUNNING"));
    case PumpState::MANUAL_STOP:
        return String(F("ERROR - STOP"));
    default:
        return String(F("UNKNOWN"));
    }
}

const String& PumpControl::GetErrorMsg() const
{
    return m_message;
}

uint16_t PumpControl::GetLastRuntime() const
{
    return m_lastRuntime;
}

float PumpControl::GetPumpTemp() const
{
    // Dallas methods are not const ...
    const_cast<DallasTemperature&>(m_sensors).requestTemperaturesByIndex(SENSOR_PUMP_ID);
    return const_cast<DallasTemperature&>(m_sensors).getTempCByIndex(SENSOR_PUMP_ID);
}

float PumpControl::GetSSRTemp() const
{
    const_cast<DallasTemperature&>(m_sensors).requestTemperaturesByIndex(SENSOR_SSR_ID);
    return const_cast<DallasTemperature&>(m_sensors).getTempCByIndex(SENSOR_SSR_ID);
}

const DallasTemperature& PumpControl::GetTempSensors() const
{
    return m_sensors;
}