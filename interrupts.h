/* 
 * File:   interrupts.h
 * Author: martin.aladjidi
 *
 * Created on 3. mars 2020, 13:18
 */

extern uint8_t islFault;
extern uint8_t batFault;
extern uint8_t time10ms;
extern uint8_t time1s;
extern uint8_t time10msOverCurrent;
#if BLE_MODULE == 1
    extern uint8_t irqBt;
#endif
//void __interrupt(high_priority) high_isr(void);
//void __interrupt(low_priority) low_isr(void);
