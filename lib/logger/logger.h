#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) logger_start(const char *path);
CORE_DECLARE(void) logger_stop();

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__LOGGER_H__ */
