#include <PicoSyncZone.h>
#include <pico/platform.h>
#include <Arduino.h>

PicoSyncZone::PicoSyncZone() {}

// Software Locking (Peterson's / Dekker's Algorithm)

void PicoSyncZone::enter() {
  int thread_id = get_core_num();
  int other = 1 - thread_id;
  // Announce the intention to enter
  flag[thread_id] = true;
  turn = other;
  int busy = 0;
  // Busy-wait (spin) while the other thread is active and it is their turn
  while (flag[other]) {
    if (turn == other) {
      if (!busy) {
        busy = 1;
        Serial.printf("!!! Other core %i keeps the lock\n", other);
      }
      flag[thread_id] = false;
      while (turn == other);
      flag[thread_id] = true;
    }
  }
  if (busy) {
    Serial.printf("!!! Other core %i released the lock\n", other);
  }
}

void PicoSyncZone::exit() {
  int thread_id = get_core_num();
  int other = 1 - thread_id;
  flag[thread_id] = false;
}
