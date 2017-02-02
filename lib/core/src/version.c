#include "core_version.h"
#include "core_general.h" /* for CORE_STRINGIFY */

CORE_DECLARE(void) core_version(core_version_t *pvsn)
{
    pvsn->major = CORE_MAJOR_VERSION;
    pvsn->minor = CORE_MINOR_VERSION;
    pvsn->patch = CORE_PATCH_VERSION;
#ifdef CORE_IS_DEV_VERSION
    pvsn->is_dev = 1;
#else
    pvsn->is_dev = 0;
#endif
}

CORE_DECLARE(const char *) core_version_string(void)
{
    return CORE_VERSION_STRING;
}
