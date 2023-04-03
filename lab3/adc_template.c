/*
   adc.c
   University of Michigan
   EECS 461, Embedded Control Systems
   Analog to Digital Converter
   Read Chapter 42 in S32K144 User's Manual

   Code based off of ADC.c (c) 2016 Freescale Semiconductor, Inc.
   19 Jul 2016 Osvaldo Romero: Initial version

   Revision History:
     2019-01    jfr
     2019-01    Paul Domanico
     2021-01-12 jfr
     2021-09-10 jfr
     2022-08-12 jfr
*/


#include "adc.h"


void init_ADC0_single(void)  {
  /* Table 27-9 Peripheral module clocking */
  /* 29.6.19 PCC ADC0 Register (PCC_ADC0) */
  PCC->PCCn[PCC_ADC0_INDEX]  &=~PCC_PCCn_CGC(0b1);  /* Disable clock to change PCS */
  PCC->PCCn[PCC_ADC0_INDEX]  &=~PCC_PCCn_PCS(0b111); //clear PCS bitfield
  PCC->PCCn[PCC_ADC0_INDEX]  |=PCC_PCCn_PCS(0b001);/* fill in */;  /* Select clock option 1 */
  PCC->PCCn[PCC_ADC0_INDEX]  |=PCC_PCCn_CGC(0b1);/* fill in */;  /* Enable  clock */

 
  /*42.4.2 - ADC Status and Control Register 1 (SC1AA - SC1Z)*/
  ADC0->SC1[0] |= ADC_SC1_ADCH(0x1F);  /* Disable Module  */
  ADC0->SC1[0] &= ~ADC_SC1_AIEN(0b1);       			   /* Disable interrupts */
  
  /*42.4.3 - ADC Configuration Register 1: CFG1 */								  
  ADC0->CFG1 &= ~ADC_CFG1_ADICLK(0b11);   /* Alternate clock 1 */
  ADC0->CFG1 &= ~ADC_CFG1_MODE(0b11);//clear MODE bitfield
  ADC0->CFG1 |= ADC_CFG1_MODE(0b01);              /* 12-bit conversion */
  ADC0->CFG1 &= ~ADC_CFG1_ADIV(0b11);              /*  Prescaler=1 */
  
  /*42.4.4 - ADC Configuration Register 2: CFG2 */
  ADC0->CFG2 &= ~ADC_CFG2_SMPLTS(0b11111111);			//clear MODE bitfield					  
  ADC0->CFG2 |= ADC_CFG2_SMPLTS(0b00001100);/* fill in */;   /* set sample time to 13 ADC clks */
  
  /*42.4.7 - Status and Control Register 2: SC2 */
  ADC0->SC2 &= ~ADC_SC2_ADTRG(0b1)/* fill in */;    /*  SW trigger */
  ADC0->SC2 &= ~ADC_SC2_REFSEL(0b11);              /* use voltage reference pins VREFH & VREEFL */
                                  
 /*42.4.8 - Status and Control Register 3: SC3 */								  
  ADC0->SC3 &= ~ADC_SC3_CAL(0b1)/* fill in */;    /* Do not start calibration sequence */
  ADC0->SC3 &= ~ADC_SC3_ADCO(0b1);              /* One conversion performed (single mode) */
  ADC0->SC3 &= ~ADC_SC3_AVGE(0b1);              /* HW average function disabled */
  
}


uint8_t ADC0_complete(void)  {
  /* 42.4.2.4 - COCO flag */
  return ((ADC0->SC1[0]&ADC_SC1_COCO_MASK)>>ADC_SC1_COCO_SHIFT );  /*return COCO flag*/
}


uint32_t read_ADC0_single(uint16_t inputChannel)  {
  uint16_t adc_result=0;
 
  /* inform students of the mask and functions in header for ADCH */
  ADC0->SC1[0] &= ~ADC_SC1_ADCH(0x1F);	      /* Clear prior ADCH bits */
  ADC0->SC1[0] |= ADC_SC1_ADCH(inputChannel);       /* Initiate Conversion*/

  while(ADC0_complete() == 0); // wait for completion
 
  /* 42.4.31.2 - Data Result Register*/
  adc_result = ADC0->R[0]&ADC_R_D_MASK;      /* For SW trigger mode, R[0] is used */
  
  /* Convert result to mv for 0-5V range */
  /* Hint: can this be done without floating point math for speed? */
  return  adc_result*5*1000/4096; 
}
