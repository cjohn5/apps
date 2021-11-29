/*=======================================================================================
** File Name:  tc_msg.h
**
** Title:  Message Definition Header File for TC Application
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  To define TC's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-11-28 | Team 1 | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TC_MSG_H_
#define _TC_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/

 
#include "tc_iload_utils.h"

/*
** Local Defines
*/

/*
** TC command codes
*/
#define TC_NOOP_CC                 0
#define TC_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;
    uint16	       WISE_Tlm_Packets_Rcvd;
    uint8	       TCA_Current_State;
    uint8              TCA_Logging_State;

} TC_HkTlm_t;

typedef struct
{
    uint8 	      CmdHeader[CFE_SB_CMD_HDR_SIZE];

} whe_cmd_val_pipe;


#endif /* _TC_MSG_H_ */

/*=======================================================================================
** End of file tc_msg.h
**=====================================================================================*/
    
