
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_comm_failure_filter.h"

OpenAPI_sm_comm_failure_filter_t *OpenAPI_sm_comm_failure_filter_create(
    char *dnn,
    OpenAPI_snssai_t *snssai
)
{
    OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter_local_var = ogs_malloc(sizeof(OpenAPI_sm_comm_failure_filter_t));
    ogs_assert(sm_comm_failure_filter_local_var);

    sm_comm_failure_filter_local_var->dnn = dnn;
    sm_comm_failure_filter_local_var->snssai = snssai;

    return sm_comm_failure_filter_local_var;
}

void OpenAPI_sm_comm_failure_filter_free(OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_comm_failure_filter) {
        return;
    }
    if (sm_comm_failure_filter->dnn) {
        ogs_free(sm_comm_failure_filter->dnn);
        sm_comm_failure_filter->dnn = NULL;
    }
    if (sm_comm_failure_filter->snssai) {
        OpenAPI_snssai_free(sm_comm_failure_filter->snssai);
        sm_comm_failure_filter->snssai = NULL;
    }
    ogs_free(sm_comm_failure_filter);
}

cJSON *OpenAPI_sm_comm_failure_filter_convertToJSON(OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_comm_failure_filter == NULL) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed [SmCommFailureFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_comm_failure_filter->dnn) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", sm_comm_failure_filter->dnn) == NULL) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!sm_comm_failure_filter->snssai) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_comm_failure_filter->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed [snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sm_comm_failure_filter_t *OpenAPI_sm_comm_failure_filter_parseFromJSON(cJSON *sm_comm_failure_filterJSON)
{
    OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(sm_comm_failure_filterJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_comm_failure_filter_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_comm_failure_filter_parseFromJSON() failed [dnn]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(sm_comm_failure_filterJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_sm_comm_failure_filter_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    sm_comm_failure_filter_local_var = OpenAPI_sm_comm_failure_filter_create (
        ogs_strdup(dnn->valuestring),
        snssai_local_nonprim
    );

    return sm_comm_failure_filter_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_comm_failure_filter_t *OpenAPI_sm_comm_failure_filter_copy(OpenAPI_sm_comm_failure_filter_t *dst, OpenAPI_sm_comm_failure_filter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_comm_failure_filter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_comm_failure_filter_convertToJSON() failed");
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

    OpenAPI_sm_comm_failure_filter_free(dst);
    dst = OpenAPI_sm_comm_failure_filter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

