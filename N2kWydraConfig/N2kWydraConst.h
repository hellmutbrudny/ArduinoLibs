#ifndef _N2kWydraConst_h_
#define _N2kWydraConst_h_

/////
// Senders/Devices

#define N2KNODE_INDOOR_CONTROLLER 200
#define N2KNODE_OUTDOOR_CONTROLLER 201
#define N2KNODE_RUDDER_SENSOR 202
#define N2KNODE_ENGINE_DRIVER 203
#define N2KNODE_AUTOPILOT 204
#define N2KNODE_BMS 205
#define N2KNODE_POWER_MONITOR 206
#define N2KNODE_DATA_COLLECTOR 207
#define N2KNODE_GYROWEATHER_SENSOR 208

/////
// Registers
// By defalt they store configuration value in EEPROM
// 128+ means it's not persisted - constantly changing operation data

// N2KNODE_RUDDER_SENSOR
#define N2KREG_RudderMsgFreq 3 // ms

// N2KNODE_ENGINE_DRIVER
#define N2KREG_SteppersSpeed 10 // steps per second
#define N2KREG_GearIdlePos 11 // set by N2KRC_SetRegisterWithCurrent
#define N2KREG_GearReversePos 12 // set by N2KRC_SetRegisterWithCurrent
#define N2KREG_ThrottleMaxPos 14 // set by N2KRC_SetRegisterWithCurrent
#define N2KREG_EngineReportFreq 15 // ms
#define N2KREG_Gear (128+16) // 0-Idle, 1-Forward, -1-Reverse - read-only
#define N2KREG_SetThrottle (128+17) // speed -100..0..100
#define N2KREG_EngineModuleState (128+18) // 0 - ok, 1 - failed to reset throttle stepper, 2 - failed to reset gear stepper

// N2KNODE_INDOOR_CONTROLLER
// N2KNODE_OUTDOOR_CONTROLLER
#define N2KREG_CtrlAutopilotStep 20 // 1-5
#define N2KREG_CtrlThrottleStep 21 // 1-10

// N2KNODE_AUTOPILOT
#define N2KREG_SetAutopilotState (128+30) // 0-off, 1-on compass, 2-on wind, 3-stalled
#define N2KREG_SetAutopilotTurnSpeed (128+31) // turnSpeed 0-default, 1-slow, 2-fast
#define N2KREG_SetAutopilotRudderPosition (128+32) // rudder position to be set -100..0..100
#define N2KREG_SetAutopilotCourse (128+33) // course:0..359

// N2KNODE_BMS
#define N2KREG_BMSCutOffVoltage 40 // arg 1: (x1000)
#define N2KREG_BMSTurnOnVoltage 41 // arg 1: (x1000)
#define N2KREG_BMSAlertLowVoltage 42 // arg 1: (x1000)
#define N2KREG_BMSMsgFreq 43 // ms
#define N2KREG_BMSFactor0 44 // (x1000)
#define N2KREG_BMSFactor1 45 // (x1000)
#define N2KREG_BMSFactor2 46 // (x1000)
#define N2KREG_BMSFactor3 47 // (x1000)
#define N2KREG_BMSMode (128+40) // arg 1: 0-disabled, 1-enabled, 2-disabled manually, 3-enabled manually
#define N2KREG_BMSError (128+41) // arg 1: error code 0-ok, 1-underflow, 2-overflow, 3-i2c, 4-in progress, 5-timeout, 10-low voltage alert
#define N2KREG_BMSRaw0 (128+42)
#define N2KREG_BMSRaw1 (128+43)
#define N2KREG_BMSRaw2 (128+44)
#define N2KREG_BMSRaw3 (128+45)

// N2KNODE_POWER_MONITOR
#define N2KREG_PowerMsgFreq 50 // ms
#define N2KREG_TankMsgFreq 51 // ms
#define N2KREG_Tank1Min 52 // raw ADC value - set by N2KRC_SetRegisterWithCurrent
#define N2KREG_Tank1Max 53 // raw ADC value - set by N2KRC_SetRegisterWithCurrent
#define N2KREG_Tank2Min 54 // raw ADC value - set by N2KRC_SetRegisterWithCurrent
#define N2KREG_Tank2Max 55 // raw ADC value - set by N2KRC_SetRegisterWithCurrent
#define N2KREG_Tank3Min 56 // raw ADC value - set by N2KRC_SetRegisterWithCurrent
#define N2KREG_Tank3Max 57 // raw ADC value - set by N2KRC_SetRegisterWithCurrent
#define N2KREG_PowerConsumption (128+58) // 0.1A/10s
#define N2KREG_PowerGeneration (128+59) // 0.1A/10s

// N2KNODE_DATA_COLLECTOR

// N2KNODE_GYROWEATHER_SENSOR
#define N2KREG_RateOfTurnMsgFreq 80 // ms
#define N2KREG_EnvironmentMsgFreq 81 // ms
#define N2KREG_WindTempMsgFreq 82 // ms
#define N2KREG_GyroMsgFreq 83 // ms
#define N2KREG_WindMsgFreq 84 // ms
#define N2KREG_GPSPositionMsgFreq 85 // ms
#define N2KREG_GPSCourseMsgFreq 86 // ms
#define N2KREG_MagneticHeadingMsgFreq 87 // ms

#endif
