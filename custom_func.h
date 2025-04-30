/*_____ I N C L U D E S ____________________________________________________*/

/*_____ D E C L A R A T I O N S ____________________________________________*/

#define GPIO_GETLEVEL(n,Pin)                            (PORT.PPR##n &= 1u<<Pin)
#define GPIO_TOGGLE(n,Pin)                              (PORT.PNOT##n |= 1u<<Pin)
#define GPIO_HIGH(Port,Pin)                             (PORT.Port |= 1u<<Pin)
#define GPIO_LOW(Port,Pin)                              (PORT.Port &= ~(1u<<Pin))

#define CSIG_IS_BUSY(x)                                 (x.STR0 & (1 << 7))
#define CSIH_IS_BUSY(x)                                 (x.STR0 & (1 << 7))

// #define bool  _Bool

// #ifndef TRUE
// #define TRUE  (1)
// #endif

// #ifndef true
// #define true  (1)
// #endif

// #ifndef FALSE
// #define FALSE  (0)
// #endif

// #ifndef false
// #define false  (0)
// #endif

// #ifndef BIT0
// #define BIT0     (0x00000001UL)       ///< Bit 0 mask of an 32 bit integer
// #endif

// #ifndef BIT1
// #define BIT1     (0x00000002UL)       ///< Bit 1 mask of an 32 bit integer
// #endif

// #ifndef BIT2
// #define BIT2     (0x00000004UL)       ///< Bit 2 mask of an 32 bit integer
// #endif

// #ifndef BIT3
// #define BIT3     (0x00000008UL)       ///< Bit 3 mask of an 32 bit integer
// #endif

// #ifndef BIT4
// #define BIT4     (0x00000010UL)       ///< Bit 4 mask of an 32 bit integer
// #endif

// #ifndef BIT5
// #define BIT5     (0x00000020UL)       ///< Bit 5 mask of an 32 bit integer
// #endif

// #ifndef BIT6
// #define BIT6     (0x00000040UL)       ///< Bit 6 mask of an 32 bit integer
// #endif

// #ifndef BIT7
// #define BIT7     (0x00000080UL)       ///< Bit 7 mask of an 32 bit integer
// #endif

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/


unsigned short get_pwm_ch_clk_src(void);
void set_pwm_ch_clk_src(unsigned short src);
unsigned short get_pwm_ch_duty(unsigned char ch);
void set_pwm_ch_duty(unsigned char ch ,unsigned short duty);


void ostmr_1ms_IRQ(void);
void ostimer_dealyms(unsigned long ms);

void tmr_1ms_IRQ(void);

void LED_Toggle(void);

void loop(void);

void Button_Process_long_counter(void);
void Button_Process_in_polling(void);
void Button_Process_in_IRQ(void);
void UARTx_ErrorCheckProcess(unsigned char err);
void UARTx_Process(unsigned char rxbuf);
void RH850_software_reset(void);
void hardware_init(void);
