#pragma once
#include "mythread.h"
#include "afxdb.h"
#include "ReplicPacket.h"


class CDBThread :public CMyThread
{
public:
	CDBThread(_ReplicPacket packet,_ConnStr str,_HeadPacket head);
	virtual ~CDBThread(void);

	DWORD ThreadFunc();
//	void ReplicProcess(_ReplicPacket packet,CString ConnectString, CString LocalUser);

	CDatabase db;
	CString LinkServer;
	
	_ReplicPacket m_packet;
	_ConnStr m_LocalConnStr;
	_HeadPacket m_head;
	 int m_error; 
};
