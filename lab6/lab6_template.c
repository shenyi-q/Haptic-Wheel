/*
  EECS461
  Lab 6
  
  Revision history:
  2020-10-4  jfr
  2021-01-12 jfr
  2021-10-10 jfr
  2022-08-12 jfr
  
 */

#include "S32K144.h"
#include "eecs461.h"
#include "LPIT.h"
#include "LPUART.h"
#include "qd.h"
#include "pwm.h"
#include "worlds.h"
//#define LPIT0_CHANNEL  0

void wallDamper(void)
{
	/* ISR for the wall-damper system */


   static float angle = 0;

    float newangle = updateAngle();
    float velocity = (newangle-angle)*WORLDISR_FREQUENCY;
    angle=newangle;


  outputTorque(virtualWallDamper( angle, velocity));



    /* Make sure to clear interrupt flag */
    clearFlagLPIT(WORLDISR_LPIT_CHANNEL);


  /* Make sure to clear interrupt flag */

}

void springDamper(void)
{
	/* ISR for the spring-damper system */
  static float angle = 0;

  float newangle = updateAngle();
  float velocity = (newangle-angle)*WORLDISR_FREQUENCY;
  angle=newangle;



  outputTorque(virtualSpringDamper(angle, velocity));
  /*  Make sure to clear interrupt flag  */
  clearFlagLPIT(WORLDISR_LPIT_CHANNEL);
}

void springMass(void)
{
	/* ISR for the spring-mass system */


  outputTorque(virtualSpringMass(updateAngle()));



  /* Make sure to clear interrupt flag */
  clearFlagLPIT(WORLDISR_LPIT_CHANNEL);

}

void springMassDamper(void)
{
	/* ISR for the spring-mass-damper system */
  static float angle = 0;

  float newangle = updateAngle();
  float velocity = (newangle-angle)*WORLDISR_FREQUENCY;
  angle=newangle;

  outputTorque(virtualSpringMassDamper(angle,velocity));
  /* Make sure to clear interrupt flag  */
  clearFlagLPIT(WORLDISR_LPIT_CHANNEL);
}

void knobIndents(void)
{
	/* ISR for the virtual knob system */
  static float angle = 0;

  float newangle = updateAngle();
  float velocity = (newangle-angle)*WORLDISR_FREQUENCY;

  angle=newangle;
//  if (velocity>10 || velocity <-10){
//    outputTorque(0);
//    return;
//  }
  outputTorque(virtualKnob(angle,velocity));
  /* Make sure to clear interrupt flag  */
  clearFlagLPIT(WORLDISR_LPIT_CHANNEL);

}

int main(void) {
  char byte_in = 0;
  void (*interrupt)(void);
  int section = 6;

  initEECS461();
  initQD();
  enableLPIT();

  // Initialize PWMs
  initPWMPCRs();

  initPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, MOTOR_FREQUENCY, 0.5f);
  initPWM(FILTER_SUBMODULE, FILTER_CHANNEL, FILTER_FREQUENCY, 0.5f);

  if(section == 5 || section == 1)
  {
    LPUART1_init();
    LPUART1_transmit_string("\n\n\n\n\n\n\n\rSerial Output Enabled.");
  }
  switch(section)
  {
    case 1:
      interrupt = &wallDamper;
      break;

    case 2:
      interrupt = &springDamper;
      break;

    case 3:
      interrupt = &springMass;
      break;

    case 4:
      interrupt = &springMassDamper;
      break;

    case 5:
      interrupt = &springMassDamper;
      break;

    case 6:
      interrupt = &knobIndents;
      break;
  }

  //Fill in the arguments to initialize the appropriate interrupt routine for
  //each part of the lab. Be sure to use the correct channel specified in worlds.h
  initLPIT(WORLDISR_LPIT_CHANNEL, WORLDISR_FREQUENCY, interrupt, 0xC);

  while(1){
    if(section == 5 || section ==1){
      byte_in = LPUART1_receive_char();
      /* adjust K and M from the keyboard. Must be a critical section. */
      DISABLE_INTERRUPTS();
      if(byte_in=='1'){
        k*=1.1;
        b=k*T;
        bwall=0;
      }
      else if(byte_in=='2'){
        k*=0.9;
        b=k*T;
        bwall=0.25;
      }

      else if(byte_in=='3'){
        J*=1.1;
        bwall=0.5;
      }
      else if(byte_in=='4'){
        J*=0.9;
        bwall=0.75;
      }
      ENABLE_INTERRUPTS();

    }
  }

}
