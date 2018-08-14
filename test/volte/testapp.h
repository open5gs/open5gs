#ifndef __TESTAPP_H__
#define __TESTAPP_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) test_app_initialize(
        const char *config_path, const char *log_path, const char *pid_path);
CORE_DECLARE(void) test_app_terminate(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TESTAPP_H__ */
