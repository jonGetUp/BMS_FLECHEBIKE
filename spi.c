#include <htc.h>
#include <stdint.h>

uint8_t	rxBuffer[40];
uint8_t	txBuffer[40];
uint8_t rxIndex = 0;
#define _XTAL_FREQ 64000000                  // Fosc  frequency for _delay()  library

/************************************************************************************/
/* DETAILS      : see .h definition file                                      	    */
/************************************************************************************/
void SPI_init(void)
{    
    //initiated in user.c
}

void SPI_write(uint8_t data)
{
    volatile uint8_t dummy;
    RA4PPS=0b100000;                  // Pin select, RA4 is the /SS signal
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};      // control register: check no pending communication
    //Use the Transfer Count Zero Interrupt Flag (the TCZIF bit of SPIxINTF) to accurately
    //determine that the Transfer Counter has reached zero.
    SPI1TCNT = 1;                     // number of bytes to send = 1
    SPI1TXB = data;                   // Transmit FIFO
    while(PIR2bits.SPI1RXIF==0);      // while transmission occurs, wait 
    dummy = SPI1RXB;                  // dummy read to drive /SS high
    
    while (SPI1CON2bits.BUSY){};      // control register: check no pending communication
    //Use the Transfer Count Zero Interrupt Flag (the TCZIF bit of SPIxINTF) to accurately
    //determine that the Transfer Counter has reached zero.
    SPI1TCNT = 1;                     // number of bytes to send = 1
    SPI1TXB = data;                   // Transmit FIFO
    while(PIR2bits.SPI1RXIF==0);      // while transmission occurs, wait 
    dummy = SPI1RXB;                  // dummy read to drive /SS high
    
    RA4PPS=0b000000;                  // Pin select, RA4 is a GPIO high
    __delay_ms(5);
}

uint16_t SPI_read(void)
{   
    volatile uint8_t dummy;
    uint16_t result;
    RA4PPS=0b100000;                    // RA4 is the CS signal
    //wait- - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=0;                          // write nothing to read
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    result = SPI1RXB;                   // get 6 useful msb
    //get 8 lsb  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    while (SPI1CON2bits.BUSY){};        // check no pending communication
    SPI1TCNT = 1;                       // bytes to send = 1
    SPI1TXB=0;                          // write nothing to read
    while(PIR2bits.SPI1RXIF==0);        // while transmission occurs, wait 
    result = (result << 8) | SPI1RXB;   // get 8 lsb       
    RA4PPS=0b000000;                    // Pin select, RA4 is a GPIO high
    return result;        
}

void resetIndex(void){   
    rxIndex = 0;
}

void resetRxBuffer(void){
    for(int i=0; i < 40; i++){    //return pointer on first element
        rxBuffer[i] = 0;
    }
}
