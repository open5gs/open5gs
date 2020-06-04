
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_registration_info.h"

OpenAPI_smf_registration_info_t *OpenAPI_smf_registration_info_create(
    OpenAPI_list_t *smf_registration_list
    )
{
    OpenAPI_smf_registration_info_t *smf_registration_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_smf_registration_info_t));
    if (!smf_registration_info_local_var) {
        return NULL;
    }
    smf_registration_info_local_var->smf_registration_list = smf_registration_list;

    return smf_registration_info_local_var;
}

void OpenAPI_smf_registration_info_free(OpenAPI_smf_registration_info_t *smf_registration_info)
{
    if (NULL == smf_registration_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(smf_registration_info->smf_registration_list, node) {
        OpenAPI_smf_registration_free(node->data);
    }
    OpenAPI_list_free(smf_registration_info->smf_registration_list);
    ogs_free(smf_registration_info);
}

cJSON *OpenAPI_smf_registration_info_convertToJSON(OpenAPI_smf_registration_info_t *smf_registration_info)
{
    cJSON *item = NULL;

    if (smf_registration_info == NULL) {
        ogs_error("OpenAPI_smf_registration_info_convertToJSON() failed [SmfRegistrationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_registration_info->smf_registration_list) {
        ogs_error("OpenAPI_smf_registration_info_convertToJSON() failed [smf_registration_list]");
        goto end;
    }
    cJSON *smf_registration_listList = cJSON_AddArrayToObject(item, "smfRegistrationList");
    if (smf_registration_listList == NULL) {
        ogs_error("OpenAPI_smf_registration_info_convertToJSON() failed [smf_registration_list]");
        goto end;
    }

    OpenAPI_lnode_t *smf_registration_list_node;
    if (smf_registration_info->smf_registration_list) {
        OpenAPI_list_for_each(smf_registration_info->smf_registration_list, smf_registration_list_node) {
            cJSON *itemLocal = OpenAPI_smf_registration_convertToJSON(smf_registration_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_smf_registration_info_convertToJSON() failed [smf_registration_list]");
                goto end;
            }
            cJSON_AddItemToArray(smf_registration_listList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_smf_registration_info_t *OpenAPI_smf_registration_info_parseFromJSON(cJSON *smf_registration_infoJSON)
{
    OpenAPI_smf_registration_info_t *smf_registration_info_local_var = NULL;
    cJSON *smf_registration_list = cJSON_GetObjectItemCaseSensitive(smf_registration_infoJSON, "smfRegistrationList");
    if (!smf_registration_list) {
        ogs_error("OpenAPI_smf_registration_info_parseFromJSON() failed [smf_registration_list]");
        goto end;
    }

    OpenAPI_list_t *smf_registration_listList;

    cJSON *smf_registration_list_local_nonprimitive;
    if (!cJSON_IsArray(smf_registration_list)) {
        ogs_error("OpenAPI_smf_registration_info_parseFromJSON() failed [smf_registration_list]");
        goto end;
    }

    smf_registration_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(smf_registration_list_local_nonprimitive, smf_registration_list ) {
        if (!cJSON_IsObject(smf_registration_list_local_nonprimitive)) {
            ogs_error("OpenAPI_smf_registration_info_parseFromJSON() failed [smf_registration_list]");
            goto end;
        }
        OpenAPI_smf_registration_t *smf_registration_listItem = OpenAPI_smf_registration_parseFromJSON(smf_registration_list_local_nonprimitive);

        OpenAPI_list_add(smf_registration_listList, smf_registration_listItem);
    }

    smf_registration_info_local_var = OpenAPI_smf_registration_info_create (
        smf_registration_listList
        );

    return smf_registration_info_local_var;
end:
    return NULL;
}

