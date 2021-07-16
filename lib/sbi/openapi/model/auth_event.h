/*
 * auth_event.h
 *
 * 
 */

#ifndef _OpenAPI_auth_event_H_
#define _OpenAPI_auth_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "auth_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_auth_event_s OpenAPI_auth_event_t;
typedef struct OpenAPI_auth_event_s {
    char *nf_instance_id;
    int success;
    char *time_stamp;
    OpenAPI_auth_type_e auth_type;
    char *serving_network_name;
    bool is_auth_removal_ind;
    int auth_removal_ind;
} OpenAPI_auth_event_t;

OpenAPI_auth_event_t *OpenAPI_auth_event_create(
    char *nf_instance_id,
    int success,
    char *time_stamp,
    OpenAPI_auth_type_e auth_type,
    char *serving_network_name,
    bool is_auth_removal_ind,
    int auth_removal_ind
);
void OpenAPI_auth_event_free(OpenAPI_auth_event_t *auth_event);
OpenAPI_auth_event_t *OpenAPI_auth_event_parseFromJSON(cJSON *auth_eventJSON);
cJSON *OpenAPI_auth_event_convertToJSON(OpenAPI_auth_event_t *auth_event);
OpenAPI_auth_event_t *OpenAPI_auth_event_copy(OpenAPI_auth_event_t *dst, OpenAPI_auth_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_auth_event_H_ */

