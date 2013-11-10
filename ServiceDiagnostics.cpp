#include <stdio.h>
#include "winsock2.h"
#include "stdafx.h"
#include "ServiceDiagnostics.h"

//#pragma comment (lib, "Ws2_32.lib")


CServiceDiagnostics::CServiceDiagnostics(int kod_error)
{
  
}
CServiceDiagnostics::~CServiceDiagnostics(void)
{
}

_DiagPacket CServiceDiagnostics::ReturnPacket()
{
   _DiagPacket packet;
   packet.Result=0;
   return packet;
}



