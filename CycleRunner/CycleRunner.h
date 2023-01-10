#ifndef CycleEntry_h
#define CycleEntry_h

#include "Arduino.h"

class CycleEntry {
  public:
    CycleEntry(unsigned long interval, void (* handler)(), bool continueCycle = false);
    unsigned long handle(unsigned long time);
    void setInterval(unsigned long interval);
    bool isNotLast();
  private:
    unsigned long _lastTime;
    unsigned long _interval;
    void (* _handler)();
    bool _continueCycle;
};

class CycleRunner {
  public:
    CycleRunner(CycleEntry *cycleEntries);
    void runCycle();
  private:
    CycleEntry *_cycleEntries;
};

#endif
