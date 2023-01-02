// To be defined on the device code level
// #define N2KNODE_DEVICE

#define N2K_DEVICE_REGISTRATION 666 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
#if N2KNODE_DEVICE == N2KNODE_RUDDER_SENSOR
  // Arduino Nano Every
  #define N2K_DEVICE_NAME "Whocares Rudder Sensor" // Max 33 chars.
  #define N2K_DEVICE_VERSION "1.0 (2021-12-02)" // Max 40 chars
  #define N2K_DEVICE_SERIAL 951213 // Unique, 21 bit resolution, max 2097151. Each device from same manufacturer should have unique number.
  #define N2K_DEVICE_FUNCTION 155
  #define N2K_DEVICE_CLASS 40
  #define N2K_MSG_HANDLING 0
  #define REGISTER_COUNT 3
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={127245L,0};
  uint16_t registers[] = {N2KREG_RudderMaxPort, N2KREG_RudderMaxSter, N2KREG_RudderMsgFreq};
  uint16_t registerValues[] = {80, 485, 1000};
  // Virtual
  void handleRegChange(int idx);
  uint16_t sensorValueForReg(int idx);
#elif N2KNODE_DEVICE == N2KNODE_POWER_MONITOR
  // Arduino Due
  #define N2K_DEVICE_NAME "Whocares Power Monitor"
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_SERIAL 011213
  #define N2K_DEVICE_FUNCTION 150
  #define N2K_DEVICE_CLASS 75
  #define N2K_MSG_HANDLING 0
  #define REGISTER_COUNT 1
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={0};
  uint16_t registers[] = {N2KREG_PMMsgFreq};
  uint16_t registerValues[] = {1000};
  // Virtual
  void handleRegChange(int idx);
  uint16_t sensorValueForReg(int idx) {return 0;}
#elif N2KNODE_DEVICE == N2KNODE_ENGINE_DRIVER
  // Raspberry Pi Pico
  #define N2K_DEVICE_NAME "Whocares Engine Driver"
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_SERIAL 961213
  #define N2K_DEVICE_FUNCTION 140
  #define N2K_DEVICE_CLASS 50
  #define PSEUDO_EEPROM 1
  #define REGISTER_COUNT 6
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={130666L,0};
  uint16_t registers[] = {N2KREG_GearForward, N2KREG_GearIdle, N2KREG_GearReverse, N2KREG_ThrottleMin, N2KREG_ThrottleMax, N2KREG_EngineReportFreq};
  uint16_t registerValues[] = {0, 0, 0, 0, 0, 1500};
  // Virtual
  #define N2K_MSG_HANDLING 1
  void handleN2kMsg(const tN2kMsg &N2kMsg);
  void handleRegChange(int idx);
  uint16_t sensorValueForReg(int idx);
#elif N2KNODE_DEVICE == N2KNODE_AUTOPILOT
  // CANBed - Arduino CAN-Bus RP2040
  #define N2K_DEVICE_NAME "Whocares Autopilot"
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_SERIAL 991213
  #define N2K_DEVICE_FUNCTION 150
  #define N2K_DEVICE_CLASS 40
  #define REGISTER_COUNT 0
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={0};
  // Virtual
  #define N2K_MSG_HANDLING 1
  void handleN2kMsg(const tN2kMsg &N2kMsg);
#elif N2KNODE_DEVICE == N2KNODE_BMS
  // CANBed - Arduino CAN-Bus RP2040
  #define N2K_DEVICE_NAME "Whocares BMS"
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_SERIAL 001213
  #define N2K_DEVICE_FUNCTION 170
  #define N2K_DEVICE_CLASS 35
  #define N2K_MSG_HANDLING 0
  #define PSEUDO_EEPROM 1
  #define REGISTER_COUNT 3
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={0};
  uint16_t registers[] = {N2KREG_BMSCutOffVoltage, N2KREG_BMSTurnOnVoltage, N2KREG_BMSMsgFreq};
  uint16_t registerValues[] = {365, 350, 5000};
  // Virtual
  void handleRegChange(int idx);
  uint16_t sensorValueForReg(int idx) {return 0;}
#elif N2KNODE_DEVICE == N2KNODE_GYROWEATHER_SENSOR
  // Teensy 3.2
  #define N2K_DEVICE_NAME "Whocares GyroWeather Sensor"
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_SERIAL 031213
  #define N2K_DEVICE_FUNCTION 130
  #define N2K_DEVICE_CLASS 85
  #define N2K_MSG_HANDLING 0
  #define REGISTER_COUNT 8
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={0};
  uint16_t registers[] = {N2KREG_RateOfTurnMsgFreq, N2KREG_EnvironmentMsgFreq, N2KREG_WindTempMsgFreq, N2KREG_GyroMsgFreq, N2KREG_WindMsgFreq, N2KREG_GPSPositionMsgFreq, N2KREG_GPSCourseMsgFreq, N2KREG_MagneticHeadingMsgFreq};
  uint16_t registerValues[] = {100, 5000, 500, 1000, 1000, 1000, 1000, 10000};
  // Virtual
  void handleRegChange(int idx);
  uint16_t sensorValueForReg(int idx) {return 0;}
#elif N2KNODE_DEVICE == N2KNODE_INDOOR_CONTROLLER || N2KNODE_DEVICE == N2KNODE_OUTDOOR_CONTROLLER
  // Teensy 4.0
  #if N2KNODE_DEVICE == N2KNODE_INDOOR_CONTROLLER
    #define N2K_DEVICE_NAME "Whocares Indoor Controller"
    #define N2K_DEVICE_SERIAL 971213
  #else
    #define N2K_DEVICE_NAME "Whocares Outdoor Controller"
    #define N2K_DEVICE_SERIAL 981213
  #endif
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_FUNCTION 130
  #define N2K_DEVICE_CLASS 120
  #define REGISTER_COUNT 2
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={128275L, 0};
  uint16_t registers[] = {N2KREG_CtrlAutopilotStep, N2KREG_CtrlThrottleStep};
  uint16_t registerValues[] = {1, 10};
  // Virtual
  #define N2K_MSG_HANDLING 1
  void handleN2kMsg(const tN2kMsg &N2kMsg);
  void handleRegChange(int idx);
  uint16_t sensorValueForReg(int idx) {return 0;}
#elif N2KNODE_DEVICE == N2KNODE_DATA_COLLECTOR
  // Raspberry Pi CM4
  #define N2K_DEVICE_NAME "Whocares Data Collector"
  #define N2K_DEVICE_VERSION "1.0"
  #define N2K_DEVICE_SERIAL 021213
  #define N2K_DEVICE_FUNCTION 140
  #define N2K_DEVICE_CLASS 20
  #define REGISTER_COUNT 0
  const unsigned long N2K_DEVICE_TRANSMIT_MESSAGES[] PROGMEM={0};
  // Virtual
  #define N2K_MSG_HANDLING 1
  void handleN2kMsg(const tN2kMsg &N2kMsg);
#endif
