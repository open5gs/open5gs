
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_info.h"

OpenAPI_smf_info_t *OpenAPI_smf_info_create(
    OpenAPI_list_t *s_nssai_smf_info_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    char *pgw_fqdn,
    OpenAPI_list_t *pgw_ip_addr_list,
    OpenAPI_list_t *access_type,
    bool is_priority,
    int priority,
    bool is_vsmf_support_ind,
    int vsmf_support_ind,
    OpenAPI_list_t *pgw_fqdn_list,
    bool is_smf_onboarding_capability,
    int smf_onboarding_capability,
    bool is_ismf_support_ind,
    int ismf_support_ind,
    bool is_smf_uprp_capability,
    int smf_uprp_capability
)
{
    OpenAPI_smf_info_t *smf_info_local_var = ogs_malloc(sizeof(OpenAPI_smf_info_t));
    ogs_assert(smf_info_local_var);

    smf_info_local_var->s_nssai_smf_info_list = s_nssai_smf_info_list;
    smf_info_local_var->tai_list = tai_list;
    smf_info_local_var->tai_range_list = tai_range_list;
    smf_info_local_var->pgw_fqdn = pgw_fqdn;
    smf_info_local_var->pgw_ip_addr_list = pgw_ip_addr_list;
    smf_info_local_var->access_type = access_type;
    smf_info_local_var->is_priority = is_priority;
    smf_info_local_var->priority = priority;
    smf_info_local_var->is_vsmf_support_ind = is_vsmf_support_ind;
    smf_info_local_var->vsmf_support_ind = vsmf_support_ind;
    smf_info_local_var->pgw_fqdn_list = pgw_fqdn_list;
    smf_info_local_var->is_smf_onboarding_capability = is_smf_onboarding_capability;
    smf_info_local_var->smf_onboarding_capability = smf_onboarding_capability;
    smf_info_local_var->is_ismf_support_ind = is_ismf_support_ind;
    smf_info_local_var->ismf_support_ind = ismf_support_ind;
    smf_info_local_var->is_smf_uprp_capability = is_smf_uprp_capability;
    smf_info_local_var->smf_uprp_capability = smf_uprp_capability;

    return smf_info_local_var;
}

void OpenAPI_smf_info_free(OpenAPI_smf_info_t *smf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_info) {
        return;
    }
    if (smf_info->s_nssai_smf_info_list) {
        OpenAPI_list_for_each(smf_info->s_nssai_smf_info_list, node) {
            OpenAPI_snssai_smf_info_item_free(node->data);
        }
        OpenAPI_list_free(smf_info->s_nssai_smf_info_list);
        smf_info->s_nssai_smf_info_list = NULL;
    }
    if (smf_info->tai_list) {
        OpenAPI_list_for_each(smf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(smf_info->tai_list);
        smf_info->tai_list = NULL;
    }
    if (smf_info->tai_range_list) {
        OpenAPI_list_for_each(smf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(smf_info->tai_range_list);
        smf_info->tai_range_list = NULL;
    }
    if (smf_info->pgw_fqdn) {
        ogs_free(smf_info->pgw_fqdn);
        smf_info->pgw_fqdn = NULL;
    }
    if (smf_info->pgw_ip_addr_list) {
        OpenAPI_list_for_each(smf_info->pgw_ip_addr_list, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(smf_info->pgw_ip_addr_list);
        smf_info->pgw_ip_addr_list = NULL;
    }
    if (smf_info->access_type) {
        OpenAPI_list_free(smf_info->access_type);
        smf_info->access_type = NULL;
    }
    if (smf_info->pgw_fqdn_list) {
        OpenAPI_list_for_each(smf_info->pgw_fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smf_info->pgw_fqdn_list);
        smf_info->pgw_fqdn_list = NULL;
    }
    ogs_free(smf_info);
}

cJSON *OpenAPI_smf_info_convertToJSON(OpenAPI_smf_info_t *smf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_info == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [SmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_info->s_nssai_smf_info_list) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [s_nssai_smf_info_list]");
        return NULL;
    }
    cJSON *s_nssai_smf_info_listList = cJSON_AddArrayToObject(item, "sNssaiSmfInfoList");
    if (s_nssai_smf_info_listList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [s_nssai_smf_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(smf_info->s_nssai_smf_info_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_smf_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [s_nssai_smf_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_smf_info_listList, itemLocal);
    }

    if (smf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(smf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (smf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(smf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (smf_info->pgw_fqdn) {
    if (cJSON_AddStringToObject(item, "pgwFqdn", smf_info->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    if (smf_info->pgw_ip_addr_list) {
    cJSON *pgw_ip_addr_listList = cJSON_AddArrayToObject(item, "pgwIpAddrList");
    if (pgw_ip_addr_listList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [pgw_ip_addr_list]");
        goto end;
    }
    OpenAPI_list_for_each(smf_info->pgw_ip_addr_list, node) {
        cJSON *itemLocal = OpenAPI_ip_addr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [pgw_ip_addr_list]");
            goto end;
        }
        cJSON_AddItemToArray(pgw_ip_addr_listList, itemLocal);
    }
    }

    if (smf_info->access_type != OpenAPI_access_type_NULL) {
    cJSON *access_typeList = cJSON_AddArrayToObject(item, "accessType");
    if (access_typeList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [access_type]");
        goto end;
    }
    OpenAPI_list_for_each(smf_info->access_type, node) {
        if (cJSON_AddStringToObject(access_typeList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [access_type]");
            goto end;
        }
    }
    }

    if (smf_info->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", smf_info->priority) == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (smf_info->is_vsmf_support_ind) {
    if (cJSON_AddBoolToObject(item, "vsmfSupportInd", smf_info->vsmf_support_ind) == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [vsmf_support_ind]");
        goto end;
    }
    }

    if (smf_info->pgw_fqdn_list) {
    cJSON *pgw_fqdn_listList = cJSON_AddArrayToObject(item, "pgwFqdnList");
    if (pgw_fqdn_listList == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [pgw_fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(smf_info->pgw_fqdn_list, node) {
        if (cJSON_AddStringToObject(pgw_fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_smf_info_convertToJSON() failed [pgw_fqdn_list]");
            goto end;
        }
    }
    }

    if (smf_info->is_smf_onboarding_capability) {
    if (cJSON_AddBoolToObject(item, "smfOnboardingCapability", smf_info->smf_onboarding_capability) == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [smf_onboarding_capability]");
        goto end;
    }
    }

    if (smf_info->is_ismf_support_ind) {
    if (cJSON_AddBoolToObject(item, "ismfSupportInd", smf_info->ismf_support_ind) == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [ismf_support_ind]");
        goto end;
    }
    }

    if (smf_info->is_smf_uprp_capability) {
    if (cJSON_AddBoolToObject(item, "smfUPRPCapability", smf_info->smf_uprp_capability) == NULL) {
        ogs_error("OpenAPI_smf_info_convertToJSON() failed [smf_uprp_capability]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_info_t *OpenAPI_smf_info_parseFromJSON(cJSON *smf_infoJSON)
{
    OpenAPI_smf_info_t *smf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_smf_info_list = NULL;
    OpenAPI_list_t *s_nssai_smf_info_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *pgw_fqdn = NULL;
    cJSON *pgw_ip_addr_list = NULL;
    OpenAPI_list_t *pgw_ip_addr_listList = NULL;
    cJSON *access_type = NULL;
    OpenAPI_list_t *access_typeList = NULL;
    cJSON *priority = NULL;
    cJSON *vsmf_support_ind = NULL;
    cJSON *pgw_fqdn_list = NULL;
    OpenAPI_list_t *pgw_fqdn_listList = NULL;
    cJSON *smf_onboarding_capability = NULL;
    cJSON *ismf_support_ind = NULL;
    cJSON *smf_uprp_capability = NULL;
    s_nssai_smf_info_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "sNssaiSmfInfoList");
    if (!s_nssai_smf_info_list) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [s_nssai_smf_info_list]");
        goto end;
    }
        cJSON *s_nssai_smf_info_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_smf_info_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [s_nssai_smf_info_list]");
            goto end;
        }

        s_nssai_smf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_smf_info_list_local, s_nssai_smf_info_list) {
            if (!cJSON_IsObject(s_nssai_smf_info_list_local)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [s_nssai_smf_info_list]");
                goto end;
            }
            OpenAPI_snssai_smf_info_item_t *s_nssai_smf_info_listItem = OpenAPI_snssai_smf_info_item_parseFromJSON(s_nssai_smf_info_list_local);
            if (!s_nssai_smf_info_listItem) {
                ogs_error("No s_nssai_smf_info_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_smf_info_listList, s_nssai_smf_info_listItem);
        }

    tai_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    pgw_fqdn = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "pgwFqdn");
    if (pgw_fqdn) {
    if (!cJSON_IsString(pgw_fqdn) && !cJSON_IsNull(pgw_fqdn)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    pgw_ip_addr_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "pgwIpAddrList");
    if (pgw_ip_addr_list) {
        cJSON *pgw_ip_addr_list_local = NULL;
        if (!cJSON_IsArray(pgw_ip_addr_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [pgw_ip_addr_list]");
            goto end;
        }

        pgw_ip_addr_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pgw_ip_addr_list_local, pgw_ip_addr_list) {
            if (!cJSON_IsObject(pgw_ip_addr_list_local)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [pgw_ip_addr_list]");
                goto end;
            }
            OpenAPI_ip_addr_t *pgw_ip_addr_listItem = OpenAPI_ip_addr_parseFromJSON(pgw_ip_addr_list_local);
            if (!pgw_ip_addr_listItem) {
                ogs_error("No pgw_ip_addr_listItem");
                goto end;
            }
            OpenAPI_list_add(pgw_ip_addr_listList, pgw_ip_addr_listItem);
        }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "accessType");
    if (access_type) {
        cJSON *access_type_local = NULL;
        if (!cJSON_IsArray(access_type)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [access_type]");
            goto end;
        }

        access_typeList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_type_local, access_type) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_type_local)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [access_type]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(access_type_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_type\" is not supported. Ignoring it ...",
                         access_type_local->valuestring);
            } else {
                OpenAPI_list_add(access_typeList, (void *)localEnum);
            }
        }
        if (access_typeList->count == 0) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed: Expected access_typeList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    priority = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    vsmf_support_ind = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "vsmfSupportInd");
    if (vsmf_support_ind) {
    if (!cJSON_IsBool(vsmf_support_ind)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [vsmf_support_ind]");
        goto end;
    }
    }

    pgw_fqdn_list = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "pgwFqdnList");
    if (pgw_fqdn_list) {
        cJSON *pgw_fqdn_list_local = NULL;
        if (!cJSON_IsArray(pgw_fqdn_list)) {
            ogs_error("OpenAPI_smf_info_parseFromJSON() failed [pgw_fqdn_list]");
            goto end;
        }

        pgw_fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pgw_fqdn_list_local, pgw_fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pgw_fqdn_list_local)) {
                ogs_error("OpenAPI_smf_info_parseFromJSON() failed [pgw_fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(pgw_fqdn_listList, ogs_strdup(pgw_fqdn_list_local->valuestring));
        }
    }

    smf_onboarding_capability = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "smfOnboardingCapability");
    if (smf_onboarding_capability) {
    if (!cJSON_IsBool(smf_onboarding_capability)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [smf_onboarding_capability]");
        goto end;
    }
    }

    ismf_support_ind = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "ismfSupportInd");
    if (ismf_support_ind) {
    if (!cJSON_IsBool(ismf_support_ind)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [ismf_support_ind]");
        goto end;
    }
    }

    smf_uprp_capability = cJSON_GetObjectItemCaseSensitive(smf_infoJSON, "smfUPRPCapability");
    if (smf_uprp_capability) {
    if (!cJSON_IsBool(smf_uprp_capability)) {
        ogs_error("OpenAPI_smf_info_parseFromJSON() failed [smf_uprp_capability]");
        goto end;
    }
    }

    smf_info_local_var = OpenAPI_smf_info_create (
        s_nssai_smf_info_listList,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        pgw_fqdn && !cJSON_IsNull(pgw_fqdn) ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        pgw_ip_addr_list ? pgw_ip_addr_listList : NULL,
        access_type ? access_typeList : NULL,
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        vsmf_support_ind ? true : false,
        vsmf_support_ind ? vsmf_support_ind->valueint : 0,
        pgw_fqdn_list ? pgw_fqdn_listList : NULL,
        smf_onboarding_capability ? true : false,
        smf_onboarding_capability ? smf_onboarding_capability->valueint : 0,
        ismf_support_ind ? true : false,
        ismf_support_ind ? ismf_support_ind->valueint : 0,
        smf_uprp_capability ? true : false,
        smf_uprp_capability ? smf_uprp_capability->valueint : 0
    );

    return smf_info_local_var;
end:
    if (s_nssai_smf_info_listList) {
        OpenAPI_list_for_each(s_nssai_smf_info_listList, node) {
            OpenAPI_snssai_smf_info_item_free(node->data);
        }
        OpenAPI_list_free(s_nssai_smf_info_listList);
        s_nssai_smf_info_listList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (pgw_ip_addr_listList) {
        OpenAPI_list_for_each(pgw_ip_addr_listList, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(pgw_ip_addr_listList);
        pgw_ip_addr_listList = NULL;
    }
    if (access_typeList) {
        OpenAPI_list_free(access_typeList);
        access_typeList = NULL;
    }
    if (pgw_fqdn_listList) {
        OpenAPI_list_for_each(pgw_fqdn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pgw_fqdn_listList);
        pgw_fqdn_listList = NULL;
    }
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

