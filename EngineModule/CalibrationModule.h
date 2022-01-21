#ifndef CalibrationModule_h
#define CalibrationModule_h

#include "Arduino.h"
#include <Wire.h>
#include <DFRobot_LCD.h>
#include "EngineModule.h"

#define TESTWIRE Wire

class CalibrationModule {
  public:
    CalibrationModule(EngineModule *engine);
    void init();
    void refreshDataOnLCD();
    void readKeyboard();
    void showOnLCD(String text);
    byte isActive = 0;
  private:
    byte keyboardRead(byte channel);
    int adjustToBaseLine(byte line);
    void changeOperationMode(int delta);
    EngineModule *_engine;
    byte baseLine = 0;
    byte resetKey = 0;
    DFRobot_LCD lcd = DFRobot_LCD(16, 2, &TESTWIRE);
};	
    
#endif
