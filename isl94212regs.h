/********************************************************************************/
/**
 * \file
 *
 * \brief Constants for registers address and commands 
 *
 * \author pascal.sartoretti@hevs.ch
 *
 */
/********************************************************************************/
//--------------------------------------------------------------------------------
//Cell voltage access: page nr. 1 | address change (read only))
#define ISL_VBATT   (((0x01 << 6) | (0x00)) << 6)   ///< pack voltage
#define ISL_CELL1   (((0x01 << 6) | (0x01)) << 6)   ///< cell voltage
#define ISL_CELL2   (((0x01 << 6) | (0x02)) << 6)   ///< cell voltage
#define ISL_CELL3   (((0x01 << 6) | (0x03)) << 6)   ///< cell voltage
#define ISL_CELL4   (((0x01 << 6) | (0x04)) << 6)   ///< cell voltage
#define ISL_CELL5   (((0x01 << 6) | (0x05)) << 6)   ///< cell voltage
#define ISL_CELL6   (((0x01 << 6) | (0x06)) << 6)   ///< cell voltage
#define ISL_CELL7   (((0x01 << 6) | (0x07)) << 6)   ///< cell voltage
#define ISL_CELL8   (((0x01 << 6) | (0x08)) << 6)   ///< cell voltage
#define ISL_CELL9   (((0x01 << 6) | (0x09)) << 6)   ///< cell voltage
#define ISL_CELL10  (((0x01 << 6) | (0x0A)) << 6)   ///< cell voltage
#define ISL_CELL11  (((0x01 << 6) | (0x0B)) << 6)   ///< cell voltage
#define ISL_CELL12  (((0x01 << 6) | (0x0C)) << 6)   ///< cell voltage
#define ISL_ALLCELL (((0x01 << 6) | (0x0F)) << 6)   ///< stream Vbat + 12 cell
//--------------------------------------------------------------------------------
// temperatures and others : page 1 | different address (read only))
#define ISL_INTTEMP     (((0x01 << 6) | (0x10)) << 6)   ///< internal temperature
#define ISL_EXTTEMP1    (((0x01 << 6) | (0x11)) << 6)   ///< external temperature
#define ISL_EXTTEMP2    (((0x01 << 6) | (0x12)) << 6)   ///< external temperature
#define ISL_EXTTEMP3    (((0x01 << 6) | (0x13)) << 6)   ///< external temperature
#define ISL_EXTTEMP4    (((0x01 << 6) | (0x14)) << 6)   ///< external temperature
#define ISL_REFVOLT     (((0x01 << 6) | (0x15)) << 6)   ///< ref. voltage
#define ISL_SCANCOUNT   (((0x01 << 6) | (0x16)) << 6)   ///< scan counter
#define ISL_ALLTEMP     (((0x01 << 6) | (0x1F)) << 6)   ///< scan all above
//--------------------------------------------------------------------------------
// fault registers: page 2 | different address (can be written)
#define ISL_OVERVOLT_F    (((0x02 << 6) | (0x00)) << 6)   ///< one of 12 cell overvolt
#define ISL_UNDERVOLT_F   (((0x02 << 6) | (0x01)) << 6)   ///< one of 12 cell undervolt
#define ISL_OPENWIRE_F    (((0x02 << 6) | (0x02)) << 6)   ///< one of 12 cell openwire
#define ISL_FAULTSETUP    (((0x02 << 6) | (0x03)) << 6)   ///< scan interval, ...
#define ISL_FS_TEMP_I    (1 << 8)
#define ISL_FS_TEMP_1    (1 << 9)
#define ISL_FS_TEMP_2    (1 << 10)
#define ISL_FS_TEMP_3    (1 << 11)
#define ISL_FS_TEMP_4    (1 << 12)
#define ISL_FS_SAMPLE_1    (0 << 5)
#define ISL_FS_SAMPLE_2    (1 << 5)
#define ISL_FS_SAMPLE_4    (2 << 5)
#define ISL_FS_SAMPLE_8    (3 << 5)
#define ISL_FS_SAMPLE_16   (4 << 5)
#define ISL_FS_SAMPLE_32   (5 << 5)
#define ISL_FS_SAMPLE_64   (6 << 5)
#define ISL_FS_SAMPLE_128  (7 << 5)
#define ISL_FS_INTERVAL_SHIFT   (0)



#define ISL_FAULTSTATUS   (((0x02 << 6) | (0x04)) << 6)   ///< see datasheet p.65
#define ISL_CELLSETUP     (((0x02 << 6) | (0x05)) << 6)   ///< cell check config
#define ISL_OVERTEMP_F    (((0x02 << 6) | (0x06)) << 6)   ///< over temperature fault
#define ISL_ALL_FAULT     (((0x02 << 6) | (0x0F)) << 6)   ///< scan all above
//--------------------------------------------------------------------------------
// setup registers: page 2 | different address 
#define ISL_OVERVOLT_SET  (((0x02 << 6) | (0x10)) << 6)   ///< overvoltage limit
#define ISL_UNDERVOLT_SET (((0x02 << 6) | (0x11)) << 6)   ///< undervoltage limit
#define ISL_EXTTEMP_SET   (((0x02 << 6) | (0x12)) << 6)   ///< external temp. limit
#define ISL_BALANCE_SET   (((0x02 << 6) | (0x13)) << 6)   ///< balance, see p. 67
#define ISL_BALANCE_STAT  (((0x02 << 6) | (0x14)) << 6)   ///< balance, see p. 67
#define ISL_BALANCE_WDT   (((0x02 << 6) | (0x15)) << 6)   ///< balance, see p. 67
#define ISL_USER1         (((0x02 << 6) | (0x16)) << 6)   ///< 14 bits any user usage
#define ISL_USER2         (((0x02 << 6) | (0x17)) << 6)   ///< 14 bits any user usage
#define ISL_COMMSETUP     (((0x02 << 6) | (0x18)) << 6)   ///< see datasheet p.68
#define ISL_DEVICESETUP   (((0x02 << 6) | (0x19)) << 6)   ///< see datasheet p.68
#define ISL_INTTEMP_LIMIT (((0x02 << 6) | (0x1A)) << 6)   ///< internal temp. limit
#define ISL_SERIAL_NR1    (((0x02 << 6) | (0x1B)) << 6)   ///< serial number 14 bits
#define ISL_SERIAL_NR2    (((0x02 << 6) | (0x1C)) << 6)   ///< serial number 14 bits
#define ISL_TRIM_VOLT     (((0x02 << 6) | (0x1D)) << 6)   ///< serial number 14 bits
#define ISL_ALL_SETUP     (((0x02 << 6) | (0x1F)) << 6)   ///< scan all above
//--------------------------------------------------------------------------------
// balance registers: page 2 | different address fro 0x20 to 0x37
#define ISL_BALANCE_BASE  (((0x02 << 6) | (0x20)) << 6)   ///< 12 cell * 2 regs
//--------------------------------------------------------------------------------
// ref coefficients registers: page 2 | different address 
#define ISL_REF_COEF_C    (((0x02 << 6) | (0x38)) << 6)   ///< datasheet p.70
#define ISL_REF_COEF_B    (((0x02 << 6) | (0x39)) << 6)   ///< datasheet p.70
#define ISL_REF_COEF_A    (((0x02 << 6) | (0x3A)) << 6)   ///< datasheet p.70
//--------------------------------------------------------------------------------
// cell balance enabled register: page 2 | address 0x3B
#define ISL_CELL_IN_BALANCE (((0x02 << 6) | (0x3B)) << 6)   ///< read only
//--------------------------------------------------------------------------------
// device commands: page 3 | different address
#define ISL_CMD_SCANVOLT  (((0x03 << 6) | (0x01)) << 6)   ///< scan voltages
#define ISL_CMD_SCANTEMP  (((0x03 << 6) | (0x02)) << 6)   ///< scan temperature
#define ISL_CMD_SCANMIX   (((0x03 << 6) | (0x03)) << 6)   ///< scan vbat, call, ext1
#define ISL_CMD_SCANWIRES (((0x03 << 6) | (0x04)) << 6)   ///< scan connections
#define ISL_CMD_SCANALL   (((0x03 << 6) | (0x05)) << 6)   ///< scan all
#define ISL_CMD_CONTINUOUS (((0x03 << 6) | (0x06)) << 6)   ///< scan continuous mode
#define ISL_CMD_INHIBIT    (((0x03 << 6) | (0x07)) << 6)   ///< stop scan continuous
#define ISL_CMD_MEASURE    (((0x03 << 6) | (0x08)) << 6)   ///< do a single measure
//some registers not used in standalone mode
#define ISL_SLEEP          (((0x03 << 6) | (0x0A)) << 6)    ///< enter sleep mode
//some registers not used in standalone mode
#define ISL_RESET          (((0x03 << 6) | (0x12)) << 6)    ///< reset all config
//some registers not used 
//--------------------------------------------------------------------------------
// definitions of bits in registers
//--------------------------------------------------------------------------------
typedef union {
    struct {
        unsigned SCN        :4;     // scan interval code
        unsigned WSCN       :1;     // scan wire timing control
        unsigned TOT        :3;     // fault totalization
        unsigned TST        :5;     // temperature testing enable
    };
    struct {
        uint16_t Reg;
    };
}FaultSetupBits_t;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef union {
    struct {
        unsigned reserved   :2;     // not used, reserved
        unsigned OSC        :1;     // oscillator fault
        unsigned WDGF       :1;     // watchdog timeout
        unsigned OT         :1;     // over temperature fault
        unsigned OV         :1;     // over voltage fault
        unsigned UV         :1;     // under voltage fault
        unsigned OW         :1;     // open wire fault
        unsigned OVBAT      :1;     // open wire fault on VBAT
        unsigned OVSS       :1;     // open wire fault on VSS
        unsigned PAR        :1;     // register checksum parity error
        unsigned REF        :1;     // voltage reference fault
        unsigned REG        :1;     // voltage regulator fault (V3P3,VCC,V2P5)
        unsigned MUX        :1;     // temperature multiplexer error
    };
    struct {
        uint16_t Reg;
    };
}FaultStatusBits_t;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef union {
    struct {
        unsigned BDM        :2;     // balance mode (OFF,manual,timed,auto)
        unsigned BWT        :3;     // balance wait time
        unsigned BSP        :4;     // balance status register pointer
        unsigned BEN        :1;     // balance enable bit
    };
    struct {
        uint16_t Reg;
    };
}BalanceSetupBits_t;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef union {
    struct {
        unsigned WDG        :7;     // watchdog timeout setting
        unsigned BTM        :7;     // balance timeout setting
    };
    struct {
        uint16_t Reg;
    };
}BalanceWdtTimeBits_t;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef union {
    struct {
        unsigned PIN39      :1;     // signal on pin 39
        unsigned PIN37      :1;     // signal on pin 37
        unsigned rvd1       :1;     // reserved 0
        unsigned EOB        :1;     // end of balance
        unsigned SCAN       :1;     // scan continuous mode status
        unsigned ISCN       :1;     // scan current source (150uA/1mA)
        unsigned rvd2       :1;     // reserved 0
        unsigned BDDS       :1;     // balance condition, see datasheet
        unsigned WP         :6;     // watchdog disable password (0x3A to disable)
    };
    struct {
        uint16_t Reg;
    };
}DeviceSetupBits_t;


