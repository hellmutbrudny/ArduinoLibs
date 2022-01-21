#ifndef EngineModule_h
#define EngineModule_h

#include "Acctuators.h"

////////////////////////////////////////////
// Engine module 
////////////////////////////////////////////

#define MODE_CTRL 0 // Speed is set by controls - normal operation
#define MODE_SPD 1 //Display controls - keyboard changes speed
#define MODE_TH 2 //Keyboard changes throttle position
#define MODE_GR 3 //Keyboard changes gear position

class EngineModule {
  public:
    Notifier notifier = Notifier();
    ThrottleState throttleState = ThrottleState(&notifier);
    GearState gearState = GearState(&notifier);
    Acctuator throttleAcctuator = Acctuator(39, 37, &throttleState);
    Acctuator gearAcctuator = Acctuator(43, 41, &gearState);
    ThrottleCounter phtThrottle = ThrottleCounter(23, 0, &notifier, &throttleState);
    Stopper phtIdle = Stopper(25, 0, &notifier, &gearAcctuator, STATE_IDLE);
    Stopper btnMin = Stopper(27, 1, &notifier, &throttleAcctuator, STATE_MIN);
    Stopper btnMax = Stopper(29, 1, &notifier, &throttleAcctuator, STATE_MAX);
    Stopper btnReverse = Stopper(31, 1, &notifier, &gearAcctuator, STATE_REVERSE);
    Stopper btnForward = Stopper(33, 1, &notifier, &gearAcctuator, STATE_FORWARD);
    void init();
    void resetAcctuators();
    void handleControlRequest();
    void checkStoppers();
    int controlRequest; // last requested control position scaled to range -THROTTLE_POS_MAX..THROTTLE_POS_MAX - the state we want to be
    byte operationMode = MODE_CTRL;
  private:
    byte checkAndChangeGear(byte dstGear);
};

#endif
