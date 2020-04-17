/********************************************************************************/
/**
 * \file
 *
 * \brief Library offering isl94212 function access 
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/
/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __isl94212_H
#define __isl94212_H
#include "isl94212regs.h"

/********************************************************************************/
/**
 * \brief  Initialise the cell driver
* This function inits the SPI, the battery pack controller
*/
/********************************************************************************/
void isl_init(void);

/********************************************************************************/
/**
 * \brief  Scan all voltages and update global values
*/
/********************************************************************************/
void isl_scan_update_voltages(void);

/********************************************************************************/
/**
 * \brief  Calculate the reference voltage and temperatures
* This function calculate the true reference voltage
*/
/********************************************************************************/
void isl_calc_vref_and_temp(void);

/********************************************************************************/
/**
 * \brief  Reads a register from battery manager
* \param regAddr Register to read (see isl94212regs.h)
* \return Return the register value (14 useful bits)
*/
/********************************************************************************/
uint16_t isl_read(uint16_t regAddr);

/********************************************************************************/
/**
 * \brief  Writes a register into battery manager
* \param regAddr Register to write (see isl94212regs.h)
* \param data The data to write to register
*/
/********************************************************************************/
void isl_write(uint16_t regAddr, uint16_t data);

/********************************************************************************/
/**
 * \brief  Send a command to the battery manager
* \param command Command to execute (see isl94212regs.h)
* \note This function wait datasheet time depending on command to execute  
*/
/********************************************************************************/
void isl_command(uint16_t command);

/********************************************************************************/
/**
 * \brief  Converts milivolt value to register value
* \param mV Value in milivolts
* \return Return the register value to set (14 useful bits)
*/
/********************************************************************************/
uint16_t isl_conv_mv2Cell(int16_t mV);

/********************************************************************************/
/**
 * \brief  Converts cell register value to milivolts
* \param rawVal Value in register
* \return Return the cell voltage in mV
*/
/********************************************************************************/
int16_t isl_conv_cell2mV(uint16_t rawVal);

/********************************************************************************/
/**
 * \brief  Converts battery voltage register value to milivolts
* \param rawVal Value in register
* \return Return the battery voltage in mV
*/
/********************************************************************************/
int16_t isl_conv_vbat2mV(uint16_t rawVal);

/********************************************************************************/
/**
 * \brief  Convert NTC raw temperature measure to degree
* \param rawVal Value in register
* \return Return the temperature in degree
* 
* \note This function must be called for external NTC measures only
*/
/********************************************************************************/
int8_t isl_conv_raw2deg(uint16_t rawVal);

/********************************************************************************/
/**
 * \brief  Convert temperature to value to set in limit register
* \param degree Value in degree (positive only, from 0 to 140 degree)
* \return Return the raw value to set in register
* 
* \note This function round value to step of 5 degree as input
*/
/********************************************************************************/
uint16_t isl_conv_deg2raw(int8_t degree);

#endif /* __isl94212_H */
