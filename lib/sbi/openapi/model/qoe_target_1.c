
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qoe_target_1.h"

OpenAPI_qoe_target_1_t *OpenAPI_qoe_target_1_create(
    char *supi,
    char *imsi
)
{
    OpenAPI_qoe_target_1_t *qoe_target_1_local_var = ogs_malloc(sizeof(OpenAPI_qoe_target_1_t));
    ogs_assert(qoe_target_1_local_var);

    qoe_target_1_local_var->supi = supi;
    qoe_target_1_local_var->imsi = imsi;

    return qoe_target_1_local_var;
}

void OpenAPI_qoe_target_1_free(OpenAPI_qoe_target_1_t *qoe_target_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qoe_target_1) {
        return;
    }
    if (qoe_target_1->supi) {
        ogs_free(qoe_target_1->supi);
        qoe_target_1->supi = NULL;
    }
    if (qoe_target_1->imsi) {
        ogs_free(qoe_target_1->imsi);
        qoe_target_1->imsi = NULL;
    }
    ogs_free(qoe_target_1);
}

cJSON *OpenAPI_qoe_target_1_convertToJSON(OpenAPI_qoe_target_1_t *qoe_target_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qoe_target_1 == NULL) {
        ogs_error("OpenAPI_qoe_target_1_convertToJSON() failed [QoeTarget_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qoe_target_1->supi) {
    if (cJSON_AddStringToObject(item, "supi", qoe_target_1->supi) == NULL) {
        ogs_error("OpenAPI_qoe_target_1_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (qoe_target_1->imsi) {
    if (cJSON_AddStringToObject(item, "imsi", qoe_target_1->imsi) == NULL) {
        ogs_error("OpenAPI_qoe_target_1_convertToJSON() failed [imsi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qoe_target_1_t *OpenAPI_qoe_target_1_parseFromJSON(cJSON *qoe_target_1JSON)
{
    OpenAPI_qoe_target_1_t *qoe_target_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *imsi = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(qoe_target_1JSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_qoe_target_1_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    imsi = cJSON_GetObjectItemCaseSensitive(qoe_target_1JSON, "imsi");
    if (imsi) {
    if (!cJSON_IsString(imsi) && !cJSON_IsNull(imsi)) {
        ogs_error("OpenAPI_qoe_target_1_parseFromJSON() failed [imsi]");
        goto end;
    }
    }

    qoe_target_1_local_var = OpenAPI_qoe_target_1_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        imsi && !cJSON_IsNull(imsi) ? ogs_strdup(imsi->valuestring) : NULL
    );

    return qoe_target_1_local_var;
end:
    return NULL;
}

OpenAPI_qoe_target_1_t *OpenAPI_qoe_target_1_copy(OpenAPI_qoe_target_1_t *dst, OpenAPI_qoe_target_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qoe_target_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qoe_target_1_convertToJSON() failed");
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

    OpenAPI_qoe_target_1_free(dst);
    dst = OpenAPI_qoe_target_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

