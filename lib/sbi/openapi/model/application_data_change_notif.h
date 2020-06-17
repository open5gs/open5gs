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
#include "bdt_policy_data.h"
#include "iptv_config_data.h"
#include "pfd_change_notification.h"
#include "service_parameter_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_application_data_change_notif_s OpenAPI_application_data_change_notif_t;
typedef struct OpenAPI_application_data_change_notif_s {
    struct OpenAPI_iptv_config_data_s *iptv_config_data;
    struct OpenAPI_pfd_change_notification_s *pfd_data;
    struct OpenAPI_bdt_policy_data_s *bdt_policy_data;
    char *res_uri;
    struct OpenAPI_service_parameter_data_s *ser_param_data;
} OpenAPI_application_data_change_notif_t;

OpenAPI_application_data_change_notif_t *OpenAPI_application_data_change_notif_create(
    OpenAPI_iptv_config_data_t *iptv_config_data,
    OpenAPI_pfd_change_notification_t *pfd_data,
    OpenAPI_bdt_policy_data_t *bdt_policy_data,
    char *res_uri,
    OpenAPI_service_parameter_data_t *ser_param_data
    );
void OpenAPI_application_data_change_notif_free(OpenAPI_application_data_change_notif_t *application_data_change_notif);
OpenAPI_application_data_change_notif_t *OpenAPI_application_data_change_notif_parseFromJSON(cJSON *application_data_change_notifJSON);
cJSON *OpenAPI_application_data_change_notif_convertToJSON(OpenAPI_application_data_change_notif_t *application_data_change_notif);
OpenAPI_application_data_change_notif_t *OpenAPI_application_data_change_notif_copy(OpenAPI_application_data_change_notif_t *dst, OpenAPI_application_data_change_notif_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_application_data_change_notif_H_ */

