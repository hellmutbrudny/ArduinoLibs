#ifndef _PicoSyncZone_h_
#define _PicoSyncZone_h_
// #include <pico/multicore.h>

class PicoSyncZone {
  public:
    PicoSyncZone();
    void enter();
    void exit();
  private:
    bool flag[2] = {false, false};
    int turn = 0;
};

#endif
