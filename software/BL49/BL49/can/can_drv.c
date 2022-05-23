/*
 * can_drv.c
 	
 	The contents of this file were originally from the CAN Software Library 
 	provided by Atmel. Use of this file is subject to Atmel's End User 
	License Agreement.
 	
 */ 

//******************************************************************************
//! @file $RCSfile: can_drv.c,v $
//!
//! Copyright (c) 2007 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level functions (drivers) of:
//!             - CAN (Controller Array Network)
//!             - for AT90CAN128/64/32
//!
//! This file can be parsed by Doxygen for automatic documentation generation.
//! This file has been validated with AVRStudio-413528/WinAVR-20070122.
//!
//! @version $Revision: 3.20 $ $Name: jtellier $
//!
//! @todo
//! @bug
//******************************************************************************

//_____ I N C L U D E S ________________________________________________________
#include "can_drv.h"
#include <avr/io.h>

//_____ D E F I N I T I O N S __________________________________________________

//_____ F U N C T I O N S ______________________________________________________

//------------------------------------------------------------------------------
//  @fn can_clear_all_mob
//!
//! This function clears the Mailbox content.
//! It reset CANSTMOB, CANCDMOB, CANIDTx & CANIDMx and clears data FIFO of
//! MOb[0] upto MOb[LAST_MOB_NB].
//!
//! @warning: This version doesn't clears the data FIFO
//!
//! @param none
//!
//! @return none
//------------------------------------------------------------------------------
void can_clear_all_mob(void)
{
uint8_t  mob_number;
/*
    uint8_t  data_index;
*/

    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        CANPAGE = (mob_number << 4);    //! Page index
        Can_clear_mob();                //! All MOb Registers=0
/*
        for (data_index = 0; data_index < NB_DATA_MAX; data_index++)
        {
            CANMSG = 0;                 //! MOb data FIFO
        }
*/
    }
}

//------------------------------------------------------------------------------
//  @fn can_get_mob_free
//!
//! This function returns the number of the first MOb available or 0xFF if
//! no MOb is available.
//!
//! @warning none.
//!
//! @param  none.
//!
//! @return Handle of MOb.
//!          - MOb[0] upto MOb[LAST_MOB_NB]
//!          - 0xFF if no MOb
//------------------------------------------------------------------------------
uint8_t can_get_mob_free(void)
{
    uint8_t mob_number, page_saved;

    page_saved = CANPAGE;
    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        Can_set_mob(mob_number);
        if ((CANCDMOB & 0xC0) == 0x00) //! Disable configuration
        {
            CANPAGE = page_saved;
            return (mob_number);
        }
    }
    CANPAGE = page_saved;
    return (NO_MOB);
}

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
//------------------------------------------------------------------------------
uint8_t can_get_mob_status(void)
{
    uint8_t mob_status, canstmob_copy;

    // Test if MOb ENABLE or DISABLE
    if ((CANCDMOB & 0xC0) == 0x00) {return(MOB_DISABLE);}

    canstmob_copy = CANSTMOB; // Copy for test integrity

    // If MOb is ENABLE, test if MOb is COMPLETED
    // - MOb Status = 0x20 then MOB_RX_COMPLETED
    // - MOb Status = 0x40 then MOB_TX_COMPLETED
    // - MOb Status = 0xA0 then MOB_RX_COMPLETED_DLCW
    mob_status = canstmob_copy & ((1<<DLCW)|(1<<TXOK)|(1<<RXOK));
    if ( (mob_status==MOB_RX_COMPLETED) ||   \
         (mob_status==MOB_TX_COMPLETED) ||   \
         (mob_status==MOB_RX_COMPLETED_DLCW) ) { return(mob_status); }

    // If MOb is ENABLE & NOT_COMPLETED, test if MOb is in ERROR
    // - MOb Status bit_0 = MOB_ACK_ERROR
    // - MOb Status bit_1 = MOB_FORM_ERROR
    // - MOb Status bit_2 = MOB_CRC_ERROR
    // - MOb Status bit_3 = MOB_STUFF_ERROR
    // - MOb Status bit_4 = MOB_BIT_ERROR
    mob_status = canstmob_copy & ERR_MOB_MSK;
    if (mob_status != 0) { return(mob_status); }

    // If CANSTMOB = 0 then MOB_NOT_COMPLETED
    return(MOB_NOT_COMPLETED);
}

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
//------------------------------------------------------------------------------
void can_get_data(uint8_t* p_can_message_data)
{
    uint8_t data_index;

    for (data_index = 0; data_index < (Can_get_dlc()); data_index++)
    {
        *(p_can_message_data + data_index) = CANMSG;
    }
}

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
//------------------------------------------------------------------------------
uint8_t can_auto_baudrate (uint8_t mode)
{
    uint8_t  u8_temp0;                               //! Temporary variable
    uint8_t  brp, prs, ntq, phs1, phs2;              //! Bit timing segment variables
    uint8_t  phs1_inc;                               //! Computing needed
    uint8_t  bt_not_found, wait_for_rx, evaluate;    //! Keys for "while()" loops
    uint8_t  try_conf;                               //! Key for configurate CAN
    uint8_t  ovrtim_flag=0;                          //! Timer overflow count
    uint16_t conf_index;                             //! Count of bit timing configuration tried
    uint8_t  bt_performed;                           //! Return flag

    //! --- Default setting
    phs1_inc = evaluate = 0;
    bt_performed = 0;
    conf_index = 0;
    bt_not_found = 1;

    //! --- Init segment variables with MIN values if mode=0
    //!     or init segment variables with CANBTx if mode=1
    if (mode==0)
    {
        brp  = BRP_MIN;
        ntq  = NTQ_MIN;
        phs1 = PHS1_MIN;
        phs2 = PHS2_MIN;
        prs  = ntq - ( phs1 + phs2 + 1 );
        try_conf = 1;       //! Try this configuration
        wait_for_rx = 1;    //! Enable "while (wait_for_rx ..." loop
    }
    else //! mode = 1
    {
        brp  = Max ((((CANBT1 &  BRP_MSK) >> 1) +1) , BRP_MIN );
        prs  = Max ((((CANBT2 &  PRS_MSK) >> 1) +1) , PRS_MIN );
        phs1 = Max ((((CANBT3 & PHS1_MSK) >> 1) +1) , PHS1_MIN);
        phs2 = Max ((((CANBT3 & PHS2_MSK) >> 4) +1) , PHS2_MIN);
        ntq  = Max ((prs + phs1 + phs2 + 1) , NTQ_MIN);
        phs1_inc = evaluate = 1;   //! To enter in "while (evaluate ..." loop
        try_conf = 0;       //! Look for the next configuration
        wait_for_rx = 0;    //! Skip "while (wait_for_rx ..." loop
    }

    //! --- Clear all MOb's (CANMSG not cleared)
    for (u8_temp0 = 0; u8_temp0 < NB_MOB; u8_temp0++)
    {
        Can_set_mob(u8_temp0);  //! Page index
        Can_clear_mob();        //! All MOb Registers = 0x00
    }

    while (bt_not_found == 1)
    {
        if (try_conf == 1)
        {
            Can_reset();
            conf_index++;
            ovrtim_flag=0;

            //! --- CANBTx registers update (sjw = phs2/2, 3 sample points)
            CANBT1 = ((brp-1) << BRP);
            CANBT2 = (((phs2 >> 1)-1) << SJW) |((prs-1) << PRS);
            CANBT3 = (((phs2-1) << PHS2) | ((phs1-1) << PHS1) | (1<<SMP));

            //! --- Set CAN-Timer - Used for time-out
            //!     There are 641 (0x281) possible evaluations. The first one provides the faster
            //!         the faster bit timing, the last one gives the slower. It is necessary to
            //!         modulate the time-out versus bit timing (0x281>>3=0x50, matching an U8).
            CANTCON = (uint8_t)(conf_index >> 3);

            //! --- MOb configuration
            Can_set_mob(MOB_0);                 //! Use MOb-0
            CANSTMOB = 0;                       //! Reset MOb status (undone by "Can_reset()")
            CANCDMOB = (MOB_Rx_ENA  << CONMOB); //! MOb 0 in receive mode

            //! CAN controller configuration
            CANGCON = (1<<LISTEN) | (1<<ENASTB);//! Enable CAN controller in "listen" mode
            while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
            CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
        }

        //! --- WAIT_FOR_RX LOOP:
        //!     ================
        //!     Try to perform a CAN message reception in "LISTEN" mode without error and
        //!     before a time_out done by CAN-Timer.
        //!     Else gives the hand to "EVALUATE LOOP" to have a new set of bit timing.
        while (wait_for_rx == 1)
        {
            u8_temp0 = CANSTMOB;
            //! --- RxOK received ?
            if ((u8_temp0 & (1<<RXOK)) != 0)
            {   //! --- It is the successful output of "can_auto_baudrate" function
                wait_for_rx = 0;    //! Out of "while (wait_for_rx ..." loop
                evaluate = 0;       //! Will skip "while (evaluate ..." loop
                bt_not_found = 0;   //! Out of "while (bt_not_found ..." loop
                bt_performed = 1;   //! Return flag = TRUE
                DISABLE_MOB;        //! Disable MOb-0
                CANGCON = 0x00;     //! Disable CAN controller & reset "listen" mode
                while ((CANGSTA & (1<<ENFG)) != 0); //! Wait for Disable OK
            }
             //! --- Else stop if any errors
             else
            {
                //! --- MOb error ?
                if ((u8_temp0 & ((1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR))) !=0)
                {
                    evaluate = 1;       //! Will enter in "while (evaluate ..." loop
                    wait_for_rx = 0;    //! Out of "while (wait_for_rx ..." loop
                }

                u8_temp0 = CANGIT;

                //! --- Time_out reached ?
                if ((u8_temp0 & (1<<OVRTIM)) !=0 )
                {
                    if (ovrtim_flag==0)
                    {
                        //! --- First Time_out
                        CANGIT |= (1<<OVRTIM);  // Reset OVRTIM
                        ovrtim_flag++;
                    }
                    else
                    {
                        //! --- Second Time_out
                        CANGIT |= (1<<OVRTIM);  // Reset OVRTIM
                        evaluate = 1;           //! Will enter in "while (evaluate ..." loop
                        wait_for_rx = 0;        //! Out of "while (wait_for_rx ..." loop
                    }
                }

                //! --- General error ?
                if ((u8_temp0 & ((1<<SERG)|(1<<CERG)|(1<<FERG)|(1<<AERG))) !=0)
                {
                    evaluate = 1;       //! Will enter in "while (evaluate ..." loop
                    wait_for_rx = 0;    //! Out of "while (wait_for_rx ..." loop
                    try_conf = 1;       //! Try this configuration
                }
            }
        } // while (wait_for_rx ...

        //! --- EVALUATE LOOP:
        //!     =============
        //!     Compute a new bit timing configuration. First, Phase 1 is increased,
        //!     then Phase2=Phase1 and if Phase1>5, Phase1 can be equal to Phase2 or
        //!     Phase2+1. After this, the number of TQ is increased up to its high
        //!     limit and after it is the Prescaler. During the computing high (80%)
        //!     and low (75%) limits of sampling point location are tested. SJW and
        //!     the number of sampling points are not calculated in this loop.
        while (evaluate == 1)
        {
            if (phs1_inc != 0) phs1++;
            phs1_inc = 1;

            // --- The following test takes into account the previous incrementation of phs1
            if ((phs1 > PHS1_MAX) && (phs2 >= PHS2_MAX))
            {
                phs1 = PHS1_MIN;
                phs2 = PHS2_MIN;
                phs1_inc = 0;
                if (ntq != NTQ_MAX) ntq++;
                else
                {
                    ntq = NTQ_MIN;
                    if (brp != BRP_MAX) brp++;
                    else
                    {
                        //! --- It is the failing of "can_auto_baudrate" function
                        evaluate = 0;       //! Out of "while (evaluate ..." loop
                        bt_performed = 0;   //! Return flag = FALSE
                        bt_not_found = 0;   //! Out of "while (bt_not_found ..." loop
                        DISABLE_MOB;        //! Disable MOb-0
                        CANGCON = 0x00;     //! Disable CAN controller & reset "listen" mode
                        while ((CANGSTA & (1<<ENFG)) != 0); //! Wait for Disable OK
                    }
                }
            }
            else    // if (phs1 > PHS1_MAX ...
            {
                //! --- If psh1 > 5 then phs1 =phs2 or =phs2+1, else phs1=phs2
                if (phs1>5)
                {
                    if (phs1>(phs2+1)) phs1=(++phs2);
                }
                else
                {
                phs2=phs1;
                }
                prs = ntq - ( phs1 + phs2 + 1 );

                //! --- Test PRS limits
                if ((prs <= PRS_MAX) && (prs >= PRS_MIN))
                {
                    //! --- Values  accepted if  80% >= sampling point >= 75%
                    if (((phs2<<2) >= (1+prs+phs1)) && ((phs2+phs2+phs2) <= (1+prs+phs1)))
                    {
                        evaluate = 0;     //! Out of "while (evaluate ..." loop &
                        wait_for_rx = 1;  //!    new "while (bt_not_found ..." loop
                    }
                }
            }
        } // while (evaluate ...
    } // while (bt_not_found ...

    return (bt_performed);
}

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
//------------------------------------------------------------------------------
uint8_t can_fixed_baudrate(uint8_t mode)
{
    Can_reset();
    Can_conf_bt();
    return 1;
}