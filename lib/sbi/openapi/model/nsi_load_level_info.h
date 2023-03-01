/*
 * nsi_load_level_info.h
 *
 * Represents the network slice and optionally the associated network slice instance and the  load level information. 
 */

#ifndef _OpenAPI_nsi_load_level_info_H_
#define _OpenAPI_nsi_load_level_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"
#include "number_average.h"
#include "resource_usage.h"
#include "snssai.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsi_load_level_info_s OpenAPI_nsi_load_level_info_t;
typedef struct OpenAPI_nsi_load_level_info_s {
    int load_level_information;
    struct OpenAPI_snssai_s *snssai;
    char *nsi_id;
    struct OpenAPI_resource_usage_s *res_usage;
    bool is_num_of_exceed_load_level_thr;
    int num_of_exceed_load_level_thr;
    bool is_exceed_load_level_thr_ind;
    int exceed_load_level_thr_ind;
    struct OpenAPI_network_area_info_s *network_area;
    struct OpenAPI_time_window_s *time_period;
    OpenAPI_list_t *res_usg_thr_cross_time_period;
    struct OpenAPI_number_average_s *num_of_ues;
    struct OpenAPI_number_average_s *num_of_pdu_sess;
    bool is_confidence;
    int confidence;
} OpenAPI_nsi_load_level_info_t;

OpenAPI_nsi_load_level_info_t *OpenAPI_nsi_load_level_info_create(
    int load_level_information,
    OpenAPI_snssai_t *snssai,
    char *nsi_id,
    OpenAPI_resource_usage_t *res_usage,
    bool is_num_of_exceed_load_level_thr,
    int num_of_exceed_load_level_thr,
    bool is_exceed_load_level_thr_ind,
    int exceed_load_level_thr_ind,
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_time_window_t *time_period,
    OpenAPI_list_t *res_usg_thr_cross_time_period,
    OpenAPI_number_average_t *num_of_ues,
    OpenAPI_number_average_t *num_of_pdu_sess,
    bool is_confidence,
    int confidence
);
void OpenAPI_nsi_load_level_info_free(OpenAPI_nsi_load_level_info_t *nsi_load_level_info);
OpenAPI_nsi_load_level_info_t *OpenAPI_nsi_load_level_info_parseFromJSON(cJSON *nsi_load_level_infoJSON);
cJSON *OpenAPI_nsi_load_level_info_convertToJSON(OpenAPI_nsi_load_level_info_t *nsi_load_level_info);
OpenAPI_nsi_load_level_info_t *OpenAPI_nsi_load_level_info_copy(OpenAPI_nsi_load_level_info_t *dst, OpenAPI_nsi_load_level_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsi_load_level_info_H_ */

