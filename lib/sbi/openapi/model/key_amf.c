
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "key_amf.h"

OpenAPI_key_amf_t *OpenAPI_key_amf_create(
    OpenAPI_key_amf_type_e key_type,
    char *key_val
)
{
    OpenAPI_key_amf_t *key_amf_local_var = ogs_malloc(sizeof(OpenAPI_key_amf_t));
    ogs_assert(key_amf_local_var);

    key_amf_local_var->key_type = key_type;
    key_amf_local_var->key_val = key_val;

    return key_amf_local_var;
}

void OpenAPI_key_amf_free(OpenAPI_key_amf_t *key_amf)
{
    if (NULL == key_amf) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(key_amf->key_val);
    ogs_free(key_amf);
}

cJSON *OpenAPI_key_amf_convertToJSON(OpenAPI_key_amf_t *key_amf)
{
    cJSON *item = NULL;

    if (key_amf == NULL) {
        ogs_error("OpenAPI_key_amf_convertToJSON() failed [KeyAmf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "keyType", OpenAPI_key_amf_type_ToString(key_amf->key_type)) == NULL) {
        ogs_error("OpenAPI_key_amf_convertToJSON() failed [key_type]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "keyVal", key_amf->key_val) == NULL) {
        ogs_error("OpenAPI_key_amf_convertToJSON() failed [key_val]");
        goto end;
    }

end:
    return item;
}

OpenAPI_key_amf_t *OpenAPI_key_amf_parseFromJSON(cJSON *key_amfJSON)
{
    OpenAPI_key_amf_t *key_amf_local_var = NULL;
    cJSON *key_type = cJSON_GetObjectItemCaseSensitive(key_amfJSON, "keyType");
    if (!key_type) {
        ogs_error("OpenAPI_key_amf_parseFromJSON() failed [key_type]");
        goto end;
    }

    OpenAPI_key_amf_type_e key_typeVariable;
    if (!cJSON_IsString(key_type)) {
        ogs_error("OpenAPI_key_amf_parseFromJSON() failed [key_type]");
        goto end;
    }
    key_typeVariable = OpenAPI_key_amf_type_FromString(key_type->valuestring);

    cJSON *key_val = cJSON_GetObjectItemCaseSensitive(key_amfJSON, "keyVal");
    if (!key_val) {
        ogs_error("OpenAPI_key_amf_parseFromJSON() failed [key_val]");
        goto end;
    }

    if (!cJSON_IsString(key_val)) {
        ogs_error("OpenAPI_key_amf_parseFromJSON() failed [key_val]");
        goto end;
    }

    key_amf_local_var = OpenAPI_key_amf_create (
        key_typeVariable,
        ogs_strdup(key_val->valuestring)
    );

    return key_amf_local_var;
end:
    return NULL;
}

OpenAPI_key_amf_t *OpenAPI_key_amf_copy(OpenAPI_key_amf_t *dst, OpenAPI_key_amf_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_key_amf_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_key_amf_convertToJSON() failed");
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

    OpenAPI_key_amf_free(dst);
    dst = OpenAPI_key_amf_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

