#ifndef __CORE_VERSION_H__
#define __CORE_VERSION_H__

/**
 * @file version.h
 * @brief CORE Versioning Interface
 *
 * CORE's Version
 *
 * There are several different mechanisms for accessing the version. There
 * is a string form, and a set of numbers; in addition, there are constants
 * which can be compiled into your application, and you can query the library
 * being used for its actual version.
 *
 * Note that it is possible for an application to detect that it has been
 * compiled against a different version of CORE by use of the compile-time
 * constants and the use of the run-time query function.
 *
 */


/* The numeric compile-time version constants. These constants are the
 * authoritative version numbers for CORE.
 */

/** major version
 * Major API changes that could cause compatibility problems for older
 * programs such as structure size changes.  No binary compatibility is
 * possible across a change in the major version.
 */
#define CORE_MAJOR_VERSION       1

/** minor version
 * Minor API changes that do not cause binary compatibility problems.
 * Reset to 0 when upgrading CORE_MAJOR_VERSION
 */
#define CORE_MINOR_VERSION       0

/** patch level
 * The Patch Level never includes API changes, simply bug fixes.
 * Reset to 0 when upgrading CORE_MINOR_VERSION
 */
#define CORE_PATCH_VERSION       0

/**
 * The symbol CORE_IS_DEV_VERSION is only defined for internal,
 * "development" copies of CORE.  It is undefined for released versions
 * of CORE.
 */
#define CORE_IS_DEV_VERSION

/**
 * Check at compile time if the CORE version is at least a certain
 * level.
 * @param major The major version component of the version checked
 * for (e.g., the "1" of "1.3.0").
 * @param minor The minor version component of the version checked
 * for (e.g., the "3" of "1.3.0").
 * @param patch The patch level component of the version checked
 * for (e.g., the "0" of "1.3.0").
 * @remark This macro is available with CORE versions starting with
 * 1.3.0.
 */
#define CORE_VERSION_AT_LEAST(major,minor,patch)                    \
(((major) < CORE_MAJOR_VERSION)                                     \
 || ((major) == CORE_MAJOR_VERSION && (minor) < CORE_MINOR_VERSION) \
 || ((major) == CORE_MAJOR_VERSION && (minor) == CORE_MINOR_VERSION && (patch) <= CORE_PATCH_VERSION))

#if defined(CORE_IS_DEV_VERSION) || defined(DOXYGEN)
/** Internal: string form of the "is dev" flag */
#define CORE_IS_DEV_STRING "-dev"
#else
#define CORE_IS_DEV_STRING ""
#endif

/* STRINGIFY is defined here, and also in general.h, so wrap it */
#ifndef STRINGIFY
/** Properly quote a value as a string in the C preprocessor */
#define STRINGIFY(n) STRINGIFY_HELPER(n)
/** Helper macro for STRINGIFY */
#define STRINGIFY_HELPER(n) #n
#endif

/** The formatted string of CORE's version */
#define CORE_VERSION_STRING \
     STRINGIFY(CORE_MAJOR_VERSION) "." \
     STRINGIFY(CORE_MINOR_VERSION) "." \
     STRINGIFY(CORE_PATCH_VERSION) \
     CORE_IS_DEV_STRING

/** An alternative formatted string of CORE's version */
/* macro for Win32 .rc files using numeric csv representation */
#define CORE_VERSION_STRING_CSV CORE_MAJOR_VERSION ##, \
                             ##CORE_MINOR_VERSION ##, \
                             ##CORE_PATCH_VERSION


#ifndef __CORE_VERSION_ONLY__

/* The C language API to access the version at run time,
 * as opposed to compile time.  CORE_VERSION_ONLY may be defined
 * externally when preprocessing version.h to obtain strictly
 * the C Preprocessor macro declarations.
 */

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * The numeric version information is broken out into fields within this
 * structure.
 */
typedef struct
{
    int major;      /**< major number */
    int minor;      /**< minor number */
    int patch;      /**< patch number */
    int is_dev;     /**< is development (1 or 0) */
} core_version_t;

/**
 * Return CORE's version information information in a numeric form.
 *
 *  @param pvsn Pointer to a version structure for returning the version
 *              information.
 */
CORE_DECLARE(void) core_version(core_version_t *pvsn);

/** Return CORE's version information as a string. */
CORE_DECLARE(const char *) core_version_string(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ndef CORE_VERSION_ONLY */

#endif /* ndef CORE_VERSION_H */
