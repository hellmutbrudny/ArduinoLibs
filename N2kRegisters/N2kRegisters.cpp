#include <N2kRegisters.h>
#include <N2kMessages.h>
#include <NMEA2000.h>
#include <EEPROM.h>

#ifdef ARDUINO_ARCH_RP2040
#define PSEUDO_EEPROM
#endif

void(* rebootTheBoard) (void) = 0;

void tN2kRegisters::initN2kRegisters(const char *modelSerialCode,
                                      uint16_t registration,
                                      const char *deviceName,
                                      unsigned long deviceSerial,
                                      unsigned char deviceFunction,
                                      unsigned char deviceClass,
                                      uint8_t busSource,
                                      void (*msgHandler)(const tN2kMsg &N2kMsg)) {
  if (Serial) Serial.println("initN2kRegisters");
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

  //Serial.printf("registerCount=%d\n", registerCount);
  if (registerCount == 0) {
    //Serial.println("registerCount == 0");
    return;
  }

  #ifdef PSEUDO_EEPROM
    EEPROM.begin(256);
  #endif

  //Serial.println("init EEPROM");
  // Check if EEPROM structure is relevant to Registers
  byte check0 = EEPROM.read(0);
  byte check1 = EEPROM.read(1);
  if (check0 == 'X' && check1 == registerCount) {
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

  if (Serial) Serial.println("save defaults to EEPROM");
  //Serial.printf("Init registers to defaults. registerCount=%d\n", registerCount);
  // Save default values to EEPROM (initialization)
  EEPROM.write(0, 'X');
  EEPROM.write(1, registerCount);
  for (int i = 0; i < registerCount; i++) {
    EEPROM.write(i+2, registers[i]);
  }
  saveRegistersToEEPROM();
  // read just written for sanity and update handling
  readRegistersFromEEPROM();
}


void tN2kRegisters::handleN2kRegisterCommand(const tN2kMsg &N2kMsg) {
  //Serial.printf("Received PGN %d\n", N2kMsg.PGN);
  if (N2kMsg.PGN == 127501) {
    unsigned char p_command;
    unsigned char p_registerId;
    int32_t p_param;
    if (parseN2kRegisterCommand(N2kMsg, p_command, p_registerId, p_param)) {
      //Serial.printf("register command parsed %d->%d\n", p_command, p_registerId);
      if (p_command == N2KRC_RegisterValueInfo) {
        handleRegisterValueInfo(p_registerId, p_param);
        return;
      }
      int idx = getRegisterIndex(p_registerId);
      if (idx >= 0) {
        switch (p_command) {
          case N2KRC_GetRegister:
            handleGetRegister(p_registerId, idx);
            return;
          case N2KRC_SetRegister:
            handleSetRegister(p_registerId, p_param, idx);
            return;
          case N2KRC_SetRegisterBySensor:
            handleSetRegisterBySensor(p_registerId, idx);
            return;
          default:
            break;
        }
      }
    }
  }
  // handle other N2k messages - override virtual method if needed
  handleOtherN2kMsg(N2kMsg);
}

void tN2kRegisters::handleGetRegister(unsigned char registerId, int idx) {
  sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
}

void tN2kRegisters::handleSetRegister(unsigned char registerId, int32_t value, int idx) {
  if (registerId < 128 && registerValues[idx] != value) {
    // Hihgest bit means that given register is for constantly changing operational data and is not persisted in EEPROM
    writeEEPROM32b(4*idx + 2+registerCount, value);
    #ifdef PSEUDO_EEPROM
    EEPROM.commit();
    #endif
  }
  registerValues[idx] = value;
  handleRegisterChange(registerId, registerValues[idx]);
  sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
}

void tN2kRegisters::handleSetRegisterBySensor(unsigned char registerId, int idx) {
  int32_t sensor = sensorValueForRegister(registerId);
  if (sensor > -1) {
    if (registerValues[idx] != sensor) {
      registerValues[idx] = sensor;
      handleRegisterChange(registerId, registerValues[idx]);
      writeEEPROM32b(4*idx + 2+registerCount, registerValues[idx]);
      #ifdef PSEUDO_EEPROM
      EEPROM.commit();
      #endif
    }
    sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
  }
}

void tN2kRegisters::setN2kRegisterCommand(tN2kMsg &N2kMsg, unsigned char command, unsigned char registerId, int32_t param) {
  N2kMsg.SetPGN(127501L);
  N2kMsg.Priority = 3;
	tN2kBinaryStatus BankStatus = param;
	BankStatus <<= 8;
	BankStatus |= registerId;
	BankStatus <<= 8;
	BankStatus |= command;
	N2kMsg.AddUInt64(BankStatus);
}

void tN2kRegisters::sendN2kRegisterCommand(unsigned char command, unsigned char registerId, int32_t param) {
  tN2kMsg N2kMsg;
  //Serial.printf("sending command %d: %d=%d\n", command, registerId, param);
  setN2kRegisterCommand(N2kMsg, command, registerId, param);
  sendN2kMsg(N2kMsg);
}

bool tN2kRegisters::parseN2kRegisterCommand(const tN2kMsg &N2kMsg, unsigned char &command, unsigned char &registerId, int32_t &param) {
    tN2kBinaryStatus BankStatus;
    int Index=0;
    BankStatus=N2kMsg.GetUInt64(Index);
    unsigned char *buf = (unsigned char *)&BankStatus;
    command = *buf;
    registerId = *(buf+1);
    param = int32_t(BankStatus >> 16);
    return true;
}

void tN2kRegisters::parseN2kMessages() {
  N2K->ParseMessages();
  if (++parseCounter >= 1000) {
    parseCounter = 0;
    notSendCounter = 0;
  }
}

void tN2kRegisters::sendN2kMsg(const tN2kMsg &N2kMsg) {
  bool res = N2K->SendMsg(N2kMsg);
  if (!res && millis() > 5000) {
    // Do not count errors for the first 5s - give device time to start connection
    if (Serial) {
        Serial.print(millis());
        Serial.print(" Send error: ");
        Serial.println(notSendCounter);
    }
    if (++notSendCounter >= 10) {
        if (Serial) Serial.println("Too many send errors - reboot!!!");
        rebootTheBoard();    
    }
  }
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

bool tN2kRegisters::setRegisterValue(unsigned char registerId, int32_t value) {
  int idx = getRegisterIndex(registerId);
  if (idx >= 0 && registerValues[idx] != value) {
    if (registerId < 128 && registerValues[idx] != value) {
      // Hihgest bit means that given register is for constantly changing operational data and is not persisted in EEPROM
      writeEEPROM32b(4*idx + 2+registerCount, value);
      #ifdef PSEUDO_EEPROM
      EEPROM.commit();
      #endif
    }
    registerValues[idx] = value;
    sendN2kRegisterCommand(N2KRC_RegisterValueInfo, registerId, registerValues[idx]);
    return true;
  }
  return false;
}

void tN2kRegisters::setRegisterRawValue(int idx, int32_t value) {
  if (idx >= 0 && registerValues[idx] != value) {
    registerValues[idx] = value;
  }
}

void tN2kRegisters::saveRegistersToEEPROM() {
  if (registerCount == 0) {
    return;
  }
  for (int i = 0; i < registerCount; i++) {
    if (registers[i] < 128) {
      int addr = 4*i + 2+registerCount;
      int32_t current = readEEPROM32b(addr);
      if (current != registerValues[i]) {
        //Serial.printf("EEPROM write %d:%d->%d\n", i, addr, registerValues[i]);
        writeEEPROM32b(addr, registerValues[i]);
      }
    }
  }
  #ifdef PSEUDO_EEPROM
  //Serial.printf("EEPROM.commit\n");
  EEPROM.commit();
  #endif
}

void tN2kRegisters::readRegistersFromEEPROM() {
  //Serial.println("readRegistersFromEEPROM");
  for (int i = 0; i < registerCount; i++) {
    if (registers[i] < 128) {
      int addr = 4*i + 2+registerCount;
      registerValues[i] = readEEPROM32b(addr);
      //Serial.printf("EEPROM read %d:%d=%d\n", i, addr, registerValues[i]);
    }
  }
  for (int i = 0; i < registerCount; i++) {
    if (registers[i] < 128) {
      handleRegisterChange(registers[i], registerValues[i]);
    }
  }
}
