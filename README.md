# Sample_Project_RH850_S1_PWM_MultiCh
Sample_Project_RH850_S1_PWM_MultiCh

udpate @ 2025/04/30

1. base on EVM : Y-ASK-RH850F1KM-S1-V3 , initial below function

- TAUJ0_0 : timer interval for 1ms interrupt

- UART : RLIN3 (TX > P10_10 , RX > P10_9) , for printf and receive from keyboard

- LED : LED18 > P0_14 , LED17 > P8_5 , toggle per 1000ms

- PWM OUTPUT : 

	- P10_11/TAUB0O1 
	
	- P0_8/TAUB0O2
	
	- P10_12/TAUB0O3
	
	- P0_9/TAUB0O4
	
2. Base on PWM master value , to calculate target slave channel duty	
	
ex : 

master channel TAUB0.CDR0 : 0x01F3U

if slave channel 1 plan to set duty 100% , TAUB0.CDR1 will be (TAUB0.CDR0+1) * (4/4)

if slave channel 2 plan to set duty 75% , TAUB0.CDR2 will be (TAUB0.CDR0+1) * (3/4)

if slave channel 3 plan to set duty 50% , TAUB0.CDR3 will be (TAUB0.CDR0+1) * (2/4)

if slave channel 4 plan to set duty 25% , TAUB0.CDR4 will be (TAUB0.CDR0+1) * (1/4)
	
refer to 

```c
pwm_ch_Duty_Increase
pwm_ch_Duty_Decrease
```	
	
3. use terminal to control PWM channel duty

press digit 1 ~ 4 , to switch PWM channel

press digit A/a , to increase the target PWM channel DUTY ( ex :+5%)

press digit D/d , to decrease the target PWM channel DUTY ( ex :-5%)


below is log message :

![image](https://github.com/released/Sample_Project_RH850_S1_PWM_MultiCh/blob/main/log_pwm.jpg)


![image](https://github.com/released/Sample_Project_RH850_S1_PWM_MultiCh/blob/main/LA_pwm.jpg)


