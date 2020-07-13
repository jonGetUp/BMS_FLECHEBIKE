/******************************************************************************/
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

#include <xc.h>
#include "can.h"
#include "Pinnames.h"    /*header where all pin name are defined as in BMS plan*/

uint8_t time10ms = 0;
uint8_t time10msOverCurrent = 0;
uint8_t time1s = 0;
uint8_t islFault = 0;
uint8_t batFault = 0;
#if BLE_MODULE == 1
    uint8_t irqBt = 0;
#endif

/******************************************************************************/
/* Interrupts Routine                                                         */
/******************************************************************************/
void __interrupt(high_priority) high_isr(void)
{
    static uint8_t counter10ms=0;
//    //----------------------------------------------------------------------------
//    // interrupt from CANbus ?
//    CANISR();
    //----------------------------------------------------------------------------
    // interrupt each 10ms
    if((TMR0IE == 1) && (TMR0IF == 1))
    {
        TMR0IF = 0;                     // clear ISR flag
        // caution, simple 16 bits TMR0 writing don't works
        TMR0H = (65535L - 10000L) >> 8; // isr each 10ms
        TMR0L = 65535L - 10000L;        // isr each 10ms
        time10ms = 1;
        time10msOverCurrent = 1;
        counter10ms++;
        if(counter10ms == 100)
        {
            counter10ms = 0;
            time1s = 1;
        }
    }
    
//    //----------------------------------------------------------------------------
//    // external interrupts
//    if((IOCIE == 1)&&(IOCIF == 1))
//    {
//        //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//        if(IOCBFbits.IOCBF1 == 1)
//        {
//            IOCBFbits.IOCBF1 = 0;
//            //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//            if((PORTB & 0x02) == 0)
//            {
//                islFault = 1;
//            }
//            else
//            {
//                islFault = 0;
//            }
//        }
//        //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//        if(IOCBFbits.IOCBF0 == 1)
//        {
//            IOCBFbits.IOCBF0 = 0;
//            //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//            if((PORTB & 0x01) == 0)
//            {
//                batFault = 1;
//            }
//            else
//            {
//                batFault = 0;
//            }
//        }
//#if BLE_MODULE == 1
//        //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//        if(IOCBFbits.IOCBF2 == 1)
//        {
//            IOCBFbits.IOCBF2 = 0;   //clear flag
//            //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//            if(PORTBbits.RB2 == 1)
//            {
//                irqBt = 1;
//            }
//            else
//            {
//                irqBt = 0;
//            }
//        }
//#endif
//    }
}


