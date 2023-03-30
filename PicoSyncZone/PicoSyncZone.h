#ifndef _PicoSyncZone_h_
#define _PicoSyncZone_h_

class PicoSyncZone {
  public:
    void enter();
    void exit();

  private:
    bool busy = false;
    int coresWaiting = 0;
    bool atomicTestAndSet();
};

#endif
