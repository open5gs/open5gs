/*
 * update_nwdaf_registration_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_update_nwdaf_registration_200_response_H_
#define _OpenAPI_update_nwdaf_registration_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "context_info.h"
#include "event_id.h"
#include "nwdaf_registration.h"
#include "patch_result.h"
#include "report_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_update_nwdaf_registration_200_response_s OpenAPI_update_nwdaf_registration_200_response_t;
typedef struct OpenAPI_update_nwdaf_registration_200_response_s {
    char *nwdaf_instance_id;
    OpenAPI_list_t *analytics_ids;
    char *nwdaf_set_id;
    char *registration_time;
    struct OpenAPI_context_info_s *context_info;
    char *supported_features;
    OpenAPI_list_t *reset_ids;
    OpenAPI_list_t *report;
} OpenAPI_update_nwdaf_registration_200_response_t;

OpenAPI_update_nwdaf_registration_200_response_t *OpenAPI_update_nwdaf_registration_200_response_create(
    char *nwdaf_instance_id,
    OpenAPI_list_t *analytics_ids,
    char *nwdaf_set_id,
    char *registration_time,
    OpenAPI_context_info_t *context_info,
    char *supported_features,
    OpenAPI_list_t *reset_ids,
    OpenAPI_list_t *report
);
void OpenAPI_update_nwdaf_registration_200_response_free(OpenAPI_update_nwdaf_registration_200_response_t *update_nwdaf_registration_200_response);
OpenAPI_update_nwdaf_registration_200_response_t *OpenAPI_update_nwdaf_registration_200_response_parseFromJSON(cJSON *update_nwdaf_registration_200_responseJSON);
cJSON *OpenAPI_update_nwdaf_registration_200_response_convertToJSON(OpenAPI_update_nwdaf_registration_200_response_t *update_nwdaf_registration_200_response);
OpenAPI_update_nwdaf_registration_200_response_t *OpenAPI_update_nwdaf_registration_200_response_copy(OpenAPI_update_nwdaf_registration_200_response_t *dst, OpenAPI_update_nwdaf_registration_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_update_nwdaf_registration_200_response_H_ */

