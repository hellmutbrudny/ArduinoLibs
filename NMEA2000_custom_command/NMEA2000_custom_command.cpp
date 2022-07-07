#include "NMEA2000_custom_command.h"

//*****************************************************************************
// Custom Command

void SetN2kCustomCommand(tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand command) {
    N2kMsg.SetPGN(130666L);
    N2kMsg.Priority=2;
    N2kMsg.AddStr(deviceID, 4);
    N2kMsg.AddByte((unsigned char) command);
}

bool ParseN2kCustomCommand(const tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand &command) {
  if (N2kMsg.PGN!=130666L) return false;
  int Index=0;
  N2kMsg.GetStr(deviceID, 4, Index);
  command=(tN2kCustomCommand)(N2kMsg.GetByte(Index));
  return true;
}

void SetN2kCustomCommandTwoInts(tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand command, int16_t parameterA, int16_t parameterB) {
    N2kMsg.SetPGN(130667L);
    N2kMsg.Priority=2;
    N2kMsg.AddStr(deviceID, 3);
    N2kMsg.AddByte((unsigned char) command);
    N2kMsg.Add2ByteInt(parameterA);
    N2kMsg.Add2ByteInt(parameterB);
}

bool ParseN2kCustomCommandTwoInts(const tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand &command, int16_t &parameterA, int16_t &parameterB) {
  if (N2kMsg.PGN!=130667L) return false;
  int Index=0;
  N2kMsg.GetStr(deviceID, 3, Index);
  command=(tN2kCustomCommand)(N2kMsg.GetByte(Index));
  parameterA=N2kMsg.Get2ByteInt(Index);
  parameterB=N2kMsg.Get2ByteInt(Index);
  return true;
}

void SetN2kCustomCommandDouble(tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand command, double parameterA) {
    N2kMsg.SetPGN(130668L);
    N2kMsg.Priority=2;
    N2kMsg.AddStr(deviceID, 4);
    N2kMsg.AddByte((unsigned char) command);
    N2kMsg.Add4ByteDouble(parameterA, 4);
}

bool ParseN2kCustomCommandDouble(const tN2kMsg &N2kMsg, char *deviceID, tN2kCustomCommand &command, double &parameterA) {
  if (N2kMsg.PGN!=130668L) return false;
  int Index=0;
  N2kMsg.GetStr(deviceID, 4, Index);
  command=(tN2kCustomCommand)(N2kMsg.GetByte(Index));
  parameterA=N2kMsg.Get4ByteDouble(4, Index);
  return true;
}

