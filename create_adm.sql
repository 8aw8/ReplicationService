CREATE DATABASE [adm]
go

use adm;

CREATE TABLE arm
(
	arm_id int IDENTITY (1,1) ,
	arm_ip char(16)  NULL ,
	arm_name varchar(20)  NULL ,
	DB_name varchar(40)  NULL ,
	LinkServerName varchar(20) NULL, 
    DBSource varchar(50) NULL
)
go

ALTER TABLE arm
	ADD CONSTRAINT XPKarm PRIMARY KEY  CLUSTERED (arm_id ASC)
go


CREATE TABLE replic
(
	replic_id int IDENTITY (1,1) ,
	ReplicName varchar(20)  NULL ,
	arm_id int  NULL 
)
go


ALTER TABLE replic
	ADD CONSTRAINT XPKreplic PRIMARY KEY  CLUSTERED (replic_id ASC)
go


CREATE TABLE Replic_call
(
	call_id int IDENTITY (1,1) ,
	session_id int  NOT NULL ,
	result_id int  NULL 
)
go


ALTER TABLE Replic_call
	ADD CONSTRAINT XPKReplic_call PRIMARY KEY  CLUSTERED (call_id ASC,session_id ASC)
go


CREATE TABLE replic_change
(
	replic_id int  NOT NULL ,
	session_id int  NOT NULL ,
	result_id int  NULL 
)
go


ALTER TABLE replic_change
	ADD CONSTRAINT XPKreplic_change PRIMARY KEY  CLUSTERED (replic_id ASC,session_id ASC)
go


CREATE TABLE result
(
	result_id int IDENTITY (1,1) ,
	NameResult varchar(20)  NULL 
)
go


ALTER TABLE result
	ADD CONSTRAINT XPKresult PRIMARY KEY  CLUSTERED (result_id ASC)
go


CREATE TABLE Session
(
	session_id int IDENTITY (1,1) ,
	SessionDate datetime  NULL ,
	Operation varchar(20)  NULL 
)
go


ALTER TABLE Session
	ADD CONSTRAINT XPKSession PRIMARY KEY  CLUSTERED (session_id ASC)
go



ALTER TABLE replic
	ADD CONSTRAINT  R_14 FOREIGN KEY (arm_id) REFERENCES arm(arm_id)
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go



ALTER TABLE Replic_call
	ADD CONSTRAINT  R_21 FOREIGN KEY (session_id) REFERENCES Session(session_id)
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go

ALTER TABLE Replic_call
	ADD CONSTRAINT  R_23 FOREIGN KEY (result_id) REFERENCES result(result_id)
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go



ALTER TABLE replic_change
	ADD CONSTRAINT  R_19 FOREIGN KEY (replic_id) REFERENCES replic(replic_id)
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go

ALTER TABLE replic_change
	ADD CONSTRAINT  R_20 FOREIGN KEY (session_id) REFERENCES Session(session_id)
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go

ALTER TABLE replic_change
	ADD CONSTRAINT  R_22 FOREIGN KEY (result_id) REFERENCES result(result_id)
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go

USE [adm]
GO
/****** Объект:  StoredProcedure [dbo].[CopyCall_id]    Дата сценария: 12/24/2007 09:58:17 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[CopyCall_id] 
	-- Add the parameters for the stored procedure here
	@Call_id as int,
    @replicName as varchar(50),
    @LinkSertver as varchar(50),
    @dbServer as varchar(50)
AS
BEGIN



BEGIN TRANSACTION 
exec('use '+@replicName+';
if not exists(select id
from dbo.call where id='+@Call_id+' )
 
BEGIN

BEGIN TRANSACTION
SET IDENTITY_INSERT dbo.call ON

insert into dbo.call (id,filename,aon,direction,source_type,state,auto_deletion,dialed_number,dialed_number_format,begin_time,end_time,channel,pulse_number,tone_number)
select id,filename,aon,direction,source_type,state,auto_deletion,dialed_number,dialed_number_format,begin_time,end_time,channel,pulse_number,tone_number
from OPENQUERY('+@LinkSertver+',''select id,filename,aon,direction,source_type,state,auto_deletion,dialed_number,dialed_number_format,begin_time,end_time,channel,pulse_number,tone_number from '+@dbServer+'.dbo.call
               where id='+@Call_id+''') t1
where t1.id not in 
(select id
 from dbo.call
)

SET IDENTITY_INSERT dbo.call OFF
COMMIT TRANSACTION

select result=1

END  
ELSE
begin
  select result=222222
end
'
)
   -- Insert statements for procedure here
COMMIT TRANSACTION

END

GO

USE [adm]
GO
/****** Объект:  StoredProcedure [dbo].[CreateDataBase]    Дата сценария: 12/18/2007 14:34:19 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[CreateDataBase]
   @NameBase as varchar(200) 
AS
BEGIN

declare @replic_id as int;
declare @session_id as int;
declare @ExistsReplic as int;
declare @Result as int;
select @Result=-1;

SELECT @ExistsReplic =replic_id
FROM [adm].[dbo].[exists_replic](@NameBase)

if (@ExistsReplic=-1)
begin

insert into session (SessionDate,Operation)
values (GetDate(),'Создание реплики');
select @session_id=IDENT_CURRENT( 'session' )

INSERT INTO [adm].[dbo].[replic]([ReplicName],[arm_id])
VALUES (@NameBase,1);
select @replic_id=IDENT_CURRENT( 'replic' );

INSERT INTO [adm].[dbo].[replic_change] ([replic_id],[session_id],[result_id])
VALUES(@replic_id,@session_id,1);

             
execute('CREATE DATABASE '+@NameBase+';');

execute ('USE '+@NameBase+';

CREATE TABLE RecordingStation
(
	ip				int			NOT NULL,
	port			smallint	NOT NULL,
	name			char(64),
	
	PRIMARY KEY (ip,port)
);

CREATE TABLE Topic
(
	id				int			NOT NULL,
	name			char(64),
	description		varchar(256)
	
	PRIMARY KEY (id),
	UNIQUE (name)
);

CREATE TABLE AccessCategory
(
	id				int			NOT NULL,
	name			char(64),
	color			int,
	
	PRIMARY KEY (id),
	UNIQUE (name)
);

CREATE TABLE Comrade
(
	id				int			NOT NULL,
	number			char(64),
	name			varchar(128),
	
	PRIMARY KEY (id)
);

CREATE TABLE Department
(
	code					int NOT NULL,
	name					varchar(256)
	
	PRIMARY KEY (code)
);


CREATE TABLE CompanyStructure
(
	daughter				int NOT NULL,
	parent					int NOT NULL,
	
	PRIMARY KEY (daughter),	
	FOREIGN KEY (daughter) REFERENCES Department(code)	ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (parent) REFERENCES Department(code)	ON UPDATE NO ACTION	ON DELETE NO ACTION
);

CREATE TABLE Staff
(
	comrade					int NOT NULL,
	department				int NOT NULL,
	
	PRIMARY KEY (comrade),
	
	FOREIGN KEY (comrade) REFERENCES Comrade(id)			ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (department) REFERENCES Department(code)	ON UPDATE CASCADE	ON DELETE CASCADE
);



-- собственная рабочая часть БД

CREATE TABLE Device
(
	id					int				NOT NULL	IDENTITY,
	device_id			varchar(128)	NOT NULL,
	settings			varchar(1024),
	
	PRIMARY KEY (id),
	UNIQUE (device_id)
);

CREATE TABLE Channel
(
	id					int		NOT NULL	IDENTITY,
	name				varchar(256),
	
	PRIMARY KEY (id)
);


CREATE TABLE StoredChannel
(
	id					int		NOT NULL,
	source_phantom		char(64),
	origin_channel_id	varchar(136),
	
	PRIMARY KEY (id),		
	FOREIGN KEY (id) REFERENCES Channel(id)	ON UPDATE CASCADE	ON DELETE CASCADE,
);

CREATE TABLE ActiveChannel
(
	id					int		NOT NULL,
	number				int		NOT NULL,
	settings			varchar(1024),
	device				int		NOT NULL,
	access_category		int		NOT NULL,
	comment				varchar(256),
	
	PRIMARY KEY (id),	
	FOREIGN KEY (id) REFERENCES Channel(id)							ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (device) REFERENCES Device(id)						ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (access_category) REFERENCES AccessCategory(id)		ON UPDATE CASCADE	ON DELETE CASCADE,
);

CREATE TABLE Call
(
	id						int		NOT NULL	IDENTITY,
	filename				char(8),
	aon						varchar(64),
	direction				char,
	source_type				char,
	state					char,
	auto_deletion			char,
	dialed_number			varchar(128),
	dialed_number_format	varchar(128),
	begin_time				datetime,
	end_time				datetime,
	channel					int,	
	
	pulse_number			varchar(64),
	tone_number				varchar(64),
	
	PRIMARY KEY (id),	
	FOREIGN KEY (channel) REFERENCES StoredChannel(id)	ON UPDATE CASCADE	ON DELETE CASCADE,

	CHECK (direction IN (''I'',''O'',''U'')),
	CHECK (source_type IN (''P'',''F'',''V'',''D'')),
	CHECK (state IN (''A'',''C'',''R'')),
	CHECK (auto_deletion IN (''Y'',''N''))
);


CREATE TABLE CallPath
(
	id						int		NOT NULL	IDENTITY,
	call					int		NOT NULL,
	time					datetime,
	number					varchar(64),
	
	PRIMARY KEY (id),
	UNIQUE (call,time),	
	FOREIGN KEY (call) REFERENCES Call(id)	ON UPDATE CASCADE	ON DELETE CASCADE
);

CREATE TABLE PBXInfo
(
	call					int		NOT NULL,
	call_id					varchar(128),
	class					int,
	
	PRIMARY KEY (call),
	FOREIGN KEY (call) REFERENCES Call(id)	ON UPDATE CASCADE	ON DELETE CASCADE
);

CREATE TABLE AdditionalInfo
(
	call					int		NOT NULL,
	comment					varchar(256),

	PRIMARY KEY (call),
	FOREIGN KEY (call) REFERENCES Call(id)	ON UPDATE CASCADE	ON DELETE CASCADE
);

CREATE TABLE CallActor
(
	call					int		NOT NULL,
	comrade					int		NOT NULL,
	
	PRIMARY KEY (call,comrade),
	FOREIGN KEY (call) REFERENCES Call(id)			ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (comrade) REFERENCES Comrade(id)	ON UPDATE CASCADE	ON DELETE CASCADE
);

CREATE TABLE CallTopic
(
	call					int		NOT NULL,
	topic					int		NOT NULL,
	
	PRIMARY KEY (call,topic),
	FOREIGN KEY (call) REFERENCES Call(id)			ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (topic) REFERENCES Topic(id)		ON UPDATE CASCADE	ON DELETE CASCADE	
);

CREATE TABLE CallAccessCategory
(
	call					int		NOT NULL,
	access_category			int		NOT NULL,

	PRIMARY KEY (call),
	FOREIGN KEY (call) REFERENCES Call(id)							ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (access_category) REFERENCES AccessCategory(id)		ON UPDATE CASCADE	ON DELETE CASCADE
);


CREATE TABLE PhonedBy
(
	call_path			int		NOT NULL,
	comrade				int		NOT NULL,
	
	PRIMARY KEY (call_path),
	FOREIGN KEY (call_path) REFERENCES CallPath(id)			ON UPDATE CASCADE	ON DELETE CASCADE,
	FOREIGN KEY (comrade) REFERENCES Comrade(id)			ON UPDATE CASCADE	ON DELETE CASCADE
);
'
);
  PRINT('Ну типа функция создала базу данных '+@NameBase);
 select @result=1;

  end;

else
 begin
	print 'Реплика '+@NameBase+' уже существует.'

	insert into session (SessionDate,Operation)
	values (GetDate(),'Создание реплики');
	select @session_id=IDENT_CURRENT( 'session' )

--	INSERT INTO [adm].[dbo].[replic]([ReplicName],[arm_id])
--  VALUES (@NameBase,1);
    
    select @replic_id=r.replic_id
    from replic r
    where r.ReplicName=@NameBase

	INSERT INTO [adm].[dbo].[replic_change] ([replic_id],[session_id],[result_id])
	VALUES(@replic_id,@session_id,3);
    select @result=2;
  end

select result=@result;

END
go


-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
create PROCEDURE CreateLinkServer 
	@NameLinkServer as varchar(20),
	@DataSource as varchar(20),
    @LocalUser as varchar(20),
    @remouteUser as varchar(20),
    @remoutePass as varchar(20)

AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
declare @sqlstr as varchar(255);
declare @result as int;

select @result=2;

--select @sqlstr=

exec sp_dropserver @NameLinkServer, 'droplogins';

exec(
  'EXEC sp_addlinkedserver   
  @server='''+@NameLinkServer+''', 
  @srvproduct='''',
  @provider=''SQLNCLI'', 
  @datasrc='''+@DataSource+''''
    );

exec('EXEC master.dbo.sp_addlinkedsrvlogin @rmtsrvname=N'''+@NameLinkServer+''',@useself=N''False'',@locallogin=N'''+ @LocalUser+''',@rmtuser=N'''+@remouteUser+''',@rmtpassword='''+@remoutePass+'''');

declare @servername as sysname;

select  @servername=@NameLinkServer

 --exec @result = sp_testlinkedserver @servername=@NameLinkServer ;
 
  select 'result'=@result+1;

-- Insert statements for procedure here

END
GO

use adm;
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE PreReplication 
   @name_db as varchar(50),
   @LinkServer as varchar(50),
   @DBSource as varchar(50)

AS
BEGIN
  SET NOCOUNT ON; 



      begin transaction
       exec('use '+@name_db+'
        delete from channel
        SET IDENTITY_INSERT channel ON
        insert into channel (id,name)
        select id,name
        from OPENQUERY('+@linkServer+',''select id,name from '+@DBSource+'.dbo.channel'')
        SET IDENTITY_INSERT channel OFF
       ');

        exec ('use '+@name_db+';
               delete from storedchannel;
               insert into storedchannel (id,source_phantom,origin_channel_id)
               select id,source_phantom,origin_channel_id
               from OPENQUERY('+ @LinkServer+',''select id,source_phantom,origin_channel_id from '+@DBSource+'.dbo.storedchannel'')
              ');
commit transaction
	select 1;
END
GO

use adm;
-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

CREATE PROCEDURE TestCall_id
   	@Call_id as int,
    @replicName as varchar(50),
    @LinkSertver as varchar(50),
    @dbServer as varchar(50)
AS
BEGIN
	SET NOCOUNT ON;

exec ('if not exists(select id
from OPENQUERY('+@LinkSertver+',''select id from '+@dbServer+'.dbo.call
               where id='+@Call_id+''')
                  where id not in 
                     (select id
                       from '+@replicName+'.dbo.call
                     )
)
begin
RAISERROR(''is'', 17, 1)
 
end

');
 select result=11 
END
GO

USE [adm]
GO
/****** Объект:  UserDefinedFunction [dbo].[exists_replic]    Дата сценария: 12/19/2007 10:10:17 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE FUNCTION [dbo].[exists_replic] (@replicName varchar(20))
RETURNS @ResTable TABLE
(
  replic_id int,
  replic_name  varchar(20)
)

AS
begin

declare @id as int;

if exists (select 1
           from ADM.dbo.replic r
           where r.replicName=@replicName
           )
begin

           select @id=r.replic_id
           from ADM.dbo.replic r
           where r.replicName=@replicName
     insert @ResTable
     select @id, @replicName 
end
else
begin 
   insert @ResTable
   select -1, ' '
end
  
RETURN 
end;

go

SET IDENTITY_INSERT [adm].[dbo].[result] ON

INSERT INTO [adm].[dbo].[result]       ([result_id],[NameResult])
     VALUES(1,'Выполнено')
go
INSERT INTO [adm].[dbo].[result]       ([result_id],[NameResult])
     VALUES(2,'Не выполнено')
go
INSERT INTO [adm].[dbo].[result]       ([result_id],[NameResult])
     VALUES(3,'Реплика сущесвует')
go

SET IDENTITY_INSERT [adm].[dbo].[result] OFF

go

SET IDENTITY_INSERT [adm].[dbo].[arm] ON
go

INSERT INTO [adm].[dbo].[arm]
           ([arm_id],
            [arm_ip],
            [arm_name],
            [DB_name],
            [LinkServerName],
            [DBSource])
     VALUES
           (1,
            '127.0.0.1',
            'server',
            'comp_IP\instance_name',
            'SERVER_PHANTOM',
            'master')
go

SET IDENTITY_INSERT [adm].[dbo].[arm] OFF
go