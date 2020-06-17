/*
 * pp_active_time.h
 *
 *
 */

#ifndef _OpenAPI_pp_active_time_H_
#define _OpenAPI_pp_active_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_active_time_s OpenAPI_pp_active_time_t;
typedef struct OpenAPI_pp_active_time_s {
    int active_time;
    char *af_instance_id;
    int reference_id;
    char *validity_time;
} OpenAPI_pp_active_time_t;

OpenAPI_pp_active_time_t *OpenAPI_pp_active_time_create(
    int active_time,
    char *af_instance_id,
    int reference_id,
    char *validity_time
    );
void OpenAPI_pp_active_time_free(OpenAPI_pp_active_time_t *pp_active_time);
OpenAPI_pp_active_time_t *OpenAPI_pp_active_time_parseFromJSON(cJSON *pp_active_timeJSON);
cJSON *OpenAPI_pp_active_time_convertToJSON(OpenAPI_pp_active_time_t *pp_active_time);
OpenAPI_pp_active_time_t *OpenAPI_pp_active_time_copy(OpenAPI_pp_active_time_t *dst, OpenAPI_pp_active_time_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_active_time_H_ */

