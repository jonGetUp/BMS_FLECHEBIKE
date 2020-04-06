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

#include "isl94212regs.h"
#include "can_highlevel.h"
struct BMS_STATE
{
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
    int32_t             battery_currrent;       // current on battery in mA +/-
    //--------------------------------------------------------------------------
    uint16_t            charger_voltage;        // voltage on charger
    uint16_t            charger_current;        // current on charger
    ChargerStatus_t     charger_status;         // status of charger
};

extern struct BMS_STATE bmsState;
