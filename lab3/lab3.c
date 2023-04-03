/*
 * lab3.c
 *
 *  Created on: Sep 27, 2022
 *      Author: shenyiq
 */



 #include "eecs461.h"
 #include "S32K144.h"
 #include "gpio.h"
 #include "adc.h"


 int main(void){

   int section = 3; //Set to case you are testing

   /* Initialize EECS461, QD, GPDO */
   initEECS461();
   init_ADC0_single();

   initGPDO(LED_BASE[0],LED[0]);

   int iAnalog = 0;
   int ledout = 0;



   while(1){
     switch(section){
     case 1://Read the counter

       iAnalog=read_ADC0_single(0);


       break;

     case 2://Overflow and Underflow
         writeGPIO(LED_BASE[0],LED[0],1);
         iAnalog=read_ADC0_single(0);
         writeGPIO(LED_BASE[0],LED[0],0);
       break;

     case 3: //updateCounter and Faulty Casting

        iAnalog=read_ADC0_single(0);
        if(iAnalog<2500){
          ledout=0;
        }
        else{
          ledout=1;
        }
        writeGPIO(LED_BASE[0],LED[0],ledout);

       break;

     case 4: //Angle Calculation

       break;
     }
   }
 }


