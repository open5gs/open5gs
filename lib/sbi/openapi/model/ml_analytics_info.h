/*
 * ml_analytics_info.h
 *
 * ML Analytics Filter information supported by the Nnwdaf_MLModelProvision service
 */

#ifndef _OpenAPI_ml_analytics_info_H_
#define _OpenAPI_ml_analytics_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ml_analytics_info_s OpenAPI_ml_analytics_info_t;
#include "fl_capability_type.h"
#include "ml_model_inter_info.h"
#include "nf_type.h"
#include "nwdaf_event.h"
#include "snssai.h"
#include "tai.h"
#include "time_window.h"
#include "vfl_capability_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ml_analytics_info_s {
    OpenAPI_list_t *ml_analytics_ids;
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *tracking_area_list;
    struct OpenAPI_ml_model_inter_info_s *ml_model_inter_info;
    OpenAPI_fl_capability_type_e fl_capability_type;
    struct OpenAPI_time_window_s *fl_time_interval;
    OpenAPI_list_t *nf_type_list;
    OpenAPI_list_t *nf_set_id_list;
    OpenAPI_vfl_capability_type_e vfl_capability_type;
    bool is_vfl_client_aggr_cap;
    int vfl_client_aggr_cap;
    struct OpenAPI_time_window_s *vfl_time_interval;
    struct OpenAPI_ml_model_inter_info_s *vfl_inter_info;
    OpenAPI_list_t *feature_ids;
};

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_create(
    OpenAPI_list_t *ml_analytics_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_ml_model_inter_info_t *ml_model_inter_info,
    OpenAPI_fl_capability_type_e fl_capability_type,
    OpenAPI_time_window_t *fl_time_interval,
    OpenAPI_list_t *nf_type_list,
    OpenAPI_list_t *nf_set_id_list,
    OpenAPI_vfl_capability_type_e vfl_capability_type,
    bool is_vfl_client_aggr_cap,
    int vfl_client_aggr_cap,
    OpenAPI_time_window_t *vfl_time_interval,
    OpenAPI_ml_model_inter_info_t *vfl_inter_info,
    OpenAPI_list_t *feature_ids
);
void OpenAPI_ml_analytics_info_free(OpenAPI_ml_analytics_info_t *ml_analytics_info);
OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_parseFromJSON(cJSON *ml_analytics_infoJSON);
cJSON *OpenAPI_ml_analytics_info_convertToJSON(OpenAPI_ml_analytics_info_t *ml_analytics_info);
OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_copy(OpenAPI_ml_analytics_info_t *dst, OpenAPI_ml_analytics_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ml_analytics_info_H_ */

