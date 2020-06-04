
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcscf_restoration_notification.h"

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_create(
    char *supi
    )
{
    OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification_local_var = OpenAPI_malloc(sizeof(OpenAPI_pcscf_restoration_notification_t));
    if (!pcscf_restoration_notification_local_var) {
        return NULL;
    }
    pcscf_restoration_notification_local_var->supi = supi;

    return pcscf_restoration_notification_local_var;
}

void OpenAPI_pcscf_restoration_notification_free(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification)
{
    if (NULL == pcscf_restoration_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pcscf_restoration_notification->supi);
    ogs_free(pcscf_restoration_notification);
}

cJSON *OpenAPI_pcscf_restoration_notification_convertToJSON(OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification)
{
    cJSON *item = NULL;

    if (pcscf_restoration_notification == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [PcscfRestorationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcscf_restoration_notification->supi) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [supi]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "supi", pcscf_restoration_notification->supi) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_notification_convertToJSON() failed [supi]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pcscf_restoration_notification_t *OpenAPI_pcscf_restoration_notification_parseFromJSON(cJSON *pcscf_restoration_notificationJSON)
{
    OpenAPI_pcscf_restoration_notification_t *pcscf_restoration_notification_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_notificationJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_pcscf_restoration_notification_parseFromJSON() failed [supi]");
        goto end;
    }


    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_pcscf_restoration_notification_parseFromJSON() failed [supi]");
        goto end;
    }

    pcscf_restoration_notification_local_var = OpenAPI_pcscf_restoration_notification_create (
        ogs_strdup(supi->valuestring)
        );

    return pcscf_restoration_notification_local_var;
end:
    return NULL;
}

