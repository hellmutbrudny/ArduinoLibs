#include "CycleRunner.h"

CycleEntry::CycleEntry(unsigned long interval, void (* handler)(), bool continueCycle) {
  _interval = interval;
  _handler = handler;
  _continueCycle = continueCycle;
  _lastTime = 0;
}

bool CycleEntry::handle(unsigned long time) {
  if (_handler == nullptr) {
    return false;
  }
  if (time - _lastTime >= _interval) {
    _handler();
    _lastTime = time;
    return _continueCycle;
  }
  return true;
}

void CycleEntry::setInterval(unsigned long interval) {
  _interval = interval;
}

CycleRunner::CycleRunner(CycleEntry *cycleEntries) {
  _cycleEntries = cycleEntries;
}

void CycleRunner::runCycle() {
  unsigned long time = millis();
  byte i = 0;
  while (_cycleEntries[i].handle(time)) {i++;}
}
