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
#include "


int main(void)
{
  int section = 0;
  initEECS461();
  
  // Initialize QD and ADC
  /* fill in */
  
  // Initialize PWMs
  initPWMPCRs();
  initPWM( /* fill in */ );
  initPWM( /* fill in */ );

  while(1)
  {
    switch(section)
    {
	case 1: // 0 Torque
	  break;
    case 2: //Analog Input Control of PWM
      /* fill in */ 
      break;
    case 3: //Change PWM Frequency to 1 kHz torque = 0 N-mm
       /* fill in */ 
      break;
    case 4: //Set torque to 200 N-mm
       /* fill in */ 
      break;
    case 5: //Virtual Spring
       /* fill in */ 
      break;
    case 6: // Virtual Wall
       /* fill in */ 
      break;
    }
  }
}
