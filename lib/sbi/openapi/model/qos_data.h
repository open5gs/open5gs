/*
 * qos_data.h
 *
 * 
 */

#ifndef _OpenAPI_qos_data_H_
#define _OpenAPI_qos_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "arp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_data_s OpenAPI_qos_data_t;
typedef struct OpenAPI_qos_data_s {
    char *qos_id;
    bool is__5qi;
    int _5qi;
    char *maxbr_ul;
    char *maxbr_dl;
    char *gbr_ul;
    char *gbr_dl;
    struct OpenAPI_arp_s *arp;
    bool is_qnc;
    int qnc;
    bool is_priority_level;
    int priority_level;
    bool is_aver_window;
    int aver_window;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_reflective_qos;
    int reflective_qos;
    char *sharing_key_dl;
    char *sharing_key_ul;
    bool is_max_packet_loss_rate_dl;
    int max_packet_loss_rate_dl;
    bool is_max_packet_loss_rate_ul;
    int max_packet_loss_rate_ul;
    bool is_def_qos_flow_indication;
    int def_qos_flow_indication;
    bool is_ext_max_data_burst_vol;
    int ext_max_data_burst_vol;
    bool is_packet_delay_budget;
    int packet_delay_budget;
    char *packet_error_rate;
} OpenAPI_qos_data_t;

OpenAPI_qos_data_t *OpenAPI_qos_data_create(
    char *qos_id,
    bool is__5qi,
    int _5qi,
    char *maxbr_ul,
    char *maxbr_dl,
    char *gbr_ul,
    char *gbr_dl,
    OpenAPI_arp_t *arp,
    bool is_qnc,
    int qnc,
    bool is_priority_level,
    int priority_level,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_reflective_qos,
    int reflective_qos,
    char *sharing_key_dl,
    char *sharing_key_ul,
    bool is_max_packet_loss_rate_dl,
    int max_packet_loss_rate_dl,
    bool is_max_packet_loss_rate_ul,
    int max_packet_loss_rate_ul,
    bool is_def_qos_flow_indication,
    int def_qos_flow_indication,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol,
    bool is_packet_delay_budget,
    int packet_delay_budget,
    char *packet_error_rate
);
void OpenAPI_qos_data_free(OpenAPI_qos_data_t *qos_data);
OpenAPI_qos_data_t *OpenAPI_qos_data_parseFromJSON(cJSON *qos_dataJSON);
cJSON *OpenAPI_qos_data_convertToJSON(OpenAPI_qos_data_t *qos_data);
OpenAPI_qos_data_t *OpenAPI_qos_data_copy(OpenAPI_qos_data_t *dst, OpenAPI_qos_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_data_H_ */

