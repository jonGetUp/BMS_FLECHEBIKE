/********************************************************************************/
/**
 * \file spi.h
 *
 * \brief Library offering nRF52840 function access 
 *
 * \author Gaspoz Jonathan
 *
 */
/********************************************************************************/

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

extern	uint8_t	rxBuffer[40];
extern	uint8_t	txBuffer[40];
extern uint8_t rxIndex;
/********************************************************************************/
/**
 * \brief  SPI initialisation
*/
/********************************************************************************/
void SPI_init(void);

/********************************************************************************/
/**
 * \brief Reads message to a Bluetooth module
 * \param data: The data to send
*/
/********************************************************************************/
void SPI_write(uint8_t data);

/********************************************************************************/
/**
 * \brief Reads message from a Bluetooth module
 * \param 
 * \return Return the SPI protocol frame
*/
/********************************************************************************/
uint16_t SPI_read(void);

void resetIndex(void);

void resetRxBuffer(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

