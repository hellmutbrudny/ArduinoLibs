////////////////////////////////
// Class implementations
////////////////////////////////

#include "Arduino.h"
#include "EngineModule.h"

void EngineModule::init() {  
  btnForward.init();
  btnReverse.init();
  btnMax.init();
  btnMin.init();
  phtIdle.init();
  phtThrottle.init();
  throttleAcctuator.init();
  gearAcctuator.init();
  notifier.inputChanged = 1;
  throttleState.lastMotion = STOPPED;
  throttleState.thrtlPos = 0;
  resetAcctuators();
}

void EngineModule::resetAcctuators() {
  // Reset acctuator states to the starting position - idle gear and zero throttle
  Serial.println("reseting acctuators");
  while (gearState.state != STATE_IDLE || throttleState.state != STATE_MIN) {
    checkStoppers();
    if (throttleState.motionState == STOPPED && gearState.motionState == STOPPED) {
      if (throttleState.state != STATE_MIN) {
        // Gear is idle - go throttle to zero
        Serial.println("go throttle to zero");
        throttleAcctuator.startExtending();
      } else if (gearState.state != STATE_IDLE) {
        // First reset gear
        if (gearState.state == STATE_FORWARD) {
          // Go to idle
          Serial.println("go to idle");
          gearAcctuator.startExtending();
        } else {
          // Search Idle or Forward
          Serial.println("search idle or forward");
          gearAcctuator.startShrinking();
        }
      }
    }
    delay(10);
  }
  Serial.println("reseting acctuators done");
}

void EngineModule::handleControlRequest() {
  if (operationMode == MODE_TH || operationMode == MODE_GR) {
    return;
  }
  if (notifier.thrtlBreak) {
    return;
  }
  if (controlRequest == 0) {
    // We want to go idle
    checkAndChangeGear(STATE_IDLE);
    return;
  }
  // If not idle then check if it forward or reverse  
  byte gearNeeded = controlRequest < 0 ? STATE_REVERSE : STATE_FORWARD;
  int absCtrlPos = abs(controlRequest);
  if (absCtrlPos == throttleState.thrtlPos || notifier.thrtlBreak) {
    // We are in desired position - nothing to do
    return;
  }
  // Now check if we need to change the gear
  if (checkAndChangeGear(gearNeeded)) {
    // And we can finally adjust throttle if we are on the right gear
    if (absCtrlPos < throttleState.thrtlPos) {
      throttleAcctuator.startExtending();
    } else {
      throttleAcctuator.startShrinking();
    }
  }
}

byte EngineModule::checkAndChangeGear(byte dstGear) {
  if (gearState.state == dstGear) {
    // Gear is in place, we can continue
    return 1;
  }
  if (gearState.motionState != STOPPED || throttleState.motionState != STOPPED) {
    // Don't change gears if anything is already in motion
    return 0;
  }
  if (throttleState.thrtlPos > 0) {
    //Change gears only in 0 throttle
    throttleAcctuator.startExtending(); 
    return 0;
  }
  // Otherwise we can start changing gear
  if (dstGear == STATE_FORWARD || (dstGear == STATE_IDLE && gearState.state == STATE_REVERSE)) {
    gearAcctuator.startShrinking();
  } else if (dstGear == STATE_REVERSE || (dstGear == STATE_IDLE && gearState.state == STATE_FORWARD)){
    gearAcctuator.startExtending(); 
  }
  return 0;
}

void EngineModule::checkStoppers() {
  btnForward.checkState();
  btnReverse.checkState();
  btnMax.checkState();
  btnMin.checkState();
  phtIdle.checkState();
}


