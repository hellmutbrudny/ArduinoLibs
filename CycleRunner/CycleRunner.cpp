#include "CycleRunner.h"

CycleEntry::CycleEntry(unsigned long interval, void (* handler)(), bool breaksCycle) {
  _interval = interval;
  _handler = handler;
  _breaksCycle = breaksCycle;
}

void CycleEntry::init() {
  _lastTime = millis();
}

bool CycleEntry::handle(unsigned long time) {
  if (time - _lastTime >= _interval) {
    _handler();
    _lastTime = time;
    return _breaksCycle;
  }
  return false;
}

CycleRunner::CycleRunner(CycleEntry *cycleEntries, int entriesCount) {
  _cycleEntries = cycleEntries;
  _entriesCount = entriesCount;
}

void CycleRunner::init() {
  for (byte i = 0; i < _entriesCount; i++) {
    _cycleEntries[i].init();
  }
}

void CycleRunner::runCycle() {
  unsigned long time = millis();
  for (byte i = 0; i < _entriesCount; i++) {
    if (_cycleEntries[i].handle(time)) {
      // We only call just one of entries on each cycle check to not keep cycle running too long - unless it's non-breaking entry.
      break;  
    }
  }
}



