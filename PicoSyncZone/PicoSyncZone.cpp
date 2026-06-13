#include <PicoSyncZone.h>

PicoSyncZone::PicoSyncZone() {
  mutex_init(&_mutex);
}

void PicoSyncZone::enter() {
  mutex_enter_blocking(&_mutex);
}

void PicoSyncZone::exit() {
  mutex_exit(&_mutex);
}
