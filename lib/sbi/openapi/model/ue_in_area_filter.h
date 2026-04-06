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
typedef struct OpenAPI_ue_in_area_filter_s OpenAPI_ue_in_area_filter_t;
#include "lcs_broadcast_assistance_types_data.h"
#include "ue_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_in_area_filter_s {
    OpenAPI_ue_type_e ue_type;
    bool is_aerial_srv_dnn_ind;
    int aerial_srv_dnn_ind;
    bool is_ue_id_omit_ind;
    int ue_id_omit_ind;
    struct OpenAPI_lcs_broadcast_assistance_types_data_s *lcs_broadcasting_assistance_data_type;
};

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_create(
    OpenAPI_ue_type_e ue_type,
    bool is_aerial_srv_dnn_ind,
    int aerial_srv_dnn_ind,
    bool is_ue_id_omit_ind,
    int ue_id_omit_ind,
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcasting_assistance_data_type
);
void OpenAPI_ue_in_area_filter_free(OpenAPI_ue_in_area_filter_t *ue_in_area_filter);
OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_parseFromJSON(cJSON *ue_in_area_filterJSON);
cJSON *OpenAPI_ue_in_area_filter_convertToJSON(OpenAPI_ue_in_area_filter_t *ue_in_area_filter);
OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_copy(OpenAPI_ue_in_area_filter_t *dst, OpenAPI_ue_in_area_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_in_area_filter_H_ */

