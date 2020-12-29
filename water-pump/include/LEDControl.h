#pragma once
#include <stdint.h>

class LEDControl
{
public:
    LEDControl(uint8_t pin);
    // This should be called from main loop
    void Update();
    
    void Blink(uint16_t interval);
    void Light();
    void Dim();

private:
    uint8_t led_pin;
    uint16_t blink_timeout;
    unsigned long millis_counter;
};