/*
 * ulcl_bp_information.h
 *
 * UL CL or BP Information
 */

#ifndef _OpenAPI_ulcl_bp_information_H_
#define _OpenAPI_ulcl_bp_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ulcl_bp_information_s OpenAPI_ulcl_bp_information_t;
#include "event_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ulcl_bp_information_s {
    char *ulcl_bp_upf_id;
    OpenAPI_list_t *upf_events;
};

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_create(
    char *ulcl_bp_upf_id,
    OpenAPI_list_t *upf_events
);
void OpenAPI_ulcl_bp_information_free(OpenAPI_ulcl_bp_information_t *ulcl_bp_information);
OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_parseFromJSON(cJSON *ulcl_bp_informationJSON);
cJSON *OpenAPI_ulcl_bp_information_convertToJSON(OpenAPI_ulcl_bp_information_t *ulcl_bp_information);
OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_copy(OpenAPI_ulcl_bp_information_t *dst, OpenAPI_ulcl_bp_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ulcl_bp_information_H_ */

