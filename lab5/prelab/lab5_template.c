/*
 * lab5.c
 *
 *  Created on: Jan 21, 2018
 *      Author: domanico
 
    Revision History:
    2020-09-27  jfr
    2021-01-12  jfr
    2021-09-30  jfr
    2022-08-12  jfr
 */

#include "S32K144.h"
#include "eecs461.h"

#include "pwm.h"
#include "LPIT.h"
#include "adc.h"
#include "gpio.h"
#include "math.h"

#define LPIT0_CHANNEL  0

float sineTable[10]; // Ten samples/period
int sineIndex = 0;
const float PI = 3.14159;
int value = 0;
int pwm_freq=0;
float pwm_duty=0.5;


void IsrA(void){

 	/* Turn on LED */
	writeGPIO(LED_BASE[0],LED[0],1);

	/* Read sine value */
	value=read_ADC0_single(0);

	/* Calculate PWM duty cycle */
	
	if(readGPIO(DIP_BASE[1],DIP[1])) pwm_duty=(float)value/5000*0.8+0.1;
	else pwm_duty=(float)value/5000*0.2+0.4;

	/* Set PWM frequency based on dipswitch */
	if(readGPIO(DIP_BASE[0],DIP[0])) pwm_freq=60000;
	else pwm_freq=20000;
	/* Set PWM duty cycle and frequency */
	setPWM(0, 0, pwm_freq, pwm_duty);

	/* Turn off LED */
	writeGPIO(LED_BASE[0],LED[0],0);

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);


}

void IsrB(void){

	/* Turn on LED */
	writeGPIO(LED_BASE[0],LED[0],1);

	/* Calculate and set PWM duty cycle */
	static int i=0;
	double theta;
	double dutyCycle;
	theta=2*PI*i/10;
	i=(i+1)%10;
	dutyCycle=0.5+0.4*sin(theta);
	setPWM(0, 0, 60000, dutyCycle);

	/* Turn off LED */
	writeGPIO(LED_BASE[0],LED[0],0);

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);

}

void IsrC(void){

	/* Turn on LED */
	writeGPIO(LED_BASE[0],LED[0],1);

	/* Calculate and set PWM duty cycle */
	static int j=0;
	setPWM(0,0,60000,0.5+0.4*sinTable[j]);
	j=(j+1)%10;

	/* Turn off LED */
	writeGPIO(LED_BASE[0],LED[0],0);

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);

}

int main(){

  initEECS461();
  enableLPIT();
  init_ADC0_single();

  /* Initialize PWMs  */
  initPWMPCRs();
  initPWM( 0,0,60000,0.5);

  /* Initialize GPIO  */
  initGPDO(LED_BASE[0],LED[0]);
  initGPDI(DIP_BASE[0],DIP[0]);
  initGPDI(DIP_BASE[1],DIP[1]);

  initLPIT(LPIT0_CHANNEL, 1000, &IsrA, 0xC);

  //pre-compute sinTable
  for(int i=0;i<10;i++){
	sinTable[i]=sin(2*PI*i/10);
  }

  while(1){
    /*Loop forever */
  };
}

