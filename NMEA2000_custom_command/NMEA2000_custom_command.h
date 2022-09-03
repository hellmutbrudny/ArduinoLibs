#ifndef NMEA2000CustomCommand_h
#define NMEA2000CustomCommand_h

#include "Arduino.h"
#include <N2kMessages.h>

                          
enum tN2kCustomCommand {
  N2kCC_SetRudderZero=0, // 0 args
  N2kCC_SetRudderMaxStarboard=1, // 0 args
  N2kCC_SetRudderMaxPort=2, // 0 args
  N2kCC_SetRudderMsgFreq=3, // 1 arg
  N2kCC_SetGyroMsgFreq=4, // 1 arg
  N2kCC_SetWindMsgFreq=5, // 1 arg
  N2kCC_NotifyLuffingUpSpeed=6, // 1 arg
  N2kCC_NotifyBearingOffSpeed=7, // 1 arg
  N2kCC_NotifyWaveLengthAndHeight=8, // 2 args
  N2kCC_NotifyOffCourse=9, // 1 arg
  N2kCC_NotifyPositionOnWave=10, // 1 arg
  N2kCC_EngineHeartbeat=11, // 0 args
  N2kCC_AutopilotHeartbeat=12, // 0 args
  N2kCC_SetEngineSpeed=13, // 1 arg: -100..100
  N2kCC_AutopilotOn=14, // 1 arg: 0,1
  N2kCC_AutopilotCourse=15, // 2 args: course:0..359, turnSpeed:[0-default, 1-slow, 2-normal, 3-fast]
}; 

void SetN2kCustomCommand(tN2kMsg &N2kMsg, char *DeviceID, tN2kCustomCommand command);

bool ParseN2kCustomCommand(const tN2kMsg &N2kMsg, char *DeviceID, tN2kCustomCommand &command);

void SetN2kCustomCommandTwoInts(tN2kMsg &N2kMsg, char *DeviceID, tN2kCustomCommand command, int16_t parameterA=0, int16_t parameterB=0);

bool ParseN2kCustomCommandTwoInts(const tN2kMsg &N2kMsg, char *DeviceID, tN2kCustomCommand &command, int16_t &parameterA, int16_t &parameterB);

void SetN2kCustomCommandInt(tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand command, int16_t parameterA);

bool ParseN2kCustomCommandInt(const tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand &command, int16_t &parameterA);

#endif
