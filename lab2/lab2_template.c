/*
  EECS461
  Lab 2
  Updated: March 17th, 2014 by Scott Kenyon
           July  4th, 2014 by GPCz
           2017-03-15 RMC
           2018-03    Paul Domanico
           2021-01-12  jfr
           2021-09-02  jfr
           2021-09-17  jfr
           2022-08-12  jfr
*/


 #include "eecs461.h"
 #include "S32K144.h"
 #include "gpio.h"
 #include "qd.h"


 int main(void){
   uint16_t counter = 0;
   int32_t total = 0;
   int i;
   float angle;
   int16_t angleInt;
   int section = 4; //Set to case you are testing

   /* Initialize EECS461, QD, GPDO */
   initEECS461();
   initQD();

   for (i=0; i<16;i++)
   {

     initGPDO(LED_BASE[i],LED[i]);
   }

//   initGPDI(0, 12);
//   initGPDI(0, 13);

   while(1){
     switch(section){
     case 1://Read the counter
         counter=FTM2->CNT & FTM_CNT_COUNT_MASK;
       break;

     case 2://Overflow and Underflow
       counter=FTM2->CNT & FTM_CNT_COUNT_MASK;
       for(i=0;i<16;i++){
         writeGPIO(LED_BASE[i],LED[i],((counter>>i)&0b1));
       }
       break;

     case 3: //updateCounter and Faulty Casting
       counter=FTM2->CNT & FTM_CNT_COUNT_MASK;
       total=updateCounter();
       int16_t ledpresent=(total>>8)&0xffff;
       for(i=0;i<16;i++){
           writeGPIO(LED_BASE[i],LED[i],((ledpresent>>i)&0b1));
       }

       break;

     case 4: //Angle Calculation
       angle=updateAngle();
       angleInt=(int16_t)angle;
       for(i=0;i<16;i++){
           writeGPIO(LED_BASE[i],LED[i],((angleInt>>i)&0b1));
       }
       break;
     }
   }
 }
