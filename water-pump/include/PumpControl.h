#pragma once
// Temperature sensors
#include <OneWire.h>
#include <DallasTemperature.h>
#include "LEDControl.h"

class PumpStatus;

class PumpControl
{
    public:
        enum class PumpState {NOT_INITIALIZED, READY, START, MANUAL_STOP};
        PumpControl(LEDControl& led, uint8_t pressure_pin, uint8_t pump_pin);
        void Initialize(PumpStatus* pumpStatus);
        PumpState CheckStateLoop();
        PumpState GetState() const;
        String GetStateText() const;
        const String& GetErrorMsg() const;
        uint16_t GetLastRuntime() const;
        float GetPumpTemp() const;
        float GetSSRTemp() const;
        void TriggerBtnPressed();
        const DallasTemperature& GetTempSensors() const;
        
    private:
        OneWire m_oneWire;
        DallasTemperature m_sensors;
        LEDControl& btnLed;
        PumpState m_state;
        PumpStatus* m_pumpStatus;
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
        void SetStartState();
        static unsigned long MAX_RUNTIME;
        static uint8_t MAX_TEMP_PUMP;
        static uint8_t MAX_TEMP_SSR;
        static uint8_t SENSOR_PUMP_ID;
        static uint8_t SENSOR_SSR_ID;
};