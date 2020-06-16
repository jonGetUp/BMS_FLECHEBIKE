/******************************************************************************/


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
#ifndef __SYSTEM_LIMITS_H
#define __SYSTEM_LIMITS_H
#include "main.h"

#define PROTO_DEBUG 0                   // to simulate current (1)

#define SL_ZERO_CURRENT_OFFSET      -93L    // measured current when no current
#define CELL_PRESENCE   (0b111110011111)    // all except 5 and 6

#define SL_CURRENT_FOR_CHARGE (400L)    // at least xxx mA for charge mode
#define SL__CURRENT_FOR_LOAD (400L)    //  at least xxx mA for load mode
#define SL_CURRENT_NEAR_0   (40)        // xx mA or less is considered as 0mA
#define SL_CURRENT_MAX_PACK (8000L)     // 8 A is max charging current (for example)


#define SL_DEAD_VOLTAGE     (3200)      // 3.2V is a voltage on dead cell
#define SL_LOW_VOLTAGE      (3500)      // 3.5V is voltage require low charge current
#define SL_WARN_VOLTAGE     (3600)      // 3.6V is voltage warning for load mode
#define SL_STOP_CHARGING_VOLTAGE      (4201)      // 4.198 V is voltage for end of charge
#define SL_END_VOLTAGE      (4198)      // 4.198 V is voltage for end of charge
#define SL_HIGH_VOLTAGE     (4210)      // 4.21 V is voltage too high on cell (normally never)

#define SL_CHARGER_CURRENT_LOW  (10)    // 1A for test
#define SL_CHARGER_CURRENT_HIGH (10)    // 1A for test
#define SL_CHARGER_VOLTAGE      (420)   // 42V 
#define SL_VOLTAGE_TO_LIMIT_CURRENT (4190) // voltage on one cell to reduce charger voltage
#define SL_VOLTAGE_TO_INCREASE_CURRENT (4170) // voltage on one cell to reduce charger voltage
#define SL_CELLCOUNT_TO_BALANCE (3)     // 3 higher cells are balanced
#define SL_TIME_TO_BALANCE_CELL (4)     // do a cell balance each 4 seconds
#define SL_TIME_TO_BALANCE_VOLTAGE (8)     // do a charger voltage reduction each ...
#define SL_BALANCE_THRESHOLD    (15)    // don't balance if max-min < threshold (15mV)

#define SL_TEMP_MOS_POS         (0)     // position of mos temperature NTC
#define SL_TEMP_BALANCE_POS     (1)     // position of shunt resistors temperature NTC
#define SL_TEMP_BATTL_POS       (2)     // position of battery lower temperature NTC
#define SL_TEMP_BATTH_POS       (3)     // position of battery higher temperature NTC

#define SL_TEMP_MOS_LIMIT       (80)    // limit temp for MOSFET
#define SL_TEMP_BALANCE_LIMIT   (70)    // limit temp for shunt resistors
#define SL_TEMP_BATT_LIMIT      (50)    // limit temp for battery pack
#define SL_TEMP_INTERNAL_LIMIT  (90)    // limit temp for battery pack
#define SL_TEMP_HYSTERESIS      (5)     // 5 degree lower to be ok again

#define SL_SCAN_VOLTAGE_TIME    (4)     // rescan voltage each n seconds
#define SL_SCAN_TEMP_TIME       (5)     // rescan temperatures each n seconds
#define SL_SCAN_OPENWIRE_TIME   (20)    // rescan openwire problems

#define SL_LOAD_MAX_CURRENT     (40000L) // max. is 40 Amps for example
#define SL_LOAD_MAX_TIME        (200)   // 2 seconds for example ( 200 x 10ms)
// voltage table to display energy on pack (duty cycle display)
// values set from prc measures
const uint16_t SL_PACK[] = {41414,41007,40728,40515,40323,40116,39888,39620, 39096,36000};
#endif
