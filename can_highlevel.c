/********************************************************************************/
/**
 * \file
 *
 * \brief Library offering can highlevel function 
 *
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#endif

#include "main.h"
#include "can.h"
#include "Pinnames.h"
#include "can_highlevel.h"

 uint32_t  ACCEPTANCE_MASK_0_1 = 0xFFL << CAN_DESTINATION_SHIFT; // check destination address
 uint32_t  ACCEPTANCE_MASK_2_5 = 0xFFL << CAN_DESTINATION_SHIFT; // check destination address
 uint32_t ACCEPTANCE_FILTER_0 = CAN_BROADCAST_ADDRESS << CAN_DESTINATION_SHIFT;	// broadcast
 uint32_t ACCEPTANCE_FILTER_1 = CAN_BROADCAST_ADDRESS << CAN_DESTINATION_SHIFT;	// broadcast
 uint32_t ACCEPTANCE_FILTER_2 = CAN_BMS_ADDRESS << CAN_DESTINATION_SHIFT;       // my address
 uint32_t ACCEPTANCE_FILTER_3 = CAN_BMS_ADDRESS << CAN_DESTINATION_SHIFT;       // my address
 uint32_t ACCEPTANCE_FILTER_4 = CAN_BMS_ADDRESS << CAN_DESTINATION_SHIFT;       // my address
 uint32_t ACCEPTANCE_FILTER_5 = CAN_BMS_ADDRESS << CAN_DESTINATION_SHIFT;       // my address



/********************************************************************************/
/* Start the CAN interface                                                      */
/********************************************************************************/
void can_start(void)
{
    CAN_POWERDOWN = 0;                  // enable can driver
}

/********************************************************************************/
/* Stop the CAN communication                                                   */
/********************************************************************************/
void can_stop(void)
{
    CAN_POWERDOWN = 1;                  // disable can driver
}

/********************************************************************************/
/* Send a consign to charger                                                    */
/********************************************************************************/
void can_send_charger_consign(uint16_t maxVoltage, uint16_t maxCurrent, uint8_t off)
{
     struct CANMessage txMsg;
     txMsg.Ext = 1;
     txMsg.Address = 
             (6L << CAN_PRIO_SHIFT)|
             (CAN_MSG_PF_SET_MAX << CAN_PFCODE_SHIFT)|
             (CAN_CHARGER_ADDRESS << CAN_DESTINATION_SHIFT)|
             (CAN_BMS_ADDRESS << CAN_SOURCE_SHIFT);
     txMsg.NoOfBytes = 5;
     txMsg.Data[0] = maxVoltage >> 8;
     txMsg.Data[1] = maxVoltage;
     txMsg.Data[2] = maxCurrent >> 8;
     txMsg.Data[3] = maxCurrent;
     if(off == 0)
     {
         txMsg.Data[4] = 0;             // charger activated
     }
     else
     {
         txMsg.Data[4] = 1;             // charger stopped
     }
     txMsg.Priority = 1;
     txMsg.Remote = 0;
     CANPut(txMsg);
}

/********************************************************************************/
/* Analyse a recieved CAN message                                               */
/********************************************************************************/
int8_t can_analyse_message(void)
{
    struct CANMessage rxMsg;
    uint8_t srcAddr;
    uint8_t dstAddr;
    uint8_t pfMessageType;
    
    //----------------------------------------------------------------------------
    rxMsg = CANGet();
    srcAddr = rxMsg.Address & 0xFF;
    dstAddr = (rxMsg.Address >> CAN_DESTINATION_SHIFT) & 0xFF;
    pfMessageType = (rxMsg.Address >> CAN_PFCODE_SHIFT) & 0xFF;
    //----------------------------------------------------------------------------
    if((srcAddr == CAN_CHARGER_ADDRESS)&&
            (pfMessageType == CAN_MSG_PF_STATUS))
    {
        bmsState.charger_voltage = rxMsg.Data[0] << 8;
        bmsState.charger_voltage |= rxMsg.Data[1];
        bmsState.charger_current = rxMsg.Data[2] << 8;
        bmsState.charger_current |= rxMsg.Data[3];
        bmsState.charger_status.Reg = rxMsg.Data[4];
        return bmsState.charger_status.Reg;             // return status
    }
    //----------------------------------------------------------------------------
    return -1;              // unknow message received
}

