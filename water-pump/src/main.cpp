#include <Arduino.h>
#include "loop-call.h"
// Bounce2
#include <Bounce2.h>
// Pump control
#include "PumpControl.h"
#include "PumpStatus.h"

#define PRESSURE_SENSOR_PIN 3
#define PUMP_CONTROL_PIN 4
#define CONFIRM_BUTTON_PIN 2

// Button bounce
Bounce button = Bounce();

// Pump control class
PumpControl pump(PRESSURE_SENSOR_PIN, PUMP_CONTROL_PIN);
PumpStatus pumpStatus(pump);

void setup() 
{
    // Bounce button
    button.attach(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
    button.interval(25);

    pump.Initialize();
    // Display init status
    pumpStatus.Initialize(CONFIRM_BUTTON_PIN);
}

void presentation()
{
    pumpStatus.Present();
}

void pump_status_and_update()
{
    pump.CheckStateLoop();
    pumpStatus.Update();
}

void loop() 
{
    button.update();
    if (button.fell())
    {
        pump.TriggerBtnPressed();
    }

    CALL_LOOP(1000, pump_status_and_update);
}