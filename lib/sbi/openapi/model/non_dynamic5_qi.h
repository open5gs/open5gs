/*
 * non_dynamic5_qi.h
 *
 * 
 */

#ifndef _OpenAPI_non_dynamic5_qi_H_
#define _OpenAPI_non_dynamic5_qi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_non_dynamic5_qi_s OpenAPI_non_dynamic5_qi_t;
typedef struct OpenAPI_non_dynamic5_qi_s {
    bool is_priority_level;
    int priority_level;
    bool is_aver_window;
    int aver_window;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_ext_max_data_burst_vol;
    int ext_max_data_burst_vol;
    bool is_cn_packet_delay_budget_dl;
    int cn_packet_delay_budget_dl;
    bool is_cn_packet_delay_budget_ul;
    int cn_packet_delay_budget_ul;
} OpenAPI_non_dynamic5_qi_t;

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_create(
    bool is_priority_level,
    int priority_level,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol,
    bool is_cn_packet_delay_budget_dl,
    int cn_packet_delay_budget_dl,
    bool is_cn_packet_delay_budget_ul,
    int cn_packet_delay_budget_ul
);
void OpenAPI_non_dynamic5_qi_free(OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi);
OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_parseFromJSON(cJSON *non_dynamic5_qiJSON);
cJSON *OpenAPI_non_dynamic5_qi_convertToJSON(OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi);
OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_copy(OpenAPI_non_dynamic5_qi_t *dst, OpenAPI_non_dynamic5_qi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non_dynamic5_qi_H_ */

