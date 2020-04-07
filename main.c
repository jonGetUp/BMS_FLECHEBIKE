/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/


/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif



#include <xc.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "Pinnames.h"    /*header where all pin name are defined as in BMS plan*/


#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "isl94212.h"       /*Spi communication funtcions*/
#include "isl94212regs.h" /*header with command of isl based on the data sheet*/

#include "ADCIsense.h" /* Pic's ADC functions*/


#include "interrupts.h"
#include "main.h"
#include "can.h"
#include "adc.h"
#include "can_highlevel.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
struct BMS_STATE bmsState;

struct CANMessage txMsg;
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/    
void main(void)
{
    uint8_t i;
    int8_t retCode;
    uint16_t fault,temp16;
    //--------------------------------------------------------------------------
    ConfigureOscillator();              // setup CPU at 64MHz
    InitApp();                          // initialize I/O ports
    CANInit();                          // initialize CAN bus
    CANSetMode(CAN_NORMAL_MODE);        // normal mode
    //--------------------------------------------------------------------------
    EN_Vref = 1;                        // enable voltage reference
    nEN_Isense = 0;                     // enable current measure
    EN_ISL = 1;                         // enable battery manager
     __delay_ms(30);                    // 27.2 ms at least datasheet p.7
    //--------------------------------------------------------------------------
    isl_init();                         // init battery manager, scan all
    adc_init();                         // init the adc converter
    bmsState.battery_current = adc_getOneMeasure();  // get one measure of battery
    can_start();
    GIEH = 1;
    GIEL = 1;
    txMsg.Address = 0x123;
    txMsg.Ext = 1;
    txMsg.NoOfBytes = 2;
    txMsg.Remote = 0;
    //--------------------------------------------------------------------------
    bmsState.smMain = SM_IDLE;              // just powered on
    nSHDN = 1;                              // enable MOSFET

    isl_write(ISL_OVERVOLT_SET,isl_conv_mv2Cell(4190));
    isl_write(ISL_UNDERVOLT_SET,isl_conv_mv2Cell(3500));
    isl_write(ISL_EXTTEMP_SET,isl_conv_deg2raw(50));
    isl_write(ISL_CELLSETUP,0b000001100000);    // enable all cell except 5 & 6
    isl_write(ISL_FAULTSETUP,
            ISL_FS_TEMP_I |                 // scan internal temperature
            ISL_FS_SAMPLE_8 |               // 8 error to interrupt
            6 << ISL_FS_INTERVAL_SHIFT);    // scan interval of 1 seconds
    isl_command(ISL_CMD_CONTINUOUS);        // scan continuous mode
    //--------------------------------------------------------------------------
    while(1)
    {
        //----------------------------------------------------------------------
        if(time10ms != 0)               // actions to be defined
        {
            time10ms = 0;
            bmsState.battery_current = adc_getOneMeasure();  // get one current measure of battery
            if(bmsState.charger_fast_timer > 0)
            {
                bmsState.charger_fast_timer--;
                if(bmsState.charger_fast_timer == 0)
                {
                    bmsState.charger_fast_present = 0;
                }
            }
        }
        //----------------------------------------------------------------------
        if(time1s != 0)               // actions to be defined
        {
            time1s = 0;
            // update cells voltages
            for(i=0;i<12;i++)
            {
               bmsState.cellVolt[i] = isl_conv_cell2mV(isl_read(ISL_VBATT + ((i+1) << 6))); 
            }
        }    
        //----------------------------------------------------------------------
        if(islFault != 0)               // fault from ISL, must be cleared by ISL isr
        {
            FaultAnalyse();
        }
        //----------------------------------------------------------------------
        if(batFault != 0)               // fault from protection LTC4368
        {
            nSHDN = 1;                              // disable MOSFET (already disabled)
            FaultAnalyse();
        }
        //----------------------------------------------------------------------
        __delay_ms(100);
        if(CANRXMessageIsPending()!=0)
        {
            retCode = can_analyse_message();
            if(retCode != -1)   // fast charger present
            {
                bmsState.charger_fast_present = 1;
                bmsState.charger_fast_timer = 150;  // wait 1.5 second
            }
//        can_send_charger_consign(200, 10, 0); // 20V, 1A, on
        }
        //----------------------------------------------------------------------
        // state machine work
        //----------------------------------------------------------------------
        switch(bmsState.smMain)
        {
            // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
            case SM_IDLE: 
                bmsState.smMain = sm_execute_idle();
            break;
            // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
            case SM_ERROR_IDLE: 
                // inform user on error type in IDLE mode
                
            break;        
            // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
            case SM_LOAD: 
                // inform user on error type in IDLE mode
                
            break;        
        }
    }
}

