/*=======================================================================================
** File Name:  tc_tbldefs.h
**
** Title:  Header File for TC Application's tables
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  This header file contains declarations and definitions of data structures
**           used in TC's tables.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2021-11-28 | Team 1 | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TC_TBLDEFS_H_
#define _TC_TBLDEFS_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "common_types.h"
#include "tc_mission_cfg.h"

/*
** Local Defines
*/
#define TC_ILOAD_MAX_ENTRIES  1
#define TC_ILOAD_FILENAME     "/cf/apps/tc_iloads.tbl"
#define TC_ILOAD_TABLENAME    "ILOAD_TBL"


/*
** Local Structure Declarations
*/ 
/* Definition for Iload table entry */
typedef struct
{
    int32  iParam;

    /* TODO:  Add type declaration for ILoad parameters here.
    **
    ** Examples:
    **    int8/char            cParam;
    **    int8/char            cParams[16];
    **    uint8/unsigned char  ucParam;
    **    uint8/unsigned char  ucParams[16];
    **
    **    int16   sParam;
    **    int16   sParams[8];
    **    uint16  usParam;
    **    uint16  usParams[8];
    **
    **    int32   iParam;
    **    int32   iParams[5];
    **    uint32  uiParam;
    **    uint32  uiParams[5];
    **
    **    float  fParam;
    **    float  fParams[3];
    **
    **    double  dParam;
    **    double  dParams[3];
    */
} TC_ILoadTblEntry_t;
    
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

#endif /* _TC_TBLDEFS_H_ */

/*=======================================================================================
** End of file tc_tbldefs.h
**=====================================================================================*/
    