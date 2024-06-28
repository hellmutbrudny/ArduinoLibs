// #define N2K_DEVICE_NAME "Whocares Power Monitor"
// #define N2K_DEVICE_SERIAL 011213
// #define N2K_DEVICE_FUNCTION 150
// #define N2K_DEVICE_CLASS 75
// #define N2K_DEVICE_TRANSMIT_MESSAGES {127505L, 127508L, 0}
// const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] = {0}

#ifndef N2kRegisters_h
#define N2kRegisters_h

// Commmands

#define N2KRC_RegisterValueInfo 100
#define N2KRC_GetRegister 101
#define N2KRC_SetRegister 102
#define N2KRC_SetRegisterBySensor 103

#include <NMEA2000.h>

class tN2kRegisters
{
protected:
  tNMEA2000 *N2K;
  unsigned char registerCount = 0;
  unsigned char *registers;
  int32_t *registerValues;
  int notSendCounter = 0;
  int parseCounter = 0;

public:
  tN2kRegisters(tNMEA2000 *N2K, unsigned char *registers, int32_t *initialValues) {
    this->N2K = N2K;
    this->registers = registers;
    this->registerValues = initialValues;
    while (this->registers[registerCount++] != 0);
    registerCount--;
  }

  void initN2kRegisters(const char *modelSerialCode,
                                        uint16_t registration,
                                        const char *deviceName,
                                        unsigned long deviceSerial,
                                        unsigned char deviceFunction,
                                        unsigned char deviceClass,
                                        uint8_t busSource,
                                        void (*msgHandler)(const tN2kMsg &N2kMsg));
  void sendN2kMsg(const tN2kMsg &N2kMsg);
  void setN2kRegisterCommand(tN2kMsg &N2kMsg, unsigned char command, unsigned char registerId, int32_t param=0);
  bool parseN2kRegisterCommand(const tN2kMsg &N2kMsg, unsigned char &command, unsigned char &registerId, int32_t &param);
  void sendN2kRegisterCommand(unsigned char command, unsigned char registerId, int32_t param=0);
  void parseN2kMessages();

// raw access to registers
  int getRegisterIndex(unsigned char registerId);
  void setRegisterRawValue(int idx, int32_t value);

// internal
  void handleN2kRegisterCommand(const tN2kMsg &N2kMsg);
  void handleGetRegister(unsigned char registerId, int idx);
  void handleSetRegister(unsigned char registerId, int32_t value, int idx);
  void handleSetRegisterBySensor(unsigned char registerId, int idx);
  virtual void handleRegisterValueInfo(unsigned char registerId, int32_t param) {};
  virtual void handleOtherN2kMsg(const tN2kMsg &N2kMsg) {};
  virtual void handleRegisterChange(unsigned char registerId, int32_t newValue) {};
  virtual int32_t sensorValueForRegister(unsigned char registerId) { return 0; };

// direct register array access
  int32_t getRegisterValue(unsigned char registerId);
  bool setRegisterValue(unsigned char registerId, int32_t value);

// EEPROM handling
  uint16_t readEEPROM32b(int addr);
  void writeEEPROM32b(int addr, int32_t val);
  void saveRegistersToEEPROM();
  void readRegistersFromEEPROM();
};

#endif
