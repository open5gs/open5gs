/*
 * top_application.h
 *
 * Top application that contributes the most to the traffic.
 */

#ifndef _OpenAPI_top_application_H_
#define _OpenAPI_top_application_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flow_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_top_application_s OpenAPI_top_application_t;
typedef struct OpenAPI_top_application_s {
    char *app_id;
    struct OpenAPI_flow_info_s *ip_traffic_filter;
    bool is_ratio;
    int ratio;
} OpenAPI_top_application_t;

OpenAPI_top_application_t *OpenAPI_top_application_create(
    char *app_id,
    OpenAPI_flow_info_t *ip_traffic_filter,
    bool is_ratio,
    int ratio
);
void OpenAPI_top_application_free(OpenAPI_top_application_t *top_application);
OpenAPI_top_application_t *OpenAPI_top_application_parseFromJSON(cJSON *top_applicationJSON);
cJSON *OpenAPI_top_application_convertToJSON(OpenAPI_top_application_t *top_application);
OpenAPI_top_application_t *OpenAPI_top_application_copy(OpenAPI_top_application_t *dst, OpenAPI_top_application_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_top_application_H_ */

