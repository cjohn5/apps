/*=======================================================================================
** File Name:  tc_iload_utils.c
**
** Title:  Iload Tables' Utilities for TC Application
**
** $Author:    Team 1
** $Revision: 1.1 $
** $Date:      2021-11-28
**
** Purpose:  This source file contains definitions of ILoad table-related utility
**           function for TC application.
**
** Functions Defined:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. One source file per CFS table!
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
#include "tc_iload_utils.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/
extern TC_AppData_t  g_TC_AppData;

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Function Prototypes
*/

/*
** Function Definitions
*/
    
/*=====================================================================================
** Name: TC_InitILoadTbl
**
** Purpose: To initialize the TC's ILoad tables
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_TBL_Register
**    CFE_TBL_Load
**    CFE_TBL_Manage
**    CFE_TBL_GetAddress
**    CFE_ES_WriteToSysLog
**    TC_ValidateILoadTbl
**    TC_ProcessNewILoadTbl
**
** Called By:
**    TC_InitApp
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
** Programmer(s):  GSFC, Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_InitILoadTbl()
{
    int32  iStatus=0;

    /* Register ILoad table */
    iStatus = CFE_TBL_Register(&g_TC_AppData.ILoadTblHdl,
                               TC_ILOAD_TABLENAME,
                               (sizeof(TC_ILoadTblEntry_t) * TC_ILOAD_MAX_ENTRIES),
                               CFE_TBL_OPT_DEFAULT,
                               TC_ValidateILoadTbl);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TC - Failed to register ILoad table (0x%08X)\n", iStatus);
        goto TC_InitILoadTbl_Exit_Tag;
    }

    /* Load ILoad table file */
    iStatus = CFE_TBL_Load(g_TC_AppData.ILoadTblHdl,
                           CFE_TBL_SRC_FILE,
                           TC_ILOAD_FILENAME);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TC - Failed to load ILoad Table (0x%08X)\n", iStatus);
        goto TC_InitILoadTbl_Exit_Tag;
    }

    /* Manage ILoad table */
    iStatus = CFE_TBL_Manage(g_TC_AppData.ILoadTblHdl);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TC - Failed to manage ILoad table (0x%08X)\n", iStatus);
        goto TC_InitILoadTbl_Exit_Tag;
    }

    /* Make sure ILoad table is accessible by getting referencing it */
    iStatus = CFE_TBL_GetAddress((void*)&g_TC_AppData.ILoadTblPtr,
                                 g_TC_AppData.ILoadTblHdl);
    if (iStatus != CFE_TBL_INFO_UPDATED)
    {
        CFE_ES_WriteToSysLog("TC - Failed to get ILoad table's address (0x%08X)\n", iStatus);
        goto TC_InitILoadTbl_Exit_Tag;
    }

    /* Validate ILoad table */
    iStatus = TC_ValidateILoadTbl(g_TC_AppData.ILoadTblPtr);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TC - Failed to validate ILoad table (0x%08X)\n", iStatus);
        goto TC_InitILoadTbl_Exit_Tag;
    }

    /* Set new parameter values */
    TC_ProcessNewILoadTbl();

TC_InitILoadTbl_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: tc_ValidateILoadTbl
**
** Purpose: To validate the TC's ILoad tables
**
** Arguments:
**    TC_ILoadTblEntry_t*  iLoadTblPtr - pointer to the ILoad table
**
** Returns:
**    int32 iStatus - Status of table updates
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
** Programmer(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TC_ValidateILoadTbl(TC_ILoadTblEntry_t* iLoadTblPtr)
{
    int32  iStatus=0;

    if (iLoadTblPtr == NULL)
    {
        iStatus = -1;
        goto TC_ValidateILoadTbl_Exit_Tag;
    }

    /* TODO:  Add code to validate new data values here.
    **
    ** Examples:
    **    if (iLoadTblPtr->sParam <= 16)
    **    {
    **        CFE_ES_WriteToSysLog("TC - Invalid value for ILoad parameter sParam (%d)\n",
    **                             iLoadTblPtr->sParam);
    */

TC_ValidateILoadTbl_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: TC_ProcessNewILoadTbl
**
** Purpose: To process TC's new ILoad tables and set ILoad parameters with new values
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
** Programmer(s):  Team 1 
**
** History:  Date Written  2021-11-28
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TC_ProcessNewILoadTbl()
{
    /* TODO:  Add code to set new ILoad parameters with new values here.
    **
    ** Examples:
    **
    **    g_TC_AppData.latest_sParam = g_TC_AppData.ILoadTblPtr->sParam;
    **    g_TC_AppData.latest_fParam = g_TC.AppData.ILoadTblPtr->fParam;
    */
}
    
/*=======================================================================================
** End of file tc_iload_utils.c
**=====================================================================================*/
    