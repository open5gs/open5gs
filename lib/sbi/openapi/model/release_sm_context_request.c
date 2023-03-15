
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "release_sm_context_request.h"

OpenAPI_release_sm_context_request_t *OpenAPI_release_sm_context_request_create(
    OpenAPI_sm_context_release_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_sm_information
)
{
    OpenAPI_release_sm_context_request_t *release_sm_context_request_local_var = ogs_malloc(sizeof(OpenAPI_release_sm_context_request_t));
    ogs_assert(release_sm_context_request_local_var);

    release_sm_context_request_local_var->json_data = json_data;
    release_sm_context_request_local_var->binary_data_n2_sm_information = binary_data_n2_sm_information;

    return release_sm_context_request_local_var;
}

void OpenAPI_release_sm_context_request_free(OpenAPI_release_sm_context_request_t *release_sm_context_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == release_sm_context_request) {
        return;
    }
    if (release_sm_context_request->json_data) {
        OpenAPI_sm_context_release_data_free(release_sm_context_request->json_data);
        release_sm_context_request->json_data = NULL;
    }
    if (release_sm_context_request->binary_data_n2_sm_information) {
        ogs_free(release_sm_context_request->binary_data_n2_sm_information->data);
        release_sm_context_request->binary_data_n2_sm_information = NULL;
    }
    ogs_free(release_sm_context_request);
}

cJSON *OpenAPI_release_sm_context_request_convertToJSON(OpenAPI_release_sm_context_request_t *release_sm_context_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (release_sm_context_request == NULL) {
        ogs_error("OpenAPI_release_sm_context_request_convertToJSON() failed [ReleaseSmContext_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (release_sm_context_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_sm_context_release_data_convertToJSON(release_sm_context_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_release_sm_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_release_sm_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (release_sm_context_request->binary_data_n2_sm_information) {
    char* encoded_str_binary_data_n2_sm_information = OpenAPI_base64encode(release_sm_context_request->binary_data_n2_sm_information->data,release_sm_context_request->binary_data_n2_sm_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2SmInformation", encoded_str_binary_data_n2_sm_information) == NULL) {
        ogs_error("OpenAPI_release_sm_context_request_convertToJSON() failed [binary_data_n2_sm_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_sm_information);
    }

end:
    return item;
}

OpenAPI_release_sm_context_request_t *OpenAPI_release_sm_context_request_parseFromJSON(cJSON *release_sm_context_requestJSON)
{
    OpenAPI_release_sm_context_request_t *release_sm_context_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_sm_context_release_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n2_sm_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_sm_information = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(release_sm_context_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_sm_context_release_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n2_sm_information = cJSON_GetObjectItemCaseSensitive(release_sm_context_requestJSON, "binaryDataN2SmInformation");
    if (binary_data_n2_sm_information) {
    decoded_str_binary_data_n2_sm_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_sm_information);
    if (!cJSON_IsString(binary_data_n2_sm_information)) {
        ogs_error("OpenAPI_release_sm_context_request_parseFromJSON() failed [binary_data_n2_sm_information]");
        goto end;
    }
    decoded_str_binary_data_n2_sm_information->data = OpenAPI_base64decode(binary_data_n2_sm_information->valuestring, strlen(binary_data_n2_sm_information->valuestring), &decoded_str_binary_data_n2_sm_information->len);
    if (!decoded_str_binary_data_n2_sm_information->data) {
        ogs_error("OpenAPI_release_sm_context_request_parseFromJSON() failed [binary_data_n2_sm_information]");
        goto end;
    }
    }

    release_sm_context_request_local_var = OpenAPI_release_sm_context_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n2_sm_information ? decoded_str_binary_data_n2_sm_information : NULL
    );

    return release_sm_context_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_sm_context_release_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_release_sm_context_request_t *OpenAPI_release_sm_context_request_copy(OpenAPI_release_sm_context_request_t *dst, OpenAPI_release_sm_context_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_release_sm_context_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_release_sm_context_request_convertToJSON() failed");
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

    OpenAPI_release_sm_context_request_free(dst);
    dst = OpenAPI_release_sm_context_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

