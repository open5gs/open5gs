/*
 * restricted_status.h
 *
 * Contains reason for restricted status and the time stamp of when the status was stored. 
 */

#ifndef _OpenAPI_restricted_status_H_
#define _OpenAPI_restricted_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_restricted_status_s OpenAPI_restricted_status_t;
#include "exception_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_restricted_status_s {
    OpenAPI_exception_id_e exception_id;
    char *time_stamp;
};

OpenAPI_restricted_status_t *OpenAPI_restricted_status_create(
    OpenAPI_exception_id_e exception_id,
    char *time_stamp
);
void OpenAPI_restricted_status_free(OpenAPI_restricted_status_t *restricted_status);
OpenAPI_restricted_status_t *OpenAPI_restricted_status_parseFromJSON(cJSON *restricted_statusJSON);
cJSON *OpenAPI_restricted_status_convertToJSON(OpenAPI_restricted_status_t *restricted_status);
OpenAPI_restricted_status_t *OpenAPI_restricted_status_copy(OpenAPI_restricted_status_t *dst, OpenAPI_restricted_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_restricted_status_H_ */

