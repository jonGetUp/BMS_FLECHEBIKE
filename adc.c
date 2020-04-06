/********************************************************************************/
/**
 * \file
 *
 * \brief Library offering adc functions for load current measurement
 *
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/


#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif
#include "main.h"
#include "adc.h"

/********************************************************************************/
/* inits the ADC converter                                                      */
/********************************************************************************/
void adc_init(void)
{
    //----------------------------------------------------------------------------
    // internal voltage reference control
#if PROTO_NUM == 1
    FVRCONbits.EN = 0;          // reference voltage module not used
#elif PROTO_NUM == 2
    FVRCONbits.ADFVR = 0b11;    // voltage ref is 4.096 V --> proto 2 @ 5V VCC 
    FVRCONbits.EN = 1;          // enable reference voltage module
#else
    #error "Model of hardrare not defined ! "
#endif
    //----------------------------------------------------------------------------
    // adc init
    ADCON0bits.ON = 1;          // enabled ADC module
    ADCON0bits.CS = 0;          // clock is from Fosc / ADCLK
    ADCLK = 0x3F;               // 64MHz / 128 => 2us TAD 
    ADACQ = 3;                  // acquisition time is 3 x TAD
    ADCON0bits.FM = 1;          // data is right justified
    ADPCH = 0x00;               // Select RA0 to be ADC input
    ADREFbits.NREF = 0;         // ADC negative ref is VSS
#if PROTO_NUM == 1
    ADREFbits.PREF = 0b00;      // ADC positive ref is VCC (depend on PROTO_NUM)    
#elif PROTO_NUM == 2
    ADREFbits.PREF = 0b11;      // ADC positive ref is FVR (depend on PROTO_NUM)    
#endif
}

/********************************************************************************/
/* Do one measure                                                               */
/********************************************************************************/
int32_t adc_getOneMeasure(void)
{
#if PROTO_NUM == 1
    #define ADC_OFFSET_MV   (1600)  // offset for no current = 1.6V
    #define ADC_REF_MV      (3700)  // adc reference (vcc is about 3.6V)
    #define ADC_RESOL       (4096)  // adc resolution 12 bits
    #define GAIN            (32)    // MAX9918 gain   
    #define INV_SHUNT       (1000)  // 1/shunt resistor
#elif PROTO_NUM == 2
    #define ADC_OFFSET_MV   (2048)  // offset for no current = 2.048V
    #define ADC_REF_MV      (4096)  // adc reference internal (4.096V)
    #define ADC_RESOL       (4096)  // adc resolution 12 bits
    #define GAIN            (32)    // MAX9918 gain   
    #define INV_SHUNT       (1000)  // 1/shunt resistor
#endif
    uint16_t adValmV;
    int32_t     temp;
    
    ADCON0bits.GO = 1;          //start ADC measure

    while(ADCON0bits.GO == 1)   // wait end of conversion
    {};                         
    adValmV = ((uint32_t)ADRES * ADC_REF_MV) / ADC_RESOL;
    if(adValmV > ADC_OFFSET_MV)   // current flow from battery to motor
    {
        temp = adValmV - ADC_OFFSET_MV;     // positive current
    }
    else                        // current flow from charger to battery
    {
        temp = -(ADC_OFFSET_MV - adValmV);  // negative current
    }
    temp = (temp * INV_SHUNT) / GAIN;       // real current in mA
    return temp;
}
