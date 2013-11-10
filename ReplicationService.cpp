// SQLReplicationService.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ReplicationService.h"
#include "ServiceDiagnostics.h"
#include "Server.h"
#include "Client.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment (lib, "KernelD.lib")

DECLARE_MODULE_STUB();

CMainApp CMainApp::the_driver;

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}
#define DEF_APP_NAME "ReplicationService"

SYSSTATUS CMainApp::InitInstance()
{
	CoInitialize(NULL);
	if (InitServer(8070)!=0) 
	{
		the_log.LogT(8, "InitInstance: Failed to init server on port 8070\n");
		return STATUS_UNSUCCESSFUL;
	}
	the_log.LogT(2, "InitInstance: DONE\n");
	//the_log.SetLogFile("c:\log.txt");
	OnStartup();
	return STATUS_SUCCESS;
}

void CMainApp::ExitInstance()
{
	the_log.LogT(1, "ExitInstance:\n");
	the_log.LogT(4, "ExitInstance: OnShutdown call...\n");
	OnShutdown();
	CoUninitialize();
//	Sleep(1000);
	the_log.LogT(2, "ExitInstance: DONE\n");
	
}

void CMainApp::OnStartup()
{
	the_log.LogT(2, "OnStartup\n");
	this->StartWaitConnect();
}

void CMainApp::OnShutdown()
{
	this->StopWaitConnect();
	this->RealiseServer();
}

CLogger the_log;
