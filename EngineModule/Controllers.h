#ifndef Controllers_h
#define Controllers_h

#include "Arduino.h"
#include "Notifier.h"
#include <Adafruit_ADS1015.h>

#define CONTROL_VALUE_STEP 80
#define CONTROL_ZERO 8400
#define CONTROL_MAX 16800
#define CONTROL_MIN 0
#define CONTROL_ZERO_TOLERANCE 800

#define MODE_CTRL 0 // Speed is set by controls - normal operation
#define MODE_SPD 1 //Display controls - keyboard changes speed
#define MODE_TH 2 //Keyboard changes throttle position
#define MODE_GR 3 //Keyboard changes gear position
#define THROTTLE_POS_MAX 55

class Controllers {
  public:
    Controllers(Notifier *notifier);
    void init();
    void read();
    int controlRequest; // control position scaled to range -THROTTLE_POS_MAX..THROTTLE_POS_MAX - the state we want to be
    int16_t controlVal[2]; // current value
    byte operationMode = MODE_CTRL;
  private:
    Adafruit_ADS1115 control1 = Adafruit_ADS1115(0x48);
    Adafruit_ADS1115 control2 = Adafruit_ADS1115(0x49);
    int16_t prevControlVal[2]; // previous value
    int calculateControlRequest(int16_t val);
    Notifier *_notifier;
};

#endif
