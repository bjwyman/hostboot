/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/hwp/include/mss_unmask_errors.H $                */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2012,2014              */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
// $Id: mss_unmask_errors.H,v 1.2 2014/04/09 19:49:26 gollub Exp $
//------------------------------------------------------------------------------
// Don't forget to create CVS comments when you check in your changes!
//------------------------------------------------------------------------------
// CHANGE HISTORY:
//------------------------------------------------------------------------------
// Version:|   Date:  | Author: | Comment:
//---------|----------|---------|-----------------------------------------------
//   1.1   | 09/05/12 | gollub  | Created
//   1.2   |07-APR-14 | gollub  | Added mss_unmask_pervasive_errors
//         |          |         | Version was set back from 1.2 to 1.1 when I moved from procedures/ipl/fapi to procedures/
//   1.2   |09-APR-14 | gollub  | Checked in a second time just to get version to 1.2

#ifndef _MSS_UNMASK_ERRORS_H
#define _MSS_UNMASK_ERRORS_H

/** @file  mss_unmask_errors.H
 *  @brief Utility functions to set action regs and unmask FIR bits 
 *         at the end of various mss IPL procedures.
 */


//------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------

#include <fapi.H>
#include <ecmdDataBufferBase.H>



//------------------------------------------------------------------------------
// Constants and enums
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// mss_unmask_pervasive_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of cen_mem_startclocks.C
 *         Sets action regs and mask settings for pervasive errors to their 
 *         runtime settings.
 *
 * @param  i_target         Centaur target
 * @param  i_bad_rc         If cen_mem_startclocks.C already has a bad rc
 *                          before it calls this function, we pass it in as 
 *                          i_bad_rc. If this function gets it's own bad local 
 *                          l_rc, i_bad_rc will be commited, and l_rc will be 
 *                          passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_pervasive_errors( const fapi::Target & i_target,
                                              fapi::ReturnCode i_bad_rc );



//------------------------------------------------------------------------------
// mss_unmask_inband_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of proc_cen_framelock.C
 *         Sets action regs and mask settings for inband errors to their 
 *         runtime settings.
 *
 * @param  i_target         Centaur target
 * @param  i_bad_rc         If proc_cen_framelock.C already has a bad rc
 *                          before it calls this function, we pass it in as 
 *                          i_bad_rc. If this function gets it's own bad local 
 *                          l_rc, i_bad_rc will be commited, and l_rc will be 
 *                          passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_inband_errors( const fapi::Target & i_target,
                                           fapi::ReturnCode i_bad_rc );


//------------------------------------------------------------------------------
// mss_unmask_ddrphy_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of mss_ddr_phy_reset.C.
 *         Sets action regs and mask settings for ddr phy errors to their 
 *         runtime settings.
 *
 * @param  i_target         MBA target
 * @param  i_bad_rc         If mss_ddr_phy_reset.C already has a bad rc
 *                          before it calls this function, we pass it in as 
 *                          i_bad_rc. If this function gets it's own bad local 
 *                          l_rc, i_bad_rc will be commited, and l_rc will be 
 *                          passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_ddrphy_errors( const fapi::Target & i_target,
                                           fapi::ReturnCode i_bad_rc );


//------------------------------------------------------------------------------
// mss_unmask_draminit_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of mss_draminit.C.
 *         Sets MBACALFIR action regs to their runtime settings, and unmasks
 *         errors that are valid for PRD to handle after mss_draminit procedure.
 *
 * @param  i_target         MBA target
 * @param  i_bad_rc         If mss_draminit.C already has a bad rc
 *                          before it calls this function, we pass it in as 
 *                          i_bad_rc. If this function gets it's own bad local 
 *                          l_rc, i_bad_rc will be commited, and l_rc will be 
 *                          passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_draminit_errors( const fapi::Target & i_target,
                                             fapi::ReturnCode i_bad_rc );


//------------------------------------------------------------------------------
// mss_unmask_draminit_training_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of mss_draminit_training.C.
 *         Unmasks MBACALFIR errors that are valid for PRD to handle after
 *         mss_draminit_training procedure.
 *
 * @param  i_target         MBA target
 * @param  i_bad_rc         If mss_draminit_training.C already has a bad rc
 *                          before it calls this function, we pass it in as 
 *                          i_bad_rc. If this function gets it's own bad local 
 *                          l_rc, i_bad_rc will be commited, and l_rc will be 
 *                          passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_draminit_training_errors( 
                                            const fapi::Target & i_target,
                                            fapi::ReturnCode i_bad_rc );


//------------------------------------------------------------------------------
// mss_unmask_draminit_training_advanced_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of mss_draminit_training_advanced.C.
 *         Unmasks MBACALFIR errors that are valid for PRD to handle after
 *         mss_draminit_training_advanced procedure.
 *
 * @param  i_target         MBA target
 * @param  i_bad_rc         If mss_draminit_training_advanced.C already has a 
 *                          bad rc before it calls this function, we pass it in 
 *                          as i_bad_rc. If this function gets it's own bad  
 *                          local l_rc, i_bad_rc will be commited, and l_rc will 
 *                          be passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_draminit_training_advanced_errors( 
                                            const fapi::Target & i_target,
                                            fapi::ReturnCode i_bad_rc );


//------------------------------------------------------------------------------
// mss_unmask_maint_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of mss_draminit_mc.C.
 *         Sets action regs and unmasks maint errors prior to the maint logic
 *         being used in memdiags so that PRD will be able to handle them 
 *         if they happen during memdiags.
 *
 * @param  i_target         MBA target
 * @param  i_bad_rc         If mss_draminit_mc already has a 
 *                          bad rc before it calls this function, we pass it in 
 *                          as i_bad_rc. If this function gets it's own bad  
 *                          local l_rc, i_bad_rc will be commited, and l_rc will 
 *                          be passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_maint_errors(const fapi::Target & i_target,
                                         fapi::ReturnCode i_bad_rc );


//------------------------------------------------------------------------------
// mss_unmask_fetch_errors
//------------------------------------------------------------------------------


/**
 * @brief  To be called at the end of mss_thermal_init.C.
 *         Sets action regs and unmasks fetch errors prior to the start of
 *         mainline traffic.
 *
 * @param  i_target         Centaur target
 * @param  i_bad_rc         If mss_thermal_init already has a 
 *                          bad rc before it calls this function, we pass it in 
 *                          as i_bad_rc. If this function gets it's own bad  
 *                          local l_rc, i_bad_rc will be commited, and l_rc will 
 *                          be passed back as return value. Else if no l_rc, 
 *                          i_bad_rc will be be passed back as return value.
 * @return Non-SUCCESS if i_bad_rc Non_SUCCESS, or if internal function fails, 
 *         SUCCESS otherwise.
 */
fapi::ReturnCode mss_unmask_fetch_errors(const fapi::Target & i_target,
                                         fapi::ReturnCode i_bad_rc );



//------------------------------------------------------------------------------
// fapiGetScom_w_retry
//------------------------------------------------------------------------------

/**
 * @brief     Reads a SCOM register from a Chip and retries once if SCOM fails.
 *            Retry is done with assumption that hostboot will switch from
 *            inband SCOM to FSI, so if inband failed due to channel fail,
 *            FSI may still work.
 * @param[in] i_target      Target to operate on
 * @param[in] i_address     Scom address to read from
 * @param[out] o_data       ecmdDataBufferBase object that holds data read from
 *                             address
 * @return ReturnCode. Zero on success, else platform specified error
 */
fapi::ReturnCode fapiGetScom_w_retry(const fapi::Target& i_target,
                                     const uint64_t i_address,
                                     ecmdDataBufferBase & o_data);

//------------------------------------------------------------------------------
// fapiPutScom_w_retry
//------------------------------------------------------------------------------

/**
 * @brief     Writes a SCOM register on a Chip and retries once if SCOM fails.
 *            Retry is done with assumption that hostboot will switch from
 *            inband SCOM to FSI, so if inband failed due to channel fail,
 *            FSI may still work.
 * @param[in] i_target Target to operate on
 * @param[in] i_address Scom address to write to
 * @param[in] i_data ecmdDataBufferBase object that holds data to write into
 *                   address
 * @return ReturnCode. Zero on success, else platform specified error
 */
fapi::ReturnCode fapiPutScom_w_retry(const fapi::Target& i_target,
                                     const uint64_t i_address,
                                     ecmdDataBufferBase & i_data);






#endif /* _MSS_UNMASK_ERRORS_H */
