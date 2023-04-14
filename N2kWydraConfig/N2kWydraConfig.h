#ifndef _N2kWydraConfig_h_
#define _N2kWydraConfig_h_

#include <N2kWydraConst.h>
#include <N2kRegisters.h>
#include <N2kMessages.h>
#include <NMEA2000_CAN.h>

// To be defined on the device code level
// #define N2KNODE_DEVICE

// Common values
const char * N2K_DEVICE_MODEL_SERIAL = "666";
const uint16_t N2K_DEVICE_REGISTRATION = 666; // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf

#if N2KNODE_DEVICE == N2KNODE_RUDDER_SENSOR
  // Arduino Nano Every
  const char * N2K_DEVICE_NAME = "Wydra Rudder Sensor"; // Max 33 chars.
  const unsigned long N2K_DEVICE_SERIAL = 9000; // Unique, 21 bit resolution, max 2097151. Each device from same manufacturer should have unique number.
  const unsigned char N2K_DEVICE_FUNCTION = 155;
  const unsigned char N2K_DEVICE_CLASS = 40;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_RudderMaxPort, N2KREG_RudderMaxSter, N2KREG_RudderMsgFreq, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {80, 485, 1000};
  #define OVER_handleRegisterChange
  #define OVER_sensorValueForReg
#elif N2KNODE_DEVICE == N2KNODE_POWER_MONITOR
  // Raspberry Pi Pico
  const char * N2K_DEVICE_NAME = "Wydra Power Monitor";
  const unsigned long N2K_DEVICE_SERIAL = 9001;
  const unsigned char N2K_DEVICE_FUNCTION = 150;
  const unsigned char N2K_DEVICE_CLASS = 75;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_PowerMsgFreq, N2KREG_TankMsgFreq, N2KREG_Tank1Min, N2KREG_Tank1Max, N2KREG_Tank2Min, N2KREG_Tank2Max, N2KREG_Tank3Min, N2KREG_Tank3Max, N2KREG_PowerConsumption, N2KREG_PowerGeneration, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {1000, 10000, 0, 0, 0, 0, 0, 0, 0, 0};
  #define OVER_handleRegisterChange
  #define OVER_sensorValueForReg
#elif N2KNODE_DEVICE == N2KNODE_ENGINE_DRIVER
  // Raspberry Pi Pico
  const char * N2K_DEVICE_NAME = "Wydra Engine Driver";
  const unsigned long N2K_DEVICE_SERIAL = 9002;
  const unsigned char N2K_DEVICE_FUNCTION = 140;
  const unsigned char N2K_DEVICE_CLASS = 50;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_SteppersSpeed, N2KREG_GearIdlePos, N2KREG_GearReversePos, N2KREG_ThrottleMaxPos, N2KREG_EngineReportFreq, N2KREG_Gear, N2KREG_SetThrottle, N2KREG_EngineModuleState, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {200, 750, 1500, 2500, 4000, 0, 0, 0};
  #define OVER_handleRegisterChange
  #define OVER_sensorValueForReg
#elif N2KNODE_DEVICE == N2KNODE_AUTOPILOT
  // CANBed - Arduino CAN-Bus RP2040
  const char * N2K_DEVICE_NAME = "Wydra Autopilot Driver";
  const unsigned long N2K_DEVICE_SERIAL = 9003;
  const unsigned char N2K_DEVICE_FUNCTION = 150;
  const unsigned char N2K_DEVICE_CLASS = 40;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_AutopilotCalibrationP, N2KREG_AutopilotCalibrationI, N2KREG_AutopilotCalibrationD, N2KREG_SetAutopilotState, N2KREG_SetAutopilotCourse, N2KREG_SetAutopilotTurnSpeed, N2KREG_SetAutopilotRudderPosition, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {1000, 1000, 1000, 0, 0, 0, 0};
  #define OVER_handleRegisterChange
  #define OVER_handleOtherN2kMsg
#elif N2KNODE_DEVICE == N2KNODE_BMS
  // CANBed - Arduino CAN-Bus RP2040
  const char * N2K_DEVICE_NAME = "Wydra BMS";
  const unsigned long N2K_DEVICE_SERIAL = 9004;
  const unsigned char N2K_DEVICE_FUNCTION = 170;
  const unsigned char N2K_DEVICE_CLASS = 35;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_BMSCutOffVoltage, N2KREG_BMSTurnOnVoltage, N2KREG_BMSAlertLowVoltage, N2KREG_BMSMsgFreq, N2KREG_BMSFactor0, N2KREG_BMSFactor1, N2KREG_BMSFactor2, N2KREG_BMSFactor3, N2KREG_BMSMode, N2KREG_BMSError, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {3600, 3370, 3225, 5000, 2000, 4000, 6000, 8000, 0, 0};
  #define OVER_handleRegisterChange
#elif N2KNODE_DEVICE == N2KNODE_GYROWEATHER_SENSOR
  // Teensy 3.2
  const char * N2K_DEVICE_NAME = "Wydra GyroWeather Sensor";
  const unsigned long N2K_DEVICE_SERIAL = 9005;
  const unsigned char N2K_DEVICE_FUNCTION = 130;
  const unsigned char N2K_DEVICE_CLASS = 85;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_RateOfTurnMsgFreq, N2KREG_EnvironmentMsgFreq, N2KREG_WindTempMsgFreq, N2KREG_GyroMsgFreq, N2KREG_WindMsgFreq, N2KREG_GPSPositionMsgFreq, N2KREG_GPSCourseMsgFreq, N2KREG_MagneticHeadingMsgFreq, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {100, 5000, 500, 1000, 1000, 1000, 1000, 10000};
  #define OVER_handleRegisterChange
#elif N2KNODE_DEVICE == N2KNODE_INDOOR_CONTROLLER || N2KNODE_DEVICE == N2KNODE_OUTDOOR_CONTROLLER
  // Teensy 4.0
  #if N2KNODE_DEVICE == N2KNODE_INDOOR_CONTROLLER
    const char * N2K_DEVICE_NAME = "Wydra Indoor Controller";
    const unsigned long N2K_DEVICE_SERIAL = 9006;
  #else
    const char * N2K_DEVICE_NAME = "Wydra Outdoor Controller";
    const unsigned long N2K_DEVICE_SERIAL = 9007;
  #endif
  const unsigned char N2K_DEVICE_FUNCTION = 130;
  const unsigned char N2K_DEVICE_CLASS = 120;
  unsigned char REGISTERS_ARRAY[] = {N2KREG_CtrlAutopilotStep, N2KREG_CtrlThrottleStep, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {1, 10};
  #define OVER_handleOtherN2kMsg
  #define OVER_handleRegisterChange
  #define OVER_handleRegisterValueInfo
#elif N2KNODE_DEVICE == N2KNODE_DATA_COLLECTOR
  // Raspberry Pi CM4
  const char * N2K_DEVICE_NAME = "Wydra Data Collector";
  const unsigned long N2K_DEVICE_SERIAL = 9008;
  const unsigned char N2K_DEVICE_FUNCTION = 140;
  const unsigned char N2K_DEVICE_CLASS = 20;
  unsigned char REGISTERS_ARRAY[] = {0};
  int32_t REGISTER_VALUES_ARRAY[0];
  #define OVER_handleOtherN2kMsg
  #define OVER_handleRegisterValueInfo
#endif

class OverN2kRegisters : public tN2kRegisters {
public:
  OverN2kRegisters(tNMEA2000 *N2K, unsigned char *registers, int32_t *initialValues) : tN2kRegisters(N2K, registers, initialValues) {};

#ifdef OVER_handleRegisterValueInfo
  virtual void handleRegisterValueInfo(unsigned char registerId, int32_t param);
#endif
#ifdef OVER_handleOtherN2kMsg
  virtual void handleOtherN2kMsg(const tN2kMsg &N2kMsg);
#endif
#ifdef OVER_handleRegisterChange
  virtual void handleRegisterChange(unsigned char registerId, int32_t newValue);
#endif
#ifdef OVER_sensorValueForReg
  virtual int32_t sensorValueForRegister(unsigned char registerId);
#endif
};

OverN2kRegisters N2kRegisters(&NMEA2000, REGISTERS_ARRAY, REGISTER_VALUES_ARRAY);

void handleN2kMessage(const tN2kMsg &N2kMsg) {
  //Serial.printf("handleN2kMessage PGN=%d SOURCE=%d\n", N2kMsg.PGN, N2kMsg.Source);
  N2kRegisters.handleN2kRegisterCommand(N2kMsg);
}

void parseN2kMessages() {
  N2kRegisters.parseN2kMessages();
}

void initRegisters() {
  N2kRegisters.initN2kRegisters(N2K_DEVICE_MODEL_SERIAL,
               N2K_DEVICE_REGISTRATION,
               N2K_DEVICE_NAME,
               N2K_DEVICE_SERIAL,
               N2K_DEVICE_FUNCTION,
               N2K_DEVICE_CLASS,
               N2KNODE_DEVICE,
               handleN2kMessage);
}

#ifdef ARDUINO_ARCH_RP2040
void switchNMEAToSPI1(int rx, int tx, int sck, int cs) {
  tNMEA2000_mcp* N2kMCP = (tNMEA2000_mcp*)&NMEA2000;
  N2kMCP->SetSPI(&SPI1);
  SPI1.setCS(cs);
  SPI1.setRX(rx);
  SPI1.setTX(tx);
  SPI1.setSCK(sck);
}
#endif

#endif
