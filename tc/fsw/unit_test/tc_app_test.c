#include "tc_app_test.h"
#include "tc_app.h"
#include "tc_test_utils.h"
#include "wise_msgids.h"
#include "wise_msg.h"
#include "wise_app.h"

#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
 
extern TC_AppData_t  g_TC_AppData;

void log_CC_Code(uint32 uiCmdCode, uint value)
{
#define WISE_TLM_RESET_CNTS_CC  

    switch (uiCmdCode)
    {
    case WISE_NOOP_CC:
        UtPrintf("WISE_NOOP_CC\n");
        break;

    case WISE_CAP_ACTIVE_CC:
        UtPrintf("WISE_CAP_ACTIVE_CC\n");
        break;

    case WISE_CAP_DISCHARGE_CC:
        UtPrintf("WISE_CAP_DISCHARGE_CC\n");
        break;

    case WISE_HTR_TOGGLE_CC:
        UtPrintf("WISE_HTR_TOGGLE_CC = %d\n", value);
        break;
    
    case WISE_LVR_TOGGLE_CC:
        UtPrintf("WISE_LVR_TOGGLE_CC = %d\n", value);
        break;

    case WISE_OBS_START_CC:
        UtPrintf("WISE_OBS_START_CC\n");
        break;

    case WISE_OBS_STOP_CC:
        UtPrintf("WISE_OBS_STOP_CC\n");
        break;

    case WISE_SEND_OBS_CC:
        UtPrintf("WISE_SEND_OBS_CC\n");
        break;

    default:
        break;
    }
}

void log_TCA_State(uint8 state)
{
    switch (state)
    {
    case TC_STATE_READY:
        UtPrintf("TC_STATE_READY\n");
        break;

    case TC_STATE_MONITORING:
        UtPrintf("TC_STATE_MONITORING\n");
        break;

    case TC_STATE_HEATING:
        UtPrintf("TC_STATE_HEATING\n");
        break;

    case TC_STATE_COOLING:
        UtPrintf("TC_STATE_COOLING\n");
        break;

    case TC_STATE_ERROR:
        UtPrintf("TC_STATE_ERROR\n");
        break;
        
    default:
        break;
    }
}

void TC_Algorithm_Test_Needs_Heating(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    //set up any needed variables (global or local)
        //Set the values of the mocked TLM packet to trigger the desired conditions
        //Set SB to contain a received message on the TLM pipe with ID == WISE_HK_TLM_MID (containing the tlm mocked TLM packet)
        //Set the TCA states to trigger the desired conditions
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsHeating_Inactive - 5;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    //set up any needed hook functions and return values
    //Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGID_INDEX, &Tc_Test_Injected_TLM);
    
    /* Execute the function being tested */
    TC_ProcessWiseTlm(&wise_tlm);
    
    /* Verify results */
        //verify any expected event messages
        //verify any expected variable changes
        //verify correct number of event messages
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    printf("This is the actual state: %i defined\n", g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_HEATING, "TCA_Current_State == HEATING");
    uiCmdCode = CFE_SB_GetCmdCode(&g_TC_AppData.wise_cmd);

    //TODO: WISE uses parameterized commands instead of basic commands
    log_CC_Code(uiCmdCode, 99);
    UtAssert_True (uiCmdCode == WISE_HTR_TOGGLE_CC, "last sent command code == TOGGLE HEATER");

} /* end TC_Algorithm_Test_Needs_Heating */

void TC_Algorithm_Test_Needs_Cooling(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    //set up any needed variables (global or local)
        //Set the values of the mocked TLM packet to trigger the desired conditions
        //Set SB to contain a received message on the TLM pipe with ID == WISE_HK_TLM_MID (containing the tlm mocked TLM packet)
        //Set the TCA states to trigger the desired conditions
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsCooling_Inactive + 30;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    //set up any needed hook functions and return values
    //Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGID_INDEX, &Tc_Test_Injected_TLM);
    
    /* Execute the function being tested */
    TC_ProcessWiseTlm(&wise_tlm);
    
    /* Verify results */
        //verify any expected event messages
        //verify any expected variable changes
        //verify correct number of event messages
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_COOLING, "TCA_Current_State == COOLING");
    uiCmdCode = CFE_SB_GetCmdCode(&g_TC_AppData.wise_cmd);

    //TODO: WISE uses parameterized commands instead of basic commands
    log_CC_Code(uiCmdCode, 99);
    UtAssert_True (uiCmdCode == WISE_LVR_TOGGLE_CC, "last sent command code == TOGGLE LOUVER");

} /* end TC_Algorithm_Test_Needs_Heating */

void TC_App_Test_AddTestCases(void)
{
    UtTest_Add(TC_Algorithm_Test_Needs_Heating, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Heating");
    UtTest_Add(TC_Algorithm_Test_Needs_Cooling, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Cooling");
    //UtTest_Add(Example_Function1_Test_Case3, TC_Test_Setup, TC_Test_TearDown, "Example_Function1_Test_Case3");


} /* end TC_App_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
