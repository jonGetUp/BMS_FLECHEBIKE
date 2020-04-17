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
#include <stdlib.h>
#endif

#include "Pinnames.h"    /*header where all pin name are defined as in BMS plan*/
//#include "main.h"
#include "user.h"
#include "isl94212regs.h"
#include "isl94212.h"
#include "system_limits.h"
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
    //  1    INPUT_A    VLoad_Measure (hardware rev.2)
    //  2    OUTPUT     /CS for LEDs (hardware rev.2)
    //  3    INPUT_A    4.096 Vref (analogic reference voltage)
    //  4    OUTPUT     /CS for Bluetooth (hardware rev.2)
    //  5    OUTPUT     /SS (chip select for ISL94212INZ)
    //  6    OUTPUT     crystal 32.768kHz
    //  7    INPUT      crystal 32.768kHz    
    LATA = 0x34;
    TRISA = 0x4B;
    ANSELA= 0x0B;
    //--------------------------------------------------------------------------
    // PORTB connections:
    // BIT   DIR        NAME
    //  0    INPUT_INT  /Batt_FAULT from LTC4368HMS
    //  1    INPUT_INT  /FAULT from ISL94212INZ
    //  2    INPUT_?    /DRDY from ISL94212INZ
    //  3    INPUT      CAN_RX
    //  4    OUTPUT     CAN_TX
    //  5    OUTPUT     FAST_CHARGE (hardware rev.2)
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
    //  6    OUTPUT     /EN_Isense to MAX9918ASA (hardware rev.1)
    //  6    INPUT      user BUTTON input (hardware rev.2)
    //  7    OUTPUT     EN_Vref enable reference voltage
    LATC = 0b01000100;
#if PROTO_NUM == 1
    TRISC = 0x10;
#elif PROTO_NUM == 2
    TRISC = 0x50;
#endif
    ANSELC = 0;
    //--------------------------------------------------------------------------        
    /*properly map pins for SPI communication (MASTER FULL DUPLEX )*/
    //SPI OUTPUTS
    // select PORTC<3> as SPI1(SCK) output
    RC3PPS=0b011110;
    //select PORTC<5> as SPI1(SDO) output
    RC5PPS=0b011111;
    //select PORTA<5> as SPI1(nSS) output
    RA5PPS=0b000000;    // RA5 (CS for ISL) is default GPIO high
    RA2PPS=0b000000;    // RA2 (CS for LEDs) is default GPIO high
    RA4PPS=0b000000;    // RA4 (CS for Bluetooth) is default GPIO high
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
    // setup timer 0 for 10 ms interrupt
    T0CON0bits.EN = 1;                  // enable timer 0
    T0CON0bits.MD16 = 1;                // 16 bits timer
    T0CON1bits.CS = 2;                  // osc = Fosc/4 -> 16MHz
    T0CON1bits.CKPS = 4;                // divide by 16 -> 1MHz
    T0CON0bits.OUTPS = 0;               // divide by 1 -> 1MHz
    TMR0 = 65535 - 10000;               // isr each 10ms
    TMR0IE = 1;                         // enable interrupts
    //--------------------------------------------------------------------------            
    // external interrupt control
    IOCIE = 1;                          // enable external interrupts
    IOCBP = 0x03;                       // enable rising on RB0, RB1
    IOCBN = 0x03;                       // enable falling on RB0, RB1
}

/*********************************************************************************/
/*********************************************************************************/
void led_display(ledDisplay display)
{
    volatile uint8_t dummy;
    
    
    RA2PPS=0b100000;                    // RA2 is the CS signal
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=display;                    // write leds activity
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high    
    RA2PPS=0b000000;                    // RA2 is GPIO high
}
/*********************************************************************************/
/*********************************************************************************/
void led_display_charge(uint16_t packVoltage)
{
    volatile uint8_t dummy;
    uint8_t display = 0b11111111;       // default all led on
    for(uint8_t i=0;i<8;i++)
    {
        if(packVoltage < SL_PACK[i])
        {
            display = display >> 1;
        }
    }    
    RA2PPS=0b100000;                    // RA2 is the CS signal
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=display;                    // write leds activity
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    dummy = SPI1RXB;                    //dummy read to drive ss high    
    RA2PPS=0b000000;                    // RA2 is GPIO high
}

/*********************************************************************************/
/*********************************************************************************/
uint8_t check_cell_underV(uint16_t cellVoltage)
{
    for(uint8_t i=0;i<12;i++)
    {
        if((bmsState.cellVolt[i] < cellVoltage)&&
           ((CELL_PRESENCE & (1<<i)) != 0))
        {
            return 1;
        }
    }
    return 0;
}

/*********************************************************************************/
/*********************************************************************************/
uint8_t check_cell_overV(uint16_t cellVoltage)
{
    for(uint8_t i=0;i<12;i++)
    {
        if((bmsState.cellVolt[i] > cellVoltage)&&
           ((CELL_PRESENCE & (1<<i)) != 0))
        {
            return 1;
        }
    }
    return 0;
}

/*********************************************************************************/
/*********************************************************************************/
uint8_t check_overTemp(uint8_t mos_t, uint8_t bal_t, uint8_t pack_t, uint8_t internal_t)
{
    uint8_t retInfo=0;
    //--------------------------------------------------------------------------
    if(bmsState.extDegree[SL_TEMP_MOS_POS] > mos_t)
    {
        retInfo |= (1 << 1);
    }
    //--------------------------------------------------------------------------
    if(bmsState.extDegree[SL_TEMP_BALANCE_POS] > bal_t)
    {
        retInfo |= (1 << 2);
    }
    //--------------------------------------------------------------------------
    if((bmsState.extDegree[SL_TEMP_BATTH_POS] > pack_t)||
       (bmsState.extDegree[SL_TEMP_BATTL_POS] > pack_t))
    {
        retInfo |= (1 << 3);
    }
    //--------------------------------------------------------------------------
    if(bmsState.intDegree > internal_t)     // check internal temperature
    {
        retInfo |= (1 << 0);
    }
    return retInfo;
}

/*********************************************************************************/
/*********************************************************************************/
uint16_t get_higher_voltage(void)
{
    uint16_t higher = 0;
    for(uint8_t i=0;i<12;i++)
    {
        if((bmsState.cellVolt[i] > higher)&&
           ((CELL_PRESENCE & (1<<i)) != 0))
        {
            higher = bmsState.cellVolt[i];
        }
    }
    return higher;
}

/*********************************************************************************/
/*********************************************************************************/
uint16_t get_lower_voltage(void)
{
    uint16_t lower = 4200;      // max cell voltage
    for(uint8_t i=0;i<12;i++)
    {
        if((bmsState.cellVolt[i] < lower)&&
           ((CELL_PRESENCE & (1<<i)) != 0))
        {
            lower = bmsState.cellVolt[i];
        }
    }
    return lower;
}

/*********************************************************************************/
/*********************************************************************************/
uint16_t balance_pack(uint8_t cellCount)
{
    uint16_t cellsToCheck = CELL_PRESENCE;
    uint16_t lastMeasured;
    uint8_t posMeasured;
    uint16_t cellsToBalance = 0;
    
    //--------------------------------------------------------------------------
    // special case to stop balancing
    //--------------------------------------------------------------------------
    if(cellCount == 0)                      // user choice
    {
        bmsState.balanceInWork = 0;
        isl_command(ISL_BALANCE_INHIBIT);    // disable balancing
        return 0;
    }
    //--------------------------------------------------------------------------    
    if((get_higher_voltage() - get_lower_voltage()) < SL_BALANCE_THRESHOLD)
    {
        bmsState.balanceInWork = 0;
        isl_command(ISL_BALANCE_INHIBIT);    // disable balancing
        return 0;
    }        
    //--------------------------------------------------------------------------    
    // reduce cell balancing if temperature reach higher limit
    // example limit @ cell / 2 for 10 degree below limit stop
    //--------------------------------------------------------------------------    
    if(check_overTemp(SL_TEMP_MOS_LIMIT,
                      SL_TEMP_BALANCE_LIMIT - 10,
                      SL_TEMP_BATT_LIMIT,
                      SL_TEMP_INTERNAL_LIMIT))
    {
        cellCount = cellCount / 2;
    }
    //--------------------------------------------------------------------------    
    // balancing cells
    //--------------------------------------------------------------------------    
    for(uint8_t cells=0;cells<cellCount;cells++)    // for nB cell to balance
    {
        lastMeasured = 0;
        for(uint8_t i=0;i<12;i++)                   // check the highest to check
        {
            if((bmsState.cellVolt[i] > lastMeasured)&&
               ((cellsToCheck & (1<<i)) != 0))
            {
                lastMeasured = bmsState.cellVolt[i];
                posMeasured = i;
            }
        }
        cellsToBalance |= (1 << posMeasured);
        cellsToCheck = cellsToCheck & ~(1 << posMeasured);
    }
    //--------------------------------------------------------------------------    
    isl_write(ISL_BALANCE_SET,1);   // balance in  manual mode
    isl_write(ISL_BALANCE_STAT,cellsToBalance);    // set cells to balance
    isl_command(ISL_BALANCE_ENABLE);    // activate balancing
    bmsState.balanceInWork = 1;
    return isl_read(ISL_CELL_IN_BALANCE);   // return balanced cells bits list
}
/*********************************************************************************/
/*********************************************************************************/
void balance_current(void)
{
    // TODO, depending on max cell voltage, reduce charger voltage to reduce current
    if(get_higher_voltage() )
    {}
}

/*********************************************************************************/
/*********************************************************************************/
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
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_idle(void)
{
    if((bmsState.curFault != NO_FAULT)||        // BMS error (internal temp, OSC, ...)
       (check_cell_underV(SL_DEAD_VOLTAGE))||   // lower than limit low
       (check_overTemp(SL_TEMP_MOS_LIMIT,
                       SL_TEMP_BALANCE_LIMIT,
                       SL_TEMP_BATT_LIMIT,
                       SL_TEMP_INTERNAL_LIMIT))  
       )
    {
        nSHDN = 0;                  // disable MOSFET
        return SM_ERROR_IDLE; 
    }
    //--------------------------------------------------------------
    if(bmsState.battery_current > 50)
    {
        return SM_LOAD; 
    }
    //--------------------------------------------------------------
    if(bmsState.battery_current < -50)
    {
        return SM_SLOW_CHARGE_START; 
    }
    //--------------------------------------------------------------
    if(bmsState.charger_fast_present != 0)
    {
        return SM_FAST_CHARGE_START;
    }    
    return SM_IDLE;
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_error_idle(void)
{
    uint8_t temp8;
    if(check_cell_underV(SL_DEAD_VOLTAGE))  // cell voltage too low
    {
        nSHDN = 0;                          // disable MOSFET
        return SM_BATTERY_DEAD; 
    }
    //--------------------------------------------------------------
    if(check_cell_underV(SL_LOW_VOLTAGE))
    {
        nSHDN = 0;                          // disable MOSFET
        if(bmsState.charger_fast_present != 0)
        {
            led_display(LED_OFF);
            nSHDN = 1;                      // enable MOSFET
            return SM_FAST_CHARGE_START; 
        }
        if(bmsState.charger_slow_present)
        {
            nSHDN = 1;                      // enable MOSFET
            led_display(LED_OFF);
            return SM_SLOW_CHARGE_START; 
        }
        led_display(LED_UNDERVOLTAGE);
        return SM_ERROR_IDLE;
    }
    //--------------------------------------------------------------
    if(check_cell_overV(SL_HIGH_VOLTAGE))
    {
        // normally never arrive
        led_display(LED_OVERVOLTAGE);
        return SM_ERROR_IDLE;
    }
    //--------------------------------------------------------------
    if(temp8 = check_overTemp(SL_TEMP_MOS_LIMIT,
                      SL_TEMP_BALANCE_LIMIT,
                      SL_TEMP_BATT_LIMIT,
                      SL_TEMP_INTERNAL_LIMIT))
    {
        led_display(LED_TEMPHIGH | temp8);
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE;
    }    
    //--------------------------------------------------------------
    if(bmsState.curFault != NO_FAULT)
    {
        // see datasheet isl94212 p.65 for details
        led_display(LED_BMS_ERROR | (bmsState.curFaultDetail>>7));
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE;
    }    
    led_display(LED_OFF);
    return SM_IDLE;     // no more error they was previously
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_load(void)
{
    if(check_cell_underV(SL_WARN_VOLTAGE))  // lower than warning limit
    {
        led_display(LED_WARN_LOW);
        return SM_LOAD; 
    }
    //--------------------------------------------------------------
    if(check_overTemp(SL_TEMP_MOS_LIMIT,
                      SL_TEMP_BALANCE_LIMIT,
                      SL_TEMP_BATT_LIMIT,
                      SL_TEMP_INTERNAL_LIMIT))
    {
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE;
    }   
    //--------------------------------------------------------------
    if(check_cell_underV(SL_LOW_VOLTAGE))  // lower than warning limit
    {
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE; 
    }
    //--------------------------------------------------------------
    // TODO -> check current too high for a too long time
    
    
    
    //--------------------------------------------------------------
    if(abs(bmsState.battery_current) < SL_CURRENT_NEAR_0)
    {
        return SM_IDLE;
    }    
    return SM_LOAD;
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_fast_charge_start(void)
{
    if(check_cell_underV(SL_DEAD_VOLTAGE))  // lower than dead cell limit
    {
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE; 
    }
    //--------------------------------------------------------------
    if(check_cell_underV(SL_LOW_VOLTAGE))  // lower than warning limit
    {
        bmsState.charger_voltage_to_set = SL_CHARGER_VOLTAGE;
        bmsState.charger_current_to_set = SL_CHARGER_CURRENT_LOW;
        return SM_FAST_CHARGE_LOW; 
    }
    bmsState.charger_current_to_set = SL_CHARGER_CURRENT_HIGH;
    return SM_FAST_CHARGE_HIGH; 
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_fast_charge_low(void)
{
    //--------------------------------------------------------------
    if(check_overTemp(SL_TEMP_MOS_LIMIT,
                      SL_TEMP_BALANCE_LIMIT,
                      SL_TEMP_BATT_LIMIT,
                      SL_TEMP_INTERNAL_LIMIT))
    {
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE;
    }   
    //--------------------------------------------------------------
    if(check_cell_underV(SL_LOW_VOLTAGE) == 0)  // no more low voltage cell
    {
        bmsState.charger_current_to_set = SL_CHARGER_CURRENT_HIGH;
        return SM_FAST_CHARGE_HIGH; 
    }
    //--------------------------------------------------------------
    if(check_cell_overV(SL_HIGH_VOLTAGE))  // one cell too high and one loo low -> pack dead
    {
        led_display(LED_BATTERY_DEAD);
        nSHDN = 0;                          // disable MOSFET
        return SM_BATTERY_DEAD; 
    }
    //--------------------------------------------------------------
    if(bmsState.charger_fast_present == 0)  // no more charger connected
    {
        return SM_IDLE; 
    }
    return SM_FAST_CHARGE_LOW;
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_fast_charge_high(void)
{
    //--------------------------------------------------------------
    if(check_overTemp(SL_TEMP_MOS_LIMIT,
                      SL_TEMP_BALANCE_LIMIT,
                      SL_TEMP_BATT_LIMIT,
                      SL_TEMP_INTERNAL_LIMIT))  
    {
        balance_pack(0);                    // stop balance pack
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE;
    }   
    //--------------------------------------------------------------
    if(check_cell_overV(SL_END_VOLTAGE))    // one cell at limit
    {
        led_display(LED_CHARGE_END);
        balance_pack(0);                    // stop balance pack
        nSHDN = 0;                          // disable MOSFET
        return SM_FAST_CHARGE_STOP; 
    }
    //--------------------------------------------------------------
    if(bmsState.charger_fast_present == 0)  // no more charger connected
    {
        balance_pack(0);        // stop balance pack
        return SM_IDLE; 
    }
    return SM_FAST_CHARGE_HIGH;
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_fast_charge_stop(void)
{
    //--------------------------------------------------------------
    if(bmsState.charger_fast_present == 0)  // no more charger connected
    {
        led_display(LED_OFF);
        nSHDN = 1;                          // enable MOSFET
        return SM_IDLE; 
    }
    return SM_FAST_CHARGE_STOP;
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_slow_charge_start(void)
{
    if(check_cell_underV(SL_DEAD_VOLTAGE))  // lower than dead cell limit
    {
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE; 
    }
    //--------------------------------------------------------------
    if(check_cell_underV(SL_LOW_VOLTAGE))  // lower than low limit
    {
        return SM_SLOW_CHARGE;              // could be changed
    }
    return SM_SLOW_CHARGE; 
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_slow_charge(void)
{
    //--------------------------------------------------------------
    if(check_overTemp(SL_TEMP_MOS_LIMIT,
                      SL_TEMP_BALANCE_LIMIT,
                      SL_TEMP_BATT_LIMIT,
                      SL_TEMP_INTERNAL_LIMIT))  
    {
        balance_pack(0);                    // stop balance pack
        nSHDN = 0;                          // disable MOSFET
        return SM_ERROR_IDLE;
    }   
    //--------------------------------------------------------------
    if(check_cell_overV(SL_END_VOLTAGE))    // one cell at limit
    {
        led_display(LED_CHARGE_END);
        balance_pack(0);                    // stop balance pack
        nSHDN = 0;                          // disable MOSFET
        return SM_SLOW_CHARGE_STOP;
    }
    //--------------------------------------------------------------
     if(bmsState.battery_current < -SL_CURRENT_MAX_PACK)
     {
        led_display(LED_CURRENT_HIGH);
        balance_pack(0);                    // stop balance pack
        nSHDN = 0;                          // disable MOSFET
        return SM_SLOW_CHARGE_STOP; 
     }
    //--------------------------------------------------------------
    if(abs(bmsState.battery_current) < SL_CURRENT_NEAR_0)  // no more charger connected
    {
        balance_pack(0);                    // stop balance pack
        nSHDN = 0;                          // disable MOSFET
        return SM_SLOW_CHARGE_STOP; 
    }
    return SM_SLOW_CHARGE;
}
/*********************************************************************************/
/*********************************************************************************/
smMain sm_execute_slow_charge_stop(void)
{
    //--------------------------------------------------------------
    if(bmsState.charger_slow_present == 0)
    {
        led_display(LED_OFF);
        nSHDN = 1;                          // enable MOSFET
        return SM_IDLE; 
    }
    return SM_SLOW_CHARGE_STOP;
}
