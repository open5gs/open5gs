#ifndef __CORE_GENERAL_H__
#define __CORE_GENERAL_H__

/**
 * @file core_general.h
 * This is collection of oddballs that didn't fit anywhere else,
 * and might move to more appropriate headers with the release
 * of CORE 1.0.
 * @brief CORE Miscellaneous library routines
 */

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup core_general Miscellaneous library routines
 * @ingroup CORE
 * This is collection of oddballs that didn't fit anywhere else,
 * and might move to more appropriate headers with the release
 * of CORE 1.0.
 * @{
 */

/** a space */
#define ASCII_BLANK  '\040'
/** a carrige return */
#define ASCII_CR     '\015'
/** a line feed */
#define ASCII_LF     '\012'
/** a tab */
#define ASCII_TAB    '\011'

/**
 * Alignment macros
 */

/* C_ALIGN() is only to be used to align on a power of 2 boundary */
#define C_ALIGN(size, boundary) \
    (((size) + ((boundary) - 1)) & ~((boundary) - 1))

/** Default alignment */
#define C_ALIGN_DEFAULT(size) C_ALIGN(size, 8)


/**
 * String and memory functions
 */

/* STRINGIFY is defined here, and also in core_release.h, so wrap it */
#ifndef STRINGIFY
/** Properly quote a value as a string in the C preprocessor */
#define STRINGIFY(n) STRINGIFY_HELPER(n)
/** Helper macro for STRINGIFY */
#define STRINGIFY_HELPER(n) #n
#endif

/** @} */

/**
 * @defgroup core_library Library initialization and termination
 * @{
 */

/**
 * Setup any CORE internal data structures.  This MUST be the first function
 * called for any CORE library.
 * @remark See core_app_initialize if this is an application, rather than
 * a library consumer of core.
 */
CORE_DECLARE(status_t) core_initialize(void);

/**
 * Set up an application with normalized argc, argv (and optionally env) in
 * order to deal with platform-specific oddities, such as Win32 services,
 * code pages and signals.  This must be the first function called for any
 * CORE program.
 * @param argc Pointer to the argc that may be corrected
 * @param argv Pointer to the argv that may be corrected
 * @param env Pointer to the env that may be corrected, may be NULL
 * @remark See core_initialize if this is a library consumer of core.
 * Otherwise, this call is identical to core_initialize, and must be closed
 * with a call to core_terminate at the end of program execution.
 */
CORE_DECLARE(status_t) core_app_initialize(int *argc,
                                             char const * const * *argv,
                                             char const * const * *env);

/**
 * Tear down any CORE internal data structures which aren't torn down
 * automatically.
 * @remark An CORE program must call this function at termination once it
 *         has stopped using CORE services.  The CORE developers suggest using
 *         atexit to ensure this is called.  When using CORE from a language
 *         other than C that has problems with the calling convention, use
 *         core_terminate2() instead.
 */
CORE_DECLARE_NONSTD(void) core_terminate(void);

/**
 * Tear down any CORE internal data structures which aren't torn down
 * automatically, same as core_terminate
 * @remark An CORE program must call either the core_terminate or
 *         core_terminate2 function once it it has finished using CORE
 *         services.  The CORE developers suggest using atexit(core_terminate)
 *         to ensure this is done. core_terminate2 exists to allow non-c
 *         language apps to tear down core, while core_terminate is
 *         recommended from c language applications.
 */
CORE_DECLARE(void) core_terminate2(void);

/** @} */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! CORE_GENERAL_H */
