#include <Arduino.h>
#include "LED.h"

LED::LED(uint8_t pin) : led_pin(pin), millis_counter(millis())
{
    pinMode(pin, OUTPUT);
}

void LED::Update()
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

void LED::Blink(uint16_t interval)
{
    blink_timeout = interval;
}

void LED::Light()
{
    blink_timeout = 0l;
    digitalWrite(led_pin, HIGH);
}

void LED::Dim()
{
    blink_timeout = 0l;
    digitalWrite(led_pin, LOW);
}