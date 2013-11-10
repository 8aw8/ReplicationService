MessageIdTypedef = NTSTATUS

SeverityNames = (
	Success		  = 0x0:STATUS_SEVERITY_SUCCESS
	Informational = 0x1:STATUS_SEVERITY_INFORMATIONAL
	Warning		  = 0x2:STATUS_SEVERITY_WARNING
	Error		  = 0x3:STATUS_SEVERITY_ERROR
	)

FacilityNames = (
	System		= 0x0
	RpcRuntime	= 0x2:FACILITY_RPC_RUNTIME
	RpcStubs	= 0x3:FACILITY_RPC_STUBS
	Io			= 0x4:FACILITY_IO_ERROR_CODE
	MDIS		= 0x15:FACILITY_MDIS
	)

LanguageNames = (
	English		= 0x409:EvtMsgEn
	)

MessageId=0x0001
Facility=MDIS
Severity=Success
SymbolicName=EVENTLOG_MSG_INIT_SUCCESS
Language=English
Initialized successfully.
.

MessageId=+1
Facility=MDIS
Severity=Success
SymbolicName=EVENTLOG_MSG_ZNET_SUCCESS
Language=English
Successfully connect to Phantom.
.

MessageId=+1
Facility=MDIS
Severity=Success
SymbolicName=EVENTLOG_MSG_ADO_SUCCESS
Language=English
Successfully connet to DB.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_ADO_ERROR
Language=English
Connection to DB is closed.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_ZNET_ERROR
Language=English
Connection to Phantom is closed.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_RECORD_ERROR
Language=English
Error: failed to process record '%1'.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_INIT_FAILED_REGISTRY
Language=English
Initialization error: failed to open windows registry.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_INIT_FAILED_REGISTRY_SETTINGS
Language=English
Initialization error: failed to read settings from windows registry.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_INIT_FAILED_PHANTOM
Language=English
Initialization error: failed to initialize Phantom enviroment.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_INIT_FAILED_VOLUME
Language=English
Initialization error: failed to open volume '%1'.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_INIT_FAILED_JOURNAL
Language=English
Initialization error: failed to create change journal.
.

MessageId=+1
Facility=MDIS
Severity=Error
SymbolicName=EVENTLOG_MSG_INIT_FAILED_JOURNAL_START
Language=English
Initialization error: failed to start change journal.
.

MessageId=+1
Facility=MDIS
Severity=Warning
SymbolicName=EVENTLOG_MSG_INIT_PROBLEM_ADO
Language=English
Initialization problem: failed to connect to DB.
.

MessageId=+1
Facility=MDIS
Severity=Warning
SymbolicName=EVENTLOG_MSG_FILE_EXPORT_PROBLEM
Language=English
Error: failed to export file.
.
