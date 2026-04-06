
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecn_marking_congestion_info_status.h"

OpenAPI_ecn_marking_congestion_info_status_t *OpenAPI_ecn_marking_congestion_info_status_create(
    int qfi,
    OpenAPI_activation_status_e activation_status
)
{
    OpenAPI_ecn_marking_congestion_info_status_t *ecn_marking_congestion_info_status_local_var = ogs_malloc(sizeof(OpenAPI_ecn_marking_congestion_info_status_t));
    ogs_assert(ecn_marking_congestion_info_status_local_var);

    ecn_marking_congestion_info_status_local_var->qfi = qfi;
    ecn_marking_congestion_info_status_local_var->activation_status = activation_status;

    return ecn_marking_congestion_info_status_local_var;
}

void OpenAPI_ecn_marking_congestion_info_status_free(OpenAPI_ecn_marking_congestion_info_status_t *ecn_marking_congestion_info_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecn_marking_congestion_info_status) {
        return;
    }
    ogs_free(ecn_marking_congestion_info_status);
}

cJSON *OpenAPI_ecn_marking_congestion_info_status_convertToJSON(OpenAPI_ecn_marking_congestion_info_status_t *ecn_marking_congestion_info_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecn_marking_congestion_info_status == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_convertToJSON() failed [EcnMarkingCongestionInfoStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "qfi", ecn_marking_congestion_info_status->qfi) == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_convertToJSON() failed [qfi]");
        goto end;
    }

    if (ecn_marking_congestion_info_status->activation_status == OpenAPI_activation_status_NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_convertToJSON() failed [activation_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "activationStatus", OpenAPI_activation_status_ToString(ecn_marking_congestion_info_status->activation_status)) == NULL) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_convertToJSON() failed [activation_status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ecn_marking_congestion_info_status_t *OpenAPI_ecn_marking_congestion_info_status_parseFromJSON(cJSON *ecn_marking_congestion_info_statusJSON)
{
    OpenAPI_ecn_marking_congestion_info_status_t *ecn_marking_congestion_info_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qfi = NULL;
    cJSON *activation_status = NULL;
    OpenAPI_activation_status_e activation_statusVariable = 0;
    qfi = cJSON_GetObjectItemCaseSensitive(ecn_marking_congestion_info_statusJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_parseFromJSON() failed [qfi]");
        goto end;
    }
    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_parseFromJSON() failed [qfi]");
        goto end;
    }

    activation_status = cJSON_GetObjectItemCaseSensitive(ecn_marking_congestion_info_statusJSON, "activationStatus");
    if (!activation_status) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_parseFromJSON() failed [activation_status]");
        goto end;
    }
    if (!cJSON_IsString(activation_status)) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_parseFromJSON() failed [activation_status]");
        goto end;
    }
    activation_statusVariable = OpenAPI_activation_status_FromString(activation_status->valuestring);

    ecn_marking_congestion_info_status_local_var = OpenAPI_ecn_marking_congestion_info_status_create (
        
        qfi->valuedouble,
        activation_statusVariable
    );

    return ecn_marking_congestion_info_status_local_var;
end:
    return NULL;
}

OpenAPI_ecn_marking_congestion_info_status_t *OpenAPI_ecn_marking_congestion_info_status_copy(OpenAPI_ecn_marking_congestion_info_status_t *dst, OpenAPI_ecn_marking_congestion_info_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecn_marking_congestion_info_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_status_convertToJSON() failed");
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

    OpenAPI_ecn_marking_congestion_info_status_free(dst);
    dst = OpenAPI_ecn_marking_congestion_info_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

