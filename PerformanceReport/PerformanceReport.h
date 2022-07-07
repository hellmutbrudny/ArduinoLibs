#ifndef PerformanceReport_h
#define PerformanceReport_h

#include "Arduino.h"

struct tPerformanceReportData {
    int count;
    long sum;
    long startTime;
    char *name;
};

class PerformanceReport {
  public:
    PerformanceReport(int size);
    void setName(int idx, char *name);
    void start(int idx);
    void stop(int idx);
    void report();
  private:
    void reset();
    int size;
    tPerformanceReportData *data;
    long startTime;
};

#endif
