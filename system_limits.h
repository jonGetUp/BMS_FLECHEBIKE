/******************************************************************************/


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
#ifndef __SYSTEM_LIMITS_H
#define __SYSTEM_LIMITS_H
#include "main.h"

#define CELL_PRESENCE   (0b111110011111)    // all except 5 and 6


#define SL_DEAD_VOLTAGE     (3200)      // 3.2V is a voltage on dead cell
#define SL_LOW_VOLTAGE      (3500)      // 3.5V is voltage require low charge current
#define SL_WARN_VOLTAGE     (3600)      // 3.6V is voltage warning for load mode
#define SL_END_VOLTAGE      (4190)      // 4.19 V is voltage for end of charge
#define SL_HIGH_VOLTAGE     (4210)      // 4.21 V is voltage too high on cell

#define SL_CURRENT_NEAR_0   (20)        // 20 mA or less is considered as 0mA
#define SL_CURRENT_MAX_PACK (8000)      // 8 A is max current (for example)

#define SL_CHARGER_CURRENT_LOW  (1)     // 100mA for test
#define SL_CHARGER_CURRENT_HIGH (5)     // 500mA for test
#define SL_CHARGER_VOLTAGE      (420)   // 42V 

#define SL_CELLCOUNT_TO_BALANCE (3)     // 3 higher cells are balanced
#define SL_TIME_TO_BALANCE      (4)     // do a balance each 4 seconds
#define SL_BALANCE_THRESHOLD    (80)    // don't balance if max-min < threshold

#define SL_TEMP_MOS_POS         (0)     // position of mos temperature NTC
#define SL_TEMP_BALANCE_POS     (1)     // position of shunt resistors temperature NTC
#define SL_TEMP_BATTL_POS       (2)     // position of battery lower temperature NTC
#define SL_TEMP_BATTH_POS       (3)     // position of battery higher temperature NTC

#define SL_TEMP_MOS_LIMIT       (80)    // limit temp for MOSFET
#define SL_TEMP_BALANCE_LIMIT   (70)    // limit temp for shunt resistors
#define SL_TEMP_BATT_LIMIT      (50)    // limit temp for battery pack
#define SL_TEMP_INTERNAL_LIMIT  (90)    // limit temp for battery pack

#define SL_SCAN_VOLTAGE_TIME    (4)     // rescan voltage each n seconds
#define SL_SCAN_TEMP_TIME       (5)     // rescan temperatures each n seconds
#define SL_SCAN_OPENWIRE_TIME   (20)    // rescan openwire problems

// voltage table to display energy on pack (8 leds to 0 led)
// TODO -> values to be defined seriously
const uint16_t SL_PACK[] = {41200,40000,39000,38500,38200,37800,37000,36200};
#endif
