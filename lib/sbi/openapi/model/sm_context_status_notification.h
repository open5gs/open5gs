/*
 * sm_context_status_notification.h
 *
 * 
 */

#ifndef _OpenAPI_sm_context_status_notification_H_
#define _OpenAPI_sm_context_status_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "apn_rate_status.h"
#include "small_data_rate_status.h"
#include "status_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_status_notification_s OpenAPI_sm_context_status_notification_t;
typedef struct OpenAPI_sm_context_status_notification_s {
    struct OpenAPI_status_info_s *status_info;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
    bool is_ddn_failure_status;
    int ddn_failure_status;
    OpenAPI_list_t *notify_correlation_ids_forddn_failure;
    char *new_smf_id;
    char *new_smf_set_id;
    char *old_smf_id;
    char *old_sm_context_ref;
    char *alt_anchor_smf_uri;
    char *alt_anchor_smf_id;
} OpenAPI_sm_context_status_notification_t;

OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_create(
    OpenAPI_status_info_t *status_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_ddn_failure_status,
    int ddn_failure_status,
    OpenAPI_list_t *notify_correlation_ids_forddn_failure,
    char *new_smf_id,
    char *new_smf_set_id,
    char *old_smf_id,
    char *old_sm_context_ref,
    char *alt_anchor_smf_uri,
    char *alt_anchor_smf_id
);
void OpenAPI_sm_context_status_notification_free(OpenAPI_sm_context_status_notification_t *sm_context_status_notification);
OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_parseFromJSON(cJSON *sm_context_status_notificationJSON);
cJSON *OpenAPI_sm_context_status_notification_convertToJSON(OpenAPI_sm_context_status_notification_t *sm_context_status_notification);
OpenAPI_sm_context_status_notification_t *OpenAPI_sm_context_status_notification_copy(OpenAPI_sm_context_status_notification_t *dst, OpenAPI_sm_context_status_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_status_notification_H_ */

