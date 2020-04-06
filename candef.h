//******************************************************************************
// This file offers the ability to configure easily the can interface
//******************************************************************************

//------------------------------------------------------------------------------
// CONSTANT : BAUD_RATE_PRESC
//
// USAGE    : select the time quanta based on the CPU clock
//            Tq = (2 x BAUD_RATE_PRESC / Fosc)
// LIMITS   : this value must be between 1 to 64
//------------------------------------------------------------------------------
#define BAUD_RATE_PRESC 16

//------------------------------------------------------------------------------
// CONSTANT : SJW_TIME
//
// USAGE    : resynchronisation time in number of Tq
// LIMITS   : this value must be between 1 to 4
//------------------------------------------------------------------------------
#define SJW_TIME 1

//------------------------------------------------------------------------------
// CONSTANT : SAMPLES
//
// USAGE    : select the number of sample in reception
// LIMITS   : this value must be 1 or 3
//------------------------------------------------------------------------------
#define SAMPLES 3

//------------------------------------------------------------------------------
// CONSTANT : PROP_TIME
//
// USAGE    : select the number of propagation time in Tq
// LIMITS   : this value must be between 1 to 8
//------------------------------------------------------------------------------
#define PROP_TIME 2

//------------------------------------------------------------------------------
// CONSTANT : PH_SEG_1
//
// USAGE    : select the number of phase segment 1 in Tq
// LIMITS   : this value must be between 1 to 8
//------------------------------------------------------------------------------
#define PH_SEG_1 2

//------------------------------------------------------------------------------
// CONSTANT : PH_SEG_2
//
// USAGE    : select the number of phase segment 2 in Tq
// LIMITS   : this value must be between 1 to 8
//------------------------------------------------------------------------------
#define PH_SEG_2 3

//------------------------------------------------------------------------------
// CONSTANT : LPBACK
//
// USAGE    : select the loopback mode
//            uncomment this line to use loopback test mode
//------------------------------------------------------------------------------
//#define LPBACK

//------------------------------------------------------------------------------
// CONSTANT : CAN_INT_LOW
//
// USAGE    : select the interrupt routine priority
//            uncomment this line to use low priority (default is high)
//------------------------------------------------------------------------------
//#define CAN_INT_LOW

//------------------------------------------------------------------------------
// CONSTANT : CAN_ERROR_HANDLER_ENABLE
//
// USAGE    : if we want to control the hardware errors
//            uncomment this line to use the error handler
//            The function CANErrorHandler() has to be written !!!
//------------------------------------------------------------------------------
//#define CAN_ERROR_HANDLER_ENABLE

//------------------------------------------------------------------------------
// CONSTANT : RX_BUFFER
//
// USAGE    : select the number of software reception buffers
// LIMITS   : this value has to be in the limits of CPU memory
//------------------------------------------------------------------------------
#define RX_BUFFER 4

//------------------------------------------------------------------------------
// CONSTANT : TX_BUFFER
//
// USAGE    : select the number of software transmission buffers
// LIMITS   : this value has to be in the limits of CPU memory
//------------------------------------------------------------------------------
#define TX_BUFFER 8

//------------------------------------------------------------------------------
// CONSTANT : STANDARD or EXTENDED identifiers for filters (11 or 29 bits)
//
// USAGE    : uncomment the lines to use standard identifiers
//------------------------------------------------------------------------------
//#define ST_FILTER_0
//#define ST_FILTER_1
//#define ST_FILTER_2
//#define ST_FILTER_3
//#define ST_FILTER_4
//#define ST_FILTER_5

//------------------------------------------------------------------------------
// CONSTANT : FILTERS AND MASKS
//
// USAGE    : The variables below have to be placed on a .c file in your
//            project.
//------------------------------------------------------------------------------
// uint32_t  ACCEPTANCE_MASK_0_1;	//Acceptance mask for filters 0 and 1
// uint32_t  ACCEPTANCE_MASK_2_5;	//Acceptance mask for filters 2, 3, 4 and 5
// uint32_t ACCEPTANCE_FILTER_0;	//Acceptance filter 0
// uint32_t ACCEPTANCE_FILTER_1;	//Acceptance filter 1
// uint32_t ACCEPTANCE_FILTER_2;        //Acceptance filter 2
// uint32_t ACCEPTANCE_FILTER_3;	//Acceptance filter 3
// uint32_t ACCEPTANCE_FILTER_4;        //Acceptance filter 4
// uint32_t ACCEPTANCE_FILTER_5;        //Acceptance filter 5

//------------------------------------------------------------------------------
//      ! ! ! ! ! ! ! ! ! ! DON'T CHANGE ANY LINE BELOW ! ! ! ! ! ! ! ! ! !
//------------------------------------------------------------------------------
#if (BAUD_RATE_PRESC < 1) || (BAUD_RATE_PRESC > 64)
#error "Baud rate invalid"
#endif
#if (SJW_TIME < 1) || (SJW_TIME > 4)
#error "Jump width invalid"
#endif
#if (PROP_TIME < 1) || (PROP_TIME > 8)
#error "Propagation time invalid"
#endif
#if (PH_SEG_1 < 1) || (PH_SEG_1 > 8)
#error "Phase segment 1 invalid"
#endif
#if (PH_SEG_2 < 1) || (PH_SEG_2 > 8)
#error "Phase segment 2 invalid"
#endif
#define CAN_CONFIG_1 BAUD_RATE_PRESC-1|(SJW_TIME-1<<6)
#if SAMPLES == 1
#define CAN_CONFIG_2 0x00|(PH_SEG_1-1<<3)|(PROP_TIME-1)
#elif SAMPLES == 3
#define CAN_CONFIG_2 0x40|(PH_SEG_1-1<<3)|(PROP_TIME-1)
#else
#error "Number of samples is out of range"
#endif
#define CAN_CONFIG_3 PH_SEG_2-1
