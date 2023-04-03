/*
   worlds.c
   University of Michigan
   EECS 461, Embedded Control Systems
   Virtual Worlds

   Revision History
     2011-12-01  Jay Patel
     2014-02-24  Scott Kenyon
     2017-03-14  RMC
     2020-09-23  jfr
     2021-01-12  jfr
     2022-08-12  jfr
 */

#include "worlds.h"
#include "gpio.h"
#include "math.h"
 
float k = 17.78;
float J = 0.45;
float T = 0.001;
float b = 0.01778;
float bwall = 0.75;

//====================LAB 4================================
/***************************************************************************
 * Virtual Wall
 ***************************************************************************/
float virtualWall(float angle)
{
  if(angle>0) return -750*(angle-WALL_POSITION);
  else return 0;
}

/***************************************************************************
 * Virtual Spring 
 ***************************************************************************/
float virtualSpring(float angle)
{ 
  return -10*(angle-WALL_POSITION);
}

//====================LAB 6================================
/***************************************************************************
 * Virtual Spring Damper
***************************************************************************/
float virtualSpringDamper(float angle, float velocity)
{
  return -10*(angle-WALL_POSITION)-0.5*velocity;
//  return -10*(angle-WALL_POSITION)-0.6475*velocity;
}

/***************************************************************************
 * Virtual Wall Damper
***************************************************************************/
float virtualWallDamper(float angle, float velocity)
{

  if(angle>0) return -500*(angle-WALL_POSITION)-bwall*velocity;
  else return 0;
}

/***************************************************************************
 * Virtual Spring Mass
***************************************************************************/
float virtualSpringMass(float angle)
{
  static float x1=0;
  static float x2=0;
  float torque=-17.78*(x1-angle);
  float old_x1=x1;
  x1=x1+0.001*x2;
  x2=-0.0395*old_x1+x2+0.0395*angle;
  return torque;        
}

/***************************************************************************
 * Virtual Spring Mass Damper
***************************************************************************/
float virtualSpringMassDamper(float angle, float velocity) 
{

  static float x1=0;
  static float x2=0;
  float torque=k*(x1-angle)+b*(x2-velocity);
  float old_x1 = x1;
  x1=x1+T*x2;
  x2=-k*T/J*old_x1+(1-b*T/J)*x2+k*T/J*angle+b*T/J*velocity;
  return torque;

}

/***************************************************************************
 * Virtual Knob
***************************************************************************/ 
float virtualKnob(float angle, float velocity) 
{     
  //There are many ways this can be implemented
   float real_angle = fmod(angle,360);

  if((real_angle>40 && real_angle<50)){
      return 20*(real_angle-45)-5*velocity;
  }
  else if((real_angle>130 && real_angle<140)){
      return 20*(real_angle-135)-5*velocity;
  }
  else if((real_angle>220 && real_angle<230)){
      return 20*(real_angle-225)-5*velocity;
  }
  else if((real_angle>310 && real_angle<329)){
      return 20*(real_angle-315)-5*velocity;
  }
//
//  if((angle>40 && angle<50)){
//      return 500*(angle-45)-0.25*velocity;
//  }
//  else if((angle>130 && angle<140)){
//      return 500*(angle-135)-0.25*velocity;
//  }
//  else if((angle>220 && angle<230)){
//      return 500*(angle-225)-0.25*velocity;
//  }
//  else if((angle>310 && angle<320)){
//      return 500*(angle-315)-0.25*velocity;
//  }
//

  

}

