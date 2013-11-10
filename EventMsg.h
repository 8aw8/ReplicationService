//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_RPC_STUBS               0x3
#define FACILITY_RPC_RUNTIME             0x2
#define FACILITY_MDIS                    0x15
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: EVENTLOG_MSG_INIT_SUCCESS
//
// MessageText:
//
//  Initialized successfully.
//
#define EVENTLOG_MSG_INIT_SUCCESS        ((NTSTATUS)0x20150001L)

//
// MessageId: EVENTLOG_MSG_ZNET_SUCCESS
//
// MessageText:
//
//  Successfully connect to Phantom.
//
#define EVENTLOG_MSG_ZNET_SUCCESS        ((NTSTATUS)0x20150002L)

//
// MessageId: EVENTLOG_MSG_ADO_SUCCESS
//
// MessageText:
//
//  Successfully connet to DB.
//
#define EVENTLOG_MSG_ADO_SUCCESS         ((NTSTATUS)0x20150003L)

//
// MessageId: EVENTLOG_MSG_ADO_ERROR
//
// MessageText:
//
//  Connection to DB is closed.
//
#define EVENTLOG_MSG_ADO_ERROR           ((NTSTATUS)0xE0150004L)

//
// MessageId: EVENTLOG_MSG_ZNET_ERROR
//
// MessageText:
//
//  Connection to Phantom is closed.
//
#define EVENTLOG_MSG_ZNET_ERROR          ((NTSTATUS)0xE0150005L)

//
// MessageId: EVENTLOG_MSG_RECORD_ERROR
//
// MessageText:
//
//  Error: failed to process record '%1'.
//
#define EVENTLOG_MSG_RECORD_ERROR        ((NTSTATUS)0xE0150006L)

//
// MessageId: EVENTLOG_MSG_INIT_FAILED_REGISTRY
//
// MessageText:
//
//  Initialization error: failed to open windows registry.
//
#define EVENTLOG_MSG_INIT_FAILED_REGISTRY ((NTSTATUS)0xE0150007L)

//
// MessageId: EVENTLOG_MSG_INIT_FAILED_REGISTRY_SETTINGS
//
// MessageText:
//
//  Initialization error: failed to read settings from windows registry.
//
#define EVENTLOG_MSG_INIT_FAILED_REGISTRY_SETTINGS ((NTSTATUS)0xE0150008L)

//
// MessageId: EVENTLOG_MSG_INIT_FAILED_PHANTOM
//
// MessageText:
//
//  Initialization error: failed to initialize Phantom enviroment.
//
#define EVENTLOG_MSG_INIT_FAILED_PHANTOM ((NTSTATUS)0xE0150009L)

//
// MessageId: EVENTLOG_MSG_INIT_FAILED_VOLUME
//
// MessageText:
//
//  Initialization error: failed to open volume '%1'.
//
#define EVENTLOG_MSG_INIT_FAILED_VOLUME  ((NTSTATUS)0xE015000AL)

//
// MessageId: EVENTLOG_MSG_INIT_FAILED_JOURNAL
//
// MessageText:
//
//  Initialization error: failed to create change journal.
//
#define EVENTLOG_MSG_INIT_FAILED_JOURNAL ((NTSTATUS)0xE015000BL)

//
// MessageId: EVENTLOG_MSG_INIT_FAILED_JOURNAL_START
//
// MessageText:
//
//  Initialization error: failed to start change journal.
//
#define EVENTLOG_MSG_INIT_FAILED_JOURNAL_START ((NTSTATUS)0xE015000CL)

//
// MessageId: EVENTLOG_MSG_INIT_PROBLEM_ADO
//
// MessageText:
//
//  Initialization problem: failed to connect to DB.
//
#define EVENTLOG_MSG_INIT_PROBLEM_ADO    ((NTSTATUS)0xA015000DL)

//
// MessageId: EVENTLOG_MSG_FILE_EXPORT_PROBLEM
//
// MessageText:
//
//  Error: failed to export file.
//
#define EVENTLOG_MSG_FILE_EXPORT_PROBLEM ((NTSTATUS)0xA015000EL)

