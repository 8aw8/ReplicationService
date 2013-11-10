// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// _WINDOWS defined
// _MFC_ defined
//#define _WINDOWS
#define _MFC_
#include <Kernel/prerequisites.h>

#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#define WIN32_NO_STATUS 
#include <afx.h>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
//#include <ATLComTime.h>

// TODO: reference additional headers your program requires here
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <list>
//#include "DBThread.h"

#include <mmreg.h>

#include <Kernel/kernel.h>
#pragma warning (disable: 4244)
#include <Kernel/KFile.h>
#pragma warning (default: 4244)

#include <LibTSAPI/Logger.h>

//#define SQL_SELECT				"SELECT * FROM %s WHERE RecordIndex = %d AND RecordStation = %d;"

//#define REG_PARAM_ADO_PROVIDER		"ADOProvider"
//#define REG_PARAM_TABLE_NAME		"TableName"
//#define REG_PARAM_DEST_DIR			"DestDir"
//#define REG_PARAM_USERNAME			"UserName"
//#define REG_PARAM_PASSWORD			"Password"
//#define REG_PARAM_NOFILES			"NoFiles"

extern CLogger the_log;

