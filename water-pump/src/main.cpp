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
  Serial.begin(9600);

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Display init
  if (u8g2.begin())
  {
    u8g2.setFlipMode(1);
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.setCursor(0, 10);
      u8g2.print("Init: "); u8g2.print(u8g2.getDisplayWidth()); u8g2.print(" x "); u8g2.println(u8g2.getDisplayHeight());
    } while (u8g2.nextPage());

    Serial.print("Init: "); Serial.print(u8g2.getDisplayWidth()); Serial.print(" x "); Serial.println(u8g2.getDisplayHeight());
  }
}

void drawText(float temp1, float temp2)
{
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setCursor(0, 10);
    u8g2.print("Temperature1: "); u8g2.print(temp1); 
    u8g2.setCursor(0, 20);
    u8g2.print("Temperature2: "); u8g2.print(temp2);
  } while (u8g2.nextPage());

}

unsigned long counter1sec;
void loop() 
{
  if (millis() % 10000 == 0)
  {
    sensors.requestTemperatures();
    Serial.print("Temp1: "); Serial.println(sensors.getTempCByIndex(0));
    Serial.print("Temp2: "); Serial.println(sensors.getTempCByIndex(1));

    drawText(sensors.getTempCByIndex(0), sensors.getTempCByIndex(1));
  }

  if (millis() - counter1sec > 1000)
  {
    counter1sec = millis();
    bool lowPressure = digitalRead(PRESSURE_SENSOR_PIN) == LOW ? true : false;
    pump.CheckStateLoop(lowPressure);
  }
}