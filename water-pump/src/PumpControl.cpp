#include "PumpControl.h"

// Maximum run time of the pump: 5 minutes 
unsigned long PumpControl::MAX_RUNTIME = 5ul * 60 * 1000;
uint8_t PumpControl::MAX_TEMP_PUMP = 30;
uint8_t PumpControl::MAX_TEMP_SSR = 50;
uint8_t PumpControl::SENSOR_PUMP_ID = 0;
uint8_t PumpControl::SENSOR_SSR_ID = 1;
volatile bool PumpControl::m_confirm_btn_pressed = false;

PumpControl::PumpControl(DallasTemperature& tempSensors, uint8_t pressure_pin, uint8_t pump_pin, uint8_t button_pin) : 
    m_state(PumpControl::PumpState::NOT_INITIALIZED), m_sensors(tempSensors), m_pressure_sensor_pin(pressure_pin),
    m_pump_control_pin(pump_pin), m_confirm_button_pin(button_pin), m_lastRuntime(0)
{
}

void PumpControl::Initialize()
{
    pinMode(m_pressure_sensor_pin, INPUT_PULLUP);
    pinMode(m_pump_control_pin, OUTPUT);
    pinMode(m_confirm_button_pin, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(m_confirm_button_pin), ConfirmButtonPress_ISR, FALLING);
    m_state = PumpState::READY;
}

void PumpControl::ConfirmButtonPress_ISR()
{
    m_confirm_btn_pressed = true;
}

PumpControl::PumpState PumpControl::CheckStateLoop()
{
    switch (m_state)
    {
        case PumpState::READY:
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
            break;
        case PumpState::START:
            if (CheckLowPressure() == false)
            {
                StopPump();
                m_state = PumpState::READY;
                break;
            }
            if (CheckTemperature() == false || CheckRuntime() == false)
            {
                StopPump();
                SetManualState();
            }
            break;
        case PumpState::MANUAL_STOP:
            if (CheckButtonPressed())
            {
                m_confirm_btn_pressed = false;
                m_message = F("Confirm button pressed");
                //SetReadyState();  only reset
            }
            break;
        case PumpState::NOT_INITIALIZED:
            m_message = F("Not initialized");
            Serial.print(m_message);
            break;
    }
    return m_state;
}

void PumpControl::SetManualState()
{
    attachInterrupt(digitalPinToInterrupt(m_confirm_button_pin), ConfirmButtonPress_ISR, FALLING);
    m_state = PumpState::MANUAL_STOP;
}

void PumpControl::SetReadyState()
{
    detachInterrupt(digitalPinToInterrupt(m_confirm_button_pin));
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
        m_message = F("Temp. sensors not connected");
        return false;
    }
    if (!m_sensors.isConnected(addr0) || !m_sensors.isConnected(addr1))
    {
        m_message = F("Temp. sensors error");
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
    return m_confirm_btn_pressed;
}

PumpControl::PumpState PumpControl::GetState() const
{
    return m_state;
}

String PumpControl::GetStateText() const
{
    String result;
    switch (m_state)
    {
    case PumpState::READY: 
        result = F("READY");
        break;
    case PumpState::START:
        result = F("RUNNING");
        break;
    case PumpState::MANUAL_STOP:
        result = F("ERROR - STOP");
        break;
    default:
        result = F("UNKNOWN");
    }
    return result;
}

String& PumpControl::GetErrorMsg()
{
    return m_message;
}

uint16_t PumpControl::GetLastRuntime() const
{
    return m_lastRuntime;
}

float PumpControl::GetPumpTemp() const
{
    m_sensors.requestTemperaturesByIndex(SENSOR_PUMP_ID);
    return m_sensors.getTempCByIndex(SENSOR_PUMP_ID);
}

float PumpControl::GetSSRTemp() const
{
    m_sensors.requestTemperaturesByIndex(SENSOR_SSR_ID);
    return m_sensors.getTempCByIndex(SENSOR_SSR_ID);
}