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
#include "system_limits.h"

/********************************************************************************/
/* inits the ADC converter                                                      */
/********************************************************************************/
void adc_init(void)
{
    //----------------------------------------------------------------------------
    // internal voltage reference control
    FVRCONbits.EN = 0;          // reference voltage module not used
    //----------------------------------------------------------------------------
    // adc init
    ADCON0bits.ON = 1;          // enabled ADC module
    ADCON0bits.CS = 0;          // clock is from Fosc / ADCLK
    ADCLK = 0x3F;               // 64MHz / 128 => 2us TAD 
    ADACQ = 3;                  // acquisition time is 3 x TAD
    ADCON0bits.FM = 1;          // data is right justified
    ADREFbits.NREF = 0;         // ADC negative ref is VSS
#if PROTO_NUM == 1
    ADREFbits.PREF = 0b00;      // ADC positive ref is VCC (depend on PROTO_NUM)    
#elif PROTO_NUM == 2
    ADREFbits.PREF = 0b10;      // ADC positive ref is VREF+ pin (depend on PROTO_NUM)    
#else
    #error "Model of hardrare not defined ! "
#endif
}
int32_t simulated_current = 0;  // to change with debugger for tests !
/********************************************************************************/
/* Do one measure                                                               */
/********************************************************************************/
int32_t adc_getOneMeasure(uint8_t channel)
{
#if PROTO_NUM == 1
    #define ADC_OFFSET_MV   (1600L)  // offset for no current = 1.6V
    #define ADC_REF_MV      (3700)  // adc reference (vcc is from 3.5 to 4.2V unusable)
    #define ADC_RESOL       (4096)  // adc resolution 12 bits
    #define GAIN            (32)    // MAX9918 gain   
    #define INV_SHUNT       (1000)  // 1/shunt resistor
#elif PROTO_NUM == 2
    #define ADC_OFFSET_MV   (2048L)  // offset for no current = 2.048V
    #define ADC_REF_MV      (4096)  // adc reference external (4.096V)
    #define ADC_RESOL       (4096)  // adc resolution 12 bits
    #define GAIN            (32)    // MAX9918 gain   
    #define INV_SHUNT       (1000)  // 1/shunt resistor
#endif
    uint16_t adValmV;
    int32_t     temp;
    
    ADPCH = channel;            // Select channel to measure
    ADCON0bits.GO = 1;          //start ADC measure

    while(ADCON0bits.GO == 1)   // wait end of conversion
    {};                         
    adValmV = ((uint32_t)ADRES * ADC_REF_MV) / ADC_RESOL;
    //--------------------------------------------------------------------------
    if(channel == ADC_CHANNEL_CURRENT)      // current calculation
    {
#if PROTO_DEBUG == 0        
        if(adValmV > ADC_OFFSET_MV)   // current flow from battery to motor
        {
            temp = adValmV - ADC_OFFSET_MV;     // positive current
        }
        else                        // current flow from charger to battery
        {
            temp = -(ADC_OFFSET_MV - adValmV);  // negative current
        }
        temp = (temp * INV_SHUNT) / GAIN;       // real current in mA
        temp = temp - SL_ZERO_CURRENT_OFFSET;
#else
        return simulated_current;  
#endif
    }
    //--------------------------------------------------------------------------
    if(channel == ADC_CHANNEL_VOLTAGE)       // voltage calculation (8.9 divisor)
    {
        temp = (int32_t)(adValmV * 10.51);               // divisor 620k - 5.9Meg
    }
    return temp;
}
