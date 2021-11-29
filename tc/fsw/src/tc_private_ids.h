/*=======================================================================================
** File Name:  tc_private_ids.h
**
** Title:  ID Header File for TC Application
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  This header file contains declarations and definitions of TC's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-11-28 | Team 1 | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TC_PRIVATE_IDS_H_
#define _TC_PRIVATE_IDS_H_

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
*/

/* Event IDs */
#define TC_RESERVED_EID  0

#define TC_INF_EID        1
#define TC_INIT_INF_EID   2
#define TC_ILOAD_INF_EID  3
#define TC_CDS_INF_EID    4
#define TC_CMD_INF_EID    5

#define TC_ERR_EID         51
#define TC_INIT_ERR_EID    52
#define TC_ILOAD_ERR_EID   53
#define TC_CDS_ERR_EID     54
#define TC_CMD_ERR_EID     55
#define TC_PIPE_ERR_EID    56
#define TC_MSGID_ERR_EID   57
#define TC_MSGLEN_ERR_EID  58

#define TC_EVT_CNT  14

/*
** Local Structure Declarations
*/

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

#endif /* _TC_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file tc_private_ids.h
**=====================================================================================*/
    