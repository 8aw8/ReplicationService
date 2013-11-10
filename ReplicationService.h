#pragma once
#include "Server.h"
#include "resource.h"
class CMainApp : public KDriver,CServer
{
public:	// interface
	CMainApp();
	virtual ~CMainApp();
	
protected:	// overridables
	// KDriver override
	virtual SYSSTATUS InitInstance ();
	virtual void ExitInstance ();	

protected:
	// User events
	virtual void OnStartup();
	virtual void OnShutdown();

private:
	static CMainApp the_driver;

};

