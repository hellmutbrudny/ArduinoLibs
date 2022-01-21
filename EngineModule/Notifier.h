#ifndef Notifier_h
#define Notifier_h

class Notifier {
  public:
    byte inputChanged = 0;
    byte thrtlBreak = 0; // Flag indicating that throttle acctuator was stopped by the counter (requested throttle position was achieved)
};

#endif
