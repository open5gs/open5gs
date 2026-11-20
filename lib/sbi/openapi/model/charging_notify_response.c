
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_notify_response.h"

OpenAPI_charging_notify_response_t *OpenAPI_charging_notify_response_create(
    OpenAPI_invocation_result_t *invocation_result
)
{
    OpenAPI_charging_notify_response_t *charging_notify_response_local_var = ogs_malloc(sizeof(OpenAPI_charging_notify_response_t));
    ogs_assert(charging_notify_response_local_var);

    charging_notify_response_local_var->invocation_result = invocation_result;

    return charging_notify_response_local_var;
}

void OpenAPI_charging_notify_response_free(OpenAPI_charging_notify_response_t *charging_notify_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_notify_response) {
        return;
    }
    if (charging_notify_response->invocation_result) {
        OpenAPI_invocation_result_free(charging_notify_response->invocation_result);
        charging_notify_response->invocation_result = NULL;
    }
    ogs_free(charging_notify_response);
}

cJSON *OpenAPI_charging_notify_response_convertToJSON(OpenAPI_charging_notify_response_t *charging_notify_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_notify_response == NULL) {
        ogs_error("OpenAPI_charging_notify_response_convertToJSON() failed [ChargingNotifyResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_notify_response->invocation_result) {
    cJSON *invocation_result_local_JSON = OpenAPI_invocation_result_convertToJSON(charging_notify_response->invocation_result);
    if (invocation_result_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_notify_response_convertToJSON() failed [invocation_result]");
        goto end;
    }
    cJSON_AddItemToObject(item, "invocationResult", invocation_result_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_notify_response_convertToJSON() failed [invocation_result]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_notify_response_t *OpenAPI_charging_notify_response_parseFromJSON(cJSON *charging_notify_responseJSON)
{
    OpenAPI_charging_notify_response_t *charging_notify_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *invocation_result = NULL;
    OpenAPI_invocation_result_t *invocation_result_local_nonprim = NULL;
    invocation_result = cJSON_GetObjectItemCaseSensitive(charging_notify_responseJSON, "invocationResult");
    if (invocation_result) {
    invocation_result_local_nonprim = OpenAPI_invocation_result_parseFromJSON(invocation_result);
    if (!invocation_result_local_nonprim) {
        ogs_error("OpenAPI_invocation_result_parseFromJSON failed [invocation_result]");
        goto end;
    }
    }

    charging_notify_response_local_var = OpenAPI_charging_notify_response_create (
        invocation_result ? invocation_result_local_nonprim : NULL
    );

    return charging_notify_response_local_var;
end:
    if (invocation_result_local_nonprim) {
        OpenAPI_invocation_result_free(invocation_result_local_nonprim);
        invocation_result_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_charging_notify_response_t *OpenAPI_charging_notify_response_copy(OpenAPI_charging_notify_response_t *dst, OpenAPI_charging_notify_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_notify_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_notify_response_convertToJSON() failed");
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

    OpenAPI_charging_notify_response_free(dst);
    dst = OpenAPI_charging_notify_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

