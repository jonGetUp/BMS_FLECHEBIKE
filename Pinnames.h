/* 
 * File:   Pinnames.h
 * Author: martin.aladjidi
 *
 * Created on 2. mars 2020, 13:32
 * header containing all pin names of the PIC
 */

#define Isense LATAbits.LATA0
#define nSS LATAbits.LATA5

#define nBatt_FAULT LATBbits.LATB0
#define nFAULT LATBbits.LATB1
#define nDRDY LATBbits.LATB2
#define CANRX LATBbits.LATB3
#define CANTX LATBbits.LATB4
#define FASTCHARGE LATBbits.LATB5   // (hardware rev.2)
#define ICSPCL_EXT LATBbits.LATB6
#define ICSPDAT_EXT LATBbits.LATB7

#define nSHDN LATCbits.LATC0
#define EN_ISL LATCbits.LATC1
#define CAN_POWERDOWN LATCbits.LATC2
#define SCLK LATCbits.LATC3
#define MISO LATCbits.LATC4
#define MOSI LATCbits.LATC5
#define nEN_Isense LATCbits.LATC6
#define EN_Vref LATCbits.LATC7

