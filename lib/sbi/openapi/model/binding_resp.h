/*
 * binding_resp.h
 *
 * 
 */

#ifndef _OpenAPI_binding_resp_H_
#define _OpenAPI_binding_resp_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_binding_resp_s OpenAPI_binding_resp_t;
typedef struct OpenAPI_binding_resp_s {
    char *pcf_sm_fqdn;
    OpenAPI_list_t *pcf_sm_ip_end_points;
} OpenAPI_binding_resp_t;

OpenAPI_binding_resp_t *OpenAPI_binding_resp_create(
    char *pcf_sm_fqdn,
    OpenAPI_list_t *pcf_sm_ip_end_points
);
void OpenAPI_binding_resp_free(OpenAPI_binding_resp_t *binding_resp);
OpenAPI_binding_resp_t *OpenAPI_binding_resp_parseFromJSON(cJSON *binding_respJSON);
cJSON *OpenAPI_binding_resp_convertToJSON(OpenAPI_binding_resp_t *binding_resp);
OpenAPI_binding_resp_t *OpenAPI_binding_resp_copy(OpenAPI_binding_resp_t *dst, OpenAPI_binding_resp_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_binding_resp_H_ */

