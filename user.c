/******************************************************************************/
/* Files to Include                                                           */
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

#include "Pinnames.h"    /*header where all pin name are defined as in BMS plan*/
#include "user.h"
#include "isl94212regs.h"
#include "isl94212.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    //--------------------------------------------------------------------------
    // PORTA connections:
    // BIT   DIR        NAME
    //  0    INPUT_A    Isense (analogic input from MAX9918ASA)
    //  1    OUTPUT     x
    //  2    OUTPUT     x
    //  3    INPUT_A    1.5Vref (analogic reference voltage from MAX6018)
    //  4    OUTPUT     x
    //  5    OUTPUT     /SS (chip select for ISL94212INZ)
    //  6    OUTPUT     crystal 32.768kHz
    //  7    INPUT      crystal 32.768kHz    
    LATA = 0x20;
    TRISA = 0x49;
    ANSELA= 0x09;
    //--------------------------------------------------------------------------
    // PORTB connections:
    // BIT   DIR        NAME
    //  0    INPUT_INT  /Batt_FAULT from LTC4368HMS
    //  1    INPUT_INT  /FAULT from ISL94212INZ
    //  2    INPUT_?    /DRDY from ISL94212INZ
    //  3    INPUT      CAN_RX
    //  4    OUTPUT     CAN_TX
    //  5    OUTPUT     x
    //  6    INPUT      ICSP_CLK (debug / prog)
    //  7    INPUT      ICSP_DAT (debug / prog) 
    LATB = 0;
    TRISB = 0xCF;
    ANSELB = 0;
    //--------------------------------------------------------------------------
    // PORTC connections:
    // BIT   DIR        NAME
    //  0    OUTPUT     /SHDN shutdown to LTC4368HMS
    //  1    OUTPUT     EN_ISL enable to ISL94212INZ
    //  2    OUTPUT     CAN_POWERDOWN
    //  3    OUTPUT     SCLK clock to ISL94212INZ
    //  4    INPUT      MISO data from ISL94212INZ
    //  5    OUTPUT     MOSI data to ISL94212INZ
    //  6    OUTPUT     /EN_Isense to MAX9918ASA
    //  7    OUTPUT     EN_Vref enable reference voltage
    LATC = 0b01000100;
    TRISC = 0x10;
    ANSELC = 0;
    //--------------------------------------------------------------------------        
    /*properly map pins for SPI communication (MASTER FULL DUPLEX )*/
    //SPI OUTPUTS
    // select PORTC<3> as SPI1(SCK) output
    RC3PPS=0b011110;
    //select PORTC<5> as SPI1(SDO) output
    RC5PPS=0b011111;
    //select PORTA<5> as SPI1(nSS) output
    RA5PPS=0b100000;
    //SPI INPUTS
    //select PORTC<4> as SPI1(SDI)
    SPI1SDIPPS=0b00010100;
    //--------------------------------------------------------------------------            
    /*properly map pins for CAN communication*/
    // CAN input on RB3;
    CANRXPPS=0b00001011;
    // CAN output CANTX0 on RB4
    RB4PPS=0b110011;
    //--------------------------------------------------------------------------            
    // PPSLOCK mapping can't be changed
    PPSLOCKED=1;
    //--------------------------------------------------------------------------            
    // setup timer 0 for 10 ms interrupt
    T0CON0bits.EN = 1;                  // enable timer 0
    T0CON0bits.MD16 = 1;                // 16 bits timer
    T0CON1bits.CS = 2;                  // osc = Fosc/4 -> 16MHz
    T0CON0bits.OUTPS = 15;              // divide by 16 -> 1MHz
    TMR0 = 65535 - 10000;               // isr each 10ms
    TMR0IE = 1;                         // enable interrupts
    //--------------------------------------------------------------------------            
    // external interrupt control
    IOCIE = 1;                          // enable external interrupts
    IOCBP = 0x03;                       // enable rising on RB0, RB1
    IOCBN = 0x03;                       // enable falling on RB0, RB1
}

bmsFault FaultAnalyse(void)
{
    uint16_t statFault;
    bmsState.curFault = NO_FAULT;
    //--------------------------------------------------------------------------
    statFault = isl_read(ISL_FAULTSTATUS);
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    if(statFault & 0x010)           // over temperature fault
    {
        bmsState.curFault = OVERTEMP;
        bmsState.curFaultDetail = isl_read(ISL_OVERTEMP_F);  // get overtemp sensor
        isl_write(ISL_OVERTEMP_F,0);    // clear fault flag
        isl_write(ISL_FAULTSTATUS,0);   // clear fault flag
        return bmsState.curFault;
    }    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    if(statFault & 0x020)           // over voltage fault
    {
        bmsState.curFault = OVERVOLT;
        bmsState.curFaultDetail = isl_read(ISL_OVERVOLT_F);  // get overvolt info
        isl_write(ISL_OVERVOLT_F,0);    // clear fault flag
        isl_write(ISL_FAULTSTATUS,0);   // clear fault flag
        return bmsState.curFault;
    }    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    if(statFault & 0x040)           // under voltage fault
    {
        bmsState.curFault = UNDERVOLT;
        bmsState.curFaultDetail = isl_read(ISL_UNDERVOLT_F);  // get undervolt info
        isl_write(ISL_UNDERVOLT_F,0);   // clear fault flag
        isl_write(ISL_FAULTSTATUS,0);   // clear fault flag
        return bmsState.curFault;
    }    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    if(statFault & 0x080)           // open wire fault
    {
        bmsState.curFault = OPENWIRE;
        bmsState.curFaultDetail = isl_read(ISL_OPENWIRE_F);  // get open wire info
        isl_write(ISL_OPENWIRE_F,0);    // clear fault flag
        isl_write(ISL_FAULTSTATUS,0);   // clear fault flag
        return bmsState.curFault;
    }
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    bmsState.curFault = OTHER_FAULT;
    bmsState.curFaultDetail = statFault;
    return bmsState.curFault;
}

smMain sm_execute_idle(void)
{
    if(bmsState.curFault != NO_FAULT)
    {
        return SM_ERROR_IDLE; 
    }
    //--------------------------------------------------------------
    if(bmsState.battery_current > 100)
    {
        isl_write(ISL_UNDERVOLT_SET,isl_conv_mv2Cell(3500)); // set limit for load
        return SM_LOAD; 
    }
    //--------------------------------------------------------------
    if(bmsState.battery_current < -100)
    {
        return SM_SLOW_CHARGE; 
    }
    //--------------------------------------------------------------
    if(bmsState.charger_fast_present != 0)
    {
        return SM_FAST_CHARGE;
    }    
    return SM_IDLE;
}