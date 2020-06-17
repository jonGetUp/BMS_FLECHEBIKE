/********************************************************************************/
/**
 * \file
 *
 * \brief Main system descriptors 
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/

#define _XTAL_FREQ 64000000L //(for the use of delay macro)
#ifndef __MAIN_H
#define __MAIN_H
#include "isl94212regs.h"
#include "can_highlevel.h"


typedef enum
{
    SM_IDLE,
    SM_ERROR_IDLE,
    SM_LOAD,
    SM_FAST_CHARGE_START,
    SM_FAST_CHARGE_LOW,
    SM_FAST_CHARGE_HIGH,
    SM_FAST_CHARGE_STOP,
    SM_SLOW_CHARGE_START,
    SM_SLOW_CHARGE,
    SM_SLOW_CHARGE_STOP,
    SM_BATTERY_DEAD
}smMain;

typedef enum
{
    LED_OFF=0,
    LED_CHARGE,
    LED_BATTERY_DEAD,
    LED_UNDERVOLTAGE,
    LED_OVERVOLTAGE,
    LED_TEMPHIGH,
    LED_BMS_ERROR,
    LED_WARN_LOW,
    LED_CHARGE_END,
    LED_CURRENT_HIGH,
}ledDisplay;

#define SET_LED_GREEN   0x40
#define SET_LED_RED     0x20
#define SET_LED_OFF     0x00

typedef enum
{
    NO_FAULT,
    OVERTEMP,
    OVERVOLT,
    UNDERVOLT,
    OPENWIRE,
    OTHER_FAULT
}bmsFault;

struct BMS_STATE
{
    smMain              smMain;                 // current state of BMS
    uint8_t             mosfetOldState;         // to see change for waiting capacitor down
    uint16_t            mosTimer;               // timer to count capa discharge
    bmsFault            curFault;               // fault on system
    uint8_t             curFaultDetail;         // fault on system
    uint8_t             ledDisplay;             // current led display (except button pressed)
    uint8_t             balanceInWork;          // we are balancing
    //--------------------------------------------------------------------------
    uint16_t            batVolt;                // battery voltage in mV
    uint16_t            cellVolt[12];           // each cell voltage in mV
    //--------------------------------------------------------------------------
    uint16_t            intTemp;                // internal temperature (raw)
    int8_t              intDegree;              // int. temp. in degree
    int8_t              extDegree[4];           // external temperatures in degree
    uint8_t             scanCount;              // scan counter
    //--------------------------------------------------------------------------
    uint16_t            fault_overvoltage;      // 12 cell fault detect
    uint16_t            fault_undervoltage;     // 12 cell fault detect
    uint16_t            fault_openwire;         // 13 connection fault detect
    FaultSetupBits_t    fault_setup;            // see bits
    FaultStatusBits_t   fault_status;           // see bits
    uint16_t            cell_setup;             // 12 cell enable check
    uint8_t             overtemp_fault;         // 5 over temp bit fault detect
    uint16_t            overvoltage_limit;      // overvoltage limit in raw
    uint16_t            undervoltage_limit;     // undervoltage limit in raw
    uint16_t            externalTemp_limit;     // overtemperature limit in raw TBD
    //--------------------------------------------------------------------------
    BalanceSetupBits_t  balance_setup;          // see bits
    uint16_t            balance_status;         // see datasheet
    BalanceWdtTimeBits_t balance_wdt_time;      // see bits
    //--------------------------------------------------------------------------
    uint16_t            user1;                  // any user usage (14 bits)
    uint16_t            user2;                  // any user usage (14 bits)
//  uint16_t            comms_setup;            // only used in daisy chain
    DeviceSetupBits_t   device_setup;           // see bits
    uint16_t            intTemp_limit;          // internal temperature (RO)
    uint32_t            serialNumber;           // chip serial number (RO)
    uint16_t            trimVolt;               // trim voltage (RO)
    //--------------------------------------------------------------------------
    uint32_t            cell_balance[12];       // cell balance registers
    //--------------------------------------------------------------------------
    uint16_t            cell_in_balance;        // cells in balance (RO)
    //--------------------------------------------------------------------------
    int16_t             ref_coeffA;             // reference coefficient (RO)
    int16_t             ref_coeffB;             // reference coefficient (RO)
    int16_t             ref_coeffC;             // reference coefficient (RO)
    uint16_t            refVolt;                // reference voltage raw (RO)
    float               refVolt_corrected;      // ref voltage corrected
    float               vRef;                   // true vRef (near 2.5V)
    //--------------------------------------------------------------------------
    int32_t             battery_current;        // current on battery in mA +/-
    //--------------------------------------------------------------------------
    uint16_t            charger_voltage;        // voltage on charger (0.1V)
    uint16_t            charger_current;        // current on charger (0.1A)
    ChargerStatus_t     charger_status;         // status of charger
    uint8_t             charger_fast_present;   // the fast charger is present
    uint8_t             charger_fast_timer;     // timer to detect
    uint16_t            charger_current_to_set; // current to set on charger (0.1A)
    uint16_t            charger_voltage_to_set; // voltage to set on charger (0.1V)
    
    //--------------------------------------------------------------------------
    uint8_t             charger_slow_present;   // voltage on charger
    uint16_t            external_voltage;       // voltage on load/charger
};

extern struct BMS_STATE bmsState;
#endif
