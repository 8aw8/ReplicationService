// MyThread.cpp: implementation of the CMyThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyThread.h"
#include "afxdb.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
unsigned __stdcall ThreadProc(void* lpParameter)

//extern DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	CMyThread *pMyThread=(CMyThread *)lpParameter;
	  
	  pMyThread->CriticalSection = new CRITICAL_SECTION; //the_log.LogT(2,"New %ld bytes",sizeof(CRITICAL_SECTION));
	  InitializeCriticalSection(pMyThread->CriticalSection);
      EnterCriticalSection(pMyThread->CriticalSection);
	//  printf("sizeof(*pMyThread) %ld\n",sizeof(*pMyThread));

 DWORD result=pMyThread->ThreadFunc();
       
       LeaveCriticalSection(pMyThread->CriticalSection); 
	   DeleteCriticalSection(pMyThread->CriticalSection);
       delete pMyThread->CriticalSection;
   
   // ExitThread(0);

	_endthreadex(0);

	return 0;
}


CMyThread::CMyThread()
{
	m_hThread=NULL;
	
}

CMyThread::~CMyThread()
{
//   if (m_hThread!=NULL) CloseHandle(m_hThread);
  
  // 	ExitThread(0);
	//  Terminate(FALSE);
}


BOOL CMyThread::Execute()
{
	if (m_hThread)
	{
		return FALSE;
	}
   //CMyThread* Thread = new CMyThread;
   
	//CMyThread* pData = (CMyThread*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*this));
//	the_log.LogT(2,"sizeof(this)=%ld\n",sizeof(*this));

	//pData=this;

//	m_hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&m_dwID);
	//m_hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,pData,0,&m_dwID);
    m_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadProc, this, 0, &m_dwID);
    ResumeThread(m_hThread);

//	if (m_hThread!=NULL) 
//          SetThreadPriority(m_hThread, THREAD_PRIORITY_LOWEST); 

	DWORD dwExitCode=0;
	while (dwExitCode!=STILL_ACTIVE)
	{
	  GetExitCodeThread(m_hThread,&dwExitCode);
	}

    return (m_hThread==NULL)?FALSE:TRUE;
}

DWORD CMyThread::Terminate(BOOL bCritical)
{
	DWORD dwExitCode;
	
	Sleep(100);
	GetExitCodeThread(m_hThread,&dwExitCode);
	if (dwExitCode==STILL_ACTIVE)
	{
		while (dwExitCode==STILL_ACTIVE)
		{
			GetExitCodeThread(m_hThread,&dwExitCode);
		
		}
	//	TerminateThread(m_hThread,0);
	//	_endthreadex( 0 );
		
	}
	else 
	{
    	GetExitCodeThread(m_hThread,&dwExitCode);	 
	 //   TerminateThread(m_hThread,0);
	//	_endthreadex(0);
	//	ExitThread(0);
	}
	if (m_hThread!=NULL) CloseHandle(m_hThread);
	m_hThread=NULL;
	return dwExitCode;
	//}
}



