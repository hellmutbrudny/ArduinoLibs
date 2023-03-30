#include "PicoSyncZone.h"
#include "Arduino.h"

bool PicoSyncZone::atomicTestAndSet() {
  rp2040.idleOtherCore();
  bool isFree = !busy;
  if (isFree) {
    busy = true;
  }
  rp2040.resumeOtherCore();
  return isFree;
}

void PicoSyncZone::enter() {
  while (1) {
    // Try to obtain the lock
    if (atomicTestAndSet()) {
      //Success
      return;
    }
    //Nope - wait for free lock
    coresWaiting++;
    if (coresWaiting > 1) {
      Serial.printf("\nPicoSyncZone::Deadlock!\n");
    }
    while (busy) {
      // Try not so active waiting
      delayMicroseconds(10);
    }
    coresWaiting--;
    //And try again
  }
}

void PicoSyncZone::exit() {
  //Release the lock
  busy = false;
}
