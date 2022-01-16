#include <Arduino.h>
#include <Wire.h>
// Sensor config: MYSBootloader, 16MHz Xtal, Nano board

//#define MY_DEBUG
//#define MY_DEBUG_VERBOSE_RF24
#define MY_SPLASH_SCREEN_DISABLED
#define MY_RADIO_RF24
#define MY_RF24_PA_LEVEL (RF24_PA_LOW)
// #define MY_NODE_ID 5
#define MY_TRANSPORT_WAIT_READY_MS 5000

// #define MY_RF24_DATARATE RF24_2MBPS
// #define MY_RF24_CHANNEL (0x70)

#include <MySensors.h>

#define OUTPUT_PIN 5
#define MOTION_LED 7 	// Move indication LED
#define MOTION_PIN 4	// PIR sensor
#define RFCOMM_PIN 2	// RF433 transmitter

#define CHILD_ID_LIGHT 1
#define CHILD_ID_MOTION 2
#define CHILD_ID_LUX 3
#define CHILD_ID_TIMEOUT 4
#define CHILD_ID_MOVE_DETECT 5
#define CHILD_ID_INFO 99

#include "loop-call.h"
#include "Light.h"

typedef void (*ptrFunc)(const MyMessage&);

short max_retry = 0;
bool dimmer_rcvd = false;
bool motion_rcvd = false;

void CheckMotion();
void SendIllumination();

Light light;

void setup()
{
	light.Setup();
	//doorbell.Initialize(RFCOMM_PIN);
	pinMode(OUTPUT_PIN, OUTPUT);
	pinMode(MOTION_LED, OUTPUT);
	pinMode(MOTION_PIN, INPUT);

	request(CHILD_ID_MOVE_DETECT, V_STATUS);
	wait(20);
	request(CHILD_ID_TIMEOUT, V_LEVEL);
}

void presentation()
{
	sendSketchInfo("Dimmable Light", "3.2.0");

	present(CHILD_ID_LIGHT, S_DIMMER, "Light control");
	present(CHILD_ID_MOTION, S_MOTION, "Motion activity");
	present(CHILD_ID_LUX, S_LIGHT_LEVEL, "Illumination level");
	present(CHILD_ID_TIMEOUT, S_SOUND, "Light timeout");
	present(CHILD_ID_MOVE_DETECT, S_BINARY, "Move detect switch");
}

void request_val()
{
	if (max_retry++ < 15)
	{
		if (!dimmer_rcvd)
		{
			request(CHILD_ID_LIGHT, V_DIMMER);
		}
		if (!motion_rcvd)
		{
			request(CHILD_ID_MOVE_DETECT, V_STATUS);
		}
	}
}

void loop()
{
	CALL_LOOP(1000, CheckMotion);
	CALL_LOOP(30000, SendIllumination);
	// CALL_LOOP(15000, request_val);
}

void CheckMotion()
{
	light.CheckMotion();
}

void SendIllumination()
{
	MyMessage luxMsg(CHILD_ID_LUX, V_LEVEL);
	send(luxMsg.set(light.GetAvgIllumination()));
}

void receive_retain(const MyMessage &message, uint8_t sensor, uint8_t type, ptrFunc func)
{
  // Store message from controller for V_VAR5
  // 1. Accept REQ, confirm it by send SET_RETAIN, gateway include it in the cache
  // 2. If message with SET_RETAIN command is received, send status to controller
  // Node then can request stored value from gateway by rquest()
  if (message.getSensor() == sensor)
  {
    if (message.getType() == type)
    {
      if (message.getCommand() == C_SET)
      {
        Serial.print("REQ: received value=");
        Serial.println(message.getInt());
        Serial.println("Sending cmd to retain it...");
        MyMessage msg(message);
        msg.setDestination(message.getSender());
        msg.setSender(getNodeId());
        // Very important to insert some delay here (20ms), otherwise we almost always get NACK
        // maybe due to immediately sending a response
        wait(20);
        send(msg, C_SET_RETAIN);
      }
      if (message.getCommand() == C_SET_RETAIN)
      {
        Serial.print("SET: received value=");
        Serial.println(message.getInt());
        Serial.println("Sending cmd to controller confirmation...");
		func(message);
        MyMessage msg(message);
        msg.setDestination(message.getSender());
        msg.setSender(getNodeId());
        wait(20);
        send(msg);
      }
    }
  }
}

void receive(const MyMessage &message)
{
	receive_retain(message, CHILD_ID_LIGHT, V_DIMMER, [](const MyMessage& m){light.SetIntensity(m.getInt());});
	receive_retain(message, CHILD_ID_MOVE_DETECT, V_STATUS, [](const MyMessage& m){light.SetMotionDetector(m.getBool());});

	switch (message.sensor)
	{
	case CHILD_ID_LIGHT:
		if (message.type == V_STATUS) {
			// Message from light switch (ON/OFF)
			message.getBool() == true ? light.TurnOn() : light.TurnOff();
			wait(20);
	        MyMessage switchMsg(CHILD_ID_LIGHT, V_STATUS);
        	send(switchMsg.set(light.IsLightOn()));
		}
		break;
	case CHILD_ID_TIMEOUT:
		if (message.type == V_LEVEL) {
			// Light timeout - if not set on controller, can be 0
			if (message.getInt() > 0) {
				light.SetTimeout(message.getInt());
			}
			else
			{
				// Set to default 15 minutes
				light.SetTimeout(15);
				// Send message to controller to store the value so we can request it later
				MyMessage msgTm(CHILD_ID_TIMEOUT, V_LEVEL);
				send(msgTm.set(15), C_SET_RETAIN);
			}
		}
	// 	break;
	// case CHILD_ID_MOVE_DETECT:
	// 	if (message.type == V_STATUS) {
	// 		motion_rcvd = true;
	// 		light.SetMotionDetector(message.getBool());
	// 	}
	// 	break;
	}
}
