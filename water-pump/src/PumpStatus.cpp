// Display
#include <Wire.h>
#include <U8g2lib.h>
// MySensors
#define MY_RADIO_RF24
#define MY_SPLASH_SCREEN_DISABLED
#include <MySensors.h>
#include "PumpStatus.h"

// MySensors values
#define STATUS_ID 7
#define SENSOR_NAME "Pump control"
#define SENSOR_VERSION "0.9.2"

PumpStatus::PumpStatus(const PumpControl& pumpRef) : pump(pumpRef), u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE)
{
}

void PumpStatus::Initialize(uint8_t buttonPin)
{
    DallasTemperature& sensors = const_cast<DallasTemperature&>(pump.GetTempSensors());
    // locate devices on the bus
    char msgTemp[20];
    snprintf(msgTemp, 20, "Found %d TS devices", sensors.getDeviceCount());

    // report parasite power requirements
    char msgPower[14];
    snprintf(msgPower, 14, "Power is: %s", sensors.isParasitePowerMode() ? "ON" : "OFF");

    char msgNode[12];
    snprintf(msgNode, 12, "Node ID: %d", getNodeId());

    char msgBtn[16];
    snprintf(msgBtn, 16, "Button %s", digitalRead(buttonPin) == LOW ? "PRESS" : "RELEASE");

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

void PumpStatus::Present()
{
    sendSketchInfo(F(SENSOR_NAME), F(SENSOR_VERSION));    // VAR1 - status ID, VAR2 - runtime in seconds
    present(STATUS_ID, S_INFO, F("Pump status"));         // VAR3 - pump temp, VAR4 - SSR temp, TEXT - message
}

void PumpStatus::DisplayStatus()
{
    char msgTime[24], msgTemp[20];
    snprintf(msgTime, 24, "Last run: %d:%02d", pump.GetLastRuntime() / 60, pump.GetLastRuntime() % 60);
    char tempVal[6];
    dtostrf(pump.GetPumpTemp(), 4, 1, tempVal);
    snprintf(msgTemp, 20, "Pump temp: %s", tempVal);
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
    }
    if (runtime != pump.GetLastRuntime())
    {
        runtime = pump.GetLastRuntime();
        msg.setType(V_VAR2);
        send(msg.set(runtime));
    }
    auto temp = pump.GetPumpTemp();
    if (pumpTemp != temp)
    {
        pumpTemp = temp;
        msg.setType(V_VAR3);
        send(msg.set(pumpTemp, 1));
    }
    temp = pump.GetSSRTemp();
    if (SSRTemp != temp)
    {
        SSRTemp = temp;
        msg.setType(V_VAR4);
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
    // Update is called each 1 second, send status every 5 seconds when running
    uint8_t interval = pump.GetState() == PumpControl::PumpState::START ? 5 : 30;
    
    if (++counter % interval == 0)
    {
        SendStatus();
    }
}