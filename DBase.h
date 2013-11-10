#pragma once
#include "afxdb.h"
#include "MyThread.h"
#include "DBThread.h"
#include "ReplicPacket.h"

#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS



class CDBase : public CDatabase
{
public:
	CDBase(void);
	virtual ~CDBase(void);
     
	
	//CRecordset* rs;
	CDatabase db;
	CString ConnectString;
	CString LocalUser;
	
	int CreateDSN(_ConnStr ConnStr);
	int ConnectToDB(CString DSNname,CString user,CString password);
	int ExecSQL(_ReplicPacket packet);
	void DBaseClose();
};
