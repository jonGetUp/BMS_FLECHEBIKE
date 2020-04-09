/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
#ifndef __SYSTEM_LIMITS_H
#define __SYSTEM_LIMITS_H
#include "main.h"

#define SL_DEAD_VOLTAGE     (3200)      // 3.2V is a voltage on dead cell
#define SL_LOW_VOLTAGE      (3500)      // 3.5V is voltage require low charge current
#define SL_WARN_VOLTAGE     (3600)      // 3.6V is voltage warning for load mode
#define SL_END_VOLTAGE      (4198)      // 4.198V is voltage for end of charge
#define SL_HIGH_VOLTAGE     (4210)      // 4.21V is voltage too high on cell

#define SL_HIGH_TEMP_ALL    (80)        // limit for all measures is 80 degree
#define SL_HIGH_TEMP_1      (80)        // limit for ntc 1
#define SL_HIGH_TEMP_2      (80)        // limit for ntc 2
#define SL_HIGH_TEMP_3      (80)        // limit for ntc 3
#define SL_HIGH_TEMP_4      (80)        // limit for ntc 4

#define SL_CURRENT_NEAR_0   (20)        // 20 mA or less is considered as 0mA
#define SL_CURRENT_MAX_PACK (8000)      // 8 A is max current (for example)

#define SL_CHARGER_CURRENT_LOW  (1)     // 100mA for test
#define SL_CHARGER_CURRENT_HIGH (5)     // 500mA for test
#define SL_CHARGER_VOLTAGE      (420)   // 42V 

#define SL_CELLCOUNT_TO_BALANCE (3)     // 3 higher cells are balanced
#endif
