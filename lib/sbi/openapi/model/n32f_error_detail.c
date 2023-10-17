
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n32f_error_detail.h"

OpenAPI_n32f_error_detail_t *OpenAPI_n32f_error_detail_create(
    char *attribute,
    OpenAPI_failure_reason_e msg_reconstruct_fail_reason
)
{
    OpenAPI_n32f_error_detail_t *n32f_error_detail_local_var = ogs_malloc(sizeof(OpenAPI_n32f_error_detail_t));
    ogs_assert(n32f_error_detail_local_var);

    n32f_error_detail_local_var->attribute = attribute;
    n32f_error_detail_local_var->msg_reconstruct_fail_reason = msg_reconstruct_fail_reason;

    return n32f_error_detail_local_var;
}

void OpenAPI_n32f_error_detail_free(OpenAPI_n32f_error_detail_t *n32f_error_detail)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n32f_error_detail) {
        return;
    }
    if (n32f_error_detail->attribute) {
        ogs_free(n32f_error_detail->attribute);
        n32f_error_detail->attribute = NULL;
    }
    ogs_free(n32f_error_detail);
}

cJSON *OpenAPI_n32f_error_detail_convertToJSON(OpenAPI_n32f_error_detail_t *n32f_error_detail)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n32f_error_detail == NULL) {
        ogs_error("OpenAPI_n32f_error_detail_convertToJSON() failed [N32fErrorDetail]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n32f_error_detail->attribute) {
        ogs_error("OpenAPI_n32f_error_detail_convertToJSON() failed [attribute]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "attribute", n32f_error_detail->attribute) == NULL) {
        ogs_error("OpenAPI_n32f_error_detail_convertToJSON() failed [attribute]");
        goto end;
    }

    if (n32f_error_detail->msg_reconstruct_fail_reason == OpenAPI_failure_reason_NULL) {
        ogs_error("OpenAPI_n32f_error_detail_convertToJSON() failed [msg_reconstruct_fail_reason]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "msgReconstructFailReason", OpenAPI_failure_reason_ToString(n32f_error_detail->msg_reconstruct_fail_reason)) == NULL) {
        ogs_error("OpenAPI_n32f_error_detail_convertToJSON() failed [msg_reconstruct_fail_reason]");
        goto end;
    }

end:
    return item;
}

OpenAPI_n32f_error_detail_t *OpenAPI_n32f_error_detail_parseFromJSON(cJSON *n32f_error_detailJSON)
{
    OpenAPI_n32f_error_detail_t *n32f_error_detail_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *attribute = NULL;
    cJSON *msg_reconstruct_fail_reason = NULL;
    OpenAPI_failure_reason_e msg_reconstruct_fail_reasonVariable = 0;
    attribute = cJSON_GetObjectItemCaseSensitive(n32f_error_detailJSON, "attribute");
    if (!attribute) {
        ogs_error("OpenAPI_n32f_error_detail_parseFromJSON() failed [attribute]");
        goto end;
    }
    if (!cJSON_IsString(attribute)) {
        ogs_error("OpenAPI_n32f_error_detail_parseFromJSON() failed [attribute]");
        goto end;
    }

    msg_reconstruct_fail_reason = cJSON_GetObjectItemCaseSensitive(n32f_error_detailJSON, "msgReconstructFailReason");
    if (!msg_reconstruct_fail_reason) {
        ogs_error("OpenAPI_n32f_error_detail_parseFromJSON() failed [msg_reconstruct_fail_reason]");
        goto end;
    }
    if (!cJSON_IsString(msg_reconstruct_fail_reason)) {
        ogs_error("OpenAPI_n32f_error_detail_parseFromJSON() failed [msg_reconstruct_fail_reason]");
        goto end;
    }
    msg_reconstruct_fail_reasonVariable = OpenAPI_failure_reason_FromString(msg_reconstruct_fail_reason->valuestring);

    n32f_error_detail_local_var = OpenAPI_n32f_error_detail_create (
        ogs_strdup(attribute->valuestring),
        msg_reconstruct_fail_reasonVariable
    );

    return n32f_error_detail_local_var;
end:
    return NULL;
}

OpenAPI_n32f_error_detail_t *OpenAPI_n32f_error_detail_copy(OpenAPI_n32f_error_detail_t *dst, OpenAPI_n32f_error_detail_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n32f_error_detail_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n32f_error_detail_convertToJSON() failed");
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

    OpenAPI_n32f_error_detail_free(dst);
    dst = OpenAPI_n32f_error_detail_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

