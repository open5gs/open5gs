/*
 * subscription_data.h
 *
 * Information of a subscription to notifications to NRF events, included in subscription requests and responses
 */

#ifndef _OpenAPI_subscription_data_H_
#define _OpenAPI_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"
#include "notif_condition.h"
#include "notification_event_type.h"
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscription_data_s OpenAPI_subscription_data_t;
typedef struct OpenAPI_subscription_data_s {
    char *nf_status_notification_uri;
    char *req_nf_instance_id;
    char *subscription_id;
    char *validity_time;
    OpenAPI_list_t *req_notif_events;
    struct OpenAPI_plmn_id_s *plmn_id;
    char *nid;
    struct OpenAPI_notif_condition_s *notif_condition;
    OpenAPI_nf_type_e req_nf_type;
    char *req_nf_fqdn;
    OpenAPI_list_t *req_snssais;
    OpenAPI_list_t *req_plmn_list;
} OpenAPI_subscription_data_t;

OpenAPI_subscription_data_t *OpenAPI_subscription_data_create(
    char *nf_status_notification_uri,
    char *req_nf_instance_id,
    char *subscription_id,
    char *validity_time,
    OpenAPI_list_t *req_notif_events,
    OpenAPI_plmn_id_t *plmn_id,
    char *nid,
    OpenAPI_notif_condition_t *notif_condition,
    OpenAPI_nf_type_e req_nf_type,
    char *req_nf_fqdn,
    OpenAPI_list_t *req_snssais,
    OpenAPI_list_t *req_plmn_list
    );
void OpenAPI_subscription_data_free(OpenAPI_subscription_data_t *subscription_data);
OpenAPI_subscription_data_t *OpenAPI_subscription_data_parseFromJSON(cJSON *subscription_dataJSON);
cJSON *OpenAPI_subscription_data_convertToJSON(OpenAPI_subscription_data_t *subscription_data);
OpenAPI_subscription_data_t *OpenAPI_subscription_data_copy(OpenAPI_subscription_data_t *dst, OpenAPI_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_data_H_ */

