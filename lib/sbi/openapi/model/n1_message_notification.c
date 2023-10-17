
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_message_notification.h"

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
)
{
    OpenAPI_n1_message_notification_t *n1_message_notification_local_var = ogs_malloc(sizeof(OpenAPI_n1_message_notification_t));
    ogs_assert(n1_message_notification_local_var);

    n1_message_notification_local_var->n1_notify_subscription_id = n1_notify_subscription_id;
    n1_message_notification_local_var->n1_message_container = n1_message_container;
    n1_message_notification_local_var->lcs_correlation_id = lcs_correlation_id;
    n1_message_notification_local_var->registration_ctxt_container = registration_ctxt_container;
    n1_message_notification_local_var->new_lmf_identification = new_lmf_identification;
    n1_message_notification_local_var->guami = guami;
    n1_message_notification_local_var->is_c_io_t5_gs_optimisation = is_c_io_t5_gs_optimisation;
    n1_message_notification_local_var->c_io_t5_gs_optimisation = c_io_t5_gs_optimisation;
    n1_message_notification_local_var->ecgi = ecgi;
    n1_message_notification_local_var->ncgi = ncgi;

    return n1_message_notification_local_var;
}

void OpenAPI_n1_message_notification_free(OpenAPI_n1_message_notification_t *n1_message_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n1_message_notification) {
        return;
    }
    if (n1_message_notification->n1_notify_subscription_id) {
        ogs_free(n1_message_notification->n1_notify_subscription_id);
        n1_message_notification->n1_notify_subscription_id = NULL;
    }
    if (n1_message_notification->n1_message_container) {
        OpenAPI_n1_message_container_free(n1_message_notification->n1_message_container);
        n1_message_notification->n1_message_container = NULL;
    }
    if (n1_message_notification->lcs_correlation_id) {
        ogs_free(n1_message_notification->lcs_correlation_id);
        n1_message_notification->lcs_correlation_id = NULL;
    }
    if (n1_message_notification->registration_ctxt_container) {
        OpenAPI_registration_context_container_free(n1_message_notification->registration_ctxt_container);
        n1_message_notification->registration_ctxt_container = NULL;
    }
    if (n1_message_notification->new_lmf_identification) {
        ogs_free(n1_message_notification->new_lmf_identification);
        n1_message_notification->new_lmf_identification = NULL;
    }
    if (n1_message_notification->guami) {
        OpenAPI_guami_free(n1_message_notification->guami);
        n1_message_notification->guami = NULL;
    }
    if (n1_message_notification->ecgi) {
        OpenAPI_ecgi_free(n1_message_notification->ecgi);
        n1_message_notification->ecgi = NULL;
    }
    if (n1_message_notification->ncgi) {
        OpenAPI_ncgi_free(n1_message_notification->ncgi);
        n1_message_notification->ncgi = NULL;
    }
    ogs_free(n1_message_notification);
}

cJSON *OpenAPI_n1_message_notification_convertToJSON(OpenAPI_n1_message_notification_t *n1_message_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n1_message_notification == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [N1MessageNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n1_message_notification->n1_notify_subscription_id) {
    if (cJSON_AddStringToObject(item, "n1NotifySubscriptionId", n1_message_notification->n1_notify_subscription_id) == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_notify_subscription_id]");
        goto end;
    }
    }

    if (!n1_message_notification->n1_message_container) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_message_container]");
        return NULL;
    }
    cJSON *n1_message_container_local_JSON = OpenAPI_n1_message_container_convertToJSON(n1_message_notification->n1_message_container);
    if (n1_message_container_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_message_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1MessageContainer", n1_message_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_message_container]");
        goto end;
    }

    if (n1_message_notification->lcs_correlation_id) {
    if (cJSON_AddStringToObject(item, "lcsCorrelationId", n1_message_notification->lcs_correlation_id) == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [lcs_correlation_id]");
        goto end;
    }
    }

    if (n1_message_notification->registration_ctxt_container) {
    cJSON *registration_ctxt_container_local_JSON = OpenAPI_registration_context_container_convertToJSON(n1_message_notification->registration_ctxt_container);
    if (registration_ctxt_container_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [registration_ctxt_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "registrationCtxtContainer", registration_ctxt_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [registration_ctxt_container]");
        goto end;
    }
    }

    if (n1_message_notification->new_lmf_identification) {
    if (cJSON_AddStringToObject(item, "newLmfIdentification", n1_message_notification->new_lmf_identification) == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [new_lmf_identification]");
        goto end;
    }
    }

    if (n1_message_notification->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(n1_message_notification->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (n1_message_notification->is_c_io_t5_gs_optimisation) {
    if (cJSON_AddBoolToObject(item, "cIoT5GSOptimisation", n1_message_notification->c_io_t5_gs_optimisation) == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [c_io_t5_gs_optimisation]");
        goto end;
    }
    }

    if (n1_message_notification->ecgi) {
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(n1_message_notification->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [ecgi]");
        goto end;
    }
    }

    if (n1_message_notification->ncgi) {
    cJSON *ncgi_local_JSON = OpenAPI_ncgi_convertToJSON(n1_message_notification->ncgi);
    if (ncgi_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [ncgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgi", ncgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [ncgi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_parseFromJSON(cJSON *n1_message_notificationJSON)
{
    OpenAPI_n1_message_notification_t *n1_message_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n1_notify_subscription_id = NULL;
    cJSON *n1_message_container = NULL;
    OpenAPI_n1_message_container_t *n1_message_container_local_nonprim = NULL;
    cJSON *lcs_correlation_id = NULL;
    cJSON *registration_ctxt_container = NULL;
    OpenAPI_registration_context_container_t *registration_ctxt_container_local_nonprim = NULL;
    cJSON *new_lmf_identification = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *c_io_t5_gs_optimisation = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    cJSON *ncgi = NULL;
    OpenAPI_ncgi_t *ncgi_local_nonprim = NULL;
    n1_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "n1NotifySubscriptionId");
    if (n1_notify_subscription_id) {
    if (!cJSON_IsString(n1_notify_subscription_id) && !cJSON_IsNull(n1_notify_subscription_id)) {
        ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [n1_notify_subscription_id]");
        goto end;
    }
    }

    n1_message_container = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "n1MessageContainer");
    if (!n1_message_container) {
        ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [n1_message_container]");
        goto end;
    }
    n1_message_container_local_nonprim = OpenAPI_n1_message_container_parseFromJSON(n1_message_container);
    if (!n1_message_container_local_nonprim) {
        ogs_error("OpenAPI_n1_message_container_parseFromJSON failed [n1_message_container]");
        goto end;
    }

    lcs_correlation_id = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "lcsCorrelationId");
    if (lcs_correlation_id) {
    if (!cJSON_IsString(lcs_correlation_id) && !cJSON_IsNull(lcs_correlation_id)) {
        ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [lcs_correlation_id]");
        goto end;
    }
    }

    registration_ctxt_container = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "registrationCtxtContainer");
    if (registration_ctxt_container) {
    registration_ctxt_container_local_nonprim = OpenAPI_registration_context_container_parseFromJSON(registration_ctxt_container);
    if (!registration_ctxt_container_local_nonprim) {
        ogs_error("OpenAPI_registration_context_container_parseFromJSON failed [registration_ctxt_container]");
        goto end;
    }
    }

    new_lmf_identification = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "newLmfIdentification");
    if (new_lmf_identification) {
    if (!cJSON_IsString(new_lmf_identification) && !cJSON_IsNull(new_lmf_identification)) {
        ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [new_lmf_identification]");
        goto end;
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    c_io_t5_gs_optimisation = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "cIoT5GSOptimisation");
    if (c_io_t5_gs_optimisation) {
    if (!cJSON_IsBool(c_io_t5_gs_optimisation)) {
        ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [c_io_t5_gs_optimisation]");
        goto end;
    }
    }

    ecgi = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "ecgi");
    if (ecgi) {
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }
    }

    ncgi = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "ncgi");
    if (ncgi) {
    ncgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi);
    if (!ncgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi]");
        goto end;
    }
    }

    n1_message_notification_local_var = OpenAPI_n1_message_notification_create (
        n1_notify_subscription_id && !cJSON_IsNull(n1_notify_subscription_id) ? ogs_strdup(n1_notify_subscription_id->valuestring) : NULL,
        n1_message_container_local_nonprim,
        lcs_correlation_id && !cJSON_IsNull(lcs_correlation_id) ? ogs_strdup(lcs_correlation_id->valuestring) : NULL,
        registration_ctxt_container ? registration_ctxt_container_local_nonprim : NULL,
        new_lmf_identification && !cJSON_IsNull(new_lmf_identification) ? ogs_strdup(new_lmf_identification->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        c_io_t5_gs_optimisation ? true : false,
        c_io_t5_gs_optimisation ? c_io_t5_gs_optimisation->valueint : 0,
        ecgi ? ecgi_local_nonprim : NULL,
        ncgi ? ncgi_local_nonprim : NULL
    );

    return n1_message_notification_local_var;
end:
    if (n1_message_container_local_nonprim) {
        OpenAPI_n1_message_container_free(n1_message_container_local_nonprim);
        n1_message_container_local_nonprim = NULL;
    }
    if (registration_ctxt_container_local_nonprim) {
        OpenAPI_registration_context_container_free(registration_ctxt_container_local_nonprim);
        registration_ctxt_container_local_nonprim = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    if (ncgi_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_local_nonprim);
        ncgi_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_copy(OpenAPI_n1_message_notification_t *dst, OpenAPI_n1_message_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_message_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_n1_message_notification_free(dst);
    dst = OpenAPI_n1_message_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

