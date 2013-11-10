#pragma once
#include "afxdb.h"

#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS


class CStoredProc
{
public:
	CStoredProc(void);
    CStoredProc(CString ConnectString);

  int ExecStoredProc(CString Query); 
	
	virtual ~CStoredProc(void);

	CDatabase db;
	    HSTMT hStmt;
	     BOOL ShowLog;
};
