/*
 * gbr_qos_flow_information.h
 *
 *
 */

#ifndef _OpenAPI_gbr_qos_flow_information_H_
#define _OpenAPI_gbr_qos_flow_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notification_control.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gbr_qos_flow_information_s OpenAPI_gbr_qos_flow_information_t;
typedef struct OpenAPI_gbr_qos_flow_information_s {
    char *max_fbr_dl;
    char *max_fbr_ul;
    char *gua_fbr_dl;
    char *gua_fbr_ul;
    struct OpenAPI_notification_control_s *notif_control;
    int max_packet_loss_rate_dl;
    int max_packet_loss_rate_ul;
} OpenAPI_gbr_qos_flow_information_t;

OpenAPI_gbr_qos_flow_information_t *OpenAPI_gbr_qos_flow_information_create(
    char *max_fbr_dl,
    char *max_fbr_ul,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    OpenAPI_notification_control_t *notif_control,
    int max_packet_loss_rate_dl,
    int max_packet_loss_rate_ul
    );
void OpenAPI_gbr_qos_flow_information_free(OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_information);
OpenAPI_gbr_qos_flow_information_t *OpenAPI_gbr_qos_flow_information_parseFromJSON(cJSON *gbr_qos_flow_informationJSON);
cJSON *OpenAPI_gbr_qos_flow_information_convertToJSON(OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_information);
OpenAPI_gbr_qos_flow_information_t *OpenAPI_gbr_qos_flow_information_copy(OpenAPI_gbr_qos_flow_information_t *dst, OpenAPI_gbr_qos_flow_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gbr_qos_flow_information_H_ */

