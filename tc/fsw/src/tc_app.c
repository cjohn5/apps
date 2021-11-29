/*=======================================================================================
** File Name:  tc_app.c
**
** Title:  Function Definitions for TC Application
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  This source file contains all necessary function definitions to run TC
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-11-28 | Team 1 | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>

#include "cfe.h"

#include "tc_platform_cfg.h"
#include "tc_mission_cfg.h"
#include "tc_app.h"
#include "whe_msg.h"
#include "whe_msgids.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
TC_AppData_t  g_TC_AppData;
int tlmDebug = 0;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/
    
/*=====================================================================================
** Name: TC_InitEvent
**
** Purpose: To initialize and register event table for TC application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    TC_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_TC_AppData.EventTbl
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_TC_AppData.EventTbl, 0x00, sizeof(g_TC_AppData.EventTbl));

    g_TC_AppData.EventTbl[0].EventID = TC_RESERVED_EID;
    g_TC_AppData.EventTbl[1].EventID = TC_INF_EID;
    g_TC_AppData.EventTbl[2].EventID = TC_INIT_INF_EID;
    g_TC_AppData.EventTbl[3].EventID = TC_ILOAD_INF_EID;
    g_TC_AppData.EventTbl[4].EventID = TC_CDS_INF_EID;
    g_TC_AppData.EventTbl[5].EventID = TC_CMD_INF_EID;

    g_TC_AppData.EventTbl[ 6].EventID = TC_ERR_EID;
    g_TC_AppData.EventTbl[ 7].EventID = TC_INIT_ERR_EID;
    g_TC_AppData.EventTbl[ 8].EventID = TC_ILOAD_ERR_EID;
    g_TC_AppData.EventTbl[ 9].EventID = TC_CDS_ERR_EID;
    g_TC_AppData.EventTbl[10].EventID = TC_CMD_ERR_EID;
    g_TC_AppData.EventTbl[11].EventID = TC_PIPE_ERR_EID;
    g_TC_AppData.EventTbl[12].EventID = TC_MSGID_ERR_EID;
    g_TC_AppData.EventTbl[13].EventID = TC_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_TC_AppData.EventTbl,
                               TC_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TC - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: TC_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for TC application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    TC_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_TC_AppData.usSchPipeDepth
**    g_TC_AppData.cSchPipeName
**    g_TC_AppData.SchPipeId
**    g_TC_AppData.usCmdPipeDepth
**    g_TC_AppData.cCmdPipeName
**    g_TC_AppData.CmdPipeId
**    g_TC_AppData.usTlmPipeDepth
**    g_TC_AppData.cTlmPipeName
**    g_TC_AppData.TlmPipeId
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_TC_AppData.usSchPipeDepth = TC_SCH_PIPE_DEPTH;
    memset((void*)g_TC_AppData.cSchPipeName, '\0', sizeof(g_TC_AppData.cSchPipeName));
    strncpy(g_TC_AppData.cSchPipeName, "TC_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_TC_AppData.SchPipeId,
                                 g_TC_AppData.usSchPipeDepth,
                                 g_TC_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(TC_WAKEUP_MID, g_TC_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TC - Sch Pipe failed to subscribe to TC_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto TC_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("TC - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto TC_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_TC_AppData.usCmdPipeDepth = TC_CMD_PIPE_DEPTH ;
    memset((void*)g_TC_AppData.cCmdPipeName, '\0', sizeof(g_TC_AppData.cCmdPipeName));
    strncpy(g_TC_AppData.cCmdPipeName, "TC_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_TC_AppData.CmdPipeId,
                                 g_TC_AppData.usCmdPipeDepth,
                                 g_TC_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(TC_CMD_MID, g_TC_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TC - CMD Pipe failed to subscribe to TC_CMD_MID. (0x%08X)\n", iStatus);
            goto TC_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(TC_SEND_HK_MID, g_TC_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TC - CMD Pipe failed to subscribe to TC_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto TC_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("TC - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto TC_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_TC_AppData.usTlmPipeDepth = TC_TLM_PIPE_DEPTH;
    memset((void*)g_TC_AppData.cTlmPipeName, '\0', sizeof(g_TC_AppData.cTlmPipeName));
    strncpy(g_TC_AppData.cTlmPipeName, "TC_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_TC_AppData.TlmPipeId,
                                 g_TC_AppData.usTlmPipeDepth,
                                 g_TC_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_TC_AppData.TlmPipeId);
        */

	CFE_SB_Subscribe(WHE_HK_TLM_MID, g_TC_AppData.TlmPipeId);
    }
    else
    {
        CFE_ES_WriteToSysLog("TC - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto TC_InitPipe_Exit_Tag;
    }

TC_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: TC_InitData
**
** Purpose: To initialize global variables used by TC application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    TC_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_TC_AppData.InData
**    g_TC_AppData.OutData
**    g_TC_AppData.HkTlm
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_TC_AppData.InData, 0x00, sizeof(g_TC_AppData.InData));

    /* Init output data */
    memset((void*)&g_TC_AppData.OutData, 0x00, sizeof(g_TC_AppData.OutData));
    CFE_SB_InitMsg(&g_TC_AppData.OutData,
                   TC_OUT_DATA_MID, sizeof(g_TC_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_TC_AppData.HkTlm, 0x00, sizeof(g_TC_AppData.HkTlm));
    CFE_SB_InitMsg(&g_TC_AppData.HkTlm,
                   TC_HK_TLM_MID, sizeof(g_TC_AppData.HkTlm), TRUE);

    memset((void*)&g_TC_AppData.whe_cmd, 0x00, sizeof(g_TC_AppData.whe_cmd));
    CFE_SB_InitMsg(&g_TC_AppData.whe_cmd,
                   WHE_CMD_MID, sizeof(g_TC_AppData.whe_cmd), TRUE);

    return (iStatus);
}
    
/*=====================================================================================
** Name: TC_InitApp
**
** Purpose: To initialize all data local to and used by TC application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    TC_InitEvent
**    TC_InitPipe
**    TC_InitData
**
** Called By:
**    TC_AppMain
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_TC_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TC - Failed to register the app (0x%08X)\n", iStatus);
        goto TC_InitApp_Exit_Tag;
    }

    if ((TC_InitEvent() != CFE_SUCCESS) || 
        (TC_InitPipe() != CFE_SUCCESS) || 
        (TC_InitData() != CFE_SUCCESS) ||
        (TC_InitILoadTbl() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto TC_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&TC_CleanupCallback);

TC_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(TC_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "TC - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("TC - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: TC_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
}
    
/*=====================================================================================
** Name: TC_RcvMsg
**
** Purpose: To receive and process messages for TC application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization 
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    TC_ProcessNewCmds
**    TC_ProcessNewData
**    TC_SendOutData
**
** Called By:
**    TC_Main
**
** Global Inputs/Reads:
**    g_TC_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_TC_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(TC_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_TC_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TC_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case TC_WAKEUP_MID:
                TC_ProcessNewCmds();
                TC_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */

                /* The last thing to do at the end of this Wakeup cycle should be to
                   automatically publish new output. */
                TC_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(TC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "TC - Recvd invalid SCH msgId (0x%08X)", MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or nothing */
    }
    else
    {
        /* This is an example of exiting on an error.
        ** Note that a SB read error is not always going to result in an app quitting.
        */
        CFE_EVS_SendEvent(TC_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "TC: SB pipe read error (0x%08X), app will exit", iStatus);
        g_TC_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}

void TC_ProcessWheTlm(void* TlmMsgPtr){
   whe_hk_tlm_t* whe_tlm_ptr = (whe_hk_tlm_t*)TlmMsgPtr;
   tlmDebug++;

   if(whe_tlm_ptr->whe_temp > 20 && (whe_tlm_ptr->whe_sbc_state == 1 || whe_tlm_ptr->whe_sbc_state == 0)){
       if(whe_tlm_ptr->whe_htr == 0){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_HTR_OFF_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
   }

   if(whe_tlm_ptr->whe_temp < 20 && (whe_tlm_ptr->whe_sbc_state == 1 || whe_tlm_ptr->whe_sbc_state == 0)){
       if(whe_tlm_ptr->whe_louver == 0){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_LOUVER_CLOSE_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
   }


   if(whe_tlm_ptr->whe_temp < 15 && (whe_tlm_ptr->whe_sbc_state == 1 || whe_tlm_ptr->whe_sbc_state == 2)){
       if(whe_tlm_ptr->whe_louver == 1){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_LOUVER_OPEN_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
       if(whe_tlm_ptr->whe_htr == 1){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_HTR_OFF_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
   }

	
   if(whe_tlm_ptr->whe_temp > 25 && (whe_tlm_ptr->whe_sbc_state == 1 || whe_tlm_ptr->whe_sbc_state == 2)){
       if(whe_tlm_ptr->whe_louver == 0){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_LOUVER_OPEN_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
       if(whe_tlm_ptr->whe_htr == 0){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_HTR_OFF_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
   }

      if(whe_tlm_ptr->whe_temp <= 5 && (whe_tlm_ptr->whe_sbc_state == 0 )){
       if(whe_tlm_ptr->whe_louver == 1){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_LOUVER_OPEN_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
       if(whe_tlm_ptr->whe_htr == 1){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_HTR_OFF_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
   }

	
   if(whe_tlm_ptr->whe_temp >= 40 && (whe_tlm_ptr->whe_sbc_state == 0)){
       if(whe_tlm_ptr->whe_louver == 0){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_LOUVER_OPEN_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
       if(whe_tlm_ptr->whe_htr == 0){
           CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_HTR_OFF_CC);
           CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
           CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       }
   }\
/*
   if(tlmDebug == 5){
       CFE_SB_SetCmdCode((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd, WHE_THERM_HTR_ON_CC);
       CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
       CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.whe_cmd);
   }
*/
   CFE_ES_WriteToSysLog("Temp is: %i, Heater state is: %i\n, Louver state is: %i", whe_tlm_ptr->whe_temp, whe_tlm_ptr->whe_htr, whe_tlm_ptr->whe_louver );
   //CFE_ES_WriteToSysLog("#Tlm Recieved: %i\n", tlmDebug);
}
    
/*=====================================================================================
** Name: TC_ProcessNewData
**
** Purpose: To process incoming data subscribed by TC application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**
** Called By:
**    TC_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_TC_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                /* TODO:  Add code to process all subscribed data here 
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         TC_ProcessNavData(TlmMsgPtr);
                **         break;
                */
		case(WHE_HK_TLM_MID):
			TC_ProcessWheTlm(TlmMsgPtr);
                        break;

                default:
                    CFE_EVS_SendEvent(TC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "TC - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(TC_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "TC: CMD pipe read error (0x%08X)", iStatus);
            g_TC_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}


    
/*=====================================================================================
** Name: TC_ProcessNewCmds
**
** Purpose: To process incoming command messages for TC application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    TC_ProcessNewAppCmds
**    TC_ReportHousekeeping
**
** Called By:
**    TC_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_TC_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case TC_CMD_MID:
                    TC_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case TC_SEND_HK_MID:
                    TC_ReportHousekeeping();
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         TC_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(TC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "TC - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(TC_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "TC: CMD pipe read error (0x%08X)", iStatus);
            g_TC_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: TC_ProcessNewAppCmds
**
** Purpose: To process command messages targeting TC application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    TC_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_TC_AppData.HkTlm.usCmdCnt
**    g_TC_AppData.HkTlm.usCmdErrCnt
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case TC_NOOP_CC:
                g_TC_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(TC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "TC - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case TC_RESET_CC:
                g_TC_AppData.HkTlm.usCmdCnt = 0;
                g_TC_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(TC_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "TC - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            /* TODO:  Add code to process the rest of the TC commands here */

            default:
                g_TC_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(TC_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "TC - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: TC_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TC_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, Team 1
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: TC_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TC_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TC_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TC_AppData.OutData);
}
    
/*=====================================================================================
** Name: TC_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    TC_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean TC_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(TC_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "TC - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_TC_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}
    
/*=====================================================================================
** Name: TC_AppMain
**
** Purpose: To define TC application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    TC_InitApp
**    TC_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TC_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (TC_InitApp() != CFE_SUCCESS)
    {
        g_TC_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(TC_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(TC_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(TC_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_TC_AppData.uiRunStatus) == TRUE)
    {
        TC_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(TC_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_TC_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file tc_app.c
**=====================================================================================*/
    
