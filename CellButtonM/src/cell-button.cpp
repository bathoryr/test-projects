/*  Button sensor 
	There is output for LED instead of seconf input button. Not used yet.
	1 MHz int, Optiboot
*/
//#define MY_DEBUG
#define MY_RADIO_RF24
//#define RF24_PA_MAX
#define MY_BAUD_RATE 9600
#include <MyConfig.h>
#include <MySensors.h>  

#define PIN_BUTTON_1	2
#define LED_LIGHT		3
#define ID_VOLTAGE		9

const long InternalReferenceVoltage = 1084;  // Adjust this value to your board's specific internal BG voltage
static const float VMIN = 1.8, VMAX = 3.2;

MyMessage msgBtn(PIN_BUTTON_1, V_TRIPPED);
bool msgAck;
// Forward declarations
void sendBatteryMsg();
float getBandgap();

void presentation() 
{
	sendSketchInfo("M's button", "1.0");
	present(PIN_BUTTON_1, S_DOOR, "Button 1");
}

void setup() 
{
	pinMode(PIN_BUTTON_1, INPUT);
	pinMode(LED_LIGHT, OUTPUT);
	sendBatteryMsg();
}

void loop() 
{
	int8_t btn = sleep(0, CHANGE, 3600000);
	if (btn == 0) {
		delay(20);
		msgBtn.sensor = PIN_BUTTON_1;
		msgAck = false;
		for (uint8_t i = 0; msgAck == false && i < 3; i++)
		{
			send(msgBtn.set(digitalRead(PIN_BUTTON_1) == LOW), true);
			wait(500, C_SET, V_TRIPPED);
		}
	}
	else
	{
		sendBatteryMsg();
	}
}

void receive(const MyMessage& msg)
{
	if (msg.isAck())
	{ 
		msgAck = true;
	}
}

void sendBatteryMsg()
{
	static uint8_t counter = 0;

	float volt = getBandgap();
	if (counter++ % 2 == 0)
	{
		sendBatteryLevel(MIN(100, MAX((volt - VMIN) / (VMAX - VMIN) * 100.0, 0)));
	}
	else
	{
		// Send on VAR1 channel
		MyMessage msgVar1(PIN_BUTTON_1, V_VAR1);
		send(msgVar1.set(volt, 2));
	}
}

// From example by Nick Gammon (http://www.gammon.com.au/power)
float getBandgap()
{
	// Add some delay for stabilize after sleep
	delay(50);
	// REFS0 : Selects AVcc external reference
	// MUX3 MUX2 MUX1 : Selects 1.1V (VBG)  
	ADMUX = bit(REFS0) | bit(MUX3) | bit(MUX2) | bit(MUX1);
	ADCSRA |= bit(ADSC);  // start conversion
	while (ADCSRA & bit(ADSC))
	{
	}  // wait for conversion to complete
	float result = (float)(((InternalReferenceVoltage * 1024) / ADC) + 5) / 1000;
	return result;
} // end of getBandgap