/*
 * reauthorization_details.h
 *
 * 
 */

#ifndef _OpenAPI_reauthorization_details_H_
#define _OpenAPI_reauthorization_details_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reauthorization_details_s OpenAPI_reauthorization_details_t;
#include "quota_management_indicator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reauthorization_details_s {
    bool is_service_id;
    int service_id;
    bool is_rating_group;
    int rating_group;
    OpenAPI_quota_management_indicator_e quota_management_indicator;
};

OpenAPI_reauthorization_details_t *OpenAPI_reauthorization_details_create(
    bool is_service_id,
    int service_id,
    bool is_rating_group,
    int rating_group,
    OpenAPI_quota_management_indicator_e quota_management_indicator
);
void OpenAPI_reauthorization_details_free(OpenAPI_reauthorization_details_t *reauthorization_details);
OpenAPI_reauthorization_details_t *OpenAPI_reauthorization_details_parseFromJSON(cJSON *reauthorization_detailsJSON);
cJSON *OpenAPI_reauthorization_details_convertToJSON(OpenAPI_reauthorization_details_t *reauthorization_details);
OpenAPI_reauthorization_details_t *OpenAPI_reauthorization_details_copy(OpenAPI_reauthorization_details_t *dst, OpenAPI_reauthorization_details_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reauthorization_details_H_ */

