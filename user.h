/******************************************************************************/


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
#ifndef __USER_H
#define __USER_H
#include "main.h"

void InitApp(void);         /* I/O and Peripheral Initialization */

        // temperatures to check
uint8_t check_overTemp(uint8_t mos_t, uint8_t bal_t, uint8_t pack_t, uint8_t internal_t);

void ledUpdate(ledDisplay display);

bmsFault FaultAnalyse(void);
smMain sm_execute_idle(void);
smMain sm_execute_error_idle(void);
smMain sm_execute_load(void);
smMain sm_execute_fast_charge_start(void);
smMain sm_execute_fast_charge_low(void);
smMain sm_execute_fast_charge_high(void);
smMain sm_execute_fast_charge_stop(void);
smMain sm_execute_slow_charge_start(void);
smMain sm_execute_slow_charge(void);
smMain sm_execute_slow_charge_stop(void);

uint16_t balance_pack(uint8_t cellCount);
void balance_current(void);

#endif

