#include "PumpControl.h"

// Maximum run time of the pump: 5 minutes 
unsigned long PumpControl::MAX_RUNTIME = 5ul * 60 * 1000;
uint8_t PumpControl::MAX_TEMP_PUMP = 30;
uint8_t PumpControl::MAX_TEMP_SSR = 50;
uint8_t PumpControl::SENSOR_PUMP_ID = 0;
uint8_t PumpControl::SENSOR_SSR_ID = 1;
volatile bool PumpControl::m_confirm_btn_pressed = false;

PumpControl::PumpControl(DallasTemperature& tempSensors, uint8_t pump_pin, uint8_t button_pin) : 
    m_state(PumpControl::PumpState::NOT_INITIALIZED), m_sensors(tempSensors), m_pump_control_pin(pump_pin),
    m_confirm_button_pin(button_pin), m_lastRuntime(0)
{
}

void PumpControl::Initialize()
{
    pinMode(m_pump_control_pin, OUTPUT);
    pinMode(m_confirm_button_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m_confirm_button_pin), ConfirmButtonPress, FALLING);
    m_state = PumpState::STOP;
}

void PumpControl::ConfirmButtonPress()
{
    m_confirm_btn_pressed = true;
}

void PumpControl::CheckStateLoop(bool lowPressure)
{
    switch (m_state)
    {
        case PumpState::STOP:
            if (lowPressure)
            {
                if (CheckSensors())
                {
                    StartPump();
                    m_state = PumpState::START;
                }
                else
                {
                    m_state = PumpState::MANUAL_STOP;
                }
            }
            break;
        case PumpState::START:
            if (lowPressure == false)
            {
                StopPump();
                m_state = PumpState::STOP;
                break;
            }
            if (CheckTemperature() == false || CheckRuntime() == false)
            {
                StopPump();
                m_state = PumpState::MANUAL_STOP;
            }
                m_lastRuntime = (millis() - m_startTime) / 1000;
                break;
        case PumpState::MANUAL_STOP:
            if (ConfirmButtonPressed())
            {
                m_confirm_btn_pressed = false;
                m_state = PumpState::STOP;
            }
            break;
        case PumpState::NOT_INITIALIZED:
            while(1)
                ;
            break;
    }
}

void PumpControl::StartPump()
{
    digitalWrite(m_pump_control_pin, HIGH);
    m_startTime = millis();
}

void PumpControl::StopPump()
{
    digitalWrite(m_pump_control_pin, LOW);
    
    Serial.print("Runtime: "); 
    Serial.print((millis() - m_startTime) / 1000);
    Serial.println(" s");
}

bool PumpControl::CheckSensors()
{
    DeviceAddress addr0, addr1;
    if (!m_sensors.getAddress(addr0, SENSOR_PUMP_ID) || !m_sensors.getAddress(addr1, SENSOR_SSR_ID))
    {
        m_message = "Temp. sensors not connected";
        return false;
    }
    if (!m_sensors.isConnected(addr0) || !m_sensors.isConnected(addr1))
    {
        m_message = "Temp. sensors error";
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
        m_message = "Pump temp.: ";
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
    if (millis() - m_startTime > MAX_RUNTIME)
    {
        m_message = "Max. runtime";
        return false;
    }
    return true;
}

bool PumpControl::ConfirmButtonPressed()
{
    return m_confirm_btn_pressed;
}

PumpControl::PumpState PumpControl::GetState()
{
    return m_state;
}

String PumpControl::GetStateText()
{
    String result;
    switch (m_state)
    {
    case PumpState::STOP: 
        result = "READY";
        break;
    case PumpState::START:
        result = "RUNNING";
        break;
    case PumpState::MANUAL_STOP:
        result = "ERROR - STOP";
        break;
    default:
        result = "UNKNOWN";
    }
    return result;
}

String& PumpControl::GetErrorMsg()
{
    return m_message;
}

uint16_t PumpControl::GetLastRuntime()
{
    return m_lastRuntime;
}

float PumpControl::GetPumpTemp()
{
    m_sensors.requestTemperaturesByIndex(SENSOR_PUMP_ID);
    return m_sensors.getTempCByIndex(SENSOR_PUMP_ID);
}