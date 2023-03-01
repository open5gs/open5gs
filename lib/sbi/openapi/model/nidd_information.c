
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nidd_information.h"

OpenAPI_nidd_information_t *OpenAPI_nidd_information_create(
    char *af_id,
    char *gpsi,
    char *ext_group_id
)
{
    OpenAPI_nidd_information_t *nidd_information_local_var = ogs_malloc(sizeof(OpenAPI_nidd_information_t));
    ogs_assert(nidd_information_local_var);

    nidd_information_local_var->af_id = af_id;
    nidd_information_local_var->gpsi = gpsi;
    nidd_information_local_var->ext_group_id = ext_group_id;

    return nidd_information_local_var;
}

void OpenAPI_nidd_information_free(OpenAPI_nidd_information_t *nidd_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nidd_information) {
        return;
    }
    if (nidd_information->af_id) {
        ogs_free(nidd_information->af_id);
        nidd_information->af_id = NULL;
    }
    if (nidd_information->gpsi) {
        ogs_free(nidd_information->gpsi);
        nidd_information->gpsi = NULL;
    }
    if (nidd_information->ext_group_id) {
        ogs_free(nidd_information->ext_group_id);
        nidd_information->ext_group_id = NULL;
    }
    ogs_free(nidd_information);
}

cJSON *OpenAPI_nidd_information_convertToJSON(OpenAPI_nidd_information_t *nidd_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nidd_information == NULL) {
        ogs_error("OpenAPI_nidd_information_convertToJSON() failed [NiddInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nidd_information->af_id) {
        ogs_error("OpenAPI_nidd_information_convertToJSON() failed [af_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afId", nidd_information->af_id) == NULL) {
        ogs_error("OpenAPI_nidd_information_convertToJSON() failed [af_id]");
        goto end;
    }

    if (nidd_information->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", nidd_information->gpsi) == NULL) {
        ogs_error("OpenAPI_nidd_information_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (nidd_information->ext_group_id) {
    if (cJSON_AddStringToObject(item, "extGroupId", nidd_information->ext_group_id) == NULL) {
        ogs_error("OpenAPI_nidd_information_convertToJSON() failed [ext_group_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nidd_information_t *OpenAPI_nidd_information_parseFromJSON(cJSON *nidd_informationJSON)
{
    OpenAPI_nidd_information_t *nidd_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_id = NULL;
    cJSON *gpsi = NULL;
    cJSON *ext_group_id = NULL;
    af_id = cJSON_GetObjectItemCaseSensitive(nidd_informationJSON, "afId");
    if (!af_id) {
        ogs_error("OpenAPI_nidd_information_parseFromJSON() failed [af_id]");
        goto end;
    }
    if (!cJSON_IsString(af_id)) {
        ogs_error("OpenAPI_nidd_information_parseFromJSON() failed [af_id]");
        goto end;
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(nidd_informationJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_nidd_information_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    ext_group_id = cJSON_GetObjectItemCaseSensitive(nidd_informationJSON, "extGroupId");
    if (ext_group_id) {
    if (!cJSON_IsString(ext_group_id) && !cJSON_IsNull(ext_group_id)) {
        ogs_error("OpenAPI_nidd_information_parseFromJSON() failed [ext_group_id]");
        goto end;
    }
    }

    nidd_information_local_var = OpenAPI_nidd_information_create (
        ogs_strdup(af_id->valuestring),
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        ext_group_id && !cJSON_IsNull(ext_group_id) ? ogs_strdup(ext_group_id->valuestring) : NULL
    );

    return nidd_information_local_var;
end:
    return NULL;
}

OpenAPI_nidd_information_t *OpenAPI_nidd_information_copy(OpenAPI_nidd_information_t *dst, OpenAPI_nidd_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nidd_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nidd_information_convertToJSON() failed");
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

    OpenAPI_nidd_information_free(dst);
    dst = OpenAPI_nidd_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

