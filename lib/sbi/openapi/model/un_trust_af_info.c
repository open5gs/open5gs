
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "un_trust_af_info.h"

OpenAPI_un_trust_af_info_t *OpenAPI_un_trust_af_info_create(
    char *af_id,
    OpenAPI_list_t *s_nssai_info_list,
    bool is_mapping_ind,
    int mapping_ind
)
{
    OpenAPI_un_trust_af_info_t *un_trust_af_info_local_var = ogs_malloc(sizeof(OpenAPI_un_trust_af_info_t));
    ogs_assert(un_trust_af_info_local_var);

    un_trust_af_info_local_var->af_id = af_id;
    un_trust_af_info_local_var->s_nssai_info_list = s_nssai_info_list;
    un_trust_af_info_local_var->is_mapping_ind = is_mapping_ind;
    un_trust_af_info_local_var->mapping_ind = mapping_ind;

    return un_trust_af_info_local_var;
}

void OpenAPI_un_trust_af_info_free(OpenAPI_un_trust_af_info_t *un_trust_af_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == un_trust_af_info) {
        return;
    }
    if (un_trust_af_info->af_id) {
        ogs_free(un_trust_af_info->af_id);
        un_trust_af_info->af_id = NULL;
    }
    if (un_trust_af_info->s_nssai_info_list) {
        OpenAPI_list_for_each(un_trust_af_info->s_nssai_info_list, node) {
            OpenAPI_snssai_info_item_free(node->data);
        }
        OpenAPI_list_free(un_trust_af_info->s_nssai_info_list);
        un_trust_af_info->s_nssai_info_list = NULL;
    }
    ogs_free(un_trust_af_info);
}

cJSON *OpenAPI_un_trust_af_info_convertToJSON(OpenAPI_un_trust_af_info_t *un_trust_af_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (un_trust_af_info == NULL) {
        ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed [UnTrustAfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!un_trust_af_info->af_id) {
        ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed [af_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afId", un_trust_af_info->af_id) == NULL) {
        ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed [af_id]");
        goto end;
    }

    if (un_trust_af_info->s_nssai_info_list) {
    cJSON *s_nssai_info_listList = cJSON_AddArrayToObject(item, "sNssaiInfoList");
    if (s_nssai_info_listList == NULL) {
        ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed [s_nssai_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(un_trust_af_info->s_nssai_info_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed [s_nssai_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_info_listList, itemLocal);
    }
    }

    if (un_trust_af_info->is_mapping_ind) {
    if (cJSON_AddBoolToObject(item, "mappingInd", un_trust_af_info->mapping_ind) == NULL) {
        ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed [mapping_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_un_trust_af_info_t *OpenAPI_un_trust_af_info_parseFromJSON(cJSON *un_trust_af_infoJSON)
{
    OpenAPI_un_trust_af_info_t *un_trust_af_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_id = NULL;
    cJSON *s_nssai_info_list = NULL;
    OpenAPI_list_t *s_nssai_info_listList = NULL;
    cJSON *mapping_ind = NULL;
    af_id = cJSON_GetObjectItemCaseSensitive(un_trust_af_infoJSON, "afId");
    if (!af_id) {
        ogs_error("OpenAPI_un_trust_af_info_parseFromJSON() failed [af_id]");
        goto end;
    }
    if (!cJSON_IsString(af_id)) {
        ogs_error("OpenAPI_un_trust_af_info_parseFromJSON() failed [af_id]");
        goto end;
    }

    s_nssai_info_list = cJSON_GetObjectItemCaseSensitive(un_trust_af_infoJSON, "sNssaiInfoList");
    if (s_nssai_info_list) {
        cJSON *s_nssai_info_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_info_list)) {
            ogs_error("OpenAPI_un_trust_af_info_parseFromJSON() failed [s_nssai_info_list]");
            goto end;
        }

        s_nssai_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_info_list_local, s_nssai_info_list) {
            if (!cJSON_IsObject(s_nssai_info_list_local)) {
                ogs_error("OpenAPI_un_trust_af_info_parseFromJSON() failed [s_nssai_info_list]");
                goto end;
            }
            OpenAPI_snssai_info_item_t *s_nssai_info_listItem = OpenAPI_snssai_info_item_parseFromJSON(s_nssai_info_list_local);
            if (!s_nssai_info_listItem) {
                ogs_error("No s_nssai_info_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_info_listList, s_nssai_info_listItem);
        }
    }

    mapping_ind = cJSON_GetObjectItemCaseSensitive(un_trust_af_infoJSON, "mappingInd");
    if (mapping_ind) {
    if (!cJSON_IsBool(mapping_ind)) {
        ogs_error("OpenAPI_un_trust_af_info_parseFromJSON() failed [mapping_ind]");
        goto end;
    }
    }

    un_trust_af_info_local_var = OpenAPI_un_trust_af_info_create (
        ogs_strdup(af_id->valuestring),
        s_nssai_info_list ? s_nssai_info_listList : NULL,
        mapping_ind ? true : false,
        mapping_ind ? mapping_ind->valueint : 0
    );

    return un_trust_af_info_local_var;
end:
    if (s_nssai_info_listList) {
        OpenAPI_list_for_each(s_nssai_info_listList, node) {
            OpenAPI_snssai_info_item_free(node->data);
        }
        OpenAPI_list_free(s_nssai_info_listList);
        s_nssai_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_un_trust_af_info_t *OpenAPI_un_trust_af_info_copy(OpenAPI_un_trust_af_info_t *dst, OpenAPI_un_trust_af_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_un_trust_af_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_un_trust_af_info_convertToJSON() failed");
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

    OpenAPI_un_trust_af_info_free(dst);
    dst = OpenAPI_un_trust_af_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

