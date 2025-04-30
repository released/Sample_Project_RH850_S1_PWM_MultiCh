/*_____ I N C L U D E S ____________________________________________________*/
// #include <stdio.h>
#include <string.h>
#include "r_smc_entry.h"

#include "misc_config.h"
#include "custom_func.h"
#include "retarget.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

volatile struct flag_32bit flag_PROJ_CTL;
#define FLAG_PROJ_TIMER_PERIOD_1000MS                 	(flag_PROJ_CTL.bit0)
#define FLAG_PROJ_TRIG_BTN1           	                (flag_PROJ_CTL.bit1)
#define FLAG_PROJ_TRIG_BTN2                 	        (flag_PROJ_CTL.bit2)
#define FLAG_PROJ_REVERSE3                    		    (flag_PROJ_CTL.bit3)
#define FLAG_PROJ_REVERSE4                              (flag_PROJ_CTL.bit4)
#define FLAG_PROJ_REVERSE5                              (flag_PROJ_CTL.bit5)
#define FLAG_PROJ_REVERSE6                              (flag_PROJ_CTL.bit6)
#define FLAG_PROJ_TIMER_PERIOD_SPECIFIC                 (flag_PROJ_CTL.bit7)


#define FLAG_PROJ_TRIG_1                                (flag_PROJ_CTL.bit8)
#define FLAG_PROJ_TRIG_2                                (flag_PROJ_CTL.bit9)
#define FLAG_PROJ_TRIG_3                                (flag_PROJ_CTL.bit10)
#define FLAG_PROJ_TRIG_4                                (flag_PROJ_CTL.bit11)
#define FLAG_PROJ_TRIG_5                                (flag_PROJ_CTL.bit12)
#define FLAG_PROJ_PWM_DUTY_INC                          (flag_PROJ_CTL.bit13)
#define FLAG_PROJ_PWM_DUTY_DEC                          (flag_PROJ_CTL.bit14)
#define FLAG_PROJ_REVERSE15                             (flag_PROJ_CTL.bit15)

/*_____ D E F I N I T I O N S ______________________________________________*/

volatile unsigned short counter_tick = 0U;
volatile unsigned long ostmr_tick = 0U;
volatile unsigned long btn_counter_tick = 0U;

#define BTN_PRESSED_LONG                                (2500U)

#pragma section privateData

const unsigned char dummy_3 = 0x5AU;

volatile unsigned char dummy_2 = 0xFFU;

volatile unsigned char dummy_1;

#pragma section default

volatile unsigned long g_u32_counter = 0U;

unsigned char g_uart0rxbuf = 0U;                                 /* UART0 receive buffer */
unsigned char g_uart0rxerr = 0U;                                 /* UART0 receive error status */

#define PWM_RESOLUTION                                  (100U)
volatile unsigned short pwm_clk_src = 0U;
volatile unsigned short pwm_duty[16] = {0U};
volatile unsigned char pwm_ch_idx = 0U;

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

unsigned long btn_get_tick(void)
{
	return (btn_counter_tick);
}

void btn_set_tick(unsigned long t)
{
	btn_counter_tick = t;
}

void btn_tick_counter(void)
{
	btn_counter_tick++;
    if (btn_get_tick() >= 60000U)
    {
        btn_set_tick(0U);
    }
}

void ostmr_tick_counter(void)
{
	ostmr_tick++;
}

void ostmr_1ms_IRQ(void)
{
	ostmr_tick_counter();
}

void ostimer_dealyms(unsigned long ms)
{
    R_Config_OSTM0_Start();
    ostmr_tick = 0U;

    while(ostmr_tick < ms);

    R_Config_OSTM0_Stop();

}

unsigned short get_tick(void)
{
	return (counter_tick);
}

void set_tick(unsigned short t)
{
	counter_tick = t;
}

void tick_counter(void)
{
	counter_tick++;
    if (get_tick() >= 60000U)
    {
        set_tick(0U);
    }
}

void delay_ms(unsigned long ms)
{
    unsigned long tickstart = get_tick();
    unsigned long wait = ms;
	unsigned long tmp = 0U;
	
    while (1)
    {
		if (get_tick() > tickstart)	// tickstart = 59000 , tick_counter = 60000
		{
			tmp = get_tick() - tickstart;
		}
		else // tickstart = 59000 , tick_counter = 2048
		{
			tmp = 60000U -  tickstart + get_tick();
		}		
		
		if (tmp > wait)
			break;
    }
}



/*
    TAUB0 PWM : 10K
        - SLAVE 1 : P12 
        - SLAVE 2 : P11
        - SLAVE 3 : P10
        - SLAVE 4 : P31
*/
unsigned short get_pwm_ch_clk_src(void)
{
	return (pwm_clk_src);
}

void set_pwm_ch_clk_src(unsigned short src)
{
	pwm_clk_src = src;
}

unsigned short get_pwm_ch_duty(unsigned char ch)
{
	return (pwm_duty[ch]);
}

void set_pwm_ch_duty(unsigned char ch ,unsigned short duty)
{
    pwm_duty[ch] = duty;
}


void reload_pwm_ch_idx(unsigned char ch)
{
    // Reload Data Enable Register
    TAUB0.RDE = 1U << ch;    
    // Reload Data Trigger Register
    TAUB0.RDT = 1U << ch;   // enable bit correspond to pwm ch
}

void pwm_ch_Duty_Decrease(unsigned char idx)
{
    unsigned short tmp = 0U;
    unsigned short pwm_master = 0U;
    unsigned short duty_hex = 0U;

    tmp = get_pwm_ch_duty(idx);
    pwm_master = get_pwm_ch_clk_src()+1U;

    duty_hex = (pwm_master) / PWM_RESOLUTION * 5U;   // - 5 %
    tmp = (tmp <= 0U) ? (0U) : (tmp - duty_hex ) ;  
    set_pwm_ch_duty(idx,tmp);

    #if 1   // debug
    printf("[%d]-duty:0x%02X,",idx,pwm_master);
    printf("0x%02X,",duty_hex );
    printf("0x%02X,",tmp );
    printf("(%2.2f)\r\n",(float) tmp*PWM_RESOLUTION/(pwm_master) );
    #endif

    switch(idx)
    {
        case 1U: 
            TAUB0.CDR1 = get_pwm_ch_duty(idx);
            break;
        case 2U:
            TAUB0.CDR2 = get_pwm_ch_duty(idx);
            break;
        case 3U:
            TAUB0.CDR3 = get_pwm_ch_duty(idx);
            break;
        case 4U:
            TAUB0.CDR4 = get_pwm_ch_duty(idx);
            break;
    }
    reload_pwm_ch_idx(idx);
}

void pwm_ch_Duty_Increase(unsigned char idx)
{
    unsigned short tmp = 0U;
    unsigned short pwm_master = 0U;
    unsigned short duty_hex = 0U;

    tmp = get_pwm_ch_duty(idx);
    pwm_master = get_pwm_ch_clk_src()+1U;

    duty_hex = (pwm_master) / PWM_RESOLUTION * 5U;   // + 5 %
    tmp = (tmp >= (pwm_master)) ? ((pwm_master)) : (tmp + duty_hex );   
    set_pwm_ch_duty(idx,tmp);

    #if 1   // debug
    printf("[%d]+duty:0x%02X,",idx,pwm_master);
    printf("0x%02X,",duty_hex );
    printf("0x%02X,",tmp );
    printf("(%2.2f)\r\n",(float) tmp*PWM_RESOLUTION/(pwm_master) );
    #endif

    switch(idx)
    {
        case 1U:
            TAUB0.CDR1 = get_pwm_ch_duty(idx);
            break;
        case 2U: 
            TAUB0.CDR2 = get_pwm_ch_duty(idx);
            break;
        case 3U: 
            TAUB0.CDR3 = get_pwm_ch_duty(idx);
            break;
        case 4U: 
            TAUB0.CDR4 = get_pwm_ch_duty(idx);
            break;
    }
    reload_pwm_ch_idx(idx);
}

void PWM_Process_Adjust(void)
{


    /*
        duty * 16
        --------  = target hex 
            10
    */

    if (FLAG_PROJ_PWM_DUTY_INC)
    {
        FLAG_PROJ_PWM_DUTY_INC = 0U;
        pwm_ch_Duty_Increase(pwm_ch_idx);
    }

    if (FLAG_PROJ_PWM_DUTY_DEC)
    {
        FLAG_PROJ_PWM_DUTY_DEC = 0U;
        pwm_ch_Duty_Decrease(pwm_ch_idx);
    }
}

void tmr_1ms_IRQ(void)
{
    tick_counter();

    if ((get_tick() % 1000U) == 0U)
    {
        FLAG_PROJ_TIMER_PERIOD_1000MS = 1U;
    }

    if ((get_tick() % 25U) == 0U)
    {
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 1U;
    }

    if ((get_tick() % 50U) == 0U)
    {

    }	

    Button_Process_long_counter();
}

void LED_Toggle(void)
{
    static unsigned char flag_gpio = 0U;
		
    GPIO_TOGGLE(0,14);//PORT.PNOT0 |= 1u<<14;
	
	if (!flag_gpio)
	{
		flag_gpio = 1U;
        GPIO_HIGH(P8,5);//PORT.P8 |= 1u<<5;
	}
	else
	{
		flag_gpio = 0U;
		GPIO_LOW(P8,5);//PORT.P8 &= ~(1u<<5);
	}	
}

void loop(void)
{
	// static unsigned long LOG1 = 0U;

    Button_Process_in_polling();

    if (FLAG_PROJ_TIMER_PERIOD_1000MS)
    {
        FLAG_PROJ_TIMER_PERIOD_1000MS = 0U;

        g_u32_counter++;
        LED_Toggle();   
        // tiny_printf("timer:%4d\r\n",LOG1++);
    }

    if (FLAG_PROJ_TIMER_PERIOD_SPECIFIC)
    {
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 0U;
        
    }

    PWM_Process_Adjust();
}

// F1KM S1 EVB , P8_2/INTP6 , set both edge 
void Button_Process_long_counter(void)
{
    if (FLAG_PROJ_TRIG_BTN2)
    {
        btn_tick_counter();
    }
    else
    {
        btn_set_tick(0U);
    }
}

void Button_Process_in_polling(void)
{
    static unsigned char cnt = 0U;

    if (FLAG_PROJ_TRIG_BTN1)
    {
        FLAG_PROJ_TRIG_BTN1 = 0U;
        tiny_printf("BTN pressed(%d)\r\n",cnt);

        if (cnt == 0U)   //set both edge  , BTN pressed
        {
            FLAG_PROJ_TRIG_BTN2 = 1U;
        }
        else if (cnt == 1U)  //set both edge  , BTN released
        {
            FLAG_PROJ_TRIG_BTN2 = 0U;
        }

        cnt = (cnt >= 1U) ? (0U) : (cnt+1U) ;
    }

    if ((FLAG_PROJ_TRIG_BTN2 == 1U) && 
        (btn_get_tick() > BTN_PRESSED_LONG))
    {         
        tiny_printf("BTN pressed LONG\r\n");
        btn_set_tick(0U);
        FLAG_PROJ_TRIG_BTN2 = 0U;
    }
}

void Button_Process_in_IRQ(void)    
{
    FLAG_PROJ_TRIG_BTN1 = 1U;
}


void UARTx_ErrorCheckProcess(unsigned char err)
{
    if (err)          /* Check reception error */
    {   
        /* Reception error */
        switch(err)
        {
            case _UART_PARITY_ERROR_FLAG:   /* Parity error */
                tiny_printf("uart rx:Parity Error Flag\r\n");
                break;
            case _UART_FRAMING_ERROR_FLAG:  /* Framing error */
                tiny_printf("uart rx:Framing Error Flag\r\n");
                break;
            case _UART_OVERRUN_ERROR_FLAG:  /* Overrun error */
                tiny_printf("uart rx:Overrun Error Flag\r\n");
                break;
            case _UART_BIT_ERROR_FLAG:      /* Bit error */
                tiny_printf("uart rx:Bit Error Flag\r\n");
                break;
        }
        g_uart0rxerr = 0U;
    }
}

void UARTx_Process(unsigned char rxbuf)
{    
    if (rxbuf == 0x00U)
    {
        return;
    }

    if (rxbuf > 0x7FU)
    {
        tiny_printf("invalid command\r\n");
    }
    else
    {
        tiny_printf("press:%c(0x%02X)\r\n" , rxbuf,rxbuf);   // %c :  C99 libraries.
        switch(rxbuf)
        {
            case '1':
                FLAG_PROJ_TRIG_1 = 1U;
                pwm_ch_idx = 1U;
                break;
            case '2':
                FLAG_PROJ_TRIG_2 = 1U;
                pwm_ch_idx = 2U;
                break;
            case '3':
                FLAG_PROJ_TRIG_3 = 1U;
                pwm_ch_idx = 3U;
                break;
            case '4':
                FLAG_PROJ_TRIG_4 = 1U;
                pwm_ch_idx = 4U;
                break;
            case '5':
                FLAG_PROJ_TRIG_5 = 1U;
                break;

            case 'a':
            case 'A':
                FLAG_PROJ_PWM_DUTY_INC = 1U;
                break;
            case 'd':
            case 'D':
                FLAG_PROJ_PWM_DUTY_DEC = 1U;
                break;

            case 'X':
            case 'x':
                // RL78_soft_reset(7);
                break;
            case 'Z':
            case 'z':
                // RL78_soft_reset(1);
                break;

            default:       
                // exception
                break;                
        }
    }
}

void RLIN3_UART_SendChar(unsigned char c)
{
    /*
        UTS : 0 - transmission is not in progress    
    */
    while (((RLN30.LST & _UART_TRANSMISSION_OPERATED) != 0U));    
    RLN30.LUTDR.UINT16 = c;
    // RLN30.LUTDR.UINT8[L] = (unsigned char) c;  
}

void SendChar(unsigned char ch)
{
    RLIN3_UART_SendChar(ch);
}

void hardware_init(void)
{
    EI();

    R_Config_TAUJ0_3_Start();
    R_Config_OSTM0_Start();

    /*
        LED : 
            - LED18 > P0_14
            - LED17 > P8_5 
        UART : 
            - TX > P10_10
            - RX > P10_9    
    */
    R_Config_UART0_Receive(&g_uart0rxbuf, 1U);
    R_Config_UART0_Start();
    
    /*
        button :
            - P8_2/INTP6
    */
    R_Config_INTC_INTP6_Start();


    /*
        PWM OUTPUT :
            - P10_11/TAUB0O1
            - P0_8/TAUB0O2
            - P10_12/TAUB0O3
            - P0_9/TAUB0O4
    */
    R_Config_TAUB0_Start();

    tiny_printf("\r\nhardware_init rdy\r\n");

}
