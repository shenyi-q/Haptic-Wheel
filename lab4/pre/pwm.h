/*
   pwm.h
   University of Michigan
   EECS 461, Embedded Control Systems
   Pulse-Width Modulation using FlexTimer Module
   Read Chapter 45 in S32K144 User's Manual
   Section 45.5.7: Edge-Aligned PWM (EPWM) Mode
 */
#ifndef PWM_H_
#define PWM_H_

#include "S32K144.h"

#define MOTOR_FREQUENCY 20000
#define MOTOR_SUBMODULE 0
#define MOTOR_CHANNEL 0

#define FILTER_FREQUENCY 1000000
#define FILTER_SUBMODULE 3
#define FILTER_CHANNEL 0

#define FILTER_DUTY_CYCLE 0.5f

/* Functions for PWM */
void initPWMPCRs();
void setPWM(const int submodule, const int channel, const int frequency, const float dutyCycle);
void initPWM(const int submodule, const int channel, const int frequency, const float dutyCycle);
void outputTorque(const float torque);

#endif /* PWM_H_ */
