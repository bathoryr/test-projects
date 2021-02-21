#include <stdint.h>
#define RCSwitchDisableReceiving
#include "RCSwitch.h"

class Doorbell
{
    public:
    Doorbell();
    void Initialize(uint8_t send_pin);
    void Ring();

    private:
    RCSwitch rf433trm;
};