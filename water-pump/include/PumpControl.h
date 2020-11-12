#pragma once
#include <DallasTemperature.h>

class PumpControl
{
    public:
        enum class PumpState {NOT_INITIALIZED, READY, START, MANUAL_STOP};
        PumpControl(DallasTemperature& tempSensors, uint8_t pressure_pin, uint8_t pump_pin/*, uint8_t button_pin*/);
        void Initialize();
        PumpState CheckStateLoop();
        PumpState GetState() const;
        String GetStateText() const;
        const String& GetErrorMsg() const;
        uint16_t GetLastRuntime() const;
        float GetPumpTemp() const;
        float GetSSRTemp() const;
        void TriggerBtnPressed();
        
    private:
        PumpState m_state;
        DallasTemperature& m_sensors;
        uint8_t m_pressure_sensor_pin;
        uint8_t m_pump_control_pin;
        uint8_t m_confirm_button_pin;
        void StartPump();
        void StopPump();
        bool CheckLowPressure();
        bool CheckSensors();
        bool CheckTemperature();
        bool CheckRuntime();
        bool CheckButtonPressed();

        unsigned long m_startTime;
        uint16_t m_lastRuntime;
        String m_message;
        bool m_confirm_btn_pressed;

        void SetManualState();
        void SetReadyState();
        static void ConfirmButtonPress_ISR();
        static unsigned long MAX_RUNTIME;
        static uint8_t MAX_TEMP_PUMP;
        static uint8_t MAX_TEMP_SSR;
        static uint8_t SENSOR_PUMP_ID;
        static uint8_t SENSOR_SSR_ID;
};