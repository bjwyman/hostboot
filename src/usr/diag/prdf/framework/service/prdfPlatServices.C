/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/diag/prdf/framework/service/prdfPlatServices.C $      */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2012                   */
/*                                                                        */
/* p1                                                                     */
/*                                                                        */
/* Object Code Only (OCO) source materials                                */
/* Licensed Internal Code Source Materials                                */
/* IBM HostBoot Licensed Internal Code                                    */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* Origin: 30                                                             */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

/**
 * @file prdfPlatServices.C
 * @brief PRD wrapper of external componnets ( other then targetting)
 */

//------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------

#include <prdfPlatServices.H>

#include <iipbits.h>
#include <iipsdbug.h>
#include <iipglobl.h>
#include <prdfTrace.H>

#include <fapi.H>

#ifdef __HOSTBOOT_MODULE
  #include <time.h>
  #include <errno.h>
  #include <sys/time.h>
  #include <diag/mdia/mdia.H>
  #include <diag/mdia/mdiamevent.H>
#else
  #include <iplp_registry.H>
  #include <mboxclientlib.H>
  #include <mnfgPolicyFlags.H>
  #include <smgr_ipltypes.H>
  #include <smgr_registry.H>
  #include <svpd_externals.H>
  #include <svpdextstructs.H>
  #include <GardExtInt.H>
  #include <dscvReg.H>
  #include <dadaProcMsViaProc.H>
  #include <utillib.H>
  #include <rmgrBaseClientLib.H>
  #include <services/hwas/hwsvHwAvailSvc.H> // For deconfigureTargetAtRuntime()
#endif

//------------------------------------------------------------------------------

namespace PRDF
{

namespace PlatServices
{

//##############################################################################
//##
//##                        Hostboot and FSP functions
//##
//##############################################################################

//##############################################################################
//##                     System Level Utility Functions
//##############################################################################

bool isMasterFSP()
{
    bool l_isMaster = true;
    #ifdef __HOSTBOOT_MODULE
    // Always true in hostboot
    l_isMaster = true;
    #else
    // We're going to assume master because it's better to have scom
    // errors by writing on the slave then to not do something on the
    // master.

    errlHndl_t l_errl = NULL;
    uint8_t l_roleRegValue;
    size_t l_roleRegSize = sizeof(l_roleRegValue);

    l_errl = UtilReg::read(FSP_ROLE_STR,&l_roleRegValue,
                        l_roleRegSize);

    if (NULL != l_errl)
    {
        PRDF_COMMIT_ERRL(l_errl, ERRL_ACTION_REPORT);
    }
    else
    {
        l_isMaster = (DSCV_FSP_MASTER == l_roleRegValue);
    }
    #endif

    return l_isMaster;
}

//------------------------------------------------------------------------------

bool isMemoryPreservingIpl()
{
    bool l_isMemPreservingIpl = false;
    #ifdef __HOSTBOOT_MODULE
    //TODO
    #else
    //if (inCMMode())
    //    return false;

    // Check the fsp ipl type so we know if this needs to be initialized
    uint32_t l_iplType = 0x00000000;
    size_t l_size = sizeof( l_iplType );

    // Terry Opie Comments:  First thing I see...  You're gonna want to
    // read:SMGR_REG_CECIPL_TYPE_KEY instead of the fsp ipl type.
    // The one(SMGR_REG_FSPIPL_TYPE_KEY) you're reading is strictly fsp
    // related ipl types.  resets/reset reloads, etc.

    errlHndl_t l_errl = UtilReg::read( SMGR_REG_CECIPL_TYPE_KEY,
                                    &l_iplType,
                                    l_size );
    if (NULL != l_errl)
    {
        PRDF_ERR( "[isMemoryPreservingIpl] Failed to read registry" );
        PRDF_COMMIT_ERRL(l_errl, ERRL_ACTION_REPORT);
    }
    else
    {
        // Terry Opie Comments:  Also for checking the mask..You can do
        // it the way you have it,or use a macro that are in
        // smgr_ipltypes.H. Returns true if the Main Store preserved
        // attribute bit is set #define
        // SMGR_IPLTYPE_IS_MS_PRESVD(_i_ipltype_)(((_i_ipltype_)&
        // SMGR_ATTR_MS_PRESVD_MASK) != 0)Call that
        // passing in the ipl type, and it'll return true if its Mem presv.
        if(SMGR_IPLTYPE_IS_MS_PRESVD(l_iplType))
        {
            l_isMemPreservingIpl = true;
        }
    }
    #endif

    return l_isMemPreservingIpl;
}

//------------------------------------------------------------------------------

void getECIDString(TARGETING::TargetHandle_t i_pGivenTarget, const char *o_ecidStr )
{
    o_ecidStr ="";
    PRDF_ERR( "[getECIDString] Function not implemented yet" );

}

//------------------------------------------------------------------------------

void getCurrentTime( PrdTimer & o_timer )
{
#ifdef __HOSTBOOT_MODULE

    timespec_t curTime;
    PRDF_ASSERT(0 == clock_gettime(CLOCK_MONOTONIC, &curTime))

    // Hostboot uptime in secs
    o_timer = curTime.tv_sec;

    //Since Hostboot doesn't have any System checkstop
    //We don't have to worry about the detailed time struct
    //for System checkstop timestamp

#else

    time_t thetime = time(NULL);
    struct tm * l_curEventTime = localtime(&thetime);

    // record the detailed time struct
    PrdTimer::prdftm_t l_tm(l_curEventTime->tm_sec,
                            l_curEventTime->tm_min,
                            l_curEventTime->tm_hour,
                            l_curEventTime->tm_wday,
                            l_curEventTime->tm_mday,
                            l_curEventTime->tm_yday,
                            l_curEventTime->tm_mon,
                            l_curEventTime->tm_year);
    o_timer.settm(l_tm);

#endif
}

//------------------------------------------------------------------------------

errlHndl_t syncFile( const char* i_fileName )
{
#ifdef __HOSTBOOT_MODULE
    return NULL;
#else
    return rmgrSyncFile(PRDF_COMP_ID, i_fileName);
#endif
}

//##############################################################################
//##                     MNFG Policy Flag Functions
//##############################################################################

#ifdef __HOSTBOOT_MODULE

// TODO: This is a hack until we are able to get these flags supported in
//       Hostboot.
enum
{
    MNFG_THRESHOLDS = 0,
    MNFG_HDAT_AVP_ENABLE,
    MNFG_SRC_TERM,
};

#endif

// Helper function to access the state of manufacturing policy flags.
// TODO: Need hostboot support for the following flags:
//          MNFG_AVP_ENABLE
//          MNFG_SRC_TERM
//          MNFG_NO_RBS
//          MNFG_FAST_BACKGROUND_SCRUB
//          MNFG_TEST_RBS
//          MNFG_IPL_MEMORY_CE_CHECKING
bool isMnfgFlagSet( uint32_t i_flag )
{
    bool o_rc = false;

    #ifdef __HOSTBOOT_MODULE

    // TODO - mnfgIsPolicyFlagSet() may not be supported in hostboot. Need to
    // verify how this will be done.
    PRDF_ERR( "[isMnfgFlagSet] Hostboot Function not implemented yet" );

    #else

    errlHndl_t errl = mnfgIsPolicyFlagSet( i_flag, o_rc );
    if ( NULL != errl )
    {
        PRDF_ERR( "[isMnfgFlagSet] mnfgIsPolicyFlagSet(0x%08x)", i_flag );
        PRDF_COMMIT_ERRL(errl, ERRL_ACTION_REPORT);
        o_rc = false;
    }

    #endif

    return o_rc;
}

//------------------------------------------------------------------------------

bool mfgMode()       { return isMnfgFlagSet( MNFG_THRESHOLDS      ); }
bool hdatAvpMode()   { return isMnfgFlagSet( MNFG_HDAT_AVP_ENABLE ); }
bool mnfgTerminate() { return isMnfgFlagSet( MNFG_SRC_TERM        ); }

//##############################################################################
//##                        Memory specific functions
//##############################################################################

/* TODO - Get the memory buffer raw card type (i.e. R/C A). This is needed for
          the DRAM site locations for buffered DIMMs. Should be able to get this
          from an attribute but doesn't look like this is available yet.
getMembufRawCardType()
{
}
*/

//------------------------------------------------------------------------------

/* TODO - Get the type of the card a DIMM is plugged into. This is needed for
          the DRAM site locations for IS DIMMs. Should be able to get this from
          an attribute but doesn't look like this is available yet.
getDimmPlugCardType()
{
}
*/

//------------------------------------------------------------------------------

/* TODO
int32_t getBadDqBitmap( TARGETING::TargetHandle_t i_mbaTarget
                        const uint8_t i_portSlct,
                        const uint8_t i_dimmSlct,
                        const uint8_t i_rankSlct,
                        uint8_t (&o_data)[DIMM_DQ_RANK_BITMAP_SIZE] )
{
    int32_t o_rc = SUCCESS;

    // TODO: Call dimmGetBadDqBitmap() in dimmBadDqBitmapFuncs.H.
    // NOTE: DIMM_DQ_RANK_BITMAP_SIZE is in dimmConsts.H
    // NOTE: Will need to convert TARGETING::TargetHandle_t to fapi::Target.

    return o_rc;
}

//------------------------------------------------------------------------------

int32_t setBadDqBitmap( TARGETING::TargetHandle_t i_mbaTarget
                        const uint8_t i_portSlct,
                        const uint8_t i_dimmSlct,
                        const uint8_t i_rankSlct,
                        const uint8_t (&i_data)[DIMM_DQ_RANK_BITMAP_SIZE] )
{
    int32_t o_rc = SUCCESS;

    // TODO: Call dimmSetBadDqBitmap() in dimmBadDqBitmapFuncs.H.
    // NOTE: DIMM_DQ_RANK_BITMAP_SIZE is in dimmConsts.H
    // NOTE: Will need to convert TARGETING::TargetHandle_t to fapi::Target.

    return o_rc;
}
*/

//##############################################################################
//##
//##                        Hostboot only functions
//##
//##############################################################################

#ifdef __HOSTBOOT_MODULE

bool isInMdiaMode()
{
    bool o_isInMdiaMode = false;

    MDIA::waitingForMaintCmdEvents(o_isInMdiaMode);

    return o_isInMdiaMode;
}

//------------------------------------------------------------------------------

int32_t mdiaSendCmdComplete( TARGETING::TargetHandle_t i_mbaTarget )
{
    using namespace TARGETING;

    int32_t o_rc = SUCCESS;

    do
    {
        // Verify type.
        TYPE l_type = getTargetType(i_mbaTarget);
        if ( TYPE_MBA != l_type )
        {
            PRDF_ERR( "[PlatServices::mdiaSendCmdComplete] unsupported target "
                      "type %d", l_type );
            o_rc = FAIL;
            break;
        }

        // Send command complete to MDIA.
        MDIA::MaintCommandEvent l_mdiaEvent;
        l_mdiaEvent.type   = MDIA::COMMAND_COMPLETE;
        l_mdiaEvent.target = i_mbaTarget;

        errlHndl_t errl = MDIA::processEvent( l_mdiaEvent );
        if ( NULL != errl )
        {
            PRDF_ERR( "[PlatServices::mdiaSendCmdComplete] MDIA::processEvent "
                      "failed" );
            PRDF_COMMIT_ERRL( errl, ERRL_ACTION_REPORT );
            o_rc = FAIL;
            break;
        }

    } while (0);

    if ( SUCCESS != o_rc )
    {
        PRDF_ERR( "[PlatServices::mdiaSendCmdComplete] Failed: i_target=0x%08x",
                  getHuid(i_mbaTarget) );
    }

    return o_rc;
}

#endif // __HOSTBOOT_MODULE

//##############################################################################
//##
//##                           FSP only functions
//##
//##############################################################################

#ifndef __HOSTBOOT_MODULE

errlHndl_t runtimeDeconfig( TARGETING::TargetHandle_t i_target )
{
    using namespace HWAS;
    return deconfigureTargetAtRuntime( i_target, DECONFIG_FOR_DUMP );
}

#endif // not __HOSTBOOT_MODULE

} // end namespace PlatServices

} // end namespace PRDF
