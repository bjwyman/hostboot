//  IBM_PROLOG_BEGIN_TAG
//  This is an automatically generated prolog.
//
//  $Source: src/include/usr/gcov.h $
//
//  IBM CONFIDENTIAL
//
//  COPYRIGHT International Business Machines Corp. 2012
//
//  p1
//
//  Object Code Only (OCO) source materials
//  Licensed Internal Code Source Materials
//  IBM HostBoot Licensed Internal Code
//
//  The source code for this program is not published or other-
//  wise divested of its trade secrets, irrespective of what has
//  been deposited with the U.S. Copyright Office.
//
//  Origin: 30
//
//  IBM_PROLOG_END
#ifndef __USR_GCOV_H
#define __USR_GCOV_H

/** @file gcov.h
 *  @brief Header file to generate gcov_info chain for each module.
 *
 *  Each gcov-instrumented module needs to have its own gcov_info chain
 *  and associated linking function.  Each .o file will have its own
 *  gcov_info object which is added to the chain when the module is loaded
 *  (static initializers) by calling the __gcov_init function.
 *
 *  We make the gcov_info chain unique per-module, instead of a single
 *  global chain, so that we don't have bad pointers if a module was
 *  loaded and then unloaded.
 */

#include <stddef.h>

/** @struct gcov_info
 *  @brief Structure generated by gcc.  Do not use.
 *
 *  This structure is automatically generated and instances of it created by
 *  gcc when the --coverage compile option is used.  We don't need to
 *  manipulate this structure from code except to fix up the chains as objects
 *  are added to the chain.  The rest of this structure is parsed by the
 *  Gcov.pm debug tool.
 */
struct gcov_info
{
    unsigned int version;  // Purposefully chose 'unsigned int' to match gcc.
    gcov_info* next;
    // Really there is more after here in the structure, but this is all
    // we care about from an in-memory perspective.
};

// Preprocessor magic to create a variable name based off the module name.
// GCOV_INFO_OBJ() will create a post-processed name like
// 'foobar_gcov_info_head' or 'core_gcov_info_head'.
#ifdef __HOSTBOOT_MODULE
    #define __GCOV_PREFIX __HOSTBOOT_MODULE
#else
    #define __GCOV_PREFIX core
#endif

#define __GCOV_INFO_OBJ(X,Y) X ## Y
#define _GCOV_INFO_OBJ(X,Y) __GCOV_INFO_OBJ(X,Y)
#define GCOV_INFO_OBJ() _GCOV_INFO_OBJ(__GCOV_PREFIX, _gcov_info_head)

/** Pointer to the beginning of the gcov_info chain for this module. */
gcov_info* GCOV_INFO_OBJ() = NULL;

/** Function called by module loading to add the object gcov_info instance
 *  to the chain.
 */
extern "C"
void __gcov_init(gcov_info* i_info)
{
    // Atomically push i_info onto the gcov_info_head stack.
    do
    {
        i_info->next = GCOV_INFO_OBJ();
    } while (!__sync_bool_compare_and_swap(&GCOV_INFO_OBJ(),
                                           i_info->next, i_info));
}

/** Unneeded function but must be defined to compile.
 *
 *  This function appears to be typically used by libgcov.so when instrumented
 *  on a real linux-based system.  It can be used to merge counters across
 *  multiple runs or when a 'fork' occurs.  It doesn't appear that this
 *  function ever gets called for us but the unresolved symbol is added to
 *  the module (by gcc) so we've created a stub here to pass compile.
 */
extern "C"
void __gcov_merge_add()
{
    while(1);
}

#endif
