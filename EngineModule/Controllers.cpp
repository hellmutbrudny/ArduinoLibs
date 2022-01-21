#include "Arduino.h"
#include "Controllers.h"

Controllers::Controllers(Notifier *notifier) {
  _notifier = notifier;
}

void Controllers::init() {
  control1.begin();
  control2.begin();
  read();
  controlRequest = 0;
}

void Controllers::read() {
  controlVal[0] = control1.readADC_SingleEnded(0);
  controlVal[1] = control2.readADC_SingleEnded(0);
  for (byte i = 0; i < 2; i++) {
    int delta = controlVal[i] - prevControlVal[i];
    if (delta < -CONTROL_VALUE_STEP || delta > CONTROL_VALUE_STEP) {
      prevControlVal[i] = controlVal[i];
      if (operationMode == MODE_CTRL) {
        controlRequest = calculateControlRequest(controlVal[i]);
	    _notifier->thrtlBreak = 0;
      }
      _notifier->inputChanged = 1;
    }
  }
}

int Controllers::calculateControlRequest(int16_t val) {
  // Scale sensor value range to -THROTTLE_POS_MAX..THROTTLE_POS_MAX
  // Treat range CONTROL_ZERO-CONTROL_ZERO_TOLERANCE..CONTROL_ZERO+CONTROL_ZERO_TOLERANCE as 0
  if (val >= CONTROL_ZERO - CONTROL_ZERO_TOLERANCE && val <= CONTROL_ZERO + CONTROL_ZERO_TOLERANCE) {
    return 0;
  }
  if (val > CONTROL_ZERO) {
    if (val >= CONTROL_MAX) {
      return -THROTTLE_POS_MAX;
    }
    return (val - CONTROL_ZERO - CONTROL_ZERO_TOLERANCE) * -THROTTLE_POS_MAX / (CONTROL_MAX - CONTROL_ZERO - CONTROL_ZERO_TOLERANCE);
  }
  if (val <= CONTROL_MIN) {
    return THROTTLE_POS_MAX;
  }
  return (CONTROL_ZERO - CONTROL_ZERO_TOLERANCE - val) * THROTTLE_POS_MAX / (CONTROL_ZERO - CONTROL_ZERO_TOLERANCE - CONTROL_MIN);
}
