#include "StdAfx.h"
#include "dbthread.h"

//#include ".\dbthread.h"
#include "DBase.h"
#include "ReplicPacket.h"
//#include "StoredProc.h"

using namespace std;


CDBThread::CDBThread(_ReplicPacket packet,_ConnStr str,_HeadPacket head):CMyThread()
{
  m_packet=packet;
  m_LocalConnStr=str;
  m_head=head;
}

CDBThread::~CDBThread(void)
{
}
DWORD CDBThread::ThreadFunc()
{
   DWORD result=-1;
   m_error=-1;
   CDBase dbase;
   CString user;  CString password; CString DSN;
   if (m_LocalConnStr.UseUserDSN="0") 
   {
	if(!dbase.CreateDSN(m_LocalConnStr)) 
     { 
	   printf("Error create DSN in registry!\n");
	   m_error=0x1000;
	   return 2;
	 //  MessageBox(NULL,"Error create DSN in registry!","DNS create error",MB_OK);
     } 
	 user=m_LocalConnStr.User;
	 password=m_LocalConnStr.Password;
	 DSN=m_LocalConnStr.DSNname;
   }//if (m_LocalConnStr.UseUserDSN="0") 
   else if(m_LocalConnStr.UseUserDSN="1") 
   {
		user=m_LocalConnStr.UseUSER;
		password=m_LocalConnStr.UsePassword;
		DSN=m_LocalConnStr.UserDSN;
   }
	 
	 if (dbase.ConnectToDB(DSN,user,password)!=0)
	 {
		printf("Local DB connected.\n");
		dbase.db.BeginTrans();
		m_error=dbase.ExecSQL(m_packet);
		if (m_error==0x5001) result=1;
		if (m_error==0x1000) result=2;
        if (m_error==0) result=0;
		dbase.db.CommitTrans();
      }
	 else 
	 {
	    printf("Local DB is not connected.\n");
		m_error=0x1000;
	    return 2;
	 }
    
 //	Sleep(10000);
	dbase.DBaseClose();

	return result;
}