
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ulcl_bp_information.h"

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_create(
    char *ulcl_bp_upf_id
)
{
    OpenAPI_ulcl_bp_information_t *ulcl_bp_information_local_var = ogs_malloc(sizeof(OpenAPI_ulcl_bp_information_t));
    ogs_assert(ulcl_bp_information_local_var);

    ulcl_bp_information_local_var->ulcl_bp_upf_id = ulcl_bp_upf_id;

    return ulcl_bp_information_local_var;
}

void OpenAPI_ulcl_bp_information_free(OpenAPI_ulcl_bp_information_t *ulcl_bp_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ulcl_bp_information) {
        return;
    }
    if (ulcl_bp_information->ulcl_bp_upf_id) {
        ogs_free(ulcl_bp_information->ulcl_bp_upf_id);
        ulcl_bp_information->ulcl_bp_upf_id = NULL;
    }
    ogs_free(ulcl_bp_information);
}

cJSON *OpenAPI_ulcl_bp_information_convertToJSON(OpenAPI_ulcl_bp_information_t *ulcl_bp_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ulcl_bp_information == NULL) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed [UlclBpInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ulcl_bp_information->ulcl_bp_upf_id) {
    if (cJSON_AddStringToObject(item, "ulclBpUpfId", ulcl_bp_information->ulcl_bp_upf_id) == NULL) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed [ulcl_bp_upf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_parseFromJSON(cJSON *ulcl_bp_informationJSON)
{
    OpenAPI_ulcl_bp_information_t *ulcl_bp_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ulcl_bp_upf_id = NULL;
    ulcl_bp_upf_id = cJSON_GetObjectItemCaseSensitive(ulcl_bp_informationJSON, "ulclBpUpfId");
    if (ulcl_bp_upf_id) {
    if (!cJSON_IsString(ulcl_bp_upf_id) && !cJSON_IsNull(ulcl_bp_upf_id)) {
        ogs_error("OpenAPI_ulcl_bp_information_parseFromJSON() failed [ulcl_bp_upf_id]");
        goto end;
    }
    }

    ulcl_bp_information_local_var = OpenAPI_ulcl_bp_information_create (
        ulcl_bp_upf_id && !cJSON_IsNull(ulcl_bp_upf_id) ? ogs_strdup(ulcl_bp_upf_id->valuestring) : NULL
    );

    return ulcl_bp_information_local_var;
end:
    return NULL;
}

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_copy(OpenAPI_ulcl_bp_information_t *dst, OpenAPI_ulcl_bp_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ulcl_bp_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed");
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

    OpenAPI_ulcl_bp_information_free(dst);
    dst = OpenAPI_ulcl_bp_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

