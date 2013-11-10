#pragma once

#include <stdio.h>
#include "winsock2.h"
#include "client.h"
#pragma comment (lib, "Ws2_32.lib")


struct _DiagPacket   
{
  INT a;
  INT b;
  INT Result;
}; 

class CServiceDiagnostics
{
public:
	CServiceDiagnostics(int kod_error);
	virtual ~CServiceDiagnostics(void);

	_DiagPacket ReturnPacket();
		
};
