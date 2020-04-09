/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
#ifndef __USER_H
#define __USER_H
#include "main.h"
/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */

void led_display(ledDisplay display);  //BD8388 send SPI led

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

