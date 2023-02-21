#include <N2kRegisters.h>
#include <N2kMessages.h>
#include <NMEA2000.h>
#include <EEPROM.h>

#ifdef ARDUINO_ARCH_RP2040
#define PSEUDO_EEPROM
#endif

void tN2kRegisters::initN2kRegisters(const char *modelSerialCode,
                                      uint16_t registration,
                                      const char *deviceName,
                                      unsigned long deviceSerial,
                                      unsigned char deviceFunction,
                                      unsigned char deviceClass,
                                      uint8_t busSource,
                                      void (*msgHandler)(const tN2kMsg &N2kMsg)) {
  // initalize NMEA2000 object
  N2K->SetProductInformation(modelSerialCode, // Manufacturer's Model serial code
                                 registration, // Manufacturer's product code
                                 deviceName
                                 );
  N2K->SetDeviceInformation(deviceSerial,
                                deviceFunction,
                                deviceClass,
                                registration
                                );
  // <DEBUG>
  // N2K->EnableForward(true);
  // N2K->SetForwardStream(&Serial);
  // N2K->SetForwardType(tNMEA2000::fwdt_Text);
  // N2K->SetDebugMode(tNMEA2000::dm_ClearText);
  // </DEBUG>
  N2K->EnableForward(false); // Disable all msg forwarding to USB (=Serial)

  N2K->SetHeartbeatInterval(5000);
  N2K->SetMode(tNMEA2000::N2km_ListenAndNode, busSource);
  N2K->SetMsgHandler(msgHandler);
  N2K->Open();

  if (registerCount == 0) {
    return;
  }

  #ifdef PSEUDO_EEPROM
    EEPROM.begin(256);
  #endif

  // Check if EEPROM structure is relevant to Registers
  byte check0 = EEPROM.read(0);
  byte check1 = EEPROM.read(1);
  if (check0 == 'R' && check1 == registerCount) {
    int count = 0;
    for (; count < registerCount; count++) {
      if (EEPROM.read(count+2) != registers[count]) {
        break;
      }
    }
    if (registerCount == count) {
      readRegistersFromEEPROM();
      // Registers initialized from EEPROM
      return;
    }
  }

  // Save default values to EEPROM (initialization)
  EEPROM.write(0, 'R');
  EEPROM.write(1, registerCount);
  for (int i = 0; i < registerCount; i++) {
    EEPROM.write(i+2, registers[i]);
  }
  saveRegistersToEEPROM();
  // read just written for sanity and update handling
  readRegistersFromEEPROM();
}

void tN2kRegisters::handleN2kRegisterCommand(const tN2kMsg &N2kMsg) {
  if (N2kMsg.PGN == 128275) {
    unsigned char command;
    unsigned char registerId;
    int32_t param;
    parseN2kRegisterCommand(N2kMsg, command, registerId, param);
    int idx = getRegisterIndex(registerId);
    if (idx >= 0) {
      switch (command) {
        case N2KRC_GetRegister:
          handleGetRegister(registerId, idx);
          return;
        case N2KRC_SetRegister:
          handleSetRegister(registerId, param, idx);
          return;
        case N2KRC_SetRegisterBySensor:
          handleSetRegisterBySensor(registerId, idx);
          return;
        default:
          break;
      }
    }
  }
  // handle other N2k messages - override virtual method if needed
  handleN2kMsg(N2kMsg);
}

void tN2kRegisters::handleGetRegister(unsigned char registerId, int idx) {
  sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
}

void tN2kRegisters::handleSetRegister(unsigned char registerId, int32_t value, int idx) {
  registerValues[idx] = value;
  handleRegisterChange(registerId);
  sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
  if (registerId >= 128) {
    // Hihgest bit means that given register is for constantly changing operational data and is not persisted in EEPROM
    writeEEPROM32b(2*idx + 2+registerCount, registerValues[idx]);
    #ifdef PSEUDO_EEPROM
    EEPROM.commit();
    #endif
  }
}

void tN2kRegisters::handleSetRegisterBySensor(unsigned char registerId, int idx) {
  int32_t sensor = sensorValueForReg(registerId);
  if (sensor > -1) {
    registerValues[idx] = sensor;
    handleRegisterChange(registerId);
    sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
    writeEEPROM32b(2*idx + 2+registerCount, registerValues[idx]);
    #ifdef PSEUDO_EEPROM
    EEPROM.commit();
    #endif
  }
}

void tN2kRegisters::sendN2kRegisterCommand(unsigned char command, unsigned char registerId, int32_t param) {
  tN2kMsg N2kMsg;
  N2kMsg.SetPGN(127501L);
  N2kMsg.Priority = 3;
	tN2kBinaryStatus BankStatus = param;
	BankStatus <<= 32;
	BankStatus |= registerId;
	BankStatus <<= 8;
	BankStatus |= command;
	N2kMsg.AddUInt64(BankStatus);
  N2K->SendMsg(N2kMsg);
}

bool tN2kRegisters::parseN2kRegisterCommand(const tN2kMsg &N2kMsg, unsigned char &command, unsigned char &registerId, int32_t &param) {
    tN2kBinaryStatus BankStatus;
    bool parsed = ParseN2kPGN127501(N2kMsg, command, BankStatus);
    if (parsed) {
      registerId = BankStatus & 0xff;
      BankStatus >>= 8;
      param = BankStatus & 0xffffffff;
    }
    return parsed;
}

void tN2kRegisters::parseN2kMsessages() {
  N2K->ParseMessages();
}

void tN2kRegisters::sendN2kMsg(const tN2kMsg &N2kMsg) {
  N2K->SendMsg(N2kMsg);
}

uint16_t tN2kRegisters::readEEPROM32b(int addr) {
  int32_t b1 = EEPROM.read(addr);
  int32_t b2 = EEPROM.read(addr + 1);
  int32_t b3 = EEPROM.read(addr + 2);
  int32_t b4 = EEPROM.read(addr + 3);
  return b1 << 24 | b2 << 16 |  b3 << 8 | b4;
}

void tN2kRegisters::writeEEPROM32b(int addr, int32_t val) {
  EEPROM.write(addr, val >> 24);
  EEPROM.write(addr + 1, val >> 16 & 0xFF);
  EEPROM.write(addr + 2, val >> 8 & 0xFF);
  EEPROM.write(addr + 3, val & 0xFF);
}

int tN2kRegisters::getRegisterIndex(unsigned char registerId) {
  for(int i = 0; i < registerCount; i++) {
    if (registers[i] == registerId) {
      return i;
    }
  }
  return -1;
}

int32_t tN2kRegisters::getRegisterValue(unsigned char registerId) {
  int idx = getRegisterIndex(registerId);
  if (idx >= 0) {
    return registerValues[idx];
  }
  return -1;
}

void tN2kRegisters::saveRegistersToEEPROM() {
  for (int i = 0; i < registerCount; i++) {
    writeEEPROM32b(2*i + 2+registerCount, registerValues[i]);
  }
  #ifdef PSEUDO_EEPROM
    EEPROM.commit();
  #endif
}

void tN2kRegisters::readRegistersFromEEPROM() {
  for (int i = 0; i < registerCount; i++) {
    registerValues[i] = readEEPROM32b(2*i + 2+registerCount);
  }
  for (int i = 0; i < registerCount; i++) {
    handleRegisterChange(i);
  }
}
