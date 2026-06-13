#ifndef _PicoSyncZone_h_
#define _PicoSyncZone_h_
#include <pico/multicore.h>

class PicoSyncZone {
  public:
    PicoSyncZone();
    void enter();
    void exit();
  private:
    mutex_t _mutex;
};

#endif
