#include "doorbell.h"

Doorbell::Doorbell()
{
}

void Doorbell::Initialize(uint8_t send_pin)
{
    rf433trm.enableTransmit(send_pin);
    rf433trm.setPulseLength(155);
}

void Doorbell::Ring()
{
    for(int8_t i = 0; i < 5; i++)
    {
        // Code for parent's doorbell
        rf433trm.send(16080895, 24);
    }
}