#ifndef _PicoSyncZone_h_
#define _PicoSyncZone_h_
#include <pico/multicore.h>

class PicoSyncZone {
  public:
    PicoSyncZone() {
      mutex_init(&_mutex);
    }
    void enter() {
      mutex_enter_blocking(&_mutex);
    }
    void exit() {
      mutex_exit(&_mutex);
    }
  private:
    mutex_t _mutex;
};

#endif
