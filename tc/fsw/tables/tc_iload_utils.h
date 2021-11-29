/*=======================================================================================
** File Name:  tc_iload_utils.h
**
** Title:  ILoad Tables' Utility Header File for TC Application
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  To define TC's ILoad table-related utility functions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-11-28 | Team 1 | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TC_ILOAD_UTILS_H_
#define _TC_ILOAD_UTILS_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "tc_app.h"

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

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
int32  TC_InitILoadTbl(void);
int32  TC_ValidateILoadTbl(TC_ILoadTblEntry_t*);
void   TC_ProcessNewILoadTbl(void);

#endif /* _TC_ILOAD_UTILS_H_ */

/*=======================================================================================
** End of file tc_iload_utils.h
**=====================================================================================*/
    