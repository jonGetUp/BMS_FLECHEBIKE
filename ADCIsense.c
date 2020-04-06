/*File where SPI communications and functions are defined for mesure of Isense with pic18f26k83's ADC*/

/*author : Martin Aladjidi - device : pic18f26k83 compiler : xc8 v1.45*/



/*Files to Include                                                            */
/******************************************************************************/

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

#include "ADCIsense.h"

    void setADCIsense(){
        /*initialisation of the fixed voltage ref (FVR on pic18f26)*/
        
        FVRCONbits.ADFVR=0b10; // voltage ref is 2.048 V --> Insuffisant pour Isense !
        FVRCONbits.EN=1; // fixed voltage ref module engaged
        
        /*ADC module*/
        
        /*channel selection*/
        ADPCH=0x00;// Select RA0 to be ADC input
        /*Voltage ref selection*/
        ADREFbits.NREF=0; // ADC negativ ref is VSS
        //ADREFbits.PREF=11; // ADC positiv ref is FVR (2.048V) --> insuffisant
        ADREFbits.PREF=00;// VREF+ is connected to VDD
        /*Clock source selection*/
        ADCON0bits.CS=1; //clock provided by dedicated RC osc (avoid pb changing Fosc)
        ADCON0bits.FM=1; // Data  is right justified
        ADCON0bits.ON=1; // ADC Module is enabled        
    }
    
    unsigned short getADCmesure(){
        unsigned short result;
        result=0x0000;
        
        ADCON0bits.GO=1;  //Make an ADC reading
        
        while(ADCON0bits.GO ==1); // wait for the conversion to be completed
            
       // Transfert the two byte register in a single variable
        result = ADRESH;
        result=result<<8; // eight bit left shift
        result=result|ADRESL;
        
        return (result);
    }