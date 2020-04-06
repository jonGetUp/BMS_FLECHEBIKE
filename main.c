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

/* i.e. uint8_t <variable_name>; */

/* interrupt Flags */
uint8_t seriousBatFaultFlag = 0;
uint8_t IsrFaultFlag = 0;

/* global counters */
uint8_t IslCommFailCounter = 0;

/* system's states enumeration*/



enum MACROSTATES_SYSTEM // macro states enumeration
{INITIALISATION, // initialisation of the system
CHARGEPOLLING, // try èresence of charger --> maybe replace by interrupt
CHARGE, // system in charge (communication with charger)
DISCHARGEPOLLING, //--> maybe replace by interrupt
DISCHARGE, // communication with IHM/Moteur and no charger
STANDBY, // no charge, no motor on
FAULT,};

enum INIT_SUB_STATES // sub states for devices initialisation
{
    INIT_PIC, //state where pic is initialised
    BATT_FAULT_POLLING, // trying batt fault before autorizing interrupts
    ISL_FAULT_COMMUNICATION_POLLING, // trying pic to isl communication 
    INIT_ISL,//init isl with limit values
};

enum FAULT_SUBSTATES // sub states for errors 
{   
    SERIOUS_BATTFAULT, // secure state in case of bad values from the batterie
    PB_ISL_COMM_DEF, // error state where pic and isl can't communicate
};
/*system's variables structure*/

struct BMS_STATE bmsState;

struct CANMessage txMsg;
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/    
void main(void)
{
    uint8_t i;
    int8_t retCode;
    //--------------------------------------------------------------------------
    ConfigureOscillator();              // setup CPU at 64MHz
    InitApp();                          // initialize I/O ports
    CANInit();                          // initialize CAN bus
    CANSetMode(CAN_NORMAL_MODE);        // normal mode
    //--------------------------------------------------------------------------
    EN_Vref = 1;                        // enable voltage reference
    nEN_Isense = 0;                     // enable current measure
    nSHDN = 0;                          // low high transition to
    nSHDN = 1;                          // enable circuit breaker
    EN_ISL = 1;                         // enable battery manager
     __delay_ms(30);                    // 27.2 ms at least datasheet p.7
    //--------------------------------------------------------------------------
    isl_init();                         // init battery manager, scan all
    adc_init();                         // init the adc converter
    bmsState.battery_currrent = adc_getOneMeasure();  // get one measure of battery
    can_start();
    GIEH = 1;
    GIEL = 1;
    txMsg.Address = 0x123;
    txMsg.Ext = 1;
    txMsg.NoOfBytes = 2;
    txMsg.Remote = 0;
    while(1)
    {
        can_send_charger_consign(200, 10, 0); // 20V, 1A, on
        __delay_ms(100);
        if(CANRXMessageIsPending()!=0)
        {
            retCode = can_analyse_message();
        }
    }
}

