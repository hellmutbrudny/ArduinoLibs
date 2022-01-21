////////////////////////////////
// Class implementations
////////////////////////////////

#include "Arduino.h"
#include "TGL.h"
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>

void TGLLabel::repaint(ILI9488 *tft) {
  if (needsRepaint) {
    //Serial.print("repaint ");Serial.println(label);
    if (buffer == NULL) {
      buffer = new GFXcanvas16(w, h);
    }
    buffer->fillScreen(COLOR_BKGRND);
    paintBackground();
    if (size == 0) {
      buffer->setTextSize(1);
    } else if (size == 1) {
      buffer->setFont(&FreeSans9pt7b);
    } else if (size == 2) {
      buffer->setFont(&FreeSans12pt7b);
    } else if (size == 3) {
      buffer->setFont(&FreeSans18pt7b);
    } else {
      buffer->setFont(&FreeSans24pt7b);
    }
    buffer->setTextColor(color);
    buffer->setTextWrap(false);
    int16_t tx, ty;
    uint16_t tw, th;
    buffer->getTextBounds(label, 0, 0, &tx, &ty, &tw, &th);
    int16_t dx, dy;
    if (tw > w) {
      dx = -tx;
    } else {
      if (align == ALIGN_LEFT) {
        dx = -tx;
      } else if (align == ALIGN_RIGHT) {
        dx = w - tw - tx;
      } else {
        dx = (w - tw) / 2 - tx;
      }
    }
    if (th > h) {
      dy = -ty;
    } else {
      dy = (h - th) / 2 - ty;
    }
    buffer->setCursor(dx, dy);
    buffer->print(label);
    needsRepaint = false;
    tft->drawImage(buffer->getBuffer(), x, y, w, h);
    delete buffer;
    buffer = NULL;
  }
}

void TGLLabel::setLabel(String label) {
  this->label = label;
  this->needsRepaint = true;  
}

void TGLLabel::setColor(uint16_t color) {
  this->color = color;
  this->needsRepaint = true;  
}

void TGLButton::paintBackground() {
    buffer->fillRoundRect(0, 0, w, h, 5, state == INACTIVE ? COLOR_BTNINACT : state == ACTIVE ? COLOR_BTNACT : COLOR_BTNPRS);
}

TGLScreenElement *TGLButton::handleTouch(TGLTouchEvent *event) {
    if (event->x >= x && event->x <= x+w && event->y >= y && event->y <= y+h) {
      setState(event->touching == 1 ? PRESSED : ACTIVE);       
      return this;   
    }
    return NULL;
}

void TGLButton::setState(tTGLButtonState state) {
  this->state = state;
  this->needsRepaint = true;
}

void TGLWindRose::repaint(ILI9488 *tft) {
  if (needsRepaint) {
    if (buffer == NULL) {
      buffer = new GFXcanvas16(w, h);
    }
    buffer->fillScreen(COLOR_BKGRND);
    int16_t lx, ly;
    //apparent wind
    countDeltaXY(appWind, r, &lx, &ly);
    drawArrow(-ly, -lx, COLOR_APPWIND); 
    //real wind
    countDeltaXY(realWind, r, &lx, &ly);
    drawArrow(-ly, -lx, COLOR_REALWIND);
    //circle
    buffer->drawCircle(x0, y0, r, COLOR_ROSE);
    //dead angle
    countDeltaXY(40, r, &lx, &ly);
    buffer->drawLine(x0, y0, x0+ly, y0-lx, COLOR_ROSE);
    buffer->drawLine(x0, y0, x0-ly, y0-lx, COLOR_ROSE);    
    //rudder angle
    countDeltaXY(35, r/3, &lx, &ly);
    buffer->drawLine(x0, y0, x0+ly, y0+lx, COLOR_DARKROSE);
    buffer->drawLine(x0, y0, x0-ly, y0+lx, COLOR_DARKROSE);    
    countDeltaXY(rudder, r/3, &lx, &ly);
    buffer->drawLine(x0, y0, x0+ly, y0+lx, COLOR_REALWIND);
    //main directions
    countDeltaXY(course, r-10, &lx, &ly);
    buffer->setTextColor(COLOR_DARKROSE);
    buffer->setTextSize(2);
    buffer->setCursor(x0-ly-4, y0-lx-6);
    buffer->print("N");
    buffer->setCursor(x0+lx-4, y0-ly-6);
    buffer->print("E");
    buffer->setCursor(x0+ly-4, y0+lx-6);
    buffer->print("S");
    buffer->setCursor(x0-lx-4, y0+ly-6);
    buffer->print("W");
    needsRepaint = false;
    tft->drawImage(buffer->getBuffer(), x, y, w, h);
    delete buffer;
    buffer = NULL;
  }  
}

void TGLWindRose::drawArrow(int16_t dx, int16_t dy, uint16_t color) {
  buffer->fillTriangle(x0+(dx*2/3), y0+(dy*2/3),
                       x0+dx+(dy/9), y0+dy-(dx/9),
                       x0+dx-(dy/9), y0+dy+(dx/9),
                       color);
}

void TGLWindRose::countDeltaXY(int16_t a, int16_t r, int16_t *x, int16_t *y) {
  float ar = 0.017453293 * a;
  *x = (int16_t)(cos(ar) * r); 
  *y = (int16_t)(sin(ar) * r);
}

void TGLWindRose::setCourse(int16_t course) {
  this->course = course;
  this->needsRepaint = true;
}

void TGLWindRose::setWind(int16_t realWind, int16_t appWind) {
  this->realWind = realWind;
  this->appWind = appWind;
  this->needsRepaint = true;
}

void TGLWindRose::setRudder(int16_t rudder) {
  this->rudder = rudder;
  this->needsRepaint = true;
}

void TGLScreen::repaintElements(ILI9488 *tft) {
  for (int i=0; i<elementCount; i++) {
    elements[i]->repaint(tft);
  }
}

void TGLScreen::invalidateElements() {
  for (int i=0; i<elementCount; i++) {
    elements[i]->needsRepaint = true;
  }
}

TGLScreenElement *TGLScreen::handleTouch(TGLTouchEvent *event) {
  for (int i=0; i<elementCount; i++) {
    TGLScreenElement *handler = elements[i]->handleTouch(event);
    if (handler != NULL) {
      return handler;
    }
  }
  return NULL;
}

void TGLAppInterface::init() {
  tft.begin();
  tft.setRotation(0);
  tft.invertDisplay(true);
  ts.begin();
  ts.setRotation(0);
  activeButton = NULL;
  touchEvent.touching = 0;
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
}

void TGLAppInterface::setActiveScreen(TGLScreen *screen) {
  activeScreen = screen;
  tft.fillScreen(COLOR_BKGRND);
  activeScreen->invalidateElements();
}

void TGLAppInterface::checkTouching() {
  //Serial.printf("tirqTouched=%d\n", ts.tirqTouched());
  if (ts.tirqTouched()) {
    if (touchEvent.touching == 0) {
      uint16_t x, y;
      uint8_t z;
      ts.readData(&x, &y, &z);
      x = ((x - 250) / 10) * 32 / 35;
      y = ((3950 - y) / 10) * 48 / 37;
      touchEvent.x = x;
      touchEvent.y = y;
      touchEvent.touching = 1;
      TGLButton *handler = (TGLButton *)activeScreen->handleTouch(&touchEvent);
      if (handler != NULL && handler != activeButton) {
        if (activeButton != NULL) {
          activeButton->setState(INACTIVE);
        }
        activeButton = handler;
      }
    }
  } else {
    if (touchEvent.touching == 1) {
      touchEvent.touching = 0;
      activeScreen->handleTouch(&touchEvent);
    }  
  }
}

void TGLAppInterface::checkRepainting() {
  activeScreen->repaintElements(&tft);
}


