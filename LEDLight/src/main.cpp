#include <Arduino.h>
#include <Wire.h>

// Sensor config: MYSBootloader, 16MHz Xtal, Nano board

//#define MY_DEBUG
//#define MY_DEBUG_VERBOSE_RF24
#define MY_SPLASH_SCREEN_DISABLED
#define MY_RADIO_RF24
#define MY_RF24_PA_LEVEL (RF24_PA_MAX)
#define MY_NODE_ID 5
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
#define CHILD_ID_DOORBELL 9
#define CHILD_ID_INFO 99

#include "loop-call.h"
#include "Light.h"
#include "doorbell.h"

void CheckMotion();
void SendIllumination();

Light light;
Doorbell doorbell;

void setup()
{
	light.Setup();
	doorbell.Initialize(RFCOMM_PIN);
	pinMode(OUTPUT_PIN, OUTPUT);
	pinMode(MOTION_LED, OUTPUT);
	pinMode(MOTION_PIN, INPUT);

	request(CHILD_ID_LIGHT, V_DIMMER);
	request(CHILD_ID_LIGHT, V_STATUS);
	request(CHILD_ID_MOVE_DETECT, V_STATUS);
	request(CHILD_ID_TIMEOUT, V_LEVEL);
}

void presentation()
{
	sendSketchInfo("Dimmable Light", "3.1.1");

	present(CHILD_ID_LIGHT, S_DIMMER, "Light control");
	present(CHILD_ID_MOTION, S_MOTION, "Motion activity");
	present(CHILD_ID_LUX, S_LIGHT_LEVEL, "Illumination level");
	present(CHILD_ID_TIMEOUT, S_SOUND, "Light timeout");
	present(CHILD_ID_MOVE_DETECT, S_BINARY, "Move detect switch");
	present(CHILD_ID_DOORBELL, S_BINARY, "Doorbell control");
#ifdef MY_DEBUG
	present(CHILD_ID_INFO, S_INFO, "Text info");
#endif
}

void loop()
{
	CALL_LOOP(1000, CheckMotion);
	CALL_LOOP(30000, SendIllumination);
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

void receive(const MyMessage &message)
{
#ifdef MY_DEBUG
	MyMessage msg(CHILD_ID_INFO, V_TEXT);
#endif
	switch (message.sensor)
	{
	case CHILD_ID_LIGHT:
		if (message.type == V_STATUS) {
			// Message from light switch (ON/OFF)
			message.getBool() == true ? light.TurnOn() : light.TurnOff();
		}
		if (message.type == V_DIMMER) {
			// Message from dimmer (intensity 0 - 100)
			light.SetIntensity(message.getInt());
#ifdef MY_DEBUG
			String s = "Got V_DIMMER val: ";
			send(msg.set((s + message.getString()).c_str()));
#endif
		}
		break;
	case CHILD_ID_TIMEOUT:
		if (message.type == V_LEVEL) {
			// Light timeout - if not set on controller, can be 0
			if (message.getInt() > 0) {
				light.SetTimeout(message.getInt());
#ifdef MY_DEBUG
				send(msg.set("Got V_LEVEL val"));
#endif
			}
			else
			{
				// Set to default 15 minutes
				light.SetTimeout(15);
				// Send message to controller to store the value so we can request it later
				MyMessage msgTm(CHILD_ID_TIMEOUT, V_LEVEL);
				send(msgTm.set(15));
#ifdef MY_DEBUG
				send(msg.set("Set V_LEVEL default"));
#endif
			}
		}
		break;
	case CHILD_ID_MOVE_DETECT:
		if (message.type == V_STATUS) {
			light.SetMotionDetector(message.getBool());
#ifdef MY_DEBUG
			String s = "Got V_STATUS msg: ";
			send(msg.set((s += message.getBool() ? "ON" : "OFF").c_str()));
#endif
		}
		break;
	case CHILD_ID_DOORBELL:
		if (message.type == V_STATUS)
		{
			doorbell.Ring();
		}
		break;
	}
}
