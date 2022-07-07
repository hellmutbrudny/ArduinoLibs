#ifndef USBHOSTAOA_h
#define USBHOSTAOA_h

#include "Arduino.h"
#include "USBHost_t36.h"

class USBHostAOA {
  public:
    USBHostAOA() {}
    void init();
    void process();
    void send(size_t len, uint8_t *buf);
    bool isReady();
    int receive();
    char *getReceivedBuffer();
  private:
    USBHost host = USBHost();
    ADK adk = ADK(host);
    bool adkActive = false;
    char readBuffer[512];
};

#endif
