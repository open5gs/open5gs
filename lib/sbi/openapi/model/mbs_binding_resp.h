/*
 * mbs_binding_resp.h
 *
 * Contains the binding information for a PCF for an MBS Session.
 */

#ifndef _OpenAPI_mbs_binding_resp_H_
#define _OpenAPI_mbs_binding_resp_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbs_binding_resp_s OpenAPI_mbs_binding_resp_t;
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbs_binding_resp_s {
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
};

OpenAPI_mbs_binding_resp_t *OpenAPI_mbs_binding_resp_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points
);
void OpenAPI_mbs_binding_resp_free(OpenAPI_mbs_binding_resp_t *mbs_binding_resp);
OpenAPI_mbs_binding_resp_t *OpenAPI_mbs_binding_resp_parseFromJSON(cJSON *mbs_binding_respJSON);
cJSON *OpenAPI_mbs_binding_resp_convertToJSON(OpenAPI_mbs_binding_resp_t *mbs_binding_resp);
OpenAPI_mbs_binding_resp_t *OpenAPI_mbs_binding_resp_copy(OpenAPI_mbs_binding_resp_t *dst, OpenAPI_mbs_binding_resp_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_binding_resp_H_ */

