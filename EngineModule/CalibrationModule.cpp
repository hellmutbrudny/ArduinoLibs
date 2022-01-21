///////////////////////////////////////////
// TEST/CALIBRATION MODULE (LCD + Keyboard)
///////////////////////////////////////////

#include "Arduino.h"
#include "EngineModule.h"
#include "CalibrationModule.h"

#define KEY_LEFT 36
#define KEY_UP 38
#define KEY_RIGHT 40
#define KEY_DOWN 42

CalibrationModule::CalibrationModule(EngineModule *engine) {
  _engine = engine;
}

void CalibrationModule::init() {
  //Serial.println("Intializing CalibrationModule"); 
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
 
  Serial.print("Calibration module LCD ");
  // Check if test module (lcd and keyboard) is connected.
  TESTWIRE.begin();
  TESTWIRE.setClock(50000);
  TESTWIRE.beginTransmission(0x3E);
  byte i2cError = TESTWIRE.endTransmission();
  isActive = (i2cError == 0);
  if (isActive) {
    lcd.init();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*"); 
  }
  Serial.println(calibration.isActive ? "found" : "NOT found");
}

void CalibrationModule::refreshDataOnLCD() {
  //Serial.println("refreshDataOnLCD");
  if (isActive && _engine->notifier.inputChanged) {
    int l = adjustToBaseLine(MODE_TH);
    if (l >= 0) {
      String line = "-.......-";
      if (_engine->btnMax.currentState) line.setCharAt(0, '+');
      if (_engine->btnMin.currentState) line.setCharAt(8, '+');
      if (_engine->throttleState.motionState == SHRINKING) line.setCharAt(2, '<');
      if (_engine->throttleState.motionState == EXTENDING) line.setCharAt(6, '>');
      lcd.setCursor(1, l);
      lcd.print(line); 
      lcd.setCursor(4, l);
      lcd.print(_engine->throttleState.thrtlPos);
    }
    l = adjustToBaseLine(MODE_GR);
    if (l >= 0) {
      String line = "-...-...-";
      if (_engine->btnForward.currentState) line.setCharAt(0, '+');
      if (_engine->phtIdle.currentState) line.setCharAt(4, '+');
      if (_engine->btnReverse.currentState) line.setCharAt(8, '+');
      if (_engine->gearState.state == STATE_MID_IF) {
        if (_engine->gearState.motionState == SHRINKING) line.setCharAt(2, '<');
        else if (_engine->gearState.motionState == EXTENDING) line.setCharAt(2, '>');
        else line.setCharAt(2, 'X');
      }
      if (_engine->gearState.state == STATE_MID_IR) {
        if (_engine->gearState.motionState == SHRINKING) line.setCharAt(6, '<');
        else if (_engine->gearState.motionState == EXTENDING) line.setCharAt(6, '>');
        else line.setCharAt(6, 'X');
      }
      lcd.setCursor(1, l);
      lcd.print(line);
    }
    l = adjustToBaseLine(MODE_CTRL);
    if (l >= 0) {
      lcd.setCursor(1, l);
        lcd.print("D:");
        String line = "-.....-:-.-.-";
  	    if (_engine->btnMax.currentState) line.setCharAt(0, '+');
	    if (_engine->btnMin.currentState) line.setCharAt(6, '+');
	    if (_engine->throttleState.motionState == SHRINKING) line.setCharAt(1, '<');
	    if (_engine->throttleState.motionState == EXTENDING) line.setCharAt(5, '>');
        if (_engine->btnForward.currentState) line.setCharAt(8, '+');
        if (_engine->phtIdle.currentState) line.setCharAt(10, '+');
        if (_engine->btnReverse.currentState) line.setCharAt(12, '+');
        if (_engine->gearState.state == STATE_MID_IF) {
          if (_engine->gearState.motionState == SHRINKING) line.setCharAt(9, '<');
          else if (_engine->gearState.motionState == EXTENDING) line.setCharAt(9, '>');
          else line.setCharAt(9, 'X');
        }
        if (_engine->gearState.state == STATE_MID_IR) {
          if (_engine->gearState.motionState == SHRINKING) line.setCharAt(11, '<');
          else if (_engine->gearState.motionState == EXTENDING) line.setCharAt(11, '>');
          else line.setCharAt(11, 'X');
        }
        lcd.setCursor(3, l);
        lcd.print(line);
        lcd.setCursor(5, l);
        lcd.print(_engine->throttleState.thrtlPos);
    }
    l = adjustToBaseLine(MODE_SPD);
    if (l >= 0) {
      lcd.setCursor(1, l);
      lcd.print("A:   B:   S:   ");
      lcd.setCursor(3, l);
      //lcd.print(_engine->controllers.controlVal[0] / 100); 
      lcd.setCursor(8, l);
      //lcd.print(_engine->controllers.controlVal[1] / 100); 
      lcd.setCursor(13, l);
      lcd.print(_engine->controlRequest); 
    }
    _engine->notifier.inputChanged = 0;
  }
}

// There are four lines/operation modes - find a lcd line for given mode basing on current navigation state. 0-1 display, -1 invisible.
int CalibrationModule::adjustToBaseLine(byte line) {
  if ((line < baseLine) || (line > baseLine + 1)) return -1;
  return line - baseLine;
}

void CalibrationModule::readKeyboard() {
    byte keyRight = !digitalRead(KEY_RIGHT);
    byte keyLeft = !digitalRead(KEY_LEFT);
    byte keyUp = !digitalRead(KEY_UP);
    byte keyDown = !digitalRead(KEY_DOWN);
    
    if (resetKey) {
      if (!keyUp && !keyDown) {
        resetKey = 0;
      }
    } else {
      if (keyUp) {
        changeOperationMode(-1);
        resetKey = 1;
      } else if (keyDown) {
        changeOperationMode(1);
        resetKey = 1;
      } else if (keyRight) {
        if (_engine->operationMode == MODE_TH) {
          _engine->throttleAcctuator.startExtending(); 
        } else if (_engine->operationMode == MODE_GR) {
          _engine->gearAcctuator.startExtending();  
        } else if (_engine->operationMode == MODE_SPD) {
          _engine->controlRequest += 5;
          if (_engine->controlRequest > THROTTLE_POS_MAX) {
            _engine->controlRequest = THROTTLE_POS_MAX; 
          }
          _engine->notifier.inputChanged = 1;
          _engine->notifier.thrtlBreak = 0;
        }
        _engine->notifier.inputChanged = 1; 
      } else if (keyLeft) {
        if (_engine->operationMode == MODE_TH) {
          _engine->throttleAcctuator.startShrinking(); 
        } else if (_engine->operationMode == MODE_GR) {
          _engine->gearAcctuator.startShrinking(); 
        } else if (_engine->operationMode == MODE_SPD) {
          _engine->controlRequest -= 5;
          if (_engine->controlRequest < -THROTTLE_POS_MAX) {
            _engine->controlRequest = -THROTTLE_POS_MAX;
          }
          _engine->notifier.inputChanged = 1;
          _engine->notifier.thrtlBreak = 0;
        }        
      } else { // No key
        if (_engine->operationMode == MODE_TH && _engine->throttleState.motionState != STOPPED) {
          _engine->throttleAcctuator.stop(_engine->throttleState.state);
          _engine->notifier.inputChanged = 1;
        } else if (_engine->operationMode == MODE_GR && _engine->gearState.motionState != STOPPED) {
          _engine->gearAcctuator.stop(_engine->gearState.state);
          _engine->notifier.inputChanged = 1;
        }
      }
    }
}

void CalibrationModule::changeOperationMode(int delta) {
  lcd.clear();
  if (delta == 1 && _engine->operationMode < MODE_GR) {
    _engine->operationMode += 1;
    if (baseLine < _engine->operationMode - 1) baseLine = _engine->operationMode - 1;
  } else if (delta == -1 && _engine->operationMode > MODE_CTRL) {
    _engine->operationMode -= 1;
    if (baseLine > _engine->operationMode) baseLine = _engine->operationMode;
  } 
  lcd.setCursor(0, _engine->operationMode - baseLine);
  lcd.print("*");
  //Serial.print("Mode:");Serial.println(_engine->operationMode);
  _engine->notifier.inputChanged = 1;
}

void CalibrationModule::showOnLCD(String text) {
  if (isActive) {
    lcd.setCursor(1, 0);
    lcd.print(text);
  }
}
