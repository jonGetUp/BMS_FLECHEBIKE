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
* This function execute an adc measure
* \return It returns the current measured on load +/- on mA
*/
/********************************************************************************/
int32_t adc_getOneMeasure(void);


#endif /* __adc_H */
