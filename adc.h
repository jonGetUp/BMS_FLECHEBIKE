/********************************************************************************/
/**
 * \file
 *
 * \brief Library offering adc function 
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/
/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __adc_H
#define __isl94212_H

#define ADC_CHANNEL_CURRENT 0
#define ADC_CHANNEL_VOLTAGE 1

/********************************************************************************/
/**
 * \brief  Initialise the ADC converter
* This function inits the ADC depending on Hardware selected
*/
/********************************************************************************/
void adc_init(void);

/********************************************************************************/
/**
 * \brief  Do an adc measure
* This function execute an adc measure on selecte channel
* \param channel The channel to measure
* \return It returns the current measured on load +/- on mA
*/
/********************************************************************************/
int32_t adc_getOneMeasure(uint8_t channel);


#endif /* __adc_H */
