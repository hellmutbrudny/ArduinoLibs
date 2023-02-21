#ifndef _N2kWydraConfig_h_
#define _N2kWydraConfig_h_

#include <N2kWydraConst.h>

// To be defined on the device code level
// #define N2KNODE_DEVICE

// Common values
#define N2K_DEVICE_MODEL_SERIAL "666"
#define N2K_DEVICE_REGISTRATION 666 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf

#if N2KNODE_DEVICE == N2KNODE_RUDDER_SENSOR
  // Arduino Nano Every
  #define N2K_DEVICE_NAME "Wydra Rudder Sensor" // Max 33 chars.
  #define N2K_DEVICE_SERIAL 9000 // Unique, 21 bit resolution, max 2097151. Each device from same manufacturer should have unique number.
  #define N2K_DEVICE_FUNCTION 155
  #define N2K_DEVICE_CLASS 40
  unsigned char REGISTERS_ARRAY[] = {N2KREG_RudderMaxPort, N2KREG_RudderMaxSter, N2KREG_RudderMsgFreq, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {80, 485, 1000};
#elif N2KNODE_DEVICE == N2KNODE_POWER_MONITOR
  // Raspberry Pi Pico
  #define N2K_DEVICE_NAME "Wydra Power Monitor"
  #define N2K_DEVICE_SERIAL 9001
  #define N2K_DEVICE_FUNCTION 150
  #define N2K_DEVICE_CLASS 75
  unsigned char REGISTERS_ARRAY[] = {N2KREG_PowerMsgFreq, N2KREG_TankMsgFreq, N2KREG_Tank1Min, N2KREG_Tank1Max, N2KREG_Tank2Min, N2KREG_Tank2Max, N2KREG_Tank3Min, N2KREG_Tank3Max, N2KREG_PowerConsumption, N2KREG_PowerGeneration, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {1000, 10000, 0, 0, 0, 0, 0, 0, 0, 0};
#elif N2KNODE_DEVICE == N2KNODE_ENGINE_DRIVER
  // Raspberry Pi Pico
  #define N2K_DEVICE_NAME "Wydra Engine Driver"
  #define N2K_DEVICE_SERIAL 9002
  #define N2K_DEVICE_FUNCTION 140
  #define N2K_DEVICE_CLASS 50
  unsigned char REGISTERS_ARRAY[] = {N2KREG_GearForwardPos, N2KREG_GearIdlePos, N2KREG_GearReversePos, N2KREG_ThrottleMinPos, N2KREG_ThrottleMaxPos, N2KREG_EngineReportFreq, N2KREG_Throttle, N2KREG_Gear, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {0, 0, 0, 0, 0, 1500, 0, 0};
#elif N2KNODE_DEVICE == N2KNODE_AUTOPILOT
  // CANBed - Arduino CAN-Bus RP2040
  #define N2K_DEVICE_NAME "Wydra Autopilot"
  #define N2K_DEVICE_SERIAL 9003
  #define N2K_DEVICE_FUNCTION 150
  #define N2K_DEVICE_CLASS 40
  unsigned char REGISTERS_ARRAY[] = {N2KCC_SetAutopilotOn, N2KCC_SetAutopilotCourse, N2KCC_SetAutopilotTurnSpeed, N2KCC_SetAutopilotRudderPosition, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {0, 0, 0, 0};
#elif N2KNODE_DEVICE == N2KNODE_BMS
  // CANBed - Arduino CAN-Bus RP2040
  #define N2K_DEVICE_NAME "Wydra BMS"
  #define N2K_DEVICE_SERIAL 9004
  #define N2K_DEVICE_FUNCTION 170
  #define N2K_DEVICE_CLASS 35
  unsigned char REGISTERS_ARRAY[] = {N2KREG_BMSCutOffVoltage, N2KREG_BMSTurnOnVoltage, N2KREG_BMSMsgFreq, N2KNTF_BMSStatus, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {365, 350, 5000, 0};
#elif N2KNODE_DEVICE == N2KNODE_GYROWEATHER_SENSOR
  // Teensy 3.2
  #define N2K_DEVICE_NAME "Wydra GyroWeather Sensor"
  #define N2K_DEVICE_SERIAL 9005
  #define N2K_DEVICE_FUNCTION 130
  #define N2K_DEVICE_CLASS 85
  unsigned char REGISTERS_ARRAY[] = {N2KREG_RateOfTurnMsgFreq, N2KREG_EnvironmentMsgFreq, N2KREG_WindTempMsgFreq, N2KREG_GyroMsgFreq, N2KREG_WindMsgFreq, N2KREG_GPSPositionMsgFreq, N2KREG_GPSCourseMsgFreq, N2KREG_MagneticHeadingMsgFreq, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {100, 5000, 500, 1000, 1000, 1000, 1000, 10000};
#elif N2KNODE_DEVICE == N2KNODE_INDOOR_CONTROLLER || N2KNODE_DEVICE == N2KNODE_OUTDOOR_CONTROLLER
  // Teensy 4.0
  #if N2KNODE_DEVICE == N2KNODE_INDOOR_CONTROLLER
    #define N2K_DEVICE_NAME "Wydra Indoor Controller"
    #define N2K_DEVICE_SERIAL 9006
  #else
    #define N2K_DEVICE_NAME "Wydra Outdoor Controller"
    #define N2K_DEVICE_SERIAL 9007
  #endif
  #define N2K_DEVICE_FUNCTION 130
  #define N2K_DEVICE_CLASS 120
  unsigned char REGISTERS_ARRAY[] = {N2KREG_CtrlAutopilotStep, N2KREG_CtrlThrottleStep, 0};
  int32_t REGISTER_VALUES_ARRAY[] = {1, 10};
#elif N2KNODE_DEVICE == N2KNODE_DATA_COLLECTOR
  // Raspberry Pi CM4
  #define N2K_DEVICE_NAME "Wydra Data Collector"
  #define N2K_DEVICE_SERIAL 9008
  #define N2K_DEVICE_FUNCTION 140
  #define N2K_DEVICE_CLASS 20
  unsigned char REGISTERS_ARRAY[] = {0};
  int32_t REGISTER_VALUES_ARRAY[0];
#endif

#endif
