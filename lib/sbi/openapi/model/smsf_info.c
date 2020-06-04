
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smsf_info.h"

OpenAPI_smsf_info_t *OpenAPI_smsf_info_create(
    char *smsf_instance_id,
    OpenAPI_plmn_id_t *plmn_id
    )
{
    OpenAPI_smsf_info_t *smsf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_smsf_info_t));
    if (!smsf_info_local_var) {
        return NULL;
    }
    smsf_info_local_var->smsf_instance_id = smsf_instance_id;
    smsf_info_local_var->plmn_id = plmn_id;

    return smsf_info_local_var;
}

void OpenAPI_smsf_info_free(OpenAPI_smsf_info_t *smsf_info)
{
    if (NULL == smsf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(smsf_info->smsf_instance_id);
    OpenAPI_plmn_id_free(smsf_info->plmn_id);
    ogs_free(smsf_info);
}

cJSON *OpenAPI_smsf_info_convertToJSON(OpenAPI_smsf_info_t *smsf_info)
{
    cJSON *item = NULL;

    if (smsf_info == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [SmsfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smsf_info->smsf_instance_id) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [smsf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "smsfInstanceId", smsf_info->smsf_instance_id) == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [smsf_instance_id]");
        goto end;
    }

    if (!smsf_info->plmn_id) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(smsf_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smsf_info_convertToJSON() failed [plmn_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_smsf_info_t *OpenAPI_smsf_info_parseFromJSON(cJSON *smsf_infoJSON)
{
    OpenAPI_smsf_info_t *smsf_info_local_var = NULL;
    cJSON *smsf_instance_id = cJSON_GetObjectItemCaseSensitive(smsf_infoJSON, "smsfInstanceId");
    if (!smsf_instance_id) {
        ogs_error("OpenAPI_smsf_info_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(smsf_instance_id)) {
        ogs_error("OpenAPI_smsf_info_parseFromJSON() failed [smsf_instance_id]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(smsf_infoJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_smsf_info_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;

    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    smsf_info_local_var = OpenAPI_smsf_info_create (
        ogs_strdup(smsf_instance_id->valuestring),
        plmn_id_local_nonprim
        );

    return smsf_info_local_var;
end:
    return NULL;
}

