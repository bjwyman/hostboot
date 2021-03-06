/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/hwp/fapiTestHwpConfig.C $                        */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2011,2014              */
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
/**
 *  @file fapiTestHwpConfig.C
 *
 *  @brief Implements a Hardware Procedure that exercises the FAPI System Config
 *         Query functions.
 */

/*
 * Change Log ******************************************************************
 * Flag     Defect/Feature  User        Date        Description
 * ------   --------------  ----------  ----------- ----------------------------
 *                          mjjones     09/12/2011  Created.
 *                          mjjones     10/06/2011  Updated traces.
 *                          camvang     10/26/2011  Updated traces.
 *                          mjjones     01/13/2012  Use new ReturnCode interfaces
 *                          mjjones     02/21/2012  Use new Target toEcmdString
 *
 *  HWP_IGNORE_VERSION_CHECK
 */

#include <fapiTestHwpConfig.H>

extern "C"
{

//******************************************************************************
// hwpTestConfig
//******************************************************************************
fapi::ReturnCode hwpTestConfig(const fapi::Target & i_chip)
{
    FAPI_INF("hwpTestConfig: Start HWP");

    // Print the ecmd string of the chip
    FAPI_INF("hwpTestConfig: Chip: %s", i_chip.toEcmdString());

    fapi::ReturnCode l_rc;
    std::vector<fapi::Target> l_targets;

    do {

       // Call fapiGetChildChiplets to get the child MCS chiplets
       l_rc = fapiGetChildChiplets(i_chip, fapi::TARGET_TYPE_MCS_CHIPLET,
                                l_targets);

       if (l_rc)
       {
           FAPI_ERR("hwpTestConfig: Error from fapiGetChildChiplets");
           break;
       }

       FAPI_INF("hwpTestConfig: %d MCS chiplets", l_targets.size());

       if (l_targets.size() == 0)
       {
           FAPI_ERR("hwpTestConfig: No MCS chiplets");
           FAPI_SET_HWP_ERROR(l_rc, RC_TEST_CONFIG_NO_MCS_CHIPLETS);
           break;
       }

       // Save the first MCS target
       fapi::Target l_mcs = l_targets[0];

       // Call fapiGetAssociatedDimms to get the dimms for this MCS
       l_rc = fapiGetAssociatedDimms(l_mcs, l_targets);

       if (l_rc)
       {
           FAPI_ERR("hwpTestConfig: Error from fapiGetAssociatedDimms");
           break;
       }

       FAPI_INF("hwpTestConfig: %d dimms", l_targets.size());

       // Call fapiGetParentChip to get the parent of the MCS
       fapi::Target l_chip;

       l_rc = fapiGetParentChip(l_mcs, l_chip);

       if (l_rc)
       {
           FAPI_ERR("hwpTestConfig: Error from fapiGetParentChip");
           break;
       }

       // Check that the parent chip is is same as the input chip
       if (i_chip != l_chip)
       {
           FAPI_ERR("hwpTestConfig: Chip mismatch");
           FAPI_SET_HWP_ERROR(l_rc, RC_TEST_CONFIG_PARENT_CHIP_MISMATCH);
           break;
       }

       // Call  fapiGetOtherSideOfMemChannel to get mem buffer
       fapi::Target l_mb;
       fapi::Target l_mcs2;

       FAPI_INF("hwpTestConfig: mcs: %s", l_mcs.toEcmdString());
            
       l_rc = fapiGetOtherSideOfMemChannel(l_mcs, l_mb);
       if (l_rc)
       {
           FAPI_ERR("hwpTestConfig: Error from fapiGetOtherSideOfMemChannel");
           break;
       }
       FAPI_INF("hwpTestConfig: mem buf: %s", l_mb.toEcmdString());

       // Call  fapiGetOtherSideOfMemChannel to get back to the same mcs
       l_rc = fapiGetOtherSideOfMemChannel(l_mb, l_mcs2);
       if (l_rc)
       {
           FAPI_ERR("hwpTestConfig: Error from fapiGetOtherSideOfMemChannel");
           break;
       }
       FAPI_INF("hwpTestConfig: mcs: %s", l_mcs2.toEcmdString());

       if (l_mcs != l_mcs2) 
       {
           FAPI_ERR("hwpTestConfig: fapiGetOtherSideOfMemChannel wrong mcs");
           FAPI_SET_HWP_ERROR(l_rc, RC_TEST_WRONG_MCS_RETURNED);
           break;
       }

    } while (0);

    FAPI_INF("hwpTestConfig: End HWP");
    return l_rc;
}

} // extern "C"
