#pragma once
#include <DallasTemperature.h>

class PumpControl
{
    public:
        enum class PumpState {STOP, START, MANUAL_STOP};
        PumpControl(DallasTemperature& tempSensors, uint8_t pump_pin, uint8_t button_pin);
        void CheckStateLoop(bool lowPressure);
        
    private:
        PumpState m_state;
        DallasTemperature& m_sensors;
        uint8_t m_pump_control_pin;
        uint8_t m_confirm_button_pin;
        void StartPump();
        void StopPump();
        bool CheckSensors();
        bool CheckTemperature();
        bool CheckRuntime();
        bool ConfirmButtonPressed();
};