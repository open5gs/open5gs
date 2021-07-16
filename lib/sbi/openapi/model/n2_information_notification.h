/*
 * n2_information_notification.h
 *
 * 
 */

#ifndef _OpenAPI_n2_information_notification_H_
#define _OpenAPI_n2_information_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "global_ran_node_id.h"
#include "guami.h"
#include "n2_info_container.h"
#include "n2_info_notify_reason.h"
#include "smf_change_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_information_notification_s OpenAPI_n2_information_notification_t;
typedef struct OpenAPI_n2_information_notification_s {
    char *n2_notify_subscription_id;
    struct OpenAPI_n2_info_container_s *n2_info_container;
    OpenAPI_list_t *to_release_session_list;
    char *lcs_correlation_id;
    OpenAPI_n2_info_notify_reason_e notify_reason;
    OpenAPI_list_t *smf_change_info_list;
    struct OpenAPI_global_ran_node_id_s *ran_node_id;
    char *initial_amf_name;
    char *an_n2_ipv4_addr;
    char *an_n2_ipv6_addr;
    struct OpenAPI_guami_s *guami;
    bool is_notify_source_ng_ran;
    int notify_source_ng_ran;
} OpenAPI_n2_information_notification_t;

OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_create(
    char *n2_notify_subscription_id,
    OpenAPI_n2_info_container_t *n2_info_container,
    OpenAPI_list_t *to_release_session_list,
    char *lcs_correlation_id,
    OpenAPI_n2_info_notify_reason_e notify_reason,
    OpenAPI_list_t *smf_change_info_list,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    char *initial_amf_name,
    char *an_n2_ipv4_addr,
    char *an_n2_ipv6_addr,
    OpenAPI_guami_t *guami,
    bool is_notify_source_ng_ran,
    int notify_source_ng_ran
);
void OpenAPI_n2_information_notification_free(OpenAPI_n2_information_notification_t *n2_information_notification);
OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_parseFromJSON(cJSON *n2_information_notificationJSON);
cJSON *OpenAPI_n2_information_notification_convertToJSON(OpenAPI_n2_information_notification_t *n2_information_notification);
OpenAPI_n2_information_notification_t *OpenAPI_n2_information_notification_copy(OpenAPI_n2_information_notification_t *dst, OpenAPI_n2_information_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_notification_H_ */

