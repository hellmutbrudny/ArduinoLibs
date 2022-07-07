#ifndef TGL_h
#define TGL_h

#include "Arduino.h"
#include "SPI.h"
#include <Adafruit_GFX.h>
#include "ILI9488.h"
#include "XPT2046_Touchscreen.h"

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 2
#define TFT_BL 8

#define TS_CS 14
#define TS_IRQ 15

#define COLOR_BKGRND   ILI9488_BLACK
#define COLOR_TEXT     ILI9488_WHITE
#define COLOR_BTNTEXT  ILI9488_BLACK
#define COLOR_BTNINACT 0x0380
#define COLOR_BTNACT   0x00F8
#define COLOR_BTNPRS   0x7800
#define COLOR_ROSE     0xFD20
#define COLOR_DARKROSE 0xC800
#define COLOR_APPWIND  ILI9488_LIGHTGREY
#define COLOR_REALWIND ILI9488_WHITE

class TGLTouchEvent {
  public:
    TGLTouchEvent() {}
    int16_t x, y;
    bool touching;
};

class TGLScreenElement {
  public:
    TGLScreenElement(int16_t x, int16_t y, int16_t w, int16_t h) {
    	this->x = x;
	    this->y = y;
	    this->w = w;
	    this->h = h;
	    needsRepaint = true;
    }
    virtual void repaint(ILI9488 *tft) = 0;
    virtual TGLScreenElement *handleTouch(TGLTouchEvent *event) { return NULL; }
    bool needsRepaint;
    int16_t x, y, w, h;
    GFXcanvas16 *buffer = NULL;
};

enum tTGLTextAlign {
  ALIGN_CENTER = 0,
  ALIGN_LEFT = 1,
  ALIGN_RIGHT = 2
};

class TGLLabel : public TGLScreenElement {
  public:
    TGLLabel(int16_t x, int16_t y, int16_t w, int16_t h, char *label, int8_t size = 1, tTGLTextAlign align = ALIGN_CENTER, uint16_t color = COLOR_TEXT) : TGLScreenElement(x, y, w, h) {
	    this->label = label;
	    this->size = size;
	    this->align = align;
	    this->color = color;
    };
    virtual void repaint(ILI9488 *tft);
    virtual void paintBackground() {};
    void setLabel(char *label);
    void setColor(uint16_t color);
    char *label;
    uint16_t color;
    int8_t size;
    tTGLTextAlign align;
};

class TGLNumericLabel : public TGLLabel {
  public:
    TGLNumericLabel(int16_t x, int16_t y, int16_t w, int16_t h, char *label, char *format, int8_t size = 1, tTGLTextAlign align = ALIGN_CENTER, uint16_t color = COLOR_TEXT) : TGLLabel(x, y, w, h, label, size, align, color) {
	    this->format = format;
	    this->value = 0;
    };
    void setValue(double value);
    void setFormat(char *format);
    char *format;
    double value;
};

enum tTGLButtonState {
  INACTIVE = 0,
  ACTIVE = 1,
  PRESSED = 2
};

class TGLButton : public TGLLabel {
  public:
    TGLButton(int16_t x, int16_t y, int16_t w, int16_t h, char *label) : TGLLabel(x, y, w, h, label, 2, ALIGN_CENTER, COLOR_BTNTEXT) {
	    state = INACTIVE;
    };
    virtual void paintBackground();
    virtual TGLScreenElement *handleTouch(TGLTouchEvent *event);
    void setState(tTGLButtonState state);
    tTGLButtonState state;
};

class TGLWindRose : public TGLScreenElement {
  public:
    TGLWindRose(int16_t x, int16_t y) : TGLScreenElement(x, y, 180, 180) {
      x0 = w / 2;
      y0 = h / 2;
      r = x0 - 1;
      course = 120 * 0.017453293;
      realWind = 40 * 0.017453293;
      appWind = 32 * 0.017453293;
      rudder = 10 * 0.017453293;
    };
    virtual void repaint(ILI9488 *tft);
    void setCourse(double course);
    void setWind(double realWind, double appWind);
    void setRudder(double rudder);
    double course, realWind, appWind, rudder; //in radians!
  private:
    int16_t x0, y0, r;
    void countDeltaXY(double a, int16_t r, int16_t *x, int16_t *y);
    void drawArrow(int16_t dx, int16_t dy, uint16_t color);
};

typedef TGLScreenElement *TGLScreenElementPtr;

class TGLScreen {
  public:
    TGLScreen(TGLScreenElementPtr *elements, int elementCount) {
	    this->elements = elements;
	    this->elementCount = elementCount;
    } 
    void repaintElements(ILI9488 *tft);
    void invalidateElements();
    TGLScreenElement *handleTouch(TGLTouchEvent *event);
    TGLScreenElementPtr *elements;
    int elementCount;
};

class TGLAppInterface {
  public:
    TGLAppInterface() {}
    ILI9488 tft = ILI9488(TFT_CS, TFT_DC, TFT_RST);
    XPT2046_Touchscreen ts = XPT2046_Touchscreen(TS_CS, TS_IRQ);
    TGLScreen *activeScreen;
    TGLButton *activeButton;
    TGLTouchEvent touchEvent = TGLTouchEvent();
    void init();
    void setActiveScreen(TGLScreen *screen);
    void checkTouching();
    void checkRepainting();
};

#endif
