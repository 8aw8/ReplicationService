#pragma once
#include "winsock2.h"
#include "MyThread.h"
#include <atlstr.h>
#include <vector>
//#include "StdAfx.h"
//#include "DBThread.h"

using namespace std;

struct _HeadPacket
{
  char prefix[4];
  int PacketType;
  int CodCommand;
  int Request_ID;
  int PacketSize; 
};

struct _buffer
{ 
   char* buffer;
   int SizeBuffer;
};
struct _ConnStr
{
	CString Driver;
	CString DSNname;
    CString Desc;
	CString Server;
	CString Database;
    CString User;
    CString Password;
    CString	UseUserDSN;
	CString UserDSN;
	CString UseUSER;
	CString UsePassword;
};


class CClient : public CMyThread
{
public:
	CClient(SOCKET socket);
	virtual ~CClient(void);
	void Realize(void);

	SOCKET m_socket;
 	_HeadPacket HeadPacket;
      BOOL StopingThread;
	  char IP_Addr[16];
 	
//CDBThread a;
  vector <void*> DBTVector;
  vector <void*>::iterator v_Iter;
    
//	virtual BOOL onConnected();
   DWORD ThreadFunc();
    void EchoClient();
	BOOL WorkTypePacket(_HeadPacket head);
	BOOL TrueMDISPacket(_HeadPacket head);
	 int handlErr(const int err);
    void PrintHeadPacket(_HeadPacket head);

   	void OnRecvPacket(char* buffer, int BufferSize, _HeadPacket head);

       
};
