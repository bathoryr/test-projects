#include "PumpControl.h"

// Maximum run time of the pump: 5 minutes 
unsigned long PumpControl::MAX_RUNTIME = 5ul * 60 * 1000;

PumpControl::PumpControl(DallasTemperature& tempSensors, uint8_t pump_pin, uint8_t button_pin) : 
    m_state(PumpControl::PumpState::STOP), m_sensors(tempSensors), m_pump_control_pin(pump_pin),
    m_confirm_button_pin(button_pin)
{
    pinMode(m_pump_control_pin, OUTPUT);
    pinMode(m_confirm_button_pin, INPUT_PULLUP);
    
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
            break;
        case PumpState::MANUAL_STOP:
            if (ConfirmButtonPressed())
            {
                m_state = PumpState::STOP;
            }
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
    bool result = false;
    uint8_t addr0, addr1;
    m_sensors.getAddress(&addr0, 0);
    m_sensors.getAddress(&addr1, 1);
    if (m_sensors.isConnected(&addr0) && m_sensors.isConnected(&addr1))
    {
        result = true;
    }
    return result;
}

bool PumpControl::CheckTemperature()
{
    return true;
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
    return true;
}