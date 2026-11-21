
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serving_network_function_id.h"

OpenAPI_serving_network_function_id_t *OpenAPI_serving_network_function_id_create(
    OpenAPI_nf_identification_t *serving_network_function_information,
    char *a_mfid
)
{
    OpenAPI_serving_network_function_id_t *serving_network_function_id_local_var = ogs_malloc(sizeof(OpenAPI_serving_network_function_id_t));
    ogs_assert(serving_network_function_id_local_var);

    serving_network_function_id_local_var->serving_network_function_information = serving_network_function_information;
    serving_network_function_id_local_var->a_mfid = a_mfid;

    return serving_network_function_id_local_var;
}

void OpenAPI_serving_network_function_id_free(OpenAPI_serving_network_function_id_t *serving_network_function_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == serving_network_function_id) {
        return;
    }
    if (serving_network_function_id->serving_network_function_information) {
        OpenAPI_nf_identification_free(serving_network_function_id->serving_network_function_information);
        serving_network_function_id->serving_network_function_information = NULL;
    }
    if (serving_network_function_id->a_mfid) {
        ogs_free(serving_network_function_id->a_mfid);
        serving_network_function_id->a_mfid = NULL;
    }
    ogs_free(serving_network_function_id);
}

cJSON *OpenAPI_serving_network_function_id_convertToJSON(OpenAPI_serving_network_function_id_t *serving_network_function_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (serving_network_function_id == NULL) {
        ogs_error("OpenAPI_serving_network_function_id_convertToJSON() failed [ServingNetworkFunctionID]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!serving_network_function_id->serving_network_function_information) {
        ogs_error("OpenAPI_serving_network_function_id_convertToJSON() failed [serving_network_function_information]");
        return NULL;
    }
    cJSON *serving_network_function_information_local_JSON = OpenAPI_nf_identification_convertToJSON(serving_network_function_id->serving_network_function_information);
    if (serving_network_function_information_local_JSON == NULL) {
        ogs_error("OpenAPI_serving_network_function_id_convertToJSON() failed [serving_network_function_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetworkFunctionInformation", serving_network_function_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_serving_network_function_id_convertToJSON() failed [serving_network_function_information]");
        goto end;
    }

    if (serving_network_function_id->a_mfid) {
    if (cJSON_AddStringToObject(item, "aMFId", serving_network_function_id->a_mfid) == NULL) {
        ogs_error("OpenAPI_serving_network_function_id_convertToJSON() failed [a_mfid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_serving_network_function_id_t *OpenAPI_serving_network_function_id_parseFromJSON(cJSON *serving_network_function_idJSON)
{
    OpenAPI_serving_network_function_id_t *serving_network_function_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_network_function_information = NULL;
    OpenAPI_nf_identification_t *serving_network_function_information_local_nonprim = NULL;
    cJSON *a_mfid = NULL;
    serving_network_function_information = cJSON_GetObjectItemCaseSensitive(serving_network_function_idJSON, "servingNetworkFunctionInformation");
    if (!serving_network_function_information) {
        ogs_error("OpenAPI_serving_network_function_id_parseFromJSON() failed [serving_network_function_information]");
        goto end;
    }
    serving_network_function_information_local_nonprim = OpenAPI_nf_identification_parseFromJSON(serving_network_function_information);
    if (!serving_network_function_information_local_nonprim) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON failed [serving_network_function_information]");
        goto end;
    }

    a_mfid = cJSON_GetObjectItemCaseSensitive(serving_network_function_idJSON, "aMFId");
    if (a_mfid) {
    if (!cJSON_IsString(a_mfid) && !cJSON_IsNull(a_mfid)) {
        ogs_error("OpenAPI_serving_network_function_id_parseFromJSON() failed [a_mfid]");
        goto end;
    }
    }

    serving_network_function_id_local_var = OpenAPI_serving_network_function_id_create (
        serving_network_function_information_local_nonprim,
        a_mfid && !cJSON_IsNull(a_mfid) ? ogs_strdup(a_mfid->valuestring) : NULL
    );

    return serving_network_function_id_local_var;
end:
    if (serving_network_function_information_local_nonprim) {
        OpenAPI_nf_identification_free(serving_network_function_information_local_nonprim);
        serving_network_function_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_serving_network_function_id_t *OpenAPI_serving_network_function_id_copy(OpenAPI_serving_network_function_id_t *dst, OpenAPI_serving_network_function_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_serving_network_function_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_serving_network_function_id_convertToJSON() failed");
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

    OpenAPI_serving_network_function_id_free(dst);
    dst = OpenAPI_serving_network_function_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

