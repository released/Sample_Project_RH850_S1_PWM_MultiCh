/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018, 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_TAUJ0_3.c
* Component Version: 1.5.1
* Device(s)        : R7F701684
* Description      : This file implements device driver for Config_TAUJ0_3.
***********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_TAUJ0_3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint32_t g_cg_sync_read;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAUJ0_3_Create
* Description  : This function initializes the TAUJ03 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUJ0_3_Create(void)
{
    /* Disable channel 3 counter operation */
    TAUJ0.TT |= _TAUJ_CHANNEL3_COUNTER_STOP;
    /* Disable INTTAUJ0I3 operation and clear request */
    INTC2.ICTAUJ0I3.BIT.MKTAUJ0I3 = _INT_PROCESSING_DISABLED;
    INTC2.ICTAUJ0I3.BIT.RFTAUJ0I3 = _INT_REQUEST_NOT_OCCUR;
    /* Set INTTAUJ0I3 setting */
    INTC2.ICTAUJ0I3.BIT.TBTAUJ0I3 = _INT_TABLE_VECTOR;
    INTC2.ICTAUJ0I3.UINT16 &= _INT_PRIORITY_LOWEST;
    TAUJ0.TPS &= _TAUJ_CK0_PRS_CLEAR;
    TAUJ0.TPS |= _TAUJ_CK0_PRE_PCLK_0;
    /* Set channel 3 setting */
    TAUJ0.CMOR3 = _TAUJ_SELECTION_CK0 | _TAUJ_COUNT_CLOCK_PCLK | _TAUJ_INDEPENDENT_CHANNEL | 
                  _TAUJ_START_TRIGGER_SOFTWARE | _TAUJ_OVERFLOW_AUTO_CLEAR | _TAUJ_INTERVAL_TIMER_MODE | 
                  _TAUJ_START_INT_NOT_GENERATED;
    /* Set compare match register */
    TAUJ0.CMUR3 = _TAUJ_INPUT_EDGE_UNUSED;
    TAUJ0.CDR3 = _TAUJ03_COMPARE_VALUE;
    /* Set output mode setting */
    TAUJ0.TOE |= _TAUJ_CHANNEL3_ENABLES_OUTPUT_MODE;
    TAUJ0.TOM &= _TAUJ_CHANNEL3_INDEPENDENT_OPERATION;
    TAUJ0.TOC &= _TAUJ_CHANNEL3_OPERATION_MODE1;
    TAUJ0.TOL &= _TAUJ_CHANNEL3_POSITIVE_LOGIC;
    /* Synchronization processing */
    g_cg_sync_read = TAUJ0.TPS;
    __syncp();

    /* Set TAUJ0O3 pin */
    JTAG.JPIBC0 &= _JPORT_CLEAR_BIT5;
    JTAG.JPBDC0 &= _JPORT_CLEAR_BIT5;
    JTAG.JPM0 |= _JPORT_SET_BIT5;
    JTAG.JPMC0 &= _JPORT_CLEAR_BIT5;
    JTAG.JPFC0 |= _JPORT_SET_BIT5;
    JTAG.JPMC0 |= _JPORT_SET_BIT5;
    JTAG.JPM0 &= _JPORT_CLEAR_BIT5;

    R_Config_TAUJ0_3_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TAUJ0_3_Start
* Description  : This function starts the TAUJ03 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUJ0_3_Start(void)
{
    /* Clear INTTAUJ0I3 request and enable operation */
    INTC2.ICTAUJ0I3.BIT.RFTAUJ0I3 = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICTAUJ0I3.BIT.MKTAUJ0I3 = _INT_PROCESSING_ENABLED;
    /* Enable channel0 counter operation */
    TAUJ0.TS |= _TAUJ_CHANNEL3_COUNTER_START;
}

/***********************************************************************************************************************
* Function Name: R_Config_TAUJ0_3_Stop
* Description  : This function stop the TAUJ03 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUJ0_3_Stop(void)
{
    /* Disable channel 3 counter operation */
    TAUJ0.TT |= _TAUJ_CHANNEL3_COUNTER_STOP;
    /* Disable INTTAUJ0I3 operation and clear request */
    INTC2.ICTAUJ0I3.BIT.MKTAUJ0I3 = _INT_PROCESSING_DISABLED;
    INTC2.ICTAUJ0I3.BIT.RFTAUJ0I3 = _INT_REQUEST_NOT_OCCUR;
    /* Synchronization processing */
    g_cg_sync_read = TAUJ0.TT;
    __syncp();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
