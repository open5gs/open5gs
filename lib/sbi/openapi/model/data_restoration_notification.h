/*
 * data_restoration_notification.h
 *
 * Contains identities representing those UEs potentially affected by a data-loss event, or a subscriber data migration event, at the UDR 
 */

#ifndef _OpenAPI_data_restoration_notification_H_
#define _OpenAPI_data_restoration_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_data_restoration_notification_s OpenAPI_data_restoration_notification_t;
#include "identity_range.h"
#include "plmn_id.h"
#include "snssai.h"
#include "supi_range.h"
#include "udm_data_to_resynchronize.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_data_restoration_notification_s {
    char *last_replication_time;
    char *recovery_time;
    struct OpenAPI_plmn_id_s *plmn_id;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *reset_ids;
    OpenAPI_list_t *s_nssai_list;
    OpenAPI_list_t *dnn_list;
    char *udm_group_id;
    bool is_rediscovery_ind;
    int rediscovery_ind;
    bool is_no_resynchronization_required;
    int no_resynchronization_required;
    char *resynchronization_time;
    OpenAPI_list_t *data_to_resync;
    bool is_any_ue_ind;
    int any_ue_ind;
    bool is_ausf_rediscovery_ind;
    int ausf_rediscovery_ind;
};

OpenAPI_data_restoration_notification_t *OpenAPI_data_restoration_notification_create(
    char *last_replication_time,
    char *recovery_time,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *reset_ids,
    OpenAPI_list_t *s_nssai_list,
    OpenAPI_list_t *dnn_list,
    char *udm_group_id,
    bool is_rediscovery_ind,
    int rediscovery_ind,
    bool is_no_resynchronization_required,
    int no_resynchronization_required,
    char *resynchronization_time,
    OpenAPI_list_t *data_to_resync,
    bool is_any_ue_ind,
    int any_ue_ind,
    bool is_ausf_rediscovery_ind,
    int ausf_rediscovery_ind
);
void OpenAPI_data_restoration_notification_free(OpenAPI_data_restoration_notification_t *data_restoration_notification);
OpenAPI_data_restoration_notification_t *OpenAPI_data_restoration_notification_parseFromJSON(cJSON *data_restoration_notificationJSON);
cJSON *OpenAPI_data_restoration_notification_convertToJSON(OpenAPI_data_restoration_notification_t *data_restoration_notification);
OpenAPI_data_restoration_notification_t *OpenAPI_data_restoration_notification_copy(OpenAPI_data_restoration_notification_t *dst, OpenAPI_data_restoration_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_restoration_notification_H_ */

