#ifndef CycleEntry_h
#define CycleEntry_h

#include "Arduino.h"

class CycleEntry {
  public:
    CycleEntry(unsigned long interval, void (* handler)(), bool breaksCycle = true); 
    void init();
    bool handle(unsigned long time);
  private:
    unsigned long _lastTime;
    unsigned long _interval;
    void (* _handler)();
    bool _breaksCycle;
};

class CycleRunner {
  public:
    CycleRunner(CycleEntry *cycleEntries, int entriesCount);
    void init();
    void runCycle();
  private:
    CycleEntry *_cycleEntries;
    int _entriesCount;
};

#endif
