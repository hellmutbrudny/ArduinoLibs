#include "SyncZone.h"
#include "Arduino.h"

bool SyncZoneSemaphore = 0;

void enterSyncZone() {
  rp2040.idleOtherCore();
  bool isFree = (SyncZoneSemaphore == 0);
  if (isFree) {
    SyncZoneSemaphore = 1;
  }
  rp2040.resumeOtherCore();
  if (!isFree) {
    while (SyncZoneSemaphore == 1) { /* noop */ }
  }
}

void exitSyncZone() {
  SyncZoneSemaphore = 0;
}
