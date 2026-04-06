/*
 * application_data_change_notif.h
 *
 * Contains changed application data for which notification was requested.
 */

#ifndef _OpenAPI_application_data_change_notif_H_
#define _OpenAPI_application_data_change_notif_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_application_data_change_notif_s OpenAPI_application_data_change_notif_t;
#include "af_requested_qos_data.h"
#include "am_influ_data.h"
#include "bdt_policy_data.h"
#include "dnai_eas_mapping.h"
#include "ecs_addr_data.h"
#include "iptv_config_data.h"
#include "non3gpp_dev_info.h"
#include "pfd_change_notification.h"
#include "service_parameter_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_application_data_change_notif_s {
    struct OpenAPI_iptv_config_data_s *iptv_config_data;
    struct OpenAPI_pfd_change_notification_s *pfd_data;
    struct OpenAPI_bdt_policy_data_s *bdt_policy_data;
    char *res_uri;
    struct OpenAPI_service_parameter_data_s *ser_param_data;
    struct OpenAPI_am_influ_data_s *am_influ_data;
    struct OpenAPI_dnai_eas_mapping_s *dnai_eas_data;
    struct OpenAPI_af_requested_qos_data_s *af_req_qos_data;
    struct OpenAPI_ecs_addr_data_s *ecs_addr_data;
    struct OpenAPI_non3gpp_dev_info_s *n3g_dev_data;
    char *notif_id;
};

OpenAPI_application_data_change_notif_t *OpenAPI_application_data_change_notif_create(
    OpenAPI_iptv_config_data_t *iptv_config_data,
    OpenAPI_pfd_change_notification_t *pfd_data,
    OpenAPI_bdt_policy_data_t *bdt_policy_data,
    char *res_uri,
    OpenAPI_service_parameter_data_t *ser_param_data,
    OpenAPI_am_influ_data_t *am_influ_data,
    OpenAPI_dnai_eas_mapping_t *dnai_eas_data,
    OpenAPI_af_requested_qos_data_t *af_req_qos_data,
    OpenAPI_ecs_addr_data_t *ecs_addr_data,
    OpenAPI_non3gpp_dev_info_t *n3g_dev_data,
    char *notif_id
);
void OpenAPI_application_data_change_notif_free(OpenAPI_application_data_change_notif_t *application_data_change_notif);
OpenAPI_application_data_change_notif_t *OpenAPI_application_data_change_notif_parseFromJSON(cJSON *application_data_change_notifJSON);
cJSON *OpenAPI_application_data_change_notif_convertToJSON(OpenAPI_application_data_change_notif_t *application_data_change_notif);
OpenAPI_application_data_change_notif_t *OpenAPI_application_data_change_notif_copy(OpenAPI_application_data_change_notif_t *dst, OpenAPI_application_data_change_notif_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_application_data_change_notif_H_ */

