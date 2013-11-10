#include "stdafx.h"
//#include "DB.h"

#include "afxdb.h"
#include <odbcinst.h>
#include <conio.h>
#include ".\storedproc.h"

#pragma comment(lib, "odbccp32")
#pragma comment(lib, "user32")



CStoredProc::CStoredProc(CString ConnectString)
{
	CString s;
	ShowLog=TRUE;
	int res=-1;
	try
    {
		res= db.Open( NULL, FALSE, FALSE, ConnectString, TRUE);
	}
    catch(CDBException* err)
    {
		printf("Connection string=%s\n",s.GetBuffer());
		printf("%s \n",err->m_strStateNativeOrigin);
		printf("%s \n",err->m_strError.Trim());
		err->Delete();		
	}
	{
      
    }
	if (res<0) db.Close();
}
int CStoredProc::ExecStoredProc(CString Query)
{
	 SWORD ReturnValue=0;
   //SQLINTEGER cbReturnValue=0;
   
   //CString	str="EXEC [dbo].[test] @Param1=27";
   CString str=Query;
   int rcExec=0;

try
    {
	//db.BeginTrans();

    rcExec = SQLAllocHandle(SQL_HANDLE_STMT, db.m_hdbc, &hStmt);
    if (ShowLog) printf("SQLAllocHandle(SQL_HANDLE_STMT, db.m_hdbc, &hStmt); %ld\n", rcExec);

	//rcExec = SQLPrepare(hStmt, (UCHAR*)LPCTSTR(str), SQL_NTS);
	//printf("SQLPrepare(hStmt, (UCHAR*)LPCTSTR(str), SQL_NTS); %ld\n", rcExec);
    	
  //  rcExec=SQLBindParameter(hStmt,1,SQL_PARAM_INPUT,SQL_C_SLONG,SQL_INTEGER,0,0,&ReturnValue,0,&cbReturnValue);
//	printf("SQLBindParameter(...); %ld\n", rcExec);

	if (rcExec>=0)
	{
     rcExec = SQLExecDirect(hStmt, (UCHAR*)LPCTSTR(str), SQL_NTS);
     if (ShowLog) printf("SQLExecDirect(hStmt, (SQLCHAR*)str, SQL_NTS); %ld\n", rcExec);
	}
	
	if (rcExec>=0)
	{
     rcExec = SQLFetch(hStmt);
     if (ShowLog) printf("SQLFetch(hStmt); %ld\n", rcExec);
	}
	else 
	{
		
	}

	if (rcExec>=0)
	{
	 rcExec = SQLGetData(hStmt,1, SQL_C_SSHORT, &ReturnValue, sizeof(ReturnValue), NULL);
	 if (ShowLog) printf("SQLGetData(...); %ld\n", rcExec);
	}
       // db.CommitTrans();
	

	 if (ShowLog) printf("ReturnValue= %ld \n",ReturnValue);
	 SQLFreeHandle(SQL_HANDLE_STMT,hStmt);

	}
    catch(CDBException* err)
    {
		printf("%s \n",err->m_strStateNativeOrigin);
		printf("%s \n",err->m_strError.Trim());
		err->Delete();		
	}
	{
           //db->Close();
    }

	
	 
	 if (rcExec<0)
	 {
		ReturnValue=-1;
	 }
	return ReturnValue;
 }

CStoredProc::~CStoredProc(void)
{
	db.Close();
}
