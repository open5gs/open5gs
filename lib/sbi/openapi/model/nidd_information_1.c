
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nidd_information_1.h"

OpenAPI_nidd_information_1_t *OpenAPI_nidd_information_1_create(
    char *af_id,
    char *gpsi,
    char *ext_group_id
)
{
    OpenAPI_nidd_information_1_t *nidd_information_1_local_var = ogs_malloc(sizeof(OpenAPI_nidd_information_1_t));
    ogs_assert(nidd_information_1_local_var);

    nidd_information_1_local_var->af_id = af_id;
    nidd_information_1_local_var->gpsi = gpsi;
    nidd_information_1_local_var->ext_group_id = ext_group_id;

    return nidd_information_1_local_var;
}

void OpenAPI_nidd_information_1_free(OpenAPI_nidd_information_1_t *nidd_information_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nidd_information_1) {
        return;
    }
    if (nidd_information_1->af_id) {
        ogs_free(nidd_information_1->af_id);
        nidd_information_1->af_id = NULL;
    }
    if (nidd_information_1->gpsi) {
        ogs_free(nidd_information_1->gpsi);
        nidd_information_1->gpsi = NULL;
    }
    if (nidd_information_1->ext_group_id) {
        ogs_free(nidd_information_1->ext_group_id);
        nidd_information_1->ext_group_id = NULL;
    }
    ogs_free(nidd_information_1);
}

cJSON *OpenAPI_nidd_information_1_convertToJSON(OpenAPI_nidd_information_1_t *nidd_information_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nidd_information_1 == NULL) {
        ogs_error("OpenAPI_nidd_information_1_convertToJSON() failed [NiddInformation_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nidd_information_1->af_id) {
        ogs_error("OpenAPI_nidd_information_1_convertToJSON() failed [af_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afId", nidd_information_1->af_id) == NULL) {
        ogs_error("OpenAPI_nidd_information_1_convertToJSON() failed [af_id]");
        goto end;
    }

    if (nidd_information_1->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", nidd_information_1->gpsi) == NULL) {
        ogs_error("OpenAPI_nidd_information_1_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (nidd_information_1->ext_group_id) {
    if (cJSON_AddStringToObject(item, "extGroupId", nidd_information_1->ext_group_id) == NULL) {
        ogs_error("OpenAPI_nidd_information_1_convertToJSON() failed [ext_group_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nidd_information_1_t *OpenAPI_nidd_information_1_parseFromJSON(cJSON *nidd_information_1JSON)
{
    OpenAPI_nidd_information_1_t *nidd_information_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_id = NULL;
    cJSON *gpsi = NULL;
    cJSON *ext_group_id = NULL;
    af_id = cJSON_GetObjectItemCaseSensitive(nidd_information_1JSON, "afId");
    if (!af_id) {
        ogs_error("OpenAPI_nidd_information_1_parseFromJSON() failed [af_id]");
        goto end;
    }
    if (!cJSON_IsString(af_id)) {
        ogs_error("OpenAPI_nidd_information_1_parseFromJSON() failed [af_id]");
        goto end;
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(nidd_information_1JSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_nidd_information_1_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    ext_group_id = cJSON_GetObjectItemCaseSensitive(nidd_information_1JSON, "extGroupId");
    if (ext_group_id) {
    if (!cJSON_IsString(ext_group_id) && !cJSON_IsNull(ext_group_id)) {
        ogs_error("OpenAPI_nidd_information_1_parseFromJSON() failed [ext_group_id]");
        goto end;
    }
    }

    nidd_information_1_local_var = OpenAPI_nidd_information_1_create (
        ogs_strdup(af_id->valuestring),
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        ext_group_id && !cJSON_IsNull(ext_group_id) ? ogs_strdup(ext_group_id->valuestring) : NULL
    );

    return nidd_information_1_local_var;
end:
    return NULL;
}

OpenAPI_nidd_information_1_t *OpenAPI_nidd_information_1_copy(OpenAPI_nidd_information_1_t *dst, OpenAPI_nidd_information_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nidd_information_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nidd_information_1_convertToJSON() failed");
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

    OpenAPI_nidd_information_1_free(dst);
    dst = OpenAPI_nidd_information_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

