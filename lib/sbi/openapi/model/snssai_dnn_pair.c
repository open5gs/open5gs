
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_dnn_pair.h"

OpenAPI_snssai_dnn_pair_t *OpenAPI_snssai_dnn_pair_create(
    char *dnn,
    OpenAPI_snssai_t *snssai
)
{
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pair_local_var = ogs_malloc(sizeof(OpenAPI_snssai_dnn_pair_t));
    ogs_assert(snssai_dnn_pair_local_var);

    snssai_dnn_pair_local_var->dnn = dnn;
    snssai_dnn_pair_local_var->snssai = snssai;

    return snssai_dnn_pair_local_var;
}

void OpenAPI_snssai_dnn_pair_free(OpenAPI_snssai_dnn_pair_t *snssai_dnn_pair)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snssai_dnn_pair) {
        return;
    }
    if (snssai_dnn_pair->dnn) {
        ogs_free(snssai_dnn_pair->dnn);
        snssai_dnn_pair->dnn = NULL;
    }
    if (snssai_dnn_pair->snssai) {
        OpenAPI_snssai_free(snssai_dnn_pair->snssai);
        snssai_dnn_pair->snssai = NULL;
    }
    ogs_free(snssai_dnn_pair);
}

cJSON *OpenAPI_snssai_dnn_pair_convertToJSON(OpenAPI_snssai_dnn_pair_t *snssai_dnn_pair)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snssai_dnn_pair == NULL) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed [SnssaiDnnPair]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!snssai_dnn_pair->dnn) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", snssai_dnn_pair->dnn) == NULL) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!snssai_dnn_pair->snssai) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(snssai_dnn_pair->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed [snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_snssai_dnn_pair_t *OpenAPI_snssai_dnn_pair_parseFromJSON(cJSON *snssai_dnn_pairJSON)
{
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pair_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(snssai_dnn_pairJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_snssai_dnn_pair_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_snssai_dnn_pair_parseFromJSON() failed [dnn]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(snssai_dnn_pairJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_snssai_dnn_pair_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    snssai_dnn_pair_local_var = OpenAPI_snssai_dnn_pair_create (
        ogs_strdup(dnn->valuestring),
        snssai_local_nonprim
    );

    return snssai_dnn_pair_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_snssai_dnn_pair_t *OpenAPI_snssai_dnn_pair_copy(OpenAPI_snssai_dnn_pair_t *dst, OpenAPI_snssai_dnn_pair_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_dnn_pair_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_dnn_pair_convertToJSON() failed");
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

    OpenAPI_snssai_dnn_pair_free(dst);
    dst = OpenAPI_snssai_dnn_pair_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

