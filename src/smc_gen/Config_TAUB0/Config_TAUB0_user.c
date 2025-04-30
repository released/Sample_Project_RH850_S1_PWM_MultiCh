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
* File Name        : Config_TAUB0_user.c
* Component Version: 1.5.0
* Device(s)        : R7F701684
* Description      : This file implements device driver for Config_TAUB0.
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
#include "Config_TAUB0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAUB0_Create_UserInit
* Description  : This function adds user code after initializing the TAUB0 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAUB0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    set_pwm_ch_clk_src(TAUB0.CDR0);
    set_pwm_ch_duty(1,TAUB0.CDR1);
    set_pwm_ch_duty(2,TAUB0.CDR2);
    set_pwm_ch_duty(3,TAUB0.CDR3);
    set_pwm_ch_duty(4,TAUB0.CDR4);    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUB0_channel0_interrupt
* Description  : This function is TAUB00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUB0_channel0_interrupt(enable=false, channel=142, fpu=true, callt=false)
void r_Config_TAUB0_channel0_interrupt(void)
{
    /* Start user code for r_Config_TAUB0_channel0_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUB0_channel1_interrupt
* Description  : This function is TAUB01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUB0_channel1_interrupt(enable=false, channel=143, fpu=true, callt=false)
void r_Config_TAUB0_channel1_interrupt(void)
{
    /* Start user code for r_Config_TAUB0_channel1_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUB0_channel2_interrupt
* Description  : This function is TAUB02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUB0_channel2_interrupt(enable=false, channel=144, fpu=true, callt=false)
void r_Config_TAUB0_channel2_interrupt(void)
{
    /* Start user code for r_Config_TAUB0_channel2_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUB0_channel3_interrupt
* Description  : This function is TAUB03 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUB0_channel3_interrupt(enable=false, channel=145, fpu=true, callt=false)
void r_Config_TAUB0_channel3_interrupt(void)
{
    /* Start user code for r_Config_TAUB0_channel3_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUB0_channel4_interrupt
* Description  : This function is TAUB04 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUB0_channel4_interrupt(enable=false, channel=146, fpu=true, callt=false)
void r_Config_TAUB0_channel4_interrupt(void)
{
    /* Start user code for r_Config_TAUB0_channel4_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
