// Display
#include <Wire.h>
#include <U8g2lib.h>
// MySensors
#define MY_RADIO_RF24
#define MY_RF24_PA_LEVEL RF24_PA_MAX
#define MY_SPLASH_SCREEN_DISABLED
#include <MySensors.h>
#include "PumpStatus.h"

// MySensors values
#define STATUS_ID 7
#define SENSOR_NAME "Pump control"
#define SENSOR_VERSION "0.9.4"


PumpStatus::PumpStatus(const PumpControl& pumpRef) : pump(pumpRef), u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE)
{
}

void PumpStatus::Initialize()
{
    // Display init
    if (u8g2.begin())
    {
        u8g2.setFlipMode(1);
        ShowStatus(2000);
    }
}

void PumpStatus::ShowStatus(uint16_t mseconds)
{
    DallasTemperature& sensors = const_cast<DallasTemperature&>(pump.GetTempSensors());
    // locate devices on the bus
    char msgTemp[20];
    snprintf_P(msgTemp, 20, PSTR("Found %d TS devices"), sensors.getDeviceCount());

    char msgNode[12];
    snprintf_P(msgNode, 12, PSTR("Node ID: %d"), getNodeId());

    u8g2.firstPage();
    do
    {
        u8g2.setFont(u8g2_font_9x15B_tr);
        u8g2.drawStr(0, 15, pump.GetStateText().c_str());
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.drawStr(0, 28, pump.GetErrorMsg().c_str());
        u8g2.drawStr(0, 38, msgTemp);
        u8g2.drawStr(0, 48, msgNode);
    } while (u8g2.nextPage());

    delay(mseconds);
}

void PumpStatus::Present()
{
    // VAR1 - status ID, VAR2 - total runtime in seconds, VAR3 - current runtime
    // VAR4 - pump temp, VAR5 - SSR temp, TEXT - message
    sendSketchInfo(SENSOR_NAME, SENSOR_VERSION);
    present(STATUS_ID, S_INFO, "Pump status");
}

void PumpStatus::DisplayStatus()
{
    char msgTime[24], msgTemp[20];
    snprintf_P(msgTime, 24, PSTR("Last run: %d:%02d"), pump.GetLastRuntime() / 60, pump.GetLastRuntime() % 60);
    char tempVal[6];
    dtostrf(pump.GetPumpTemp(), 4, 1, tempVal);
    snprintf_P(msgTemp, 20, PSTR("Pump temp: %s"), tempVal);
    auto status = pump.GetState();

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

void PumpStatus::SendStatus()
{
    MyMessage msg(STATUS_ID, V_VAR1);
    if (lastState != pump.GetState())
    {
        lastState = pump.GetState();
        send(msg.set(static_cast<uint8_t>(lastState)));

        if (lastState != PumpControl::PumpState::START)
        {
            msg.setType(V_VAR2);
            send(msg.set(pump.GetLastRuntime()));
        }
    }
    if (lastState == PumpControl::PumpState::START)
    {
        msg.setType(V_VAR3);
        send(msg.set(pump.GetLastRuntime()));
    }
    
    auto temp = pump.GetPumpTemp();
    if (pumpTemp != temp)
    {
        pumpTemp = temp;
        msg.setType(V_VAR4);
        send(msg.set(pumpTemp, 1));
    }
    temp = pump.GetSSRTemp();
    if (SSRTemp != temp)
    {
        SSRTemp = temp;
        msg.setType(V_VAR5);
        send(msg.set(SSRTemp, 1));
    }
    if (lastError != pump.GetErrorMsg())
    {
        lastError = pump.GetErrorMsg();
        msg.setType(V_TEXT);
        send(msg.set(lastError.c_str()));
    }
}

void PumpStatus::Update()
{
    DisplayStatus();
    SendStatus();
    // Update is called each 1 second, send status every 5 seconds when running
    /*
    uint8_t interval = pump.GetState() == PumpControl::PumpState::START ? 5 : 30;
    
    if (++counter % interval == 0)
    {
        SendStatus();
    }
    */
}