/*
 * ue_in_area_filter.h
 *
 * Additional filters for UE in Area Report event
 */

#ifndef _OpenAPI_ue_in_area_filter_H_
#define _OpenAPI_ue_in_area_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_in_area_filter_s OpenAPI_ue_in_area_filter_t;
typedef struct OpenAPI_ue_in_area_filter_s {
    struct OpenAPI_ue_type_s *ue_type;
    bool is_aerial_srv_dnn_ind;
    int aerial_srv_dnn_ind;
} OpenAPI_ue_in_area_filter_t;

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_create(
    OpenAPI_ue_type_t *ue_type,
    bool is_aerial_srv_dnn_ind,
    int aerial_srv_dnn_ind
);
void OpenAPI_ue_in_area_filter_free(OpenAPI_ue_in_area_filter_t *ue_in_area_filter);
OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_parseFromJSON(cJSON *ue_in_area_filterJSON);
cJSON *OpenAPI_ue_in_area_filter_convertToJSON(OpenAPI_ue_in_area_filter_t *ue_in_area_filter);
OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_copy(OpenAPI_ue_in_area_filter_t *dst, OpenAPI_ue_in_area_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_in_area_filter_H_ */

