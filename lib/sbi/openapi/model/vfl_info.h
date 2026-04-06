/*
 * vfl_info.h
 *
 * Vertical Federated Learning information 
 */

#ifndef _OpenAPI_vfl_info_H_
#define _OpenAPI_vfl_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vfl_info_s OpenAPI_vfl_info_t;
#include "ml_model_inter_info.h"
#include "nwdaf_event.h"
#include "time_window.h"
#include "vfl_capability_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vfl_info_s {
    OpenAPI_list_t *ml_analytics_ids;
    OpenAPI_vfl_capability_type_e vfl_capability_type;
    bool is_vfl_client_aggr_cap;
    int vfl_client_aggr_cap;
    struct OpenAPI_time_window_s *vfl_time_interval;
    struct OpenAPI_ml_model_inter_info_s *vfl_inter_info;
    OpenAPI_list_t *feature_ids;
};

OpenAPI_vfl_info_t *OpenAPI_vfl_info_create(
    OpenAPI_list_t *ml_analytics_ids,
    OpenAPI_vfl_capability_type_e vfl_capability_type,
    bool is_vfl_client_aggr_cap,
    int vfl_client_aggr_cap,
    OpenAPI_time_window_t *vfl_time_interval,
    OpenAPI_ml_model_inter_info_t *vfl_inter_info,
    OpenAPI_list_t *feature_ids
);
void OpenAPI_vfl_info_free(OpenAPI_vfl_info_t *vfl_info);
OpenAPI_vfl_info_t *OpenAPI_vfl_info_parseFromJSON(cJSON *vfl_infoJSON);
cJSON *OpenAPI_vfl_info_convertToJSON(OpenAPI_vfl_info_t *vfl_info);
OpenAPI_vfl_info_t *OpenAPI_vfl_info_copy(OpenAPI_vfl_info_t *dst, OpenAPI_vfl_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vfl_info_H_ */

