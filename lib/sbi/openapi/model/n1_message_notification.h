/*
 * n1_message_notification.h
 *
 * Data within a N1 message notification request
 */

#ifndef _OpenAPI_n1_message_notification_H_
#define _OpenAPI_n1_message_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi.h"
#include "guami.h"
#include "n1_message_container.h"
#include "ncgi.h"
#include "registration_context_container.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_message_notification_s OpenAPI_n1_message_notification_t;
typedef struct OpenAPI_n1_message_notification_s {
    char *n1_notify_subscription_id;
    struct OpenAPI_n1_message_container_s *n1_message_container;
    char *lcs_correlation_id;
    struct OpenAPI_registration_context_container_s *registration_ctxt_container;
    char *new_lmf_identification;
    struct OpenAPI_guami_s *guami;
    bool is_c_io_t5_gs_optimisation;
    int c_io_t5_gs_optimisation;
    struct OpenAPI_ecgi_s *ecgi;
    struct OpenAPI_ncgi_s *ncgi;
} OpenAPI_n1_message_notification_t;

OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_create(
    char *n1_notify_subscription_id,
    OpenAPI_n1_message_container_t *n1_message_container,
    char *lcs_correlation_id,
    OpenAPI_registration_context_container_t *registration_ctxt_container,
    char *new_lmf_identification,
    OpenAPI_guami_t *guami,
    bool is_c_io_t5_gs_optimisation,
    int c_io_t5_gs_optimisation,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ncgi_t *ncgi
);
void OpenAPI_n1_message_notification_free(OpenAPI_n1_message_notification_t *n1_message_notification);
OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_parseFromJSON(cJSON *n1_message_notificationJSON);
cJSON *OpenAPI_n1_message_notification_convertToJSON(OpenAPI_n1_message_notification_t *n1_message_notification);
OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_copy(OpenAPI_n1_message_notification_t *dst, OpenAPI_n1_message_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_message_notification_H_ */

