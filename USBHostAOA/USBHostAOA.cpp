#include "USBHostAOA.h"

const char TC_Manufacturer [] PROGMEM = "Whocares"; 
const char TC_Model [] PROGMEM = "OtCo"; 
const char TC_Desc [] PROGMEM = "OtterController v.1"; 
const char TC_Version [] PROGMEM = "1.0"; 
const char TC_URI [] PROGMEM = ""; 
const char TC_Serial [] PROGMEM = "666666"; 

void USBHostAOA::init()
{
  adk.begin(TC_Manufacturer, TC_Model, TC_Desc, TC_Version, TC_URI, TC_Serial);
  host.begin();
}

void USBHostAOA::process() {
  host.Task();
  if (adk != adkActive) {
    if (adkActive) {
      adkActive = false;
    } else {
      adkActive = true;
    }
  }
}

void USBHostAOA::send(size_t len, uint8_t *msg) {
  if (isReady()) {
    adk.write(len, msg);
  }
}

bool USBHostAOA::isReady() {
  if (adkActive && adk.ready()) {
    return true;
  } else {
    return false;
  }
}

int USBHostAOA::receive() {
  if (adk.available()) {
    int i = 0;
    int c = adk.read();
    while (c > 0 && c != 0xA && i<511) {
      readBuffer[i++] = c;
      c = adk.read();
    }
    readBuffer[i] = 0;
    return i;
  } else {
    return 0;
  }
}

char *USBHostAOA::getReceivedBuffer() {
  return &readBuffer[0];
}
