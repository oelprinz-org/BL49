/*
 * can_drv.h
	
	The contents of this file were originally from the CAN Software Library
	provided by Atmel written for AT90CAN devices. The contents were modified
	for use with ATmegaxxM1 devices. Use of this file is subject to Atmel's 
	End User License Agreement.
 
 */ 

//******************************************************************************
//! @file $RCSfile: can_drv.h,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the prototypes and the macros of the
//!        low level functions (drivers) of:
//!             - CAN (Controller Array Network)
//!             - for AT90CAN128/64/32.
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _CAN_DRV_H_
#define _CAN_DRV_H_

#include <stdint.h>
#include <avr/io.h>
#include "../config.h"

//_____ I N C L U D E S ________________________________________________________

//_____ D E F I N I T I O N S __________________________________________________

    // ----------
#ifndef FOSC
#  error  You must define FOSC in "config.h" file
#endif
    // ----------
#ifndef CAN_BAUDRATE
#  error  You must define CAN_BAUDRATE in "config.h" file
#endif
#define CAN_AUTOBAUD    0
    // ----------
#if FOSC == 16000             //!< Fclkio = 16 MHz, Tclkio = 62.5 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x12       // Tscl  = 10x Tclkio = 625 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0E       // Tscl  = 8x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x08       // Tscl  = 5x Tclkio = 312.5 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl  = 4x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl = 4x Tclkio = 250 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 125 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == CAN_AUTOBAUD  // Treated later in the file
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   else
#       error This CAN_BAUDRATE value is not in "CAN_driver.h" file
#   endif

#elif FOSC == 12000           //!< Fclkio = 12 MHz, Tclkio = 83.333 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0A       // Tscl  = 6x Tclkio = 500 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0A       // Tscl  = 6x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x04       // Tscl  = 3x Tclkio = 250 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x04       // Tscl  = 3x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 12x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 166.666 ns
#       define CONF_CANBT2  0x08       // Tsync = 1x Tscl, Tprs = 5x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x25       // Tpsh1 = 3x Tscl, Tpsh2 = 3x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 12x Tscl, sampling at 75%
#       define CONF_CANBT1  0x00       // Tscl  = 1x Tclkio = 83.333 ns
#       define CONF_CANBT2  0x08       // Tsync = 1x Tscl, Tprs = 5x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x25       // Tpsh1 = 3x Tscl, Tpsh2 = 3x Tscl, 3 sample points
#   elif CAN_BAUDRATE == CAN_AUTOBAUD  // Treated later in the file
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   else
#       error This CAN_BAUDRATE value is not in "can_drv.h" file
#   endif

#elif FOSC == 8000              //!< Fclkio = 8 MHz, Tclkio = 125 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x08       // Tscl  = 5x Tclkio = 625 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl  = 4x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x00       // Tscl  = 1x Tclkio = 125 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == CAN_AUTOBAUD  // Treated later in the file
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   else
#       error This CAN_BAUDRATE value is not in "can_drv.h" file
#   endif

#else
#   error This FOSC value is not in "can_drv.h" file
#endif
    // ----------
    //! The first action of "can_bit_timing()" is to reset the CAN controller
    //! "can_bit_timing()" lets the CAN controller disable
    //! "can_bit_timing()" returns 1 if the setting of CANBTx registers is available
    //!                and returns 0 if the setting of CANBTx registers is wrong
#if     CAN_BAUDRATE == CAN_AUTOBAUD
#   define Can_bit_timing(mode)  (can_auto_baudrate(mode) )
#else
#   define Can_bit_timing(mode)  (can_fixed_baudrate(mode))
#endif
    // ----------
#define CAN_PORT_IN     PINC // edited for ATmegaxxM1
#define CAN_PORT_DIR    DDRC // edited for ATmegaxxM1
#define CAN_PORT_OUT    PORTC // edited for ATmegaxxM1
#define CAN_INPUT_PIN   3 // edited for ATmegaxxM1
#define CAN_OUTPUT_PIN  2 // edited for ATmegaxxM1
    // ----------
#define ERR_GEN_MSK ((1<<SERG)|(1<<CERG)|(1<<FERG)|(1<<AERG))            //! MaSK for GENeral ERRors INTerrupts
#define INT_GEN_MSK ((1<<BOFFIT)|(1<<BXOK)|(ERR_GEN_MSK))                //! MaSK for GENeral INTerrupts

#define BRP_MSK     ((1<<BRP5)|(1<<BRP4)|(1<<BRP3)|(1<<BRP2)|(1<<BRP1)|(1<<BRP0))  //! Mask for BRP in CANBT1
#define SJW_MSK     ((1<<SJW1)|(1<<SJW0))                                //! MaSK for SJW  in CANBT2
#define PRS_MSK     ((1<<PRS2)|(1<<PRS1)|(1<<PRS0))                      //! MaSK for PRS  in CANBT2
#define PHS2_MSK    ((1<<PHS22)|(1<<PHS21)|(1<<PHS20))                   //! MaSK for PHS2 in CANBT2
#define PHS1_MSK    ((1<<PHS12)|(1<<PHS11)|(1<<PHS10))                   //! MaSK for PHS1 in CANBT2
#define BRP         (BRP0)                                               //! BRP  in CANBT1
#define SJW         (SJW0)                                               //! SJW  in CANBT2
#define PRS         (PRS0)                                               //! PRS  in CANBT2
#define PHS2        (PHS20)                                              //! PHS2 in CANBT2
#define PHS1        (PHS10)                                              //! PHS1 in CANBT2

#define HPMOB_MSK   ((1<<HPMOB3)|(1<<HPMOB2)|(1<<HPMOB1)|(1<<HPMOB0))    //! MaSK for MOb in HPMOB
#define MOBNB_MSK   ((1<<MOBNB3)|(1<<MOBNB2)|(1<<MOBNB1)|(1<<MOBNB0))    //! MaSK for MOb in CANPAGE

#define ERR_MOB_MSK ((1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR))  //! MaSK for MOb ERRors
#define INT_MOB_MSK ((1<<TXOK)|(1<<RXOK)|(1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR)) //! MaSK for MOb INTerrupts

#define CONMOB_MSK  ((1<<CONMOB1)|(1<<CONMOB0))                          //! MaSK for CONfiguration MOb
#define DLC_MSK     ((1<<DLC3)|(1<<DLC2)|(1<<DLC1)|(1<<DLC0))            //! MaSK for Data Length Coding
#define CONMOB      (CONMOB0)                                            //! CONfiguration MOb
#define DLC         (DLC0)                                               //! Data Length Coding
    // ----------
#define BRP_MIN     1       //! Prescaler of FOSC (TQ generation)
#define BRP_MAX     64
#define NTQ_MIN     8       //! Number of TQ in one CAN bit
#define NTQ_MAX     25
                            //! True segment values in TQ
#define PRS_MIN     1       //! Propagation segment
#define PRS_MAX     8
#define PHS1_MIN    2       //! Phase segment 1
#define PHS1_MAX    8
#define PHS2_MIN    2       //! Phase segment 2
#define PHS2_MAX    8
#define SJW_MIN     1       //! Synchro jump width
#define SJW_MAX     4
    // ----------
//#define NB_MOB       15
#define NB_MOB       6 // edited for ATmegaxxM1 
#define NB_DATA_MAX  8
#define LAST_MOB_NB  (NB_MOB-1)
#define NO_MOB       0xFF
    // ----------
//typedef enum {
//        MOB_0,  MOB_1, MOB_2,  MOB_3,  MOB_4,  MOB_5,  MOB_6, MOB_7,
//        MOB_8,  MOB_9, MOB_10, MOB_11, MOB_12, MOB_13, MOB_14        } can_mob_t;
typedef enum {MOB_0,  MOB_1, MOB_2,  MOB_3,  MOB_4,  MOB_5} can_mob_t;// edited for ATmegaxxM1
    // ----------
#define STATUS_CLEARED            0x00
    // ----------
#define MOB_NOT_COMPLETED         0x00                                              // 0x00
#define MOB_TX_COMPLETED        (1<<TXOK)                                           // 0x40
#define MOB_RX_COMPLETED        (1<<RXOK)                                           // 0x20
#define MOB_RX_COMPLETED_DLCW  ((1<<RXOK)|(1<<DLCW))                                // 0xA0
#define MOB_ACK_ERROR           (1<<AERR)                                           // 0x01
#define MOB_FORM_ERROR          (1<<FERR)                                           // 0x02
#define MOB_CRC_ERROR           (1<<CERR)                                           // 0x04
#define MOB_STUFF_ERROR         (1<<SERR)                                           // 0x08
#define MOB_BIT_ERROR           (1<<BERR)                                           // 0x10
#define MOB_PENDING            ((1<<RXOK)|(1<<TXOK))                                // 0x60
#define MOB_NOT_REACHED        ((1<<AERR)|(1<<FERR)|(1<<CERR)|(1<<SERR)|(1<<BERR))  // 0x1F
#define MOB_DISABLE               0xFF                                              // 0xFF
    // ----------
#define MOB_Tx_ENA  1
#define MOB_Rx_ENA  2
#define MOB_Rx_BENA 3
    // ----------

//_____ M A C R O S ____________________________________________________________

    // ----------
#define Can_reset()       ( CANGCON  =  (1<<SWRES) )
#define Can_enable()      ( CANGCON |=  (1<<ENASTB))
#define Can_disable()     ( CANGCON &= ~(1<<ENASTB))
    // ----------
#define Can_full_abort()  { CANGCON |=  (1<<ABRQ); CANGCON &= ~(1<<ABRQ); }
    // ----------
#define Can_conf_bt()     { CANBT1=CONF_CANBT1; CANBT2=CONF_CANBT2; CANBT3=CONF_CANBT3; }
    // ----------
#define Can_set_mob(mob)       { CANPAGE = ((mob) << 4);}
#define Can_set_mask_mob()     {  CANIDM4=0xFF; CANIDM3=0xFF; CANIDM2=0xFF; CANIDM1=0xFF; }
#define Can_clear_mask_mob()   {  CANIDM4=0x00; CANIDM3=0x00; CANIDM2=0x00; CANIDM1=0x00; }
#define Can_clear_status_mob() { CANSTMOB=0x00; }
#define Can_clear_mob()        { uint8_t  volatile *__i_; for (__i_=&CANSTMOB; __i_<&CANSTML; __i_++) { *__i_=0x00 ;}}
    // ----------
#define Can_mob_abort()   ( DISABLE_MOB )
    // ----------
#define Can_set_dlc(dlc)  ( CANCDMOB |= (dlc)        )
#define Can_set_ide()     ( CANCDMOB |= (1<<IDE)     )
#define Can_set_rtr()     ( CANIDT4  |= (1<<RTRTAG)  )
#define Can_set_rplv()    ( CANCDMOB |= (1<<RPLV)    )
    // ----------
#define Can_clear_dlc()   ( CANCDMOB &= ~DLC_MSK     )
#define Can_clear_ide()   ( CANCDMOB &= ~(1<<IDE)    )
#define Can_clear_rtr()   ( CANIDT4  &= ~(1<<RTRTAG) )
#define Can_clear_rplv()  ( CANCDMOB &= ~(1<<RPLV)   )
    // ----------
#define DISABLE_MOB       ( CANCDMOB &= (~CONMOB_MSK) )
#define Can_config_tx()        { DISABLE_MOB; CANCDMOB |= (MOB_Tx_ENA  << CONMOB); }
#define Can_config_rx()        { DISABLE_MOB; CANCDMOB |= (MOB_Rx_ENA  << CONMOB); }
#define Can_config_rx_buffer() {              CANCDMOB |= (MOB_Rx_BENA << CONMOB); }
    // ----------
#define Can_get_dlc()      ((CANCDMOB &  DLC_MSK)     >> DLC   )
#define Can_get_ide()      ((CANCDMOB &  (1<<IDE))    >> IDE   )
#define Can_get_rtr()      ((CANIDT4  &  (1<<RTRTAG)) >> RTRTAG)
    // ----------
#define Can_set_rtrmsk()   ( CANIDM4 |= (1<<RTRMSK) )
#define Can_set_idemsk()   ( CANIDM4 |= (1<<IDEMSK) )
    // ----------
#define Can_clear_rtrmsk() ( CANIDM4 &= ~(1<<RTRMSK) )
#define Can_clear_idemsk() ( CANIDM4 &= ~(1<<IDEMSK) )
    // ----------
                //!< STD ID TAG Reading
#define Can_get_std_id(identifier)  { *((uint8_t *)(&(identifier))+1) =  CANIDT1>>5              ; \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT2>>5)+(CANIDT1<<3); }
    // ----------
                //!< EXT ID TAG Reading
#define Can_get_ext_id(identifier)  { *((uint8_t *)(&(identifier))+3) =  CANIDT1>>3              ; \
                                      *((uint8_t *)(&(identifier))+2) = (CANIDT2>>3)+(CANIDT1<<5); \
                                      *((uint8_t *)(&(identifier))+1) = (CANIDT3>>3)+(CANIDT2<<5); \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT4>>3)+(CANIDT3<<5); }
    // ----------
                //!< STD ID Construction
#define CAN_SET_STD_ID_10_4(identifier)  (((*((uint8_t *)(&(identifier))+1))<<5)+((* (uint8_t *)(&(identifier)))>>3))
#define CAN_SET_STD_ID_3_0( identifier)  (( * (uint8_t *)(&(identifier))   )<<5)
    // ----------
                //!< STD ID TAG writing
#define Can_set_std_id(identifier)  { CANIDT1   = CAN_SET_STD_ID_10_4(identifier); \
                                      CANIDT2   = CAN_SET_STD_ID_3_0( identifier); \
                                      CANCDMOB &= (~(1<<IDE))                    ; }
    // ----------
                //!< STD ID MASK writing
#define Can_set_std_msk(mask)       { CANIDM1   = CAN_SET_STD_ID_10_4(mask); \
                                      CANIDM2   = CAN_SET_STD_ID_3_0( mask); }
    // ----------
                //!< EXT ID Construction
#define CAN_SET_EXT_ID_28_21(identifier)  (((*((uint8_t *)(&(identifier))+3))<<3)+((*((uint8_t *)(&(identifier))+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier)  (((*((uint8_t *)(&(identifier))+2))<<3)+((*((uint8_t *)(&(identifier))+1))>>5))
#define CAN_SET_EXT_ID_12_5( identifier)  (((*((uint8_t *)(&(identifier))+1))<<3)+((* (uint8_t *)(&(identifier))   )>>5))
#define CAN_SET_EXT_ID_4_0(  identifier)   ((* (uint8_t *)(&(identifier))   )<<3)
    // ----------
                //!< EXT ID TAG writing
#define Can_set_ext_id(identifier)  { CANIDT1   = CAN_SET_EXT_ID_28_21(identifier); \
                                      CANIDT2   = CAN_SET_EXT_ID_20_13(identifier); \
                                      CANIDT3   = CAN_SET_EXT_ID_12_5( identifier); \
                                      CANIDT4   = CAN_SET_EXT_ID_4_0(  identifier); \
                                      CANCDMOB |= (1<<IDE);                         }
    // ----------
                //!< EXT ID MASK writing
#define Can_set_ext_msk(mask)       { CANIDM1   = CAN_SET_EXT_ID_28_21(mask); \
                                      CANIDM2   = CAN_SET_EXT_ID_20_13(mask); \
                                      CANIDM3   = CAN_SET_EXT_ID_12_5( mask); \
                                      CANIDM4   = CAN_SET_EXT_ID_4_0(  mask); }
    // ----------
	
#define Max(a, b)          ( (a)>(b) ? (a) : (b) )
#define Min(a, b)          ( (a)<(b) ? (a) : (b) )

//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn can_clear_all_mob
//!
//! This function clears the Mailbox content.
//! It uses Can_clear_mob() macro and clears data FIFO o MOb[0] upto
//! MOb[LAST_MOB_NB].
//!
//! @warning none.
//!
//! @param none.
//!
//! @return none.
//!
extern  void can_clear_all_mob(void);

//------------------------------------------------------------------------------
//  @fn can_get_mob_free
//!
//! This function returns the number of the first MOb available or 0xFF if
//! no MOb available.
//!
//! @warning none.
//!
//! @param none.
//!
//! @return Handle of MOb.
//!          - MOb[0] upto MOb[LAST_MOB_NB]
//!          - 0xFF if no MOb
//!
extern  uint8_t can_get_mob_free(void);

//------------------------------------------------------------------------------
//  @fn can_get_mob_status
//!
//! This function returns information "MOB completed its job"
//! if one of the RXOK or TXOK Flag is set or "MOB not completed its job
//! if no RXOK and TXOK flags are set.
//! Previously, this function checks if the MOb is configured or not and in
//!  case of the MOB not configured, the function returns "MOB_DISABLE".
//!
//! @warning none.
//!
//! @param none.
//!
//! @return MOb Status.
//!          -  MOB_NOT_COMPLETED
//!          -  MOB_TX_COMPLETED
//!          -  MOB_RX_COMPLETED
//!          -  MOB_RX_DLC_WARNING
//!          -  MOB_DISABLE
//!          or should be a combination of the following errors
//!          -  MOB_ACK_ERROR
//!          -  MOB_FORM_ERROR
//!          -  MOB_CRC_ERROR
//!          -  MOB_STUFF_ERROR
//!          -  MOB_BIT_ERROR
//!
extern  uint8_t can_get_mob_status(void);

//------------------------------------------------------------------------------
//  @fn can_get_data
//!
//! This function copy the data from the selected MOb to the address
//! passed as parameter.
//!
//! @warning none.
//!
//! @param CAN message data address.
//!
//! @return none.
//!
extern  void can_get_data(uint8_t* p_can_message_data);

//------------------------------------------------------------------------------
//  @fn can_auto_baudrate
//!
//! This function programs itself the CANBTx registers if there is some
//! communication (activity) on the CAN bus.
//!
//! @warning complex function not yet implemented
//!
//! @param  Evaluation needed
//!         ==0: start the evaluation from faster baudrate
//!         ==1: (re)start an evaluation with CANBTx registers contents
//!
//! @return Baudrate Status
//!         ==0: research of bit timing configuration failed
//!         ==1: baudrate performed
//!
extern  uint8_t can_auto_baudrate(uint8_t eval);


//------------------------------------------------------------------------------
//  @fn can_fixed_baudrate
//!
//! This function programs the CANBTx registers with the predefined values
//! CONF_CANBT1, CONF_CANBT2, CONF_CANBT3.
//!
//! @warning
//!
//! @param (unused!)
//!
//! @return Baudrate Status
//!         fixed = 1: baudrate performed
//!
extern uint8_t can_fixed_baudrate(uint8_t eval);

//______________________________________________________________________________

#endif // _CAN_DRV_H_