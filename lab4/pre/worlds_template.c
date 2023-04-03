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
 

//====================LAB 4================================
/***************************************************************************
 * Virtual Wall
 ***************************************************************************/
float virtualWall(float angle)
{
  if(angle>0) return -500*(angle-WALL_POSITION);
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
}

/***************************************************************************
 * Virtual Wall Damper
***************************************************************************/
float virtualWallDamper(float angle, float velocity)
{
}

/***************************************************************************
 * Virtual Spring Mass
***************************************************************************/
float virtualSpringMass(float angle)
{        
}

/***************************************************************************
 * Virtual Spring Mass Damper
***************************************************************************/
float virtualSpringMassDamper(float angle, float velocity) 
{      
}

/***************************************************************************
 * Virtual Knob
***************************************************************************/ 
float virtualKnob(float angle, float velocity) 
{     
  //There are many ways this can be implemented    
}

