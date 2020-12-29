#include <Arduino.h>
#include "LEDControl.h"

LEDControl::LEDControl(uint8_t pin) : led_pin(pin), blink_timeout(0l), millis_counter(millis())
{
    pinMode(pin, OUTPUT);
}

void LEDControl::Update()
{
    if (blink_timeout != 0l)
    {
        if (millis() - millis_counter > blink_timeout)
        {
            millis_counter = millis();
            digitalWrite(led_pin, !digitalRead(led_pin));
        }
    }
}

void LEDControl::Blink(uint16_t interval)
{
    if (blink_timeout != interval)
    {
        blink_timeout = interval;
    }
}

void LEDControl::Light()
{
    blink_timeout = 0l;
    digitalWrite(led_pin, HIGH);
}

void LEDControl::Dim()
{
    blink_timeout = 0l;
    digitalWrite(led_pin, LOW);
}