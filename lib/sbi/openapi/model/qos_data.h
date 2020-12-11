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
    int _5qi;
    char *maxbr_ul;
    char *maxbr_dl;
    char *gbr_ul;
    char *gbr_dl;
    struct OpenAPI_arp_s *arp;
    int qnc;
    int priority_level;
    int aver_window;
    int max_data_burst_vol;
    int reflective_qos;
    char *sharing_key_dl;
    char *sharing_key_ul;
    int max_packet_loss_rate_dl;
    int max_packet_loss_rate_ul;
    int def_qos_flow_indication;
    int ext_max_data_burst_vol;
} OpenAPI_qos_data_t;

OpenAPI_qos_data_t *OpenAPI_qos_data_create(
    char *qos_id,
    int _5qi,
    char *maxbr_ul,
    char *maxbr_dl,
    char *gbr_ul,
    char *gbr_dl,
    OpenAPI_arp_t *arp,
    int qnc,
    int priority_level,
    int aver_window,
    int max_data_burst_vol,
    int reflective_qos,
    char *sharing_key_dl,
    char *sharing_key_ul,
    int max_packet_loss_rate_dl,
    int max_packet_loss_rate_ul,
    int def_qos_flow_indication,
    int ext_max_data_burst_vol
    );
void OpenAPI_qos_data_free(OpenAPI_qos_data_t *qos_data);
OpenAPI_qos_data_t *OpenAPI_qos_data_parseFromJSON(cJSON *qos_dataJSON);
cJSON *OpenAPI_qos_data_convertToJSON(OpenAPI_qos_data_t *qos_data);
OpenAPI_qos_data_t *OpenAPI_qos_data_copy(OpenAPI_qos_data_t *dst, OpenAPI_qos_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_data_H_ */

