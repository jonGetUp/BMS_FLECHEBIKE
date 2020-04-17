/********************************************************************************/
/**
 * \file
 *
 * \brief Library offering ISL94212 functions 
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
#include "isl94212.h"
#include "isl94212regs.h"
#define _XTAL_FREQ 64000000L

/**
* The NTC table has 33 interpolation points.
* Unit:0.1 °C
*
*/
int32_t NTC_table[33] = {
  1835, 1493, 1151, 969, 847, 754, 678, 615, 
  560, 511, 466, 425, 386, 350, 316, 282, 250, 
  218, 187, 156, 125, 93, 61, 28, -6, -43, 
  -83, -126, -176, -235, -311, -428, -545
};

/********************************************************************************/
/* inits the SPI and the battery controller                                     */
/********************************************************************************/
void isl_init(void)
{
    uint8_t i;
    // see PPS in user.c , O/I pin must have been mapped properly
    //initialisation of SPIxCON{0,1,2} register
        
    SPI1CON0bits.EN = 0;        // SPI module shut down for initialization
    SPI1CON0bits.LSBF = 0;      // data exchanged MSb first
    SPI1CON0bits.MST = 1;       // Module in Master mode
                
    SPI1CLKbits.CLKSEL = 0;     // SPI clock is Fosc @ 64Mhz
    SPI1BAUD = 31;              // fCLK=FOSC/2*(BAUD+1) -> 1MHz (max. 2MHz)
        
    SPI1CON1bits.SMP = 0;       //SDI sampled at the middle
    SPI1CON1bits.CKE = 1;       // output change on falling edge of the clock
    SPI1CON1bits.CKP = 0;       // clock polarity, idle state is low
        
    SPI1CON1bits.FST = 0;       // fast start mot enabled bit high
    SPI1CON1bits.SSP = 1;       // SS ( nCS in isl datasheet) is active low
    SPI1CON1bits.SDIP = 0;      // SDI polarity activ high
    SPI1CON1bits.SDOP = 0;      // SDO polarity activ high
        
    SPI1CON2bits.SSET = 0;      //active when transit counter is not 0, ss controled by transfert counter
    SPI1CON2bits.TXR = 1;
    SPI1CON2bits.RXR = 1;       // full duplex mode

    SPI1CON0bits.BMODE=1;       // setting applies to every byte: total bits sent is SPIxTWIDTH*SPIxTCNT, end-ofpacket occurs when SPIxTCNT = 0
        
    SPI1TWIDTH=0b00000000;      // All message will be multiple of a byte 
    SPI1CON0bits.EN=1;          // SPI module on and operating
   
    //----------------------------------------------------------------------------    
    isl_calc_vref_and_temp();   // get true reference voltage and temperatures
    //----------------------------------------------------------------------------
    // scan all parameters
    isl_command(ISL_CMD_SCANVOLT);      // scan all voltages
    bmsState.serialNumber = isl_read(ISL_SERIAL_NR1);
    bmsState.serialNumber = bmsState.serialNumber << 14;
    bmsState.serialNumber |= isl_read(ISL_SERIAL_NR2);
    bmsState.batVolt = isl_conv_vbat2mV(isl_read(ISL_VBATT));
    for(i=0;i<12;i++)
    {
       bmsState.cellVolt[i] = isl_conv_cell2mV(isl_read(ISL_VBATT + ((i+1) << 6))); 
    }
    bmsState.fault_overvoltage = isl_read(ISL_OVERVOLT_F);
    bmsState.fault_undervoltage = isl_read(ISL_UNDERVOLT_F);
    bmsState.fault_openwire =  isl_read(ISL_OPENWIRE_F);
    bmsState.fault_setup.Reg = isl_read(ISL_FAULTSETUP);
    bmsState.fault_status.Reg = isl_read(ISL_FAULTSTATUS);
    bmsState.cell_setup = isl_read(ISL_CELLSETUP);
    bmsState.overtemp_fault = isl_read(ISL_OVERTEMP_F);
    bmsState.overvoltage_limit = isl_conv_cell2mV(isl_read(ISL_OVERVOLT_SET));
    bmsState.undervoltage_limit = isl_conv_cell2mV(isl_read(ISL_UNDERVOLT_SET));
    bmsState.externalTemp_limit = isl_read(ISL_EXTTEMP_SET);
    bmsState.balance_setup.Reg = isl_read(ISL_BALANCE_SET);
    bmsState.balance_status = isl_read(ISL_BALANCE_STAT);
    bmsState.balance_wdt_time.Reg = isl_read(ISL_BALANCE_WDT);
    bmsState.user1 = isl_read(ISL_USER1);
    bmsState.user2 = isl_read(ISL_USER2);
    bmsState.device_setup.Reg = isl_read(ISL_DEVICESETUP);
    bmsState.intTemp_limit = isl_read(ISL_INTTEMP_LIMIT);
    bmsState.trimVolt = isl_read(ISL_TRIM_VOLT);
    for(i=0;i<12;i++)
    {
       bmsState.cell_balance[i] = isl_read(ISL_BALANCE_BASE + (((i*2)+1) << 6)); 
       bmsState.cell_balance[i] = bmsState.cell_balance[i] << 14;
       bmsState.cell_balance[i] |= isl_read(ISL_BALANCE_BASE + ((i*2) << 6)); 
    }    
    bmsState.cell_in_balance = isl_read(ISL_CELL_IN_BALANCE);    
    //--------------------------------------------------------------------------
}

/********************************************************************************/
/* Scan voltage and update                                                      */
/********************************************************************************/
void isl_scan_update_voltages(void)
{
    if(bmsState.balanceInWork == 1)         // if currently balancing
    {
        isl_command(ISL_BALANCE_INHIBIT);   // stop balancing
        __delay_ms(10);                     // wait a moment before measure
        isl_command(ISL_CMD_SCANVOLT);      // scan voltages
        isl_command(ISL_BALANCE_ENABLE);    // re-enable balancing
    }
    else
    {
        isl_command(ISL_CMD_SCANVOLT);       // scan voltages
    }
    // update cells & pack voltages
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    bmsState.batVolt = isl_conv_vbat2mV(isl_read(ISL_VBATT));
    for(uint8_t i=0;i<12;i++)
    {
       bmsState.cellVolt[i] = isl_conv_cell2mV(isl_read(ISL_VBATT + ((i+1) << 6))); 
    }    
}

/********************************************************************************/
/* Calculate right Vref and temperatures                                        */
/********************************************************************************/
void isl_calc_vref_and_temp(void)
{
    float dT;
    float adjustment;
    uint8_t i;
   //----------------------------------------------------------------------------
    // scan parameters for reference correction
    isl_command(ISL_CMD_SCANTEMP);      // scan all temperatures    
    bmsState.ref_coeffA = isl_read(ISL_REF_COEF_A);
    bmsState.ref_coeffA = bmsState.ref_coeffA >> 5;
    bmsState.ref_coeffB = isl_read(ISL_REF_COEF_B);
    if((bmsState.ref_coeffB & 0x2000) != 0)
    {
        bmsState.ref_coeffB = bmsState.ref_coeffB | 0xC000;  // set negative
    }
    bmsState.ref_coeffC = isl_read(ISL_REF_COEF_C);
    if((bmsState.ref_coeffC & 0x2000) != 0)
    {
        bmsState.ref_coeffC = bmsState.ref_coeffC | 0xC000;  // set negative
    }
    bmsState.refVolt = isl_read(ISL_REFVOLT);
    bmsState.intTemp = isl_read(ISL_INTTEMP);
    dT = (bmsState.intTemp - 9180) / 2;
    adjustment = (bmsState.ref_coeffA / (256 * 8192L));
    adjustment = adjustment * dT * dT;
    adjustment = adjustment - (((float)bmsState.ref_coeffB / 8192) * dT);
    adjustment = adjustment + bmsState.ref_coeffC;
    bmsState.refVolt_corrected = bmsState.refVolt - adjustment;
    bmsState.vRef = (bmsState.refVolt_corrected / 16384) * 5;

    bmsState.intDegree = (int16_t)(25 + ((bmsState.intTemp - 9180) / 31.9));
    for(i=0;i<4;i++)
    {
        bmsState.extDegree[i] = isl_conv_raw2deg(isl_read(ISL_INTTEMP + ((i+1) << 6))); 
    }
}

/********************************************************************************/
/* reads the isl registers                                                      */
/********************************************************************************/
uint16_t isl_read(uint16_t regAddr)
{
    volatile uint8_t dummy;
    uint16_t result;
    RA5PPS=0b100000;                    // RA5 is the CS signal
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(regAddr >> 8);             // write msb address
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(regAddr & 0xFF);           // write lsb address
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=0;                          // write nothing to read
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    result = SPI1RXB;                   // get 6 useful msb
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=0;                          // write nothing to read
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    result = (result << 8) | SPI1RXB;   // get 8 lsb       
    RA5PPS=0b000000;                    // RA5 is GPIO high
    return result;        
}

/********************************************************************************/
/* writes the isl registers                                                     */
/********************************************************************************/
void isl_write(uint16_t regAddr, uint16_t data)
{
    volatile uint8_t dummy;
    uint32_t dataCompiled;
    
    RA5PPS=0b100000;                    // RA5 is the CS signal
    regAddr = regAddr | 0x8000;         // set write bit    
    dataCompiled = ((uint32_t)regAddr << 8) | data;  // concat both params

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(dataCompiled >> 16);       // write msb address
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(dataCompiled >> 8);        // write lsb address with data
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(dataCompiled >> 0);        // write lsb data
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high    
    RA5PPS=0b000000;                    // RA5 is GPIO high
}

/********************************************************************************/
/* send a command to battery manager                                            */
/********************************************************************************/
void isl_command(uint16_t command)
{
    volatile uint8_t dummy;
    RA5PPS=0b100000;                    // RA5 is the CS signal
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(command >> 8);             // write msb address
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=(command & 0xFF);           // write lsb address
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=0;                          // write nothing to read
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    // get 6 useful msb
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=0;                          // write nothing to read
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    // get 8 lsb       
    RA5PPS=0b000000;                    // RA5 is GPIO high
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    switch(command)                     // delay for command to execute
    {
        case ISL_CMD_SCANVOLT:
            __delay_ms(1);              // 849 us on datasheet
        break;
        case ISL_CMD_SCANTEMP:
            __delay_ms(5);              // 2958 us on datasheet but ... ?
        break;
        case ISL_CMD_SCANMIX:
            __delay_ms(1);              // 911 us on datasheet
        break;
        case ISL_CMD_SCANALL:
            __delay_ms(70);             // 69.1 ms on datasheet
        break;
        case ISL_CMD_MEASURE:
            __delay_ms(3);             // 116 us to 2768 us depending on measure tpye
        break;
        case ISL_CMD_SCANWIRES:
            __delay_ms(66);             // 65.3 ms on datasheet
        break;
        case ISL_RESET:
            __delay_ms(30);             // 27.2 ms on datasheet
        break;
        // cmds SLEEP, CONTINUOUS, INHIBIT have no time
    }
}

/********************************************************************************/
/* convert milivolt to cell raw data                                            */
/********************************************************************************/
uint16_t isl_conv_mv2Cell(int16_t mV)
{
    return (int16_t)(((int32_t)mV * 8192) / (bmsState.vRef * 2000));
}

/********************************************************************************/
/* convert cell voltage to milivolts                                            */
/********************************************************************************/
int16_t isl_conv_cell2mV(uint16_t rawVal)
{
    float temp = rawVal;
    if(rawVal >= 8191)
    {
        temp = (int32_t)rawVal - 16384;
    }
    return (int16_t)((temp * bmsState.vRef * 2000) / 8192);
}

/********************************************************************************/
/* convert battery voltage to milivolts                                         */
/********************************************************************************/
int16_t isl_conv_vbat2mV(uint16_t rawVal)
{
    float temp = rawVal;
    if(rawVal >= 8191)
    {
        //temp = (int32_t)rawVal - 16384;
    }
    return (int16_t)((temp * 15935.0784 * bmsState.vRef) / 8192);
}

/********************************************************************************/
/* convert NTC raw temperature measure to degree                                */
/********************************************************************************/
int8_t isl_conv_raw2deg(uint16_t rawVal)
{
    int32_t p1,p2,temp32;
    /* Estimate the interpolating point before and after the ADC value. */
    p1 = NTC_table[ (rawVal >> 9)];
    p2 = NTC_table[ (rawVal >> 9)+1];
 
    /* Interpolate between both points. */
    temp32 = p1 - ( (p1-p2) * (rawVal & 0x01FF)  / 512);
    return temp32 / 10;                 // return only degree and not 0.1
}

/********************************************************************************/
/* convert degree to value to put in pack controller limit registers            */
/********************************************************************************/
uint16_t isl_conv_deg2raw(int8_t degree)
{
    // table of values for temperatures from 0 to 140 degree (step of 5 degree)
    const uint16_t table[]=
    {12198,11444,10649,9830,9006,8192,7405,6657,5958,5312,4723,
      4190, 3713, 3288,2911,2579,2285,2028,1802,1603,1429,
      1276, 1142, 1023, 919, 827, 746, 674, 611
     };
    int8_t temp;
    temp = degree / 5;
    return table[temp];
}
