#include "stdafx.h"
//#include "DB.h"

#include "afxdb.h"
#include <odbcinst.h>
#include <conio.h>
#include "DBase.h"
#include "StoredProc.h"

#pragma comment(lib, "odbccp32")
#pragma comment(lib, "user32")


CDBase::CDBase(void)
{
 
}

CDBase::~CDBase(void)
{
	db.Close();

}
int CDBase::CreateDSN(_ConnStr ConnStr)
{ 
//	'DSN=SQL_DSNName112;DESCRIPTION=SQL DSN description;SERVER=AW\SQLEXPRESS;DATABASE=ADM;'
   int nRetCode;  
   CString s;
   s="DSN="+ConnStr.DSNname+";";
   s=s+"DESCRIPTION="+ConnStr.Desc+";";
   s=s+"SERVER="+ConnStr.Server+";";
   s=s+"DATABASE="+ConnStr.Database+";";
 
   //printf("ConnStr=%s\n",s.GetBuffer());

   char *szAttributes=s.GetBuffer(s.GetLength()); 
    printf("Attributes='%s'\n",szAttributes);
   for(int i=0;i<s.GetLength();i++) 
   { 
	if(*(szAttributes+i)==(unsigned char)';') 
		*(szAttributes+i)=NULL; 
   }  
    try
    {
		nRetCode = SQLConfigDataSource(NULL, ODBC_ADD_DSN, ConnStr.Driver.GetBuffer(), szAttributes);
	}
    catch(CDBException *err)
    {
		//MessageBox(NULL,err->m_strError,"SQL Error"	,MB_OK);
    	printf("%s \n",err->m_strStateNativeOrigin);	
		printf("%s \n",err->m_strError);
		err->Delete();
	}
	{
	   //printf("Error create DSN in registry!\n"); 
    }
   return nRetCode;	
}
int CDBase::ConnectToDB(CString DSNname,CString user,CString password)
{
	int res=0;
	CString s="DSN="+DSNname+";";
	s=s+"UID="+user+";";
	s=s+"PWD="+password;
	ConnectString=s;
	LocalUser=user;
	try
    {
		res=db.OpenEx(s,CDatabase::openReadOnly);
		//res=OpenEx(s,CDatabase::noOdbcDialog);
    }
    catch(CDBException* err)
    {
		printf("Connection string=%s\n",s.GetBuffer());
		printf("%s \n",err->m_strStateNativeOrigin);
		printf("%s \n",err->m_strError.Trim());
		err->Delete();		
	}
	{
           //db->Close();
    }

//	CRecordset rs1(&db);

	return res;
}
void CDBase::DBaseClose(void)
{
	//db.Close();
}
int CDBase::ExecSQL(_ReplicPacket packet)
{
	CRecordset rs(&db);
       CString s; 
	   CString query = "SELECT replic_id FROM [adm].[dbo].[exists_replic] ('"+packet.ReplicName+"')";
		
	  // printf("%s\n",query.GetBuffer());

		   if (rs.Open(0 ,query)) 
			{
			   rs.GetFieldValue("replic_id",s);					
			}
			rs.Close();

		
    
		if (s!="-1") 
		{
			printf("Replic is exists.\n");
			
			return 0x5001;//реплика уже существует
		}
  
   printf("Start creating replic.\n"); 
 
   CStoredProc proc(ConnectString);
   proc.ShowLog=FALSE;
   CString LinkServer="SERVER_PHANTOM";
   
   int res=-1;
   if (s=="-1")
   {	
   
     res=0;
	

	CString CreateLinkServer="EXEC [dbo].[CreateLinkServer] ";
	 CreateLinkServer+="@NameLinkServer = N'"+LinkServer+"',";
	 CreateLinkServer+="@DataSource = N'"+packet.BD_name+"',";
	 CreateLinkServer+="@LocalUser = N'"+LocalUser+"',";
	 CreateLinkServer+="@remouteUser = N'"+packet.User+"',";
	 CreateLinkServer+="@remoutePass = N'"+packet.Password+"'";
	 	
	printf("CreateLinkServer=%s\n",CreateLinkServer.GetBuffer());
   
   
   // proc.db.BeginTrans();
     res = proc.ExecStoredProc(CreateLinkServer);
     printf("EXEC CreateLinkServer result=%ld\n",res);
   // proc.db.CommitTrans();
  

	if (res==-1) 
	{
		printf("Error creating linkServer.\n");
		return 0x1000;//Ошибка авторизации соеденения с удалённым сервером
	}
	else
		printf("LinkServer is created.\n");
   }
   
   if (res!=-1)
	{
	//	proc.db.BeginTrans();
		res = proc.ExecStoredProc("declare @a as int; exec @a =sp_testlinkedserver "+LinkServer+"; select result=@a;");
		printf("EXEC sp_testlinkedserver result=%ld\n",res);
	//	proc.db.CommitTrans();
		if (res==-1) 
		 return 0x1000;//Ошибка авторизации соеденения с удалённым сервером
	}


	if (res!=-1)
	{
		CString CreateDataBase="EXEC [dbo].[CreateDataBase] @NameBase = N'"+packet.ReplicName+"'";
	 //proc.db.BeginTrans();
		res = proc.ExecStoredProc(CreateDataBase);
	  //proc.db.CommitTrans();
		printf("EXEC result=%ld\n",res);
	  }

 if (res!=-1)
 {
	//proc.db.BeginTrans();
	CString PreReplication="EXEC [dbo].[PreReplication]";
    PreReplication+="@name_db = N'"+packet.ReplicName+"',";
	PreReplication+="@LinkServer = N'"+LinkServer+"',";
	PreReplication+="@DBSource = N'"+packet.DBSource+"'";
	res = proc.ExecStoredProc(PreReplication);
    printf("EXEC PreReplication result=%ld\n",res);
	if (res==-1)  
	{
		//db.ExecuteSQL();
	  return 0x1000;//Ошибка авторизации соеденения с удалённым сервером
	}
	//proc.db.CommitTrans();
 }


  if (res!=-1)
  {
      
    for (int i = 0; i< packet.call_id.size();i++ )
    {
		int id=packet.call_id[i];
            
		char str[8];
          _itoa(id,str,10);
        printf("Replication Call_id=%s\n",_itoa(id,str,10));
		CString str_id=str;
		CString s="0";
		
	   CString query2="select filename "; 
	   query2+="from OPENQUERY("+LinkServer+",'select filename from "+packet.DBSource+".dbo.call where id="+str_id+"') ";
		
		if (rs.Open(0 ,query2)) 
			{
			//   while (!rs.IsEOF())
			//   {
				
				  try
				  {
				   rs.GetFieldValue("filename",s);	
				  }
				  catch(CDBException* err)
				  {err->Delete();} {}
				  CString FilePath=packet.Path+s;
                  if (s!="0")
				  {
                    FILE* f=fopen(FilePath.GetBuffer(),"w");
					printf("==============filename is not NULL replics call %s, %s\n",s.GetBuffer(),str_id);
					fclose(f);
		    	  }
			//	rs.MoveNext();
			//  }
			}
			rs.Close();
  	
         
			CString CopyCall_id ="EXEC adm.dbo.copyCall_id ";
			CopyCall_id+="@Call_id = "+str_id+",";
			CopyCall_id+="@replicName = N'"+packet.ReplicName+"',";
			CopyCall_id+="@LinkSertver = N'"+LinkServer+"',";
			CopyCall_id+="@dbServer = N'"+packet.DBSource+"'";
		
			//printf("%s\n",CopyCall_id.GetBuffer());
		 //proc.db.BeginTrans();
			res=proc.ExecStoredProc(CopyCall_id);
		 //proc.db.CommitTrans();
			printf("EXEC [dbo].[CopyCall_id] %ld\n",res);
			if (res==-1){printf("Strored proc not executet.\n");}
			else {printf("Record is copy.\n");}
	}
  }
	return 0;
}

