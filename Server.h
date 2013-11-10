#pragma once
//#include <stdio.h>
#include "winsock2.h"
#include "MyThread.h"
#include <vector>
//#include <iostream>
#include "Client.h"

 using namespace std;

VOID CALLBACK MyTimerProc(
	HWND hwnd,      // handle to window for timer messages 
    UINT message,     // WM_TIMER message 
    UINT idTimer,     // timer identifier 
    DWORD dwTime);

class CServer: public CMyThread 
{
public:
	CServer();
	int InitServer(u_short port);
	void RealiseServer();
	CServer(u_short port);
	virtual ~CServer(void);
    bool BindListenSocket();
	
	BOOL StartWaitConnect();
	SOCKET WaitConnect();
    BOOL StopWaitConnect();

	char* EventName;
	HANDLE StopEvent;
	HANDLE StopEventClient;
	BOOL StopThread;
	
 virtual BOOL onConnected();
     
    u_short m_port;
    SOCKET ListenSocket;
	SOCKET AcceptSocket;
	sockaddr_in service;
	sockaddr_in send_service;
	CClient* client;
     
    vector <CClient*> VectorClient;
	vector <CClient*>::iterator vc_Iter;

void DeleteNotWorkingThread();

protected:
	DWORD ThreadFunc();
	
};
