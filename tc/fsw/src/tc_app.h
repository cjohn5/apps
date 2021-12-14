
/**=======================================================================================
** File Name:  tc_app.h
**
** Title:  Header File for TC Application
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  To define TC's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-11-28 | Team 1 | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TC_APP_H_
#define _TC_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "tc_platform_cfg.h"
#include "tc_mission_cfg.h"
#include "tc_private_ids.h"
#include "tc_private_types.h"
#include "tc_perfids.h"
#include "tc_msgids.h"
#include "tc_msg.h"

 
#include "tc_iload_utils.h"

/*
** Local Defines
*/
#define TC_TIMEOUT_MSEC    1000

#define TC_STATE_READY      0
#define TC_STATE_MONITORING 1
#define TC_STATE_HEATING    2
#define TC_STATE_COOLING    3
#define TC_STATE_ERROR      4

#define TC_LOG_DEBUG   0
#define TC_LOG_INFO    1
#define TC_LOG_WARNING 2
#define TC_LOG_ERROR   3
#define TC_LOG_DEFAULT_STATE TC_LOG_DEBUG
/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[TC_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;
    
    /* ILoad table-related */
    CFE_TBL_Handle_t  ILoadTblHdl;
    TC_ILoadTblEntry_t*  ILoadTblPtr;
    TC_ILoadTblEntry_t ThresholdTemps;
    
    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in tc/fsw/src/tc_private_types.h */
    TC_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in tc/fsw/src/tc_private_types.h */
    TC_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in tc/fsw/src/tc_msg.h */
    TC_HkTlm_t  HkTlm;

    whe_cmd_val_pipe whe_cmd;
    wise_cmd_val_pipe wise_cmd;

    /* TODO:  Add declarations for additional private data here */
} TC_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
int32  TC_InitApp(void);
int32  TC_InitEvent(void);
int32  TC_InitData(void);
int32  TC_InitPipe(void);

void  TC_AppMain(void);

void  TC_CleanupCallback(void);

int32  TC_RcvMsg(int32 iBlocking);

void  TC_ProcessNewData(void);
void  TC_ProcessNewCmds(void);
void  TC_ProcessNewAppCmds(CFE_SB_Msg_t*);

void TC_ProcessWiseTlm(void* TlmMsgPtr);

void  TC_ReportHousekeeping(void);
void  TC_SendOutData(void);

boolean  TC_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _TC_APP_H_ */

/*=======================================================================================
** End of file tc_app.h
**=====================================================================================*/
    
