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

void TC_Verify_Threshold_Tables_Instanciated(void)
{
    //UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_ERROR, "TCA_Current_State == ERROR");
    UtAssert_True (g_TC_AppData.ThresholdTemps.NeedsCooling_Active != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.CoolEnough_Active != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.HotEnough_Active != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.NeedsHeating_Active != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.NeedsCooling_Inactive != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.CoolEnough_Inactive != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.HotEnough_Inactive != 0, "Threshold Temps populated with a value");
    UtAssert_True (g_TC_AppData.ThresholdTemps.NeedsHeating_Inactive != 0, "Threshold Temps populated with a value");

}// end TC_Verify_Threshold_Tables_Instanciated

void TC_Algorithm_Test_Given_WiseError_ShouldPlace_TC_Into_ErrorMode(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_ERROR;  
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    //set up any needed hook functions and return values
    //Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGID_INDEX, &Tc_Test_Injected_TLM);
    
    /* Execute the function being tested */
    TC_ProcessWiseTlm(&wise_tlm);
    
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_ERROR, "TCA_Current_State == ERROR");

} /* end TC_Algorithm_Test_Given_WiseError_ShouldPlace_TC_Into_ErrorMode */

void TC_Algorithm_Test_Needs_Heating_Inactive(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsHeating_Inactive - 5;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   
    
    /* Execute the function being tested */
    TC_ProcessWiseTlm(&wise_tlm);
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_HEATING, "TCA_Current_State == HEATING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_HEATER && g_TC_AppData.last_sent_command_value == TC_HEAT_NORMAL, "last sent command set heaters to normal (1 heater)");

} /* end TC_Algorithm_Test_Needs_Heating_Inactive */

void TC_Algorithm_Test_Needs_Heating_Active(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsHeating_Active - 5;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    
    /* Execute the function being tested */
    TC_ProcessWiseTlm(&wise_tlm);

    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_HEATING, "TCA_Current_State == HEATING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_HEATER && g_TC_AppData.last_sent_command_value == TC_HEAT_NORMAL, "last sent command set heaters to normal (1 heater)");

} /* end TC_Algorithm_Test_Needs_Heating_Active */



void TC_Algorithm_Test_Needs_Cooling_Inactive(void)
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
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_LOUVER && g_TC_AppData.last_sent_command_value == TC_COOL_ON, "last sent command set louver to on");
} /* end TC_Algorithm_Test_Needs_Cooling_Inactive */

void TC_Algorithm_Test_Needs_Cooling_Inactive_Temp40(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsCooling_Inactive;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    TC_ProcessWiseTlm(&wise_tlm);
    
    /* Verify results */
        //verify any expected event messages
        //verify any expected variable changes
        //verify correct number of event messages
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_COOLING, "TCA_Current_State == COOLING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_LOUVER && g_TC_AppData.last_sent_command_value == TC_COOL_ON, "last sent command set louver to on");
} /* end TC_Algorithm_Test_Needs_Cooling_Inactive_Temp40 */

void TC_Algorithm_Test_Needs_Cooling_Active_Temp25(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = 25;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    TC_ProcessWiseTlm(&wise_tlm);
    
    /* Verify results */
        //verify any expected event messages
        //verify any expected variable changes
        //verify correct number of event messages
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_COOLING, "TCA_Current_State == COOLING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_LOUVER && g_TC_AppData.last_sent_command_value == TC_COOL_ON, "last sent command set louver to on");

} /* end TC_Algorithm_Test_Needs_Cooling_Active_Temp25 */

void TC_Algorithm_Test_Needs_Heating_Active_Temp15(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_OBSERVING;
    wise_tlm.wiseTemp = 14;
    wise_tlm.wiseHtrA_State = WISE_HTR_ON;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    TC_ProcessWiseTlm(&wise_tlm);
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_HEATING, "TCA_Current_State == HEATING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_HEATER && g_TC_AppData.last_sent_command_value == TC_HEAT_NORMAL, "last sent command set heater to on");
} /* end TC_Algorithm_Test_Needs_Heating_Active_Temp15 */

void TC_Algorithm_Test_Needs_Heating_Inactive_Temp5(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = 5;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    TC_ProcessWiseTlm(&wise_tlm);
    
    /* Verify results */
        //verify any expected event messages
        //verify any expected variable changes
        //verify correct number of event messages
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_HEATING, "TCA_Current_State == HEATING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_HEATER && g_TC_AppData.last_sent_command_value == TC_HEAT_NORMAL, "last sent command set heaters to normal (1 heater)");

} /* end TC_Algorithm_Test_Needs_Heating_Inactive_Temp5 */

void TC_Algorithm_Test_Needs_Cooling_Active(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_OBSERVING;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsCooling_Active + 30;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_MONITORING;   

    TC_ProcessWiseTlm(&wise_tlm);
    printf("Temp: %i\n", wise_tlm.wiseTemp );
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_COOLING, "TCA_Current_State == COOLING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_LOUVER && g_TC_AppData.last_sent_command_value == TC_COOL_ON, "last sent command set louvers to on");
}// end TC_Algorithm_Test_Needs_Cooling_Active

void TC_Algorithm_Test_Should_Switch_FromCoolingToHeating(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsHeating_Inactive - 5;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_COOLING;   

    TC_ProcessWiseTlm(&wise_tlm);
    printf("Temp: %i\n", wise_tlm.wiseTemp );
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_HEATING, "TCA_Current_State == HEATING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_HEATER && g_TC_AppData.last_sent_command_value == TC_HEAT_NORMAL, "last sent heaters set louvers to on");
}// end TC_Algorithm_Test_Should_Switch_FromCoolingToHeating

void TC_Algorithm_Test_Should_Switch_FromHeatingToCooling(void)
{
    //declare any needed local variables
    uint32  uiCmdCode=0;
    WISE_HkTlm_t  wise_tlm;
    
    wise_tlm.wiseSbcState = WISE_SBC_POWERED;
    wise_tlm.wiseTemp = g_TC_AppData.ThresholdTemps.NeedsCooling_Inactive + 5;
    wise_tlm.wiseHtrA_State = WISE_HTR_OFF;
    wise_tlm.wiseHtrB_State = WISE_HTR_OFF;
    wise_tlm.wiseLvrA_State = WISE_LVR_CLOSED;
    wise_tlm.wiseLvrB_State = WISE_LVR_CLOSED;
    g_TC_AppData.HkTlm.TCA_Logging_State = TC_LOG_DEBUG;
    g_TC_AppData.HkTlm.TCA_Current_State = TC_STATE_HEATING;   

    TC_ProcessWiseTlm(&wise_tlm);
    printf("Temp: %i\n", wise_tlm.wiseTemp );
    
    log_TCA_State(g_TC_AppData.HkTlm.TCA_Current_State);
    UtAssert_True (g_TC_AppData.HkTlm.TCA_Current_State == TC_STATE_COOLING, "TCA_Current_State == COOLING");
    UtAssert_True (g_TC_AppData.last_sent_command_actuator == TC_ACTUATOR_LOUVER && g_TC_AppData.last_sent_command_value == TC_COOL_ON, "last sent command set louvers to on");
}// end TC_Algorithm_Test_Should_Switch_FromCoolingToHeating

void TC_App_Test_AddTestCases(void)
{
    UtTest_Add(TC_Verify_Threshold_Tables_Instanciated, TC_Test_Setup, TC_Test_TearDown, "TC_Verify_Threshold_Tables_Instanciated");
    UtTest_Add(TC_Algorithm_Test_Given_WiseError_ShouldPlace_TC_Into_ErrorMode, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Given_WiseError_ShouldPlace_TC_Into_ErrorMode");
    UtTest_Add(TC_Algorithm_Test_Needs_Heating_Inactive, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Heating");
    UtTest_Add(TC_Algorithm_Test_Needs_Cooling_Inactive, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Cooling");
    UtTest_Add(TC_Algorithm_Test_Needs_Heating_Active, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Heating_active");
    UtTest_Add(TC_Algorithm_Test_Needs_Cooling_Active, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Cooling_active");
    UtTest_Add(TC_Algorithm_Test_Needs_Heating_Inactive_Temp5, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Heating_Inactive_Temp5");
    UtTest_Add(TC_Algorithm_Test_Needs_Heating_Active_Temp15, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Heating_Active_Temp15");
    UtTest_Add(TC_Algorithm_Test_Needs_Cooling_Active_Temp25, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Cooling_Active_Temp25");
    UtTest_Add(TC_Algorithm_Test_Needs_Cooling_Inactive_Temp40, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Needs_Cooling_active");
    UtTest_Add(TC_Algorithm_Test_Should_Switch_FromCoolingToHeating, TC_Test_Setup, TC_Test_TearDown, "TC_Algorithm_Test_Should_Switch_FromCoolingToHeating");

} /* end TC_App_Test_AddTestCases */

/************************/
/*  End of File Comment */
/************************/
