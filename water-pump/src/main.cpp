#include <Arduino.h>
// Temperature sensors
#include <OneWire.h>
#include <DallasTemperature.h>
// Display
#include <Wire.h>
#include <U8g2lib.h>
// MySensors
#define MY_RADIO_RF24
#define MY_SPLASH_SCREEN_DISABLED
#include <MySensors.h>
// Pump control
#include "PumpControl.h"

#define PRESSURE_SENSOR_PIN 3
#define PUMP_CONTROL_PIN 4
#define CONFIRM_BUTTON_PIN 2

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 9

// MySensors values
#define STATUS_ID 7
#define SENSOR_NAME "Pump control"
#define SENSOR_VERSION "0.9"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// SSD1306 Display
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Pump control class
PumpControl pump(sensors, PRESSURE_SENSOR_PIN, PUMP_CONTROL_PIN, CONFIRM_BUTTON_PIN);

void setup() 
{
  pump.Initialize();

  // Start up the library
  sensors.begin();
	sensors.setResolution(TEMPERATURE_PRECISION);

  // locate devices on the bus
  char msgTemp[24];
  snprintf(msgTemp, 24, PSTR("Found %d TS devices"), sensors.getDeviceCount());

  // report parasite power requirements
  char msgPower[24];
  snprintf(msgPower, 24, PSTR("Parasite power is: %s"), sensors.isParasitePowerMode() ? "ON" : "OFF");
  
  char msgNode[12];
  snprintf(msgNode, 12, PSTR("Node ID: %d"), getNodeId());

  char msgBtn[16];
  snprintf(msgBtn, 16, PSTR("Button %s"), digitalRead(CONFIRM_BUTTON_PIN) == LOW ? F("PRESS") : F("RELEASE"));

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
      u8g2.drawStr(0, 30, msgNode);
      u8g2.drawStr(0, 40, msgBtn);
    } while (u8g2.nextPage());
    delay(2000);
  }
}

void presentation()
{
  present(STATUS_ID, S_INFO, F("Pump status"));       // VAR1 - status ID, VAR2 - runtime in seconds
  sendSketchInfo(F(SENSOR_NAME), F(SENSOR_VERSION));  // VAR3 - pump temp, VAR4 - SSR temp, TEXT - message
}

void displayStatus(PumpControl::PumpState status)
{
  char msgTime[24], msgTemp[20];
  snprintf(msgTime, 24, "Last run: %d:%02d", pump.GetLastRuntime() / 60, pump.GetLastRuntime() % 60);
  char tempVal[6];
  dtostrf(pump.GetPumpTemp(), 4, 1, tempVal);
  snprintf(msgTemp, 20, "Pump temp: %s", tempVal);

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_9x15B_tr);
    u8g2.drawStr(0, 15, pump.GetStateText().c_str());
    u8g2.setFont(u8g2_font_8x13_tr);
    const uint8_t MAX_LEN = 15;
    if (status == PumpControl::PumpState::MANUAL_STOP || status == PumpControl::PumpState::NOT_INITIALIZED)
    {
      auto& msg = pump.GetErrorMsg();
      u8g2.drawStr(0, 28, msg.c_str());
      if (msg.length() > MAX_LEN)
      {
        u8g2.drawStr(0, 41, msg.substring(MAX_LEN + 1).c_str());
      }
    }
    else
    {
      u8g2.drawStr(0, 28, msgTime);
      u8g2.drawStr(0, 41, msgTemp);
    }
  } while (u8g2.nextPage());
}

void sendStatus(PumpControl::PumpState state)
{
  MyMessage msg(STATUS_ID, V_VAR1);
  send(msg.set(static_cast<uint8_t>(state)));
  msg.setType(V_VAR2);
  send(msg.set(pump.GetLastRuntime()));
  msg.setType(V_VAR3);
  send(msg.set(pump.GetPumpTemp(), 1));
  msg.setType(V_VAR4);
  send(msg.set(pump.GetSSRTemp(), 1));
  if (state == PumpControl::PumpState::MANUAL_STOP)
  {
    msg.setType(V_TEXT);
    send(msg.set(pump.GetErrorMsg().c_str()));
  }
}

void loop() 
{
  static PumpControl::PumpState lastState;
  PumpControl::PumpState state;

  static unsigned long counter1sec;
  if (millis() - counter1sec > 1000)
  {
    counter1sec = millis();
    state = pump.CheckStateLoop();
    displayStatus(state);

    static unsigned long counter30sec;
    if (millis() - counter30sec > 30000 || lastState != state)
    {
      counter30sec = millis();
      lastState = state;
      sendStatus(state);
    }
  }
}