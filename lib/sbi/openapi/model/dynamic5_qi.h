/*
 * dynamic5_qi.h
 *
 * It indicates the QoS Characteristics for a Non-standardised or not pre-configured 5QI for downlink and uplink. 
 */

#ifndef _OpenAPI_dynamic5_qi_H_
#define _OpenAPI_dynamic5_qi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_resource_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dynamic5_qi_s OpenAPI_dynamic5_qi_t;
typedef struct OpenAPI_dynamic5_qi_s {
    OpenAPI_qos_resource_type_e resource_type;
    int priority_level;
    int packet_delay_budget;
    char *packet_err_rate;
    bool is_aver_window;
    int aver_window;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_ext_max_data_burst_vol;
    int ext_max_data_burst_vol;
    bool is_ext_packet_del_budget;
    int ext_packet_del_budget;
    bool is_cn_packet_delay_budget_dl;
    int cn_packet_delay_budget_dl;
    bool is_cn_packet_delay_budget_ul;
    int cn_packet_delay_budget_ul;
} OpenAPI_dynamic5_qi_t;

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_create(
    OpenAPI_qos_resource_type_e resource_type,
    int priority_level,
    int packet_delay_budget,
    char *packet_err_rate,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol,
    bool is_ext_packet_del_budget,
    int ext_packet_del_budget,
    bool is_cn_packet_delay_budget_dl,
    int cn_packet_delay_budget_dl,
    bool is_cn_packet_delay_budget_ul,
    int cn_packet_delay_budget_ul
);
void OpenAPI_dynamic5_qi_free(OpenAPI_dynamic5_qi_t *dynamic5_qi);
OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_parseFromJSON(cJSON *dynamic5_qiJSON);
cJSON *OpenAPI_dynamic5_qi_convertToJSON(OpenAPI_dynamic5_qi_t *dynamic5_qi);
OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_copy(OpenAPI_dynamic5_qi_t *dst, OpenAPI_dynamic5_qi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dynamic5_qi_H_ */

