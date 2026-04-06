
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_info_1.h"

OpenAPI_amf_info_1_t *OpenAPI_amf_info_1_create(
    char *amf_instance_id,
    OpenAPI_guami_1_t *guami,
    OpenAPI_access_type_e access_type
)
{
    OpenAPI_amf_info_1_t *amf_info_1_local_var = ogs_malloc(sizeof(OpenAPI_amf_info_1_t));
    ogs_assert(amf_info_1_local_var);

    amf_info_1_local_var->amf_instance_id = amf_instance_id;
    amf_info_1_local_var->guami = guami;
    amf_info_1_local_var->access_type = access_type;

    return amf_info_1_local_var;
}

void OpenAPI_amf_info_1_free(OpenAPI_amf_info_1_t *amf_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_info_1) {
        return;
    }
    if (amf_info_1->amf_instance_id) {
        ogs_free(amf_info_1->amf_instance_id);
        amf_info_1->amf_instance_id = NULL;
    }
    if (amf_info_1->guami) {
        OpenAPI_guami_1_free(amf_info_1->guami);
        amf_info_1->guami = NULL;
    }
    ogs_free(amf_info_1);
}

cJSON *OpenAPI_amf_info_1_convertToJSON(OpenAPI_amf_info_1_t *amf_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_info_1 == NULL) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [AmfInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_info_1->amf_instance_id) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [amf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfInstanceId", amf_info_1->amf_instance_id) == NULL) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [amf_instance_id]");
        goto end;
    }

    if (!amf_info_1->guami) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [guami]");
        return NULL;
    }
    cJSON *guami_local_JSON = OpenAPI_guami_1_convertToJSON(amf_info_1->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [guami]");
        goto end;
    }

    if (amf_info_1->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(amf_info_1->access_type)) == NULL) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed [access_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_info_1_t *OpenAPI_amf_info_1_parseFromJSON(cJSON *amf_info_1JSON)
{
    OpenAPI_amf_info_1_t *amf_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_instance_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_1_t *guami_local_nonprim = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    amf_instance_id = cJSON_GetObjectItemCaseSensitive(amf_info_1JSON, "amfInstanceId");
    if (!amf_instance_id) {
        ogs_error("OpenAPI_amf_info_1_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_instance_id)) {
        ogs_error("OpenAPI_amf_info_1_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    guami = cJSON_GetObjectItemCaseSensitive(amf_info_1JSON, "guami");
    if (!guami) {
        ogs_error("OpenAPI_amf_info_1_parseFromJSON() failed [guami]");
        goto end;
    }
    guami_local_nonprim = OpenAPI_guami_1_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_1_parseFromJSON failed [guami]");
        goto end;
    }

    access_type = cJSON_GetObjectItemCaseSensitive(amf_info_1JSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_amf_info_1_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    amf_info_1_local_var = OpenAPI_amf_info_1_create (
        ogs_strdup(amf_instance_id->valuestring),
        guami_local_nonprim,
        access_type ? access_typeVariable : 0
    );

    return amf_info_1_local_var;
end:
    if (guami_local_nonprim) {
        OpenAPI_guami_1_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_amf_info_1_t *OpenAPI_amf_info_1_copy(OpenAPI_amf_info_1_t *dst, OpenAPI_amf_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_info_1_convertToJSON() failed");
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

    OpenAPI_amf_info_1_free(dst);
    dst = OpenAPI_amf_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

