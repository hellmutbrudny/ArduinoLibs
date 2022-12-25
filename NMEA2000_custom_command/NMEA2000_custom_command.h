#ifndef NMEA2000CustomCommand_h
#define NMEA2000CustomCommand_h

#include <N2kMessages.h>
#include <NMEA2000_CAN.h>
#include "NMEA2000_custom_command_devices.h"

//************
// Register handling
#if REGISTER_COUNT > 0
#include <EEPROM.h>

void sendCustomCommand(double command, uint32_t param1, uint32_t param2);

bool parseCustomCommand(const tN2kMsg &N2kMsg, uint16_t &sender, double &command, uint32_t &param1, uint32_t &param2);

uint16_t readEEPROM16b(int addr) {
  return (((uint16_t)EEPROM.read(addr))<<8) && ((uint16_t)EEPROM.read(addr + 1));
}

void writeEEPROM16b(int addr, uint16_t val) {
  EEPROM.write(addr, val >> 8);
  EEPROM.write(addr + 1, val & 0xFF);
}

int getRegisterIndex(uint16_t reg) {
  for(int i = 0; i < REGISTER_COUNT; i++) {
    if (registers[i] == reg) {
      return i;
    }
  }
  return -1;
}

void handleN2kRegisters(const tN2kMsg &N2kMsg) {
  if (N2kMsg.PGN == 128275) {
    uint16_t sender;
    double command;
    uint32_t param1;
    uint32_t param2;
    int idx;
    parseCustomCommand(N2kMsg, sender, command, param1, param2);
    switch ((int)command) {
      case N2KCC_SetRegister:
        idx = getRegisterIndex(param1);
        if (idx >= 0) {
          registerValues[idx] = param2;
          handleRegChange(idx);
          writeEEPROM16b(2*(idx+1+REGISTER_COUNT), registerValues[idx]);
          #ifdef PSEUDO_EEPROM
          EEPROM.commit();
          #endif
        }
        return;
      case N2KCC_SetRegisterWithCurrent:
        idx = getRegisterIndex(param1);
        if (idx >= 0) {
          registerValues[idx] = sensorValueForReg(param1);
          writeEEPROM16b(2*(idx+1+REGISTER_COUNT), registerValues[idx]);
          #ifdef PSEUDO_EEPROM
          EEPROM.commit();
          #endif
        }
        return;
      case N2KCC_GetRegister:
        idx = getRegisterIndex(param1);
        if (idx >= 0) {
          sendCustomCommand(N2KCC_RegisterValue, param1, registerValues[idx]);
        }
        return;
      default:
        break;
    }
  }
  #if N2K_MSG_HANDLING == 1
  handleN2kMsg(N2kMsg);
  #endif
}

void saveRegistersToEEPROM() {
  for (int i = 0; i < REGISTER_COUNT; i++) {
    writeEEPROM16b(2*(i+1+REGISTER_COUNT), registerValues[i]);
  }
  #ifdef PSEUDO_EEPROM
    EEPROM.commit();
  #endif
}

void readRegistersFromEEPROM() {
  for (int i = 0; i < REGISTER_COUNT; i++) {
    registerValues[i] = readEEPROM16b(2*(i+1+REGISTER_COUNT));
  }
}

void initRegisters() {
  #ifdef PSEUDO_EEPROM
    EEPROM.begin(256);
  #endif
  // Check if EEPROM structure is relevant to Registers
  byte check0 = EEPROM.read(0);
  byte check1 = EEPROM.read(1);
  if (check0 == 'W' && check1 == REGISTER_COUNT) {
    int count = 0;
    for (; count < REGISTER_COUNT; count++) {
      if (readEEPROM16b(2*(count+1)) != registers[count]) {
        break;
      }
    }
    if (REGISTER_COUNT == count) {
      readRegistersFromEEPROM();
      // Registers initialized from EEPROM
      return;
    }
  }
  // Save default values to EEPROM (initialization)
  EEPROM.write(0, 'W');
  EEPROM.write(1, REGISTER_COUNT);
  for (int i = 0; i < REGISTER_COUNT; i++) {
    writeEEPROM16b(2*(i+1), registers[i]);
  }
  saveRegistersToEEPROM();
}

#endif

//*****************************************************************************
// Custom Command

void initNMEA2k() {
  NMEA2000.SetProductInformation("0001", // Manufacturer's Model serial code
                                 666, // Manufacturer's product code
                                 N2K_DEVICE_NAME,  // Manufacturer's Model ID
                                 N2K_DEVICE_VERSION,  // Manufacturer's Software version code
                                 ""  // Manufacturer's Model version
                                 );
  NMEA2000.SetDeviceInformation(N2K_DEVICE_SERIAL,
                                N2K_DEVICE_FUNCTION,
                                N2K_DEVICE_CLASS,
                                N2K_DEVICE_REGISTRATION
                                );
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  // NMEA2000.SetForwardStream(&Serial);
  // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode, N2KNODE_DEVICE);
  NMEA2000.ExtendTransmitMessages(N2K_DEVICE_TRANSMIT_MESSAGES);
  NMEA2000.SetHeartbeatInterval(5000);
  #if REGISTER_COUNT > 0
  NMEA2000.SetMsgHandler(handleN2kRegisters);
  #else
  NMEA2000.SetMsgHandler(handleN2kMsg);
  #endif
  NMEA2000.Open();
}

void setCustomCommand(tN2kMsg &N2kMsg, double command, uint32_t param1, uint32_t param2) {
  SetN2kPGN128275(N2kMsg, N2KNODE_DEVICE, command, param1, param2);
}

void sendCustomCommand(double command, uint32_t param1, uint32_t param2) {
  tN2kMsg N2kMsg;
  setCustomCommand(N2kMsg, command, param1, param2);
  NMEA2000.SendMsg(N2kMsg);
}

bool parseCustomCommand(const tN2kMsg &N2kMsg, uint16_t &sender, double &command, uint32_t &param1, uint32_t &param2) {
    return ParseN2kPGN128275(N2kMsg, sender, command, param1, param2);
}

void parseN2kMsgs() {
  NMEA2000.ParseMessages();
}

#endif
