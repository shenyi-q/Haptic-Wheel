/*
  pwm.c
  University of Michigan
  EECS 461, Embedded Control Systems
  Pulse-Width Modulation using FlexTimer Module
  Read Chapter 45 in S32K144 User's Manual
  Important sections:
    45.4.2: Register Descriptions
    45.5.7: Edge-Aligned PWM (EPWM) Mode
   Revision History
     2020-09-23  jfr
     2021-01-12  jfr
     2022-08-12  jfr
 */

#include "eecs461.h"
#include "pwm.h"

#define PWM_CLOCK_FREQ  10000000

/* PWM Parameters */
#define MOTOR_PORT  PORTB
#define MOTOR_PCR  12
#define MOTOR_MUX  0b010

#define FILTER_PORT PORTB
#define FILTER_PCR 8
#define FILTER_MUX 0b010

#define DC_UPPER_LIMIT 0.76
#define DC_LOWER_LIMIT 0.24

FTM_Type * FTM_MODULE[4] = FTM_BASE_PTRS;


/******************************************************************************
 * Function:    initPWMPCRs
 * Description: Initialize the pins for each respective output PWM
 ******************************************************************************/
void initPWMPCRs()
{
  /* Initialize motor output PCR */
  MOTOR_PORT->PCR[MOTOR_PCR] |= PORT_PCR_MUX(MOTOR_MUX);

  /* Initialize filter output PCR */
  FILTER_PORT->PCR[FILTER_PCR] |= PORT_PCR_MUX(FILTER_MUX);

}

/******************************************************************************
 * Function:    initPWM
 * Description: Initializes PWM - 45.5.7: Edge-Aligned PWM (EPWM) Mode
 ******************************************************************************/
void initPWM(int submodule, int channel, int frequency, float dutyCycle)
{
  /* uint16_t cmax;*/

  /* cmax  =  fill in */  
  
  /* 45.4.3.9 - Feature Mode Selection (MODE) */
  FTM_MODULE[submodule]->MODE |= FTM_MODE_WPDIS(0b1);  /* Write protect to registers disabled (default) */
  
  /* 45.4.3.2 - Status and Control (SC) */
  FTM_MODULE[submodule]->SC = 0x00000000; /* Clear the status and control register */
  FTM_MODULE[submodule]->SC |= FTM_SC_CLKS(0b11); /* Select external clock */
  
  FTM_MODULE[submodule]->COMBINE = 0x00000000; /* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */

  /* Enable the respective channel */
  FTM_MODULE[submodule]->SC |=0b1<<(16+channel) /* fill in */;

  /* Channel Control see S45.4.3.5 and Table 45-5 (S45.5.4) */
  FTM_MODULE[submodule]->CONTROLS[channel].CnSC = 0; /* Clear the register*/
  FTM_MODULE[submodule]->CONTROLS[channel].CnSC|=FTM_CnSC_MSB(0b1);/* MSB : Edge Align PWM */
  FTM_MODULE[submodule]->CONTROLS[channel].CnSC&=~FTM_CnSC_MSA(0b1); /* MSA : Edge Align PWM */
  FTM_MODULE[submodule]->CONTROLS[channel].CnSC|=FTM_CnSC_ELSB(0b1); /* ELSB : High-true pulses */
  FTM_MODULE[submodule]->CONTROLS[channel].CnSC&=~FTM_CnSC_ELSA(0b1); /* ELSA : High-true pulses */
 
  /* 45.5.7 - Edge-Aligned PWM (EPWM) */
  FTM_MODULE[submodule]->CNTIN=0; /* Always start counter from 0 */
 /* FTM_MODULE[submodule]->   counter rolls over at MOD  */

  FTM_MODULE[submodule]->CONF |= FTM_CONF_BDMMODE(0b11); /* Optional: enable in debug mode */
  
  /* Set the PWM */
  setPWM(submodule, channel, frequency, dutyCycle);
}

/******************************************************************************
 * Function:    setPWM
 * Description: Sets the output PWM for a given channel in FTM_MODULE
 ******************************************************************************/
void setPWM(int submodule, int channel, int frequency, float dutyCycle)
{
  uint16_t cthres;
  uint16_t cmax;

  cmax = PWM_CLOCK_FREQ/frequency-1;
  cthres = dutyCycle*(cmax+1);

  FTM_MODULE[submodule]->MOD = FTM_MOD_MOD(cmax); /* Set the PWM frequency */
  FTM_MODULE[submodule]->CONTROLS[channel].CnV = FTM_CnV_VAL(cthres);/* fill in */ /* Set the PWM duty cycle */
}


/******************************************************************************
 * Function:    outputTorque
 * Description: Outputs the torque to the haptic wheel in N-mm
 ******************************************************************************/
void outputTorque(float torque)
{
  // Calculate duty cycle
  float dutyCycle = torque/1000/3.1625+0.5;
 

  // Apply DC_UPPER_LIMIT, DC_LOWER_LIMIT
  if(dutyCycle<DC_LOWER_LIMIT) dutyCycle=DC_LOWER_LIMIT;
  else if(dutyCycle>DC_UPPER_LIMIT) dutyCycle=DC_UPPER_LIMIT;
 

  // Adjust the motor PWM
  setPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, MOTOR_FREQUENCY, dutyCycle);
}
