#include "PerformanceReport.h"

PerformanceReport::PerformanceReport(int size) {
  this->size = size;
  this->data = new tPerformanceReportData[size];
  reset();
}

void PerformanceReport::reset() {
  for (int i = 0; i < size; i++) {
    data[i].count = 0;
    data[i].sum = 0;
  }
  startTime = millis();
}

void PerformanceReport::setName(int idx, char *name) {
  data[idx].name = name;
}

void PerformanceReport::start(int idx) {
  data[idx].startTime = millis();
}

void PerformanceReport::stop(int idx) {
  long endTime = millis();
  data[idx].count++;
  data[idx].sum += endTime - data[idx].startTime;
}

void PerformanceReport::report() {
  long endTime = millis();
  Serial.printf("PerformanceReport - last round took: %dms\n", endTime - startTime);
  for (int i = 0; i < size; i++) {
    Serial.printf("%s: count:%d avg:%dms\n", data[i].name, data[i].count, data[i].count > 0 ? 0 : data[i].sum / data[i].count);
  }
  reset();
}

