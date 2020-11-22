#pragma once

#include <Wire.h>
#include <BH1750.h>
#include "buffer.h"

class Light 
{
public:
    Light() {
        lightLevels = new buffer<unsigned int>(60); 
        lightState = false;
        timerLight = millis();
	}

    ~Light() {
      delete lightLevels;
    }
	
    void Setup() {
      // MUST be called from setup() function, not from the constructor, else node won't boot
	  Wire.begin();
	  lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
	}
	
    bool IsLightOn() {
        return lightState;
    }

    bool MotionDetected() {
        bool motion = digitalRead(MOTION_PIN) == HIGH;
        if (motionDetectActive)
            return motion;
        else {
            if (lastMotion != motion) {
                lastMotion = motion;
                MyMessage msg(CHILD_ID_MOTION, V_TRIPPED);
                send(msg.set(lastMotion));
            }
        }
        return false;
    }

    void TurnOff() {
        digitalWrite(OUTPUT_PIN, LOW);
        lightState = false;
        MyMessage switchMsg(CHILD_ID_LIGHT, V_STATUS);
        send(switchMsg.set(false));
    }

    void TurnOn() {
        analogWrite(OUTPUT_PIN, lightIntensity);
        lightState = true;
        ResetTimer();
        MyMessage switchMsg(CHILD_ID_LIGHT, V_STATUS);
        send(switchMsg.set(true));
    }

    void SetIntensity(int percent) {
        lightIntensity = map(percent, 0, 100, 0, 255);
        if (IsLightOn()) {
            analogWrite(OUTPUT_PIN, lightIntensity);
        }
        MyMessage msg(CHILD_ID_LIGHT, V_DIMMER);
        send(msg.set(percent));
    }

    void SetMotionDetector(bool active = true) {
        motionDetectActive = active;
    }

    void SetTimeout(int minutes) {
        lightTimeout = minutes;
    }

    bool IsDarkness(unsigned int lux = 12) {
        return GetAvgIllumination() < lux;
    }

    unsigned int GetIlluminationLevel() {
        return lightSensor.readLightLevel();
    }

    unsigned int GetAvgIllumination() {
        return lightLevels->GetAvgVal();
    }

    void CheckMotion() {
        lightLevels->add(GetIlluminationLevel());
        bool motion = MotionDetected();
        digitalWrite(MOTION_LED, motion ? HIGH : LOW);
        if (IsLightOn()) {
            if (TimerExpired()) {
                TurnOff();
				// Reset average light level right after light is turned off
				lightLevels->reset(GetIlluminationLevel());
            }
            // Have to set darkness level higher, because of light from LED
            if (motion && IsDarkness(80)) {
                ResetTimer();
            }
        }
        else {
            if (motion && IsDarkness()) {
                TurnOn();
            }
        }
    }

protected:
	bool lightState;
	unsigned long timerLight;
	bool motionDetectActive;
	bool lastMotion;
	// Light timeout in minutes
	unsigned int lightTimeout = 10;
	int lightIntensity = 50;
	BH1750 lightSensor;
	buffer<unsigned int> *lightLevels;

	inline void ResetTimer() {
		timerLight = millis();
	}

	inline bool TimerExpired() {
		return (millis() - timerLight > lightTimeout * 60000ul);
	}
};
