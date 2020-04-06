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
extern uint8_t seriousBatFaultFlag;

#include <xc.h>
#include "can.h"
#include "Pinnames.h"    /*header where all pin name are defined as in BMS plan*/
//#include "interrupts.h" /*interrupt header*/
/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/



int counter = 0;

/* High-priority service */



void InitTimer0(){ //timer de 10ms avec Fosc=1 Mhz
    //16 bits prescaler 1:1 preload 64911
  T0CON0 = 0b10010000;
  T0CON1 = 0b01000000;
  TMR0H	 = 0xFD;
  TMR0L	 = 0x8F;
  /*interrupts enabled in init function*/
}


#if defined(__XC) || defined(HI_TECH_C)
void __interrupt(high_priority) high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{


   CANISR();
      /* TODO Add High Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      if(IOCBFbits.IOCBF0) // IO change port b0 interrupt --> Batt_Fault interrupt
      { /* Clear Interrupt Flag 1 */
         
          IOCBFbits.IOCBF0=0;
          /*isr 1*/
          seriousBatFaultFlag = 1; // set the serious battfault flag to send the pic in serious batt fault mode
         
          
          
      }
      
      if (IOCBFbits.IOCBF1){ // IO change on Fault from ISR
          IOCBFbits.IOCBF1=0; // clearing interrupt flag
          //ISL interrupt handling to code
      }

      else if (PIR3bits.TMR0IF) // time0 interrupt
      {
          PIR3bits.TMR0IF=0; /* Clear Interrupt Flag 2 */
          InitTimer0();
          counter++;
          if (counter>=1000) //+-3s
          {
              counter=0;
           LATCbits.LATC0=0; // disconnecting battery with nSHDN
          }
          
       }
         
              
          
      
      else
      {
          /* Unhandled interrupts */
      }


}


