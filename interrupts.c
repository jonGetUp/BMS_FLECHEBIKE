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
uint8_t time1s = 0;
uint8_t islFault = 0;
uint8_t batFault = 0;
/******************************************************************************/
/* Interrupts Routine                                                         */
/******************************************************************************/
void __interrupt(high_priority) high_isr(void)
{
    static uint8_t counter10ms=0;
    //----------------------------------------------------------------------------
    // interrupt from CANbus ?
    CANISR();
    //----------------------------------------------------------------------------
    // interrupt each 10ms
    if((TMR0IE == 1) && (TMR0IF == 1))
    {
        TMR0IF = 0;                     // clear ISR flag
        time10ms = 1;
        counter10ms++;
        if(counter10ms == 10)
        {
            counter10ms = 0;
            time1s = 1;
        }
    }
    //----------------------------------------------------------------------------
    // external interrupts
    if((IOCIE == 1)&&(IOCIF == 1))
    {
        //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        if(IOCBFbits.IOCBF1 == 1)
        {
            IOCBFbits.IOCBF1 = 0;
            //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            if((PORTB & 0x02) == 0)
            {
                islFault = 1;
            }
            else
            {
                islFault = 0;
            }
        }
        //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        if(IOCBFbits.IOCBF0 == 1)
        {
            IOCBFbits.IOCBF0 = 0;
            //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            if((PORTB & 0x01) == 0)
            {
                batFault = 1;
            }
            else
            {
                batFault = 0;
            }
        }
    }
}


