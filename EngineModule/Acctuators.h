#ifndef Acctuators_h
#define Acctuators_h

#include "Arduino.h"
#include "Notifier.h"
#include <CytronMotorDriver.h>

#define THROTTLE_POS_MAX 59

////////////////////////////////////////////
// Acctuators
////////////////////////////////////////////

enum tAcctuatorMotionState {
  STOPPED = 0,
  SHRINKING = 1,
  EXTENDING = 2
};
			  
enum tAcctuatorState {
  STATE_UNKNOWN = 0,  
  STATE_FORWARD = 8,
  STATE_MID_IF = 24,
  STATE_IDLE = 16,
  STATE_MID_IR = 48,
  STATE_REVERSE = 32,
  STATE_MIN = 64, 
  STATE_MID = 192,
  STATE_MAX = 128
};

class AcctuatorState {
  public:
    AcctuatorState(Notifier *notifier);
    virtual byte extending();
    virtual byte shrinking();
    virtual byte stopping(tAcctuatorState stopingState);
    tAcctuatorState state;
    tAcctuatorMotionState motionState;
  private:
    Notifier *_notifier;
};

class ThrottleState: public AcctuatorState {
  // Min is the most extended, max is the most shrinked state
  public:
    ThrottleState(Notifier *notifier) : AcctuatorState{notifier} {};
    virtual byte extending();
    virtual byte shrinking();
    virtual byte stopping(tAcctuatorState stopingState);
    int thrtlPos = 0; // Actual throttle position calculated by the counter
    byte lastMotion;
};

class GearState: public AcctuatorState {
  // Forward is the most extended, reverse is the most shrinked state; 
  public:
    GearState(Notifier *notifier) : AcctuatorState{notifier} {};
    virtual byte extending();
    virtual byte shrinking();
};

class Acctuator {
  public:
    Acctuator(int pinA, int pinB, AcctuatorState *state);
    void init();
    void startExtending();
    void startShrinking();
    void stop(tAcctuatorState stopingState);
    AcctuatorState *_state;
  private:
    int _pinA;
    int _pinB;
    CytronMD *_motor;
};

class AcctuatorSensor {
  public:
    AcctuatorSensor(int pin, byte pullUp, Notifier *notifier) {
      _pin = pin;
      _pullUp = pullUp;
      _notifier = notifier;
    };
    void init();
    void checkState();
    virtual void handleChange() = 0;
    byte currentState;
  private:
    int _pin;
    byte _pullUp;
    Notifier *_notifier;
};

class Stopper: public AcctuatorSensor {
  public:
    Stopper(int pin, byte pullUp, Notifier *notifier, Acctuator *acctuator, tAcctuatorState stoppingState) : AcctuatorSensor(pin, pullUp, notifier) {
      _acctuator = acctuator;
      _stoppingState = stoppingState;
    };
    virtual void handleChange();
  private:
    Acctuator *_acctuator;
    tAcctuatorState _stoppingState;
};


class ThrottleCounter: public AcctuatorSensor {
  public:
    ThrottleCounter(int pin, byte pullUp, Notifier *notifier, ThrottleState *throttleState) : AcctuatorSensor(pin, pullUp, notifier)  {
      _throttleState = throttleState;
    };
    virtual void handleChange();
  private:
    ThrottleState* _throttleState;
};

#endif
