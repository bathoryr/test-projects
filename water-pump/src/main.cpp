#include <Arduino.h>
// Temperature sensors
#include <OneWire.h>
#include <DallasTemperature.h>
// Bounce2
#include <Bounce2.h>
// Pump control
#include "PumpControl.h"
#include "PumpStatus.h"

#define PRESSURE_SENSOR_PIN 3
#define PUMP_CONTROL_PIN 4
#define CONFIRM_BUTTON_PIN 2

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Button bounce
Bounce button = Bounce();

// Pump control class
PumpControl pump(sensors, PRESSURE_SENSOR_PIN, PUMP_CONTROL_PIN/*, CONFIRM_BUTTON_PIN*/);
PumpStatus pumpStatus(pump);

void setup() 
{
    // Start up the library
    sensors.begin();
    sensors.setResolution(TEMPERATURE_PRECISION);

    // Bounce button
    button.attach(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
    button.interval(25);

    pump.Initialize();
    // Display init status
    pumpStatus.Initialize(sensors, CONFIRM_BUTTON_PIN);
}

void presentation()
{
    pumpStatus.Present();
}

void loop() 
{
    button.update();
    if (button.fell())
    {
        pump.TriggerBtnPressed();
    }

    static unsigned long counter1sec;
    if (millis() - counter1sec > 1000)
    {
        counter1sec = millis();
        pump.CheckStateLoop();
        pumpStatus.Update();
    }
}