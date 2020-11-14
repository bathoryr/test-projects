#include <U8g2lib.h>
#include "PumpControl.h"

class PumpStatus
{
    public:
        PumpStatus(const PumpControl& pumpRef);
        void Initialize(uint8_t buttonPin);
        void Present();
        void Update();

    protected:
        void DisplayStatus();
        void SendStatus();

    private:
        const PumpControl& pump;
        uint8_t counter;

        U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;

        float pumpTemp;
        float SSRTemp;
        uint16_t runtime;
        String lastError;
        PumpControl::PumpState lastState;
};