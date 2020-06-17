
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_info.h"

OpenAPI_smf_info_t *OpenAPI_smf_info_create(
    OpenAPI_list_t *s_nssai_smf_info_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    char *pgw_fqdn,
    OpenAPI_list_t *access_type,
    int priority
    )
{
    OpenAPI_smf_info_t *smf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_smf_info_t));
    if (!smf_info_local_var) {
        return NULL;
    }
    smf_info_local_var->s_nssai_smf_info_list = s_nssai_smf_info_list;
    smf_info_local_var->tai_list = tai_list;
    smf_info_local_var->tai_range_list = tai_range_list;
    smf_info_local_var->pgw_fqdn = pgw_fqdn;
    smf_info_local_var->access_type = access_type;
    smf_info_local_var->priority = priority;

    return smf_info_local_var;
}

void OpenAPI_smf_info_free(OpenAPI_smf_info_t *smf_info)
{
    if (NULL == smf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(smf_info->s_nssai_smf_info_list, node) {
        OpenAPI_snssai_smf_info_item_free(node->data);
    }
    OpenAPI_list_free(smf_info->s_nssai_smf_info_list);
    OpenAPI_list_for_each(smf_info->tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(smf_info->tai_list);
    OpenAPI_list_for_each(smf_info->tai_range_list, node) {
        OpenAPI_tai_range_free(node->data);
    }
    OpenAPI_list_free(smf_info->tai_range_list);
    ogs_free(smf_info->pgw_fqdn);
    OpenAPI_list_free(smf_info->access_type);
    ogs_free(smf_info);
}

cJSON *OpenAPI_smf_info_convertToJSON(OpenAPI_smf_info_t *smf_info)
{
    cJSON *item = NULL;

    if (smf_info == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [SmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_info->s_nssai_smf_info_list) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [s_nssai_smf_info_list]");
        goto end;
    }
    cJSON *s_nssai_smf_info_listList = cJSON_AddArrayToObject(item, "sNssaiSmfInfoList");
    if (s_nssai_smf_info_listList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [s_nssai_smf_info_list]");
        goto end;
    }

    OpenAPI_lnode_t *s_nssai_smf_info_list_node;
    if (smf_info->s_nssai_smf_info_list) {
        OpenAPI_list_for_each(smf_info->s_nssai_smf_info_list, s_nssai_smf_info_list_node) {
            cJSON *itemLocal = OpenAPI_snssai_smf_info_item_convertToJSON(s_nssai_smf_info_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_smf_info_convertToJSON() failed [s_nssai_smf_info_list]");
                goto end;
            }
            cJSON_AddItemToArray(s_nssai_smf_info_listList, itemLocal);
        }
    }

    if (smf_info->tai_list) {
        cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
        if (tai_listList == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_list]");
            goto end;
        }

        OpenAPI_lnode_t *tai_list_node;
        if (smf_info->tai_list) {
            OpenAPI_list_for_each(smf_info->tai_list, tai_list_node) {
                cJSON *itemLocal = OpenAPI_tai_convertToJSON(tai_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_list]");
                    goto end;
                }
                cJSON_AddItemToArray(tai_listList, itemLocal);
            }
        }
    }

    if (smf_info->tai_range_list) {
        cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
        if (tai_range_listList == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }

        OpenAPI_lnode_t *tai_range_list_node;
        if (smf_info->tai_range_list) {
            OpenAPI_list_for_each(smf_info->tai_range_list, tai_range_list_node) {
                cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(tai_range_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_range_list]");
                    goto end;
                }
                cJSON_AddItemToArray(tai_range_listList, itemLocal);
            }
        }
    }

    if (smf_info->pgw_fqdn) {
        if (cJSON_AddStringToObject(item, "pgwFqdn", smf_info->pgw_fqdn) == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [pgw_fqdn]");
            goto end;
        }
    }

    if (smf_info->access_type) {
        cJSON *access_type = cJSON_AddArrayToObject(item, "accessType");
        if (access_type == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [access_type]");
            goto end;
        }
        OpenAPI_lnode_t *access_type_node;
        OpenAPI_list_for_each(smf_info->access_type, access_type_node) {
            if (cJSON_AddStringToObject(access_type, "", OpenAPI_access_type_ToString((OpenAPI_access_type_e)access_type_node->data)) == NULL) {
                ogs_error("OpenAPI_smf_info_convertToJSON() failed [access_type]");
                goto end;
            }
        }
    }

    if (smf_info->priority) {
        if (cJSON_AddNumberToObject(item, "priority", smf_info->priority) == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [priority]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_smf_info_t *OpenAPI_smf_info_parseFromJSON(cJSON *smf_infoJSON)
{
    OpenAPI_smf_info_t *smf_info_local_var = NULL;
    cJSON *s_nssai_smf_info_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "sNssaiSmfInfoList");
    if (!s_nssai_smf_info_list) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [s_nssai_smf_info_list]");
        goto end;
    }

    OpenAPI_list_t *s_nssai_smf_info_listList;

    cJSON *s_nssai_smf_info_list_local_nonprimitive;
    if (!cJSON_IsArray(s_nssai_smf_info_list)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [s_nssai_smf_info_list]");
        goto end;
    }

    s_nssai_smf_info_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(s_nssai_smf_info_list_local_nonprimitive, s_nssai_smf_info_list ) {
        if (!cJSON_IsObject(s_nssai_smf_info_list_local_nonprimitive)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [s_nssai_smf_info_list]");
            goto end;
        }
        OpenAPI_snssai_smf_info_item_t *s_nssai_smf_info_listItem = OpenAPI_snssai_smf_info_item_parseFromJSON(s_nssai_smf_info_list_local_nonprimitive);

        OpenAPI_list_add(s_nssai_smf_info_listList, s_nssai_smf_info_listItem);
    }

    cJSON *tai_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "taiList");

    OpenAPI_list_t *tai_listList;
    if (tai_list) {
        cJSON *tai_list_local_nonprimitive;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local_nonprimitive, tai_list ) {
            if (!cJSON_IsObject(tai_list_local_nonprimitive)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local_nonprimitive);

            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    cJSON *tai_range_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "taiRangeList");

    OpenAPI_list_t *tai_range_listList;
    if (tai_range_list) {
        cJSON *tai_range_list_local_nonprimitive;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local_nonprimitive, tai_range_list ) {
            if (!cJSON_IsObject(tai_range_list_local_nonprimitive)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local_nonprimitive);

            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "pgwFqdn");

    if (pgw_fqdn) {
        if (!cJSON_IsString(pgw_fqdn)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [pgw_fqdn]");
            goto end;
        }
    }

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "accessType");

    OpenAPI_list_t *access_typeList;
    if (access_type) {
        cJSON *access_type_local_nonprimitive;
        if (!cJSON_IsArray(access_type)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [access_type]");
            goto end;
        }

        access_typeList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_type_local_nonprimitive, access_type ) {
            if (!cJSON_IsString(access_type_local_nonprimitive)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [access_type]");
                goto end;
            }

            OpenAPI_list_add(access_typeList, (void *)OpenAPI_access_type_FromString(access_type_local_nonprimitive->valuestring));
        }
    }

    cJSON *priority = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "priority");

    if (priority) {
        if (!cJSON_IsNumber(priority)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [priority]");
            goto end;
        }
    }

    smf_info_local_var = OpenAPI_smf_info_create (
        s_nssai_smf_info_listList,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        pgw_fqdn ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        access_type ? access_typeList : NULL,
        priority ? priority->valuedouble : 0
        );

    return smf_info_local_var;
end:
    return NULL;
}

OpenAPI_smf_info_t *OpenAPI_smf_info_copy(OpenAPI_smf_info_t *dst, OpenAPI_smf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed");
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

    OpenAPI_smf_info_free(dst);
    dst = OpenAPI_smf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

