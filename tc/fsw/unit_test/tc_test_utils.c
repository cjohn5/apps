 /*************************************************************************
 ** File:
 **   $Id: example_test_utils.c 1.1 2016/06/14 16:47:22EDT czogby Exp  $
 **
 ** Purpose: 
 **   This file contains unit test utilities for the UT-Assert example application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 **   $Log: example_test_utils.c  $
 **   Revision 1.1 2016/06/14 16:47:22EDT czogby 
 **   Initial revision
 **   Member added to project /FSW-TOOLs-REPOSITORY/ut-assert/example/fsw/unit_test/project.pj
 *************************************************************************/

#include "tc_test_utils.h"

#include "wise_msgids.h"
#include "wise_msg.h"

extern TC_AppData_t  g_TC_AppData;
WISE_HkTlm_t  wise_tlm;

/*
 * Function Definitions
 */

void TC_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&g_TC_AppData, 0, sizeof(TC_AppData_t));
    
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

    /* Init housekeeping packet */
    memset((void*)&wise_tlm, 0x00, sizeof(wise_tlm));
    CFE_SB_InitMsg(&wise_tlm,
                   WISE_HK_TLM_MID, sizeof(wise_tlm), TRUE);

} /* end Example_Test_Setup */

void TC_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end Example_Test_TearDown */

/************************/
/*  End of File Comment */
/************************/
