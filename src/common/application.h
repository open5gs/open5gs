#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) app_will_initialize(
        const char *config_path, const char *log_path);
CORE_DECLARE(status_t) app_did_initialize(void);
CORE_DECLARE(void) app_will_terminate(void);
CORE_DECLARE(void) app_did_terminate(void);

CORE_DECLARE(status_t) config_initialize(const char *config_path);
CORE_DECLARE(void) config_terminate(void);

CORE_DECLARE(status_t) app_logger_restart(void);
CORE_DECLARE(status_t) app_log_pid(const char *pid_path, const char *app_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_H__ */
