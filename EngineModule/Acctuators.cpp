////////////////////////////////
// Class implementations
////////////////////////////////

#include "Arduino.h"
#include "EngineModule.h"

AcctuatorState::AcctuatorState(Notifier *notifier) {
  state = STATE_UNKNOWN;
  motionState = STOPPED;
  _notifier = notifier;
}

byte AcctuatorState::extending() {
  motionState = EXTENDING;
  _notifier->inputChanged = 1;
  return 1;
}

byte AcctuatorState::shrinking() {
  motionState = SHRINKING;
  _notifier->inputChanged = 1;
  return 1;
}

byte AcctuatorState::stopping(tAcctuatorState stopingState) {
  state = stopingState;
  if (motionState != STOPPED) {
    motionState = STOPPED;
    _notifier->inputChanged = 1;
    return 1;
  }
  return 0;
}

byte ThrottleState::extending() {
  if (state == STATE_MIN) {
    return 0;
  } 
  if (state == STATE_MAX) {
    state = STATE_MID;
  }
  lastMotion = EXTENDING;
  return AcctuatorState::extending(); 
}

byte ThrottleState::shrinking() {
  if (state == STATE_MAX) {
    return 0;
  } 
  if (state == STATE_MIN) {
    state = STATE_MID;
  } 
  lastMotion = SHRINKING;
  return AcctuatorState::shrinking(); 
}

byte ThrottleState::stopping(tAcctuatorState stopingState) {
  if (stopingState == STATE_MIN) {
    thrtlPos = 0;
    lastMotion = STOPPED;
  } else if (stopingState == STATE_MAX) {
    thrtlPos = THROTTLE_POS_MAX;
    lastMotion = STOPPED;
  }
  return AcctuatorState::stopping(stopingState); 
}

byte GearState::extending() {
  if (state == STATE_REVERSE) {
    return 0;
  } 
  if (state == STATE_IDLE) {
    state = STATE_MID_IR;
  } else if (state == STATE_FORWARD) {
    state = STATE_MID_IF;
  }
  return AcctuatorState::extending(); 
}

byte GearState::shrinking() {
  if (state == STATE_FORWARD) {
    return 0;
  } 
  if (state == STATE_IDLE) {
    state = STATE_MID_IF;
  } else if (state == STATE_REVERSE) {
    state = STATE_MID_IR;
  }
  return AcctuatorState::shrinking(); 
}

Acctuator::Acctuator(int pinA, int pinB, AcctuatorState *state) {
  _pinA = pinA;
  _pinB = pinB;
  _state = state;
}

void Acctuator::init() {
  _motor = new CytronMD(PWM_PWM, _pinA, _pinB);
  _motor->setSpeed(0);
}

void Acctuator::startExtending() {
  noInterrupts();
  if (_state->extending()) {
    _motor->setSpeed(255);
  }
  interrupts();
}

void Acctuator::startShrinking() {
  noInterrupts();
  if (_state->shrinking()) {
    _motor->setSpeed(-255);
  }
  interrupts();
}

void Acctuator::stop(tAcctuatorState stopingState) {
  noInterrupts();
  if (_state->stopping(stopingState)) {
    _motor->setSpeed(0);
  }
  interrupts();
} 

void AcctuatorSensor::init() {
  pinMode(_pin, _pullUp ? INPUT_PULLUP : INPUT);
}

void AcctuatorSensor::checkState() {
  byte val = digitalRead(_pin);
  if (currentState != val) {
    currentState = val;
    handleChange();
    _notifier->inputChanged = 1;
  }
}

void Stopper::handleChange() {
  if (currentState) {
    _acctuator->stop(_stoppingState);
  } 
  Serial.print("Stopper: ");Serial.print(_stoppingState);
  Serial.print(" currentState:");Serial.println(currentState);
}

void ThrottleCounter::handleChange() {
  if (_throttleState->lastMotion == EXTENDING) {
    _throttleState->thrtlPos -= 1;
  } else if (_throttleState->lastMotion == SHRINKING) {
    _throttleState->thrtlPos += 1;
  } 
}
