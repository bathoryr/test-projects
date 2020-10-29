#pragma once
#include <DallasTemperature.h>

class PumpControl
{
    public:
        enum class PumpState {NOT_INITIALIZED, READY, START, MANUAL_STOP};
        PumpControl(DallasTemperature& tempSensors, uint8_t pump_pin, uint8_t button_pin);
        void Initialize();
        void CheckStateLoop(bool lowPressure);
        PumpState GetState() const;
        String GetStateText() const;
        String& GetErrorMsg();
        uint16_t GetLastRuntime() const;
        float GetPumpTemp() const;
        
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
        bool CheckButtonPressed();

        unsigned long m_startTime;
        uint16_t m_lastRuntime;
        String m_message;
        static volatile bool m_confirm_btn_pressed;

        void SetManualState();
        void SetReadyState();
        static void ConfirmButtonPress_ISR();
        static unsigned long MAX_RUNTIME;
        static uint8_t MAX_TEMP_PUMP;
        static uint8_t MAX_TEMP_SSR;
        static uint8_t SENSOR_PUMP_ID;
        static uint8_t SENSOR_SSR_ID;
};