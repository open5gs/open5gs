
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcscf_restoration_notification.h"

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_create(
    char *supi,
    OpenAPI_pcscf_address_t *failed_pcscf
)
{
    OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification_local_var = ogs_malloc(sizeof(OpenAPI_pcscf_restoration_notification_t));
    ogs_assert(pcscf_restoration_notification_local_var);

    pcscf_restoration_notification_local_var->supi = supi;
    pcscf_restoration_notification_local_var->failed_pcscf = failed_pcscf;

    return pcscf_restoration_notification_local_var;
}

void OpenAPI_pcscf_restoration_notification_free(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcscf_restoration_notification) {
        return;
    }
    if (pcscf_restoration_notification->supi) {
        ogs_free(pcscf_restoration_notification->supi);
        pcscf_restoration_notification->supi = NULL;
    }
    if (pcscf_restoration_notification->failed_pcscf) {
        OpenAPI_pcscf_address_free(pcscf_restoration_notification->failed_pcscf);
        pcscf_restoration_notification->failed_pcscf = NULL;
    }
    ogs_free(pcscf_restoration_notification);
}

cJSON *OpenAPI_pcscf_restoration_notification_convertToJSON(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcscf_restoration_notification == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [PcscfRestorationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcscf_restoration_notification->supi) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", pcscf_restoration_notification->supi) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [supi]");
        goto end;
    }

    if (pcscf_restoration_notification->failed_pcscf) {
    cJSON *failed_pcscf_local_JSON = OpenAPI_pcscf_address_convertToJSON(pcscf_restoration_notification->failed_pcscf);
    if (failed_pcscf_local_JSON == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [failed_pcscf]");
        goto end;
    }
    cJSON_AddItemToObject(item, "failedPcscf", failed_pcscf_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [failed_pcscf]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_parseFromJSON(cJSON *pcscf_restoration_notificationJSON)
{
    OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *failed_pcscf = NULL;
    OpenAPI_pcscf_address_t *failed_pcscf_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_notificationJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_pcscf_restoration_notification_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_pcscf_restoration_notification_parseFromJSON() failed [supi]");
        goto end;
    }

    failed_pcscf = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_notificationJSON, "failedPcscf");
    if (failed_pcscf) {
    failed_pcscf_local_nonprim = OpenAPI_pcscf_address_parseFromJSON(failed_pcscf);
    if (!failed_pcscf_local_nonprim) {
        ogs_error("OpenAPI_pcscf_address_parseFromJSON failed [failed_pcscf]");
        goto end;
    }
    }

    pcscf_restoration_notification_local_var = OpenAPI_pcscf_restoration_notification_create (
        ogs_strdup(supi->valuestring),
        failed_pcscf ? failed_pcscf_local_nonprim : NULL
    );

    return pcscf_restoration_notification_local_var;
end:
    if (failed_pcscf_local_nonprim) {
        OpenAPI_pcscf_address_free(failed_pcscf_local_nonprim);
        failed_pcscf_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_copy(OpenAPI_pcscf_restoration_notification_t *dst, OpenAPI_pcscf_restoration_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcscf_restoration_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed");
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

    OpenAPI_pcscf_restoration_notification_free(dst);
    dst = OpenAPI_pcscf_restoration_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

