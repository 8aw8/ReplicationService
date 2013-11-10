#include "StdAfx.h"
#include ".\client.h"
#include ".\server.h"
#include <iostream>
#include "ServiceDiagnostics.h"
#include "ReplicPacket.h"
#include "DBThread.h"
#include "inifile.h"
#include "PacketParser.h"

int RecvBufferSize=1024;


//#include <Pkfuncs.h>

/*
void printPacket(char* Packet, int Len)
{
    printf("packet: ");
    for(int i=0; i<Len; i++)
         printf(" %d",Packet[i]);
    printf("\n");
} 
*/

CClient::CClient(SOCKET socket) :CMyThread()
{
	m_socket=socket;
	StopingThread=FALSE;
}
CClient::~CClient(void)
{
  //Realize();
}
void CClient::Realize()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	//CloseHandle(m_hThread);
}

DWORD CClient::ThreadFunc()
{
  int BytesSend = -1; 
    
  //the_log.LogT(2,"Clients Socket= %d\n",m_socket);
  BOOL StopLoop= FALSE;

  CPacketParser parser; 
  char* SendBuffer = new char[RecvBufferSize]; 
  MDIS_packet* pack;

  while (!StopLoop)
  {
	  pack=NULL;
//----------------------------------------------------
   while ((pack==NULL)&&(!StopLoop))
   {
       Sleep(50);    
	   BytesSend = recv(m_socket, SendBuffer, RecvBufferSize, 0);
	
	   if (BytesSend<0)
	   {
		   if(handlErr(WSAGetLastError())!=0)//disconect
		   {
               StopLoop=TRUE;
			   the_log.LogT(2,"Client for ip %s is disconnected.\n",IP_Addr);
		   }//disconect	
	   }
	   else 
	   {
//----------------------------------------------------   
		 if (!StopLoop)
		 {
			parser.addBuffer(SendBuffer, BytesSend);
			while ((pack=parser.getPacket())!=NULL)
			{
				printf("----------------------------------------\n");
				printf("-----------HEAD PACKET------------------\n");
				printf("----------------------------------------\n");
				printf("-------Connect %s----------\n",IP_Addr);
	
				memcpy(&HeadPacket,(char*)pack->head,sizeof(_HeadPacket));  
				delete pack->head;
				PrintHeadPacket(HeadPacket); 
			 
				if ((HeadPacket.PacketSize==0)&&(WorkTypePacket(HeadPacket))) OnRecvPacket(NULL,0,HeadPacket);
				if ((HeadPacket.PacketSize!=0)&&(WorkTypePacket(HeadPacket))) OnRecvPacket(pack->buffer,HeadPacket.PacketSize,HeadPacket);			
				
				delete pack;		  
			}//while ((pack=this->parser.getPacket())!=NULL)
		}//if (!StopLoop)
	  }//else
    }//while ((pack==NULL)&&(!StopLoop))
  }// while (!StopLoop)	 
  
  //InterlockedIncrement((long*)&StopingThread);			   
  delete [] SendBuffer;
  this->Realize();
  StopingThread=TRUE;
  
  return 0;
}

void CClient::OnRecvPacket(char* buffer, int BufferSize, _HeadPacket head)
{
	//0x200, 0x201, 0x202, 0x700
	_HeadPacket OutHead;
	
	printf("-----------BODY PACKET------------------\n");
	switch (head.CodCommand)
	{
		case 0x100://Запрос на создание новой реплики.
			{
			  printf("PacketType=0x%x\n",head.CodCommand); 
			  
			  memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x100;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
			  int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);

			  //	  printf("Sending %ld bytes\n",bytesSend);
			  PrintHeadPacket(OutHead);
//			  printPacket(buffer,BufferSize);
			  _buffer b;
			  b.buffer=buffer;
			  b.SizeBuffer=BufferSize;
			  CReplicPacket ReplicPacket(b);
			  
_ConnStr str;
	str.Database="ADM";
	str.Desc="Description";
	str.Driver="SQL Native Client";
	str.DSNname="CLIENT_PHANTOM";
	str.Server="AW\\SQLEXPRESS";
	str.User="user11";
	str.Password="user11";
	str.UseUserDSN="1";
	str.UserDSN="CLIENT_PHANTOM";
	str.UseUSER="test";
	str.UsePassword="123test";

	char* dir = new char[1024];
	CString IniPatch;
	if (GetCurrentDirectory(1024,  dir)!=0) 
		 IniPatch=dir;
	delete [] dir;
	IniPatch+="\\ReplicationService.ini";

	printf("Current dirrictory %s.\n",IniPatch.GetBuffer());

	 CIniFile IniFile(IniPatch);
     
	IniFile.WriteString("ConnectionString","Database",str.Database);
	IniFile.WriteString("ConnectionString","Description",str.Desc);
    IniFile.WriteString("ConnectionString","Driver",str.Driver);
	IniFile.WriteString("ConnectionString","DSN",str.DSNname);
	IniFile.WriteString("ConnectionString","Server",str.Server);
	IniFile.WriteString("ConnectionString","User",str.User);
	IniFile.WriteString("ConnectionString","Password",str.Password);
	IniFile.WriteString("ConnectionString","UseUserDSN",str.UseUserDSN);
	IniFile.WriteString("ConnectionString","UserDSN",str.UserDSN);
	IniFile.WriteString("ConnectionString","UseUSER",str.UseUSER);
	IniFile.WriteString("ConnectionString","UsePassword",str.UsePassword);

	
//	printf("INI UseUserDSN = %s \n",Str.GetBuffer());
   // if (str=="1")
	str.UseUserDSN=IniFile.ReadString("ConnectionString","UseUserDSN","");
 if (str.UseUserDSN="1")
 {
//	str.UseUserDSN=IniFile.ReadString("ConnectionString","UseUserDSN","");
	str.UserDSN=IniFile.ReadString("ConnectionString","UserDSN","");
	str.UseUSER=IniFile.ReadString("ConnectionString","UseUSER","");
    str.UsePassword=IniFile.ReadString("ConnectionString","UsePassword","");
 }
 else
 {
	str.Database = IniFile.ReadString("ConnectionString","Database","");
	str.Desc=IniFile.ReadString("ConnectionString","Description","");
    str.Driver=IniFile.ReadString("ConnectionString","Driver","");
	str.DSNname=IniFile.ReadString("ConnectionString","DSN","");
	str.Server=IniFile.ReadString("ConnectionString","Server","");
	str.User=IniFile.ReadString("ConnectionString","User","");
	str.Password=IniFile.ReadString("ConnectionString","Password","");
 }
	
//	printf("str.Database %s\n",str.Database.GetBuffer());
//  printf("str.DSNname %s\n",str.DSNname.GetBuffer());
//  printf("str.Server %s\n",str.Server.GetBuffer());

	
	//ошибка авторизации (код ошибки 0x1000);
	//некорректное название реплики (код ошибки 0x5000);
	//реплика с заданным именем уже существует (код ошбики 0x5001);
	//список звонков для репликации пуст (код ошибки 0x3001);
	//некорректный путь для файлов звонков (код ошибки 0x3002).
	
   printf("Count call_id = %ld \n",ReplicPacket.m_packet.call_id.size());
   
   
    if (!SetCurrentDirectory(ReplicPacket.m_packet.Path))
    {
		ReplicPacket.SendErrorPacket(m_socket,head,0x3002);
    }
	else
	if (ReplicPacket.m_packet.call_id.size()!=0)
	{
	   CDBThread* dbt= new CDBThread(ReplicPacket.m_packet,str,head);
		int error = dbt->ThreadFunc();
		
		DBTVector.push_back(dbt);

	//error 
	//1 - Реплика уже существует
	//2 - Ошибка авторизации соеденения с удалённым сервером
		if (error==0) 
		{
		      memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x100;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
			  int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);
		}
		if (error==1) ReplicPacket.SendErrorPacket(m_socket,head,0x5001);
		if (error==2) ReplicPacket.SendErrorPacket(m_socket,head,0x1000);
	   delete [] dbt;
  // dbt->Execute();
   } 
   else
   {
	  ReplicPacket.SendErrorPacket(m_socket,head,0x3001);
   }
 
   delete [] b.buffer;
	 break;
			}
		case 0x101://Запрос на добавление данных в имеющуюся реплику.
			{
			  printf("PacketType=0x%x\n",head.CodCommand);  
			  
			  memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x101;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
			 int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);
			  printf("Sending %ld bytes\n",bytesSend);
			  PrintHeadPacket(OutHead);
			 break;
	        }
		case 0x200://Запрос списка реплик.
			{
			  printf("PacketType=0x%x\n",head.CodCommand); 
			
			  memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x200;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
			 int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);
			 printf("Sending %ld bytes\n",bytesSend);
			 PrintHeadPacket(OutHead);
/*  
0x0020
0x0200
<request_id>
<size_of_list>
<list_of_repl_names>
*/
			 break;
			}
		case 0x201://Запрос на получение информации о реплике.
			{
			  printf("PacketType=0x%x\n",head.CodCommand);
			  memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x201;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
	      int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);
			  printf("Sending %ld bytes\n",bytesSend);
			  PrintHeadPacket(OutHead);
			 break;
			}
		case 0x202://Запрос на удаление реплики.
			{
			  printf("PacketType=0x%x\n",head.CodCommand);
              memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x202;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
		  int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);
			  printf("Sending %ld bytes\n",bytesSend);
			  PrintHeadPacket(OutHead);
			 break;
			}
		case 0x700://Авторизация.
			{
              printf("PacketType=0x%x\n",head.CodCommand);
			  memcpy(&OutHead.prefix,"MDIS",4);
			  OutHead.PacketType=0x20;
			  OutHead.CodCommand=0x700;
			  OutHead.Request_ID=head.Request_ID;
              OutHead.PacketSize=0;
		  int bytesSend = send(m_socket, (char*)&OutHead, sizeof(_HeadPacket), 0);
			  printf("Sending %ld bytes\n",bytesSend);
			  PrintHeadPacket(OutHead);
			 break;
			}
		default:
			 break;
	}
}


void CClient::EchoClient()
{
  the_log.LogT(2,"This is client socket = %d \n", m_socket);
}
BOOL CClient::WorkTypePacket(_HeadPacket head)
{
	BOOL res=FALSE;
    if (TrueMDISPacket(head))
	{
       // printf("%s\n",head.prefix);
	  if (head.PacketType==0x10)
	  {
		switch (head.CodCommand)
		{
		case 0x100:
			  res=TRUE;
			 break;
		case 0x101:
			  res=TRUE;
			 break;
		case 0x200:
			  res=TRUE;
			 break;
		case 0x201:
			  res=TRUE;
			 break;
		case 0x202:
			  res=TRUE;
			 break;
		case 0x700:
			  res=TRUE;
			 break;
		default:
			 res=FALSE;
			 break;
	    }//switch (head.CodCommand)
	  }//if
	}//if
	return res;
} 


int CClient::handlErr(const int err)
{
    int res=0;
    switch(err)     
    {
         case WSANOTINITIALISED     //A successful WSAStartup call must occur before using this function.
              :res = err; break;
         case WSAENETDOWN          //The network subsystem has failed.
              :res = err; break;
         case WSAENOTCONN          //The socket is not connected.
              :res = err; break;
         case WSAENOTSOCK          //The descriptor is not a socket. 
              :res = err; break;
         case WSAESHUTDOWN          //The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.
              :res = err; break;
         case WSAECONNABORTED     //The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
              :res = err; break;
         case WSAETIMEDOUT          //The connection has been dropped because of a network failure or because the peer system failed to respond.
              :res = err; break;
         case WSAECONNRESET          //The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UPD-datagram socket this error would indicate that a previous send operation resulted in an ICMP Port Unreachable message;
              :res = err; break;
         
         //case WSAEFAULT:the_log.LogT(2," The buf parameter is not completely contained in a valid part of the user address space. "); break;
         //case WSAEINTR:the_log.LogT(2," The (blocking) call was canceled through WSACancelBlockingCall. "); break;
         //case WSAEINPROGRESS:the_log.LogT(2," A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. "); break;
         //case WSAENETRESET:the_log.LogT(2,"The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress. "); break;          
         //case WSAEOPNOTSUPP:the_log.LogT(2,"MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations. "); break;
         //case WSAEWOULDBLOCK:the_log.LogT(2,"The socket is marked as nonblocking and the receive operation would block. "); break;
         //case WSAEMSGSIZE:the_log.LogT(2,"The message was too large to fit into the specified buffer and was truncated. "); break;
         //case WSAEINVAL:the_log.LogT(2,"The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative. "); break;
         
         default: res =0; break;
    }
    return res;
} 

BOOL CClient::TrueMDISPacket(_HeadPacket head)
{
	char MDIS[4];
	MDIS[0]='M';
	MDIS[1]='D';
	MDIS[2]='I';
	MDIS[3]='S';
	int i=0;
	
	while (i<=3)
	{
		if (!(head.prefix[i]==MDIS[i])) return FALSE;
		i++;
	}
	return TRUE;
}
void CClient::PrintHeadPacket(_HeadPacket head)
{
	     printf("//======= Send/Reseive ====\n");
		 printf("  Prefix: %s \n",head.prefix);
		 printf("  Type: 0x%x \n",head.PacketType);
		 printf("  Command: 0x%x \n",head.CodCommand);
		 printf("  Request ID: 0x%x \n",head.Request_ID);
		 printf("  Data Size: %ld \n",head.PacketSize);
	     printf("//========================\n");
}
