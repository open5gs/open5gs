/*
 * traffic_para_data.h
 *
 * Contains Traffic Parameter(s) related control information.
 */

#ifndef _OpenAPI_traffic_para_data_H_
#define _OpenAPI_traffic_para_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_para_data_s OpenAPI_traffic_para_data_t;
#include "reporting_frequency.h"
#include "traffic_parameter_meas.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_para_data_s {
    bool is_period_ul;
    int period_ul;
    bool is_period_dl;
    int period_dl;
    OpenAPI_list_t *req_traffic_paras;
    OpenAPI_list_t *rep_freqs;
    bool is_dl_n6_jitter_thr;
    int dl_n6_jitter_thr;
    bool is_rep_period_null;
    bool is_rep_period;
    int rep_period;
};

OpenAPI_traffic_para_data_t *OpenAPI_traffic_para_data_create(
    bool is_period_ul,
    int period_ul,
    bool is_period_dl,
    int period_dl,
    OpenAPI_list_t *req_traffic_paras,
    OpenAPI_list_t *rep_freqs,
    bool is_dl_n6_jitter_thr,
    int dl_n6_jitter_thr,
    bool is_rep_period_null,
    bool is_rep_period,
    int rep_period
);
void OpenAPI_traffic_para_data_free(OpenAPI_traffic_para_data_t *traffic_para_data);
OpenAPI_traffic_para_data_t *OpenAPI_traffic_para_data_parseFromJSON(cJSON *traffic_para_dataJSON);
cJSON *OpenAPI_traffic_para_data_convertToJSON(OpenAPI_traffic_para_data_t *traffic_para_data);
OpenAPI_traffic_para_data_t *OpenAPI_traffic_para_data_copy(OpenAPI_traffic_para_data_t *dst, OpenAPI_traffic_para_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_para_data_H_ */

