
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smsf_info_1.h"

OpenAPI_smsf_info_1_t *OpenAPI_smsf_info_1_create(
    char *smsf_instance_id,
    OpenAPI_plmn_id_1_t *plmn_id
)
{
    OpenAPI_smsf_info_1_t *smsf_info_1_local_var = ogs_malloc(sizeof(OpenAPI_smsf_info_1_t));
    ogs_assert(smsf_info_1_local_var);

    smsf_info_1_local_var->smsf_instance_id = smsf_instance_id;
    smsf_info_1_local_var->plmn_id = plmn_id;

    return smsf_info_1_local_var;
}

void OpenAPI_smsf_info_1_free(OpenAPI_smsf_info_1_t *smsf_info_1)
{
    if (NULL == smsf_info_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(smsf_info_1->smsf_instance_id);
    OpenAPI_plmn_id_1_free(smsf_info_1->plmn_id);
    ogs_free(smsf_info_1);
}

cJSON *OpenAPI_smsf_info_1_convertToJSON(OpenAPI_smsf_info_1_t *smsf_info_1)
{
    cJSON *item = NULL;

    if (smsf_info_1 == NULL) {
        ogs_error("OpenAPI_smsf_info_1_convertToJSON() failed [SmsfInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "smsfInstanceId", smsf_info_1->smsf_instance_id) == NULL) {
        ogs_error("OpenAPI_smsf_info_1_convertToJSON() failed [smsf_instance_id]");
        goto end;
    }

    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(smsf_info_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_smsf_info_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smsf_info_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_smsf_info_1_t *OpenAPI_smsf_info_1_parseFromJSON(cJSON *smsf_info_1JSON)
{
    OpenAPI_smsf_info_1_t *smsf_info_1_local_var = NULL;
    cJSON *smsf_instance_id = cJSON_GetObjectItemCaseSensitive(smsf_info_1JSON, "smsfInstanceId");
    if (!smsf_instance_id) {
        ogs_error("OpenAPI_smsf_info_1_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(smsf_instance_id)) {
        ogs_error("OpenAPI_smsf_info_1_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(smsf_info_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_smsf_info_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);

    smsf_info_1_local_var = OpenAPI_smsf_info_1_create (
        ogs_strdup(smsf_instance_id->valuestring),
        plmn_id_local_nonprim
    );

    return smsf_info_1_local_var;
end:
    return NULL;
}

OpenAPI_smsf_info_1_t *OpenAPI_smsf_info_1_copy(OpenAPI_smsf_info_1_t *dst, OpenAPI_smsf_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smsf_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smsf_info_1_convertToJSON() failed");
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

    OpenAPI_smsf_info_1_free(dst);
    dst = OpenAPI_smsf_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

