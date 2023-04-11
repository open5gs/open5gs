
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger_request.h"

OpenAPI_trigger_request_t *OpenAPI_trigger_request_create(
    char *supi,
    OpenAPI_pcscf_address_t *failed_pcscf
)
{
    OpenAPI_trigger_request_t *trigger_request_local_var = ogs_malloc(sizeof(OpenAPI_trigger_request_t));
    ogs_assert(trigger_request_local_var);

    trigger_request_local_var->supi = supi;
    trigger_request_local_var->failed_pcscf = failed_pcscf;

    return trigger_request_local_var;
}

void OpenAPI_trigger_request_free(OpenAPI_trigger_request_t *trigger_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trigger_request) {
        return;
    }
    if (trigger_request->supi) {
        ogs_free(trigger_request->supi);
        trigger_request->supi = NULL;
    }
    if (trigger_request->failed_pcscf) {
        OpenAPI_pcscf_address_free(trigger_request->failed_pcscf);
        trigger_request->failed_pcscf = NULL;
    }
    ogs_free(trigger_request);
}

cJSON *OpenAPI_trigger_request_convertToJSON(OpenAPI_trigger_request_t *trigger_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trigger_request == NULL) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [TriggerRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!trigger_request->supi) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", trigger_request->supi) == NULL) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [supi]");
        goto end;
    }

    if (trigger_request->failed_pcscf) {
    cJSON *failed_pcscf_local_JSON = OpenAPI_pcscf_address_convertToJSON(trigger_request->failed_pcscf);
    if (failed_pcscf_local_JSON == NULL) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [failed_pcscf]");
        goto end;
    }
    cJSON_AddItemToObject(item, "failedPcscf", failed_pcscf_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed [failed_pcscf]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_trigger_request_t *OpenAPI_trigger_request_parseFromJSON(cJSON *trigger_requestJSON)
{
    OpenAPI_trigger_request_t *trigger_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *failed_pcscf = NULL;
    OpenAPI_pcscf_address_t *failed_pcscf_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(trigger_requestJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_trigger_request_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_trigger_request_parseFromJSON() failed [supi]");
        goto end;
    }

    failed_pcscf = cJSON_GetObjectItemCaseSensitive(trigger_requestJSON, "failedPcscf");
    if (failed_pcscf) {
    failed_pcscf_local_nonprim = OpenAPI_pcscf_address_parseFromJSON(failed_pcscf);
    if (!failed_pcscf_local_nonprim) {
        ogs_error("OpenAPI_pcscf_address_parseFromJSON failed [failed_pcscf]");
        goto end;
    }
    }

    trigger_request_local_var = OpenAPI_trigger_request_create (
        ogs_strdup(supi->valuestring),
        failed_pcscf ? failed_pcscf_local_nonprim : NULL
    );

    return trigger_request_local_var;
end:
    if (failed_pcscf_local_nonprim) {
        OpenAPI_pcscf_address_free(failed_pcscf_local_nonprim);
        failed_pcscf_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_trigger_request_t *OpenAPI_trigger_request_copy(OpenAPI_trigger_request_t *dst, OpenAPI_trigger_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trigger_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trigger_request_convertToJSON() failed");
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

    OpenAPI_trigger_request_free(dst);
    dst = OpenAPI_trigger_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

