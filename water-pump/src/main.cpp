#include <Arduino.h>
// Temperature sensors
#include <OneWire.h>
#include <DallasTemperature.h>
// Display
#include <Wire.h>
#include <U8g2lib.h>
// Pump control
#include "PumpControl.h"

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

// SSD1306 Display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Pump control class
PumpControl pump(sensors, PUMP_CONTROL_PIN, CONFIRM_BUTTON_PIN);

void setup() 
{
  pump.Initialize();
  pinMode(PRESSURE_SENSOR_PIN, INPUT_PULLUP);

  // Start up the library
  sensors.begin();
	sensors.setResolution(TEMPERATURE_PRECISION);

  // locate devices on the bus
  char msgTemp[24];
  snprintf(msgTemp, 24, "Found %d TS devices", sensors.getDeviceCount());

  // report parasite power requirements
  char msgPower[24];
  snprintf(msgPower, 24, "Parasite power is: %s", sensors.isParasitePowerMode() ? "ON" : "OFF");
  
  // Display init
  if (u8g2.begin())
  {
    u8g2.setFlipMode(1);
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(0, 10, msgTemp);
      u8g2.drawStr(0, 20, msgPower);
    } while (u8g2.nextPage());
    delay(1000);
  }
}

void displayStatus(PumpControl::PumpState status)
{
  char msgTime[24], msgTemp[20];
  snprintf(msgTime, 24, "Last runtime: %02d:%02d", pump.GetLastRuntime() / 60, pump.GetLastRuntime() % 60);
  char tempVal[6];
  dtostrf(pump.GetPumpTemp(), 4, 1, tempVal);
  snprintf(msgTemp, 20, "Pump temp.: %s C", tempVal);

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_9x15B_tr);
    u8g2.drawStr(0, 15, pump.GetStateText().c_str());
    u8g2.setFont(u8g2_font_6x10_tr);
    if (status == PumpControl::PumpState::MANUAL_STOP || status == PumpControl::PumpState::NOT_INITIALIZED)
    {
      u8g2.drawStr(0, 25, pump.GetErrorMsg().c_str());
    }
    else
    {
      u8g2.drawStr(0, 25, msgTime);
      u8g2.drawStr(0, 35, msgTemp);
    }
  } while (u8g2.nextPage());
}

unsigned long counter1sec;

void loop() 
{
  if (millis() - counter1sec > 1000)
  {
    counter1sec = millis();
    bool lowPressure = digitalRead(PRESSURE_SENSOR_PIN) == LOW ? true : false;
    auto state = pump.CheckStateLoop(lowPressure);
    displayStatus(state);
  }
}