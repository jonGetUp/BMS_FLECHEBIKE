/********************************************************************************/
/**
 * \file
 *
 * \brief Library offering CAN high level functions
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/
/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __can_control_H
#define __can_control_H

//--------------------------------------------------------------------------------
// Mauer charger CANbus definitions
//--------------------------------------------------------------------------------
#define CAN_PRIO_SHIFT                  26      // priority position
#define CAN_R_SHIFT                     25      // R bit position
#define CAN_DP_SHIFT                    24      // DP bit position
#define CAN_PFCODE_SHIFT                16      // PFCODE position
#define CAN_DESTINATION_SHIFT           8       // destination address pos.
#define CAN_SOURCE_SHIFT                0       // source address pos.

#define CAN_MOTOR_ADDRESS               0xEFL    // address of a motor ?
#define CAN_BMS_ADDRESS                 0xF4L    // address of BMS (me)
#define CAN_CHARGER_ADDRESS             0xE5L    // address of Mauer charger
#define CAN_CHARGER2_ADDRESS            0xE6L    // another charger ?
#define CAN_BROADCAST_ADDRESS           0x50L    // broadcast message for all

#define CAN_MSG_PF_SET_MAX              0x06L    // command to set VMAX, IMAX
#define CAN_MSG_PF_STATUS               0xFFL    // status message

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef union {                         // charger status bits definition
    struct {
        unsigned HW_STATE     :1;     // 0:all is ok, 1 =hardware failure
        unsigned OVT_STATE    :1;     // 0: ok, 1 = over temperature of charger
        unsigned I_VOLT_STATE :1;     // 0:input voltage ok, 1 wrong input voltage
        unsigned START_STATE  :1;     // 0:start charging, 1=charger closed
        unsigned COMM_STATE   :1;     // 0:communication ok, 1=receive timeout
    };
    struct {
        uint8_t Reg;                    // byte access
    };
}ChargerStatus_t;


/********************************************************************************/
/**
 * \brief  Initialise the CAN and driver for communication
* This function inits the CAN controller for 250kBds to communication with charger
*/
/********************************************************************************/
void can_start(void);

/********************************************************************************/
/**
 * \brief  Stop the CAN driver to reduce power consumption
* This function disable the hardware CAN driver
*/
/********************************************************************************/
void can_stop(void);

/********************************************************************************/
/**
 * \brief  Send a charger consign
* This function send a consign to the charger Maurer 110327
* \param maxVoltage The maximum voltage of the charger in 100mV step
* \param maxCurrent The maximum current of the charger in 100mA step
* \param off If off = 0, charge is active, if off = 1, charger is off
*/
/********************************************************************************/
void can_send_charger_consign(uint16_t maxVoltage, uint16_t maxCurrent, uint8_t off);

/********************************************************************************/
/**
 * \brief  Analyse a CAN message received
* This function analyse and update BMS parameters 
* \return It returns -1 is message is unknow
*         On charger message status, it returns the status byte
*/
/********************************************************************************/
int8_t can_analyse_message(void);

#endif
