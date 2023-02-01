#include "CycleRunner.h"

CycleEntry::CycleEntry(unsigned long interval, void (* handler)(), bool continueCycle) {
  _interval = interval;
  _handler = handler;
  _continueCycle = continueCycle;
  _lastTime = 0;
}

unsigned long CycleEntry::handle(unsigned long time) {
  if (time - _lastTime >= _interval) {
    _handler();
    _lastTime = time;
    if (_continueCycle) {
      return _interval;
    }
    return 0;
  }
  return _lastTime + _interval - time;
}

bool CycleEntry::isNotLast() {
  return _handler != nullptr;
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
  unsigned long min_to_wait = 10000;
  while (_cycleEntries[i].isNotLast()) {
    unsigned long to_wait = _cycleEntries[i].handle(time);
    if (to_wait == 0) {
      return;
    }
    if (to_wait < min_to_wait) {
      min_to_wait = to_wait;
    }
    i++;
  }
  if (min_to_wait > 0) {
    delay(min_to_wait);
  }
}
