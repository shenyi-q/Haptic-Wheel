/*
  EECS461
  Lab 4
   Revision History
     2020-09-23  jfr
     2021-01-12  jfr
     2022-08012  jfr
*/
#include "eecs461.h"
#include "qd.h"
#include "pwm.h"
#include "worlds.h"
#include "S32K144.h"
#include "adc.h"
 #include "gpio.h"


int main(void)
{
  int section = 6;
  initEECS461();
  int iAnalog=0;
  float angle=0;
  // Initialize QD and ADC
  /* fill in */
  initQD();
  init_ADC0_single();
  initGPDO(LED_BASE[0],LED[0]);
  
  // Initialize PWMs
  initPWMPCRs();
  initPWM( MOTOR_SUBMODULE,MOTOR_CHANNEL,MOTOR_FREQUENCY,0.5 );
  initPWM( FILTER_SUBMODULE,FILTER_CHANNEL,FILTER_FREQUENCY,0.5 );

  while(1)
  {
    switch(section)
    {
	case 1: // 0 Torque
	  outputTorque(0);
	  break;
    case 2: //Analog Input Control of PWM


      iAnalog=read_ADC0_single(0);
      setPWM(0,0,20000,(float)iAnalog/5000*(0.76-0.24)+0.24);

      break;
    case 3: //Change PWM Frequency to 1 kHz torque = 0 N-mm
       /* fill in */ 
      outputTorque(0);
      break;
    case 4: //Set torque to 200 N-mm
       outputTorque(200);
      break;
    case 5: //Virtual Spring
      writeGPIO(LED_BASE[0],LED[0],1);
      angle=updateAngle();

      outputTorque(virtualSpring(angle));
      writeGPIO(LED_BASE[0],LED[0],0);



      break;
    case 6: // Virtual Wall
      angle=updateAngle();

      outputTorque(virtualWall(angle));
      break;
    }
  }
}
