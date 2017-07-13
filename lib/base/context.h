#ifndef __CONTEXT__
#define __CONTEXT__

#include "core_debug.h"
#include "core_param.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_CONFIG_FILE_SIZE    1024
#define MAX_DB_URI_LEN          256

typedef struct _context_t {
    c_uint8_t       log_path[MAX_FILEPATH_LEN];
    c_uint8_t       db_uri[MAX_DB_URI_LEN];
} context_t;

CORE_DECLARE(status_t)      context_init(void);
CORE_DECLARE(status_t)      context_final(void);
CORE_DECLARE(context_t*)    context_self(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CONTEXT__ */
