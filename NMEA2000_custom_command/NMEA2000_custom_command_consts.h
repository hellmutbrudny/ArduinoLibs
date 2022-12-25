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
// Commmands

#define N2KCC_SetRegister 0 // 2 args
#define N2KCC_GetRegister 1 // 1 arg
#define N2KCC_SetRegisterWithCurrent 2 // 1 arg
#define N2KCC_RegisterValue 3 // 2 args

#define N2KCC_Notify 5 // 2 args

#define N2KCC_SetEngineSpeed 10 // 1 arg: -100..100

#define N2KCC_SetAutopilotOn 14 // 1 arg: 0,1
#define N2KCC_SetAutopilotCourse 15 // 2 args: course:0..359, turnSpeed:[0-default, 1-slow, 2-fast]

/////
// Registers/Notifications - param1 - by owner

// N2KNODE_RUDDER_SENSOR
#define N2KREG_RudderMaxPort 0
#define N2KREG_RudderMaxSter 1
#define N2KREG_RudderMsgFreq 2

// N2KNODE_ENGINE_DRIVER
#define N2KREG_GearForward 100
#define N2KREG_GearIdle 101
#define N2KREG_GearReverse 102
#define N2KREG_ThrottleMin 103
#define N2KREG_ThrottleMax 104
#define N2KREG_EngineReportFreq 105

// N2KNODE_INDOOR_CONTROLLER
// N2KNODE_OUTDOOR_CONTROLLER
#define N2KREG_CtrlAutopilotStep 200
#define N2KREG_CtrlThrottleStep 201

// N2KNODE_AUTOPILOT

// N2KNODE_BMS
#define N2KREG_BMSCutOffVoltage 400 // *100
#define N2KREG_BMSTurnOnVoltage 401 // *100
#define N2KREG_BMSMsgFreq 402 //

// N2KNODE_POWER_MONITOR
#define N2KREG_PMMsgFreq 500 //

// N2KNODE_DATA_COLLECTOR
#define N2KREG_WiFiOn 600 //

// N2KNODE_GYROWEATHER_SENSOR
#define N2KREG_RateOfTurnMsgFreq 800
#define N2KREG_EnvironmentMsgFreq 801
#define N2KREG_WindTempMsgFreq 802
#define N2KREG_GyroMsgFreq 803
#define N2KREG_WindMsgFreq 804
#define N2KREG_GPSPositionMsgFreq 805
#define N2KREG_GPSCourseMsgFreq 806
#define N2KREG_MagneticHeadingMsgFreq 807

#define N2KNTF_LuffingUpSpeed 880
#define N2KNTF_BearingOffSpeed 881
#define N2KNTF_WaveLength 882
#define N2KNTF_OffCourse 883
#define N2KNTF_PositionOnWave 884
