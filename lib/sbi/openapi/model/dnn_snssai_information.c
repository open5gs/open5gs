
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_snssai_information.h"

OpenAPI_dnn_snssai_information_t *OpenAPI_dnn_snssai_information_create(
    char *dnn,
    OpenAPI_snssai_t *snssai
)
{
    OpenAPI_dnn_snssai_information_t *dnn_snssai_information_local_var = ogs_malloc(sizeof(OpenAPI_dnn_snssai_information_t));
    ogs_assert(dnn_snssai_information_local_var);

    dnn_snssai_information_local_var->dnn = dnn;
    dnn_snssai_information_local_var->snssai = snssai;

    return dnn_snssai_information_local_var;
}

void OpenAPI_dnn_snssai_information_free(OpenAPI_dnn_snssai_information_t *dnn_snssai_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_snssai_information) {
        return;
    }
    if (dnn_snssai_information->dnn) {
        ogs_free(dnn_snssai_information->dnn);
        dnn_snssai_information->dnn = NULL;
    }
    if (dnn_snssai_information->snssai) {
        OpenAPI_snssai_free(dnn_snssai_information->snssai);
        dnn_snssai_information->snssai = NULL;
    }
    ogs_free(dnn_snssai_information);
}

cJSON *OpenAPI_dnn_snssai_information_convertToJSON(OpenAPI_dnn_snssai_information_t *dnn_snssai_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_snssai_information == NULL) {
        ogs_error("OpenAPI_dnn_snssai_information_convertToJSON() failed [DnnSnssaiInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dnn_snssai_information->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", dnn_snssai_information->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_snssai_information_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (dnn_snssai_information->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(dnn_snssai_information->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_snssai_information_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_snssai_information_convertToJSON() failed [snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dnn_snssai_information_t *OpenAPI_dnn_snssai_information_parseFromJSON(cJSON *dnn_snssai_informationJSON)
{
    OpenAPI_dnn_snssai_information_t *dnn_snssai_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_snssai_informationJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_dnn_snssai_information_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(dnn_snssai_informationJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    dnn_snssai_information_local_var = OpenAPI_dnn_snssai_information_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL
    );

    return dnn_snssai_information_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_dnn_snssai_information_t *OpenAPI_dnn_snssai_information_copy(OpenAPI_dnn_snssai_information_t *dst, OpenAPI_dnn_snssai_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_snssai_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_snssai_information_convertToJSON() failed");
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

    OpenAPI_dnn_snssai_information_free(dst);
    dst = OpenAPI_dnn_snssai_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

