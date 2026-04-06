/*
 * alternative_qos_profile.h
 *
 * Alternative QoS Profile
 */

#ifndef _OpenAPI_alternative_qos_profile_H_
#define _OpenAPI_alternative_qos_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_alternative_qos_profile_s OpenAPI_alternative_qos_profile_t;
#include "pdu_set_qos_para.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_alternative_qos_profile_s {
    int index;
    char *gua_fbr_dl;
    char *gua_fbr_ul;
    bool is_packet_delay_budget;
    int packet_delay_budget;
    char *packet_err_rate;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_pdu_set_qos_dl_null;
    struct OpenAPI_pdu_set_qos_para_s *pdu_set_qos_dl;
    bool is_pdu_set_qos_ul_null;
    struct OpenAPI_pdu_set_qos_para_s *pdu_set_qos_ul;
};

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_create(
    int index,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    bool is_packet_delay_budget,
    int packet_delay_budget,
    char *packet_err_rate,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_pdu_set_qos_dl_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_dl,
    bool is_pdu_set_qos_ul_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_ul
);
void OpenAPI_alternative_qos_profile_free(OpenAPI_alternative_qos_profile_t *alternative_qos_profile);
OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_parseFromJSON(cJSON *alternative_qos_profileJSON);
cJSON *OpenAPI_alternative_qos_profile_convertToJSON(OpenAPI_alternative_qos_profile_t *alternative_qos_profile);
OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_copy(OpenAPI_alternative_qos_profile_t *dst, OpenAPI_alternative_qos_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alternative_qos_profile_H_ */

