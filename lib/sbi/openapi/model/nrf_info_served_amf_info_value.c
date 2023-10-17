
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_amf_info_value.h"

OpenAPI_nrf_info_served_amf_info_value_t *OpenAPI_nrf_info_served_amf_info_value_create(
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *backup_info_amf_failure,
    OpenAPI_list_t *backup_info_amf_removal,
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info,
    bool is_amf_onboarding_capability,
    int amf_onboarding_capability
)
{
    OpenAPI_nrf_info_served_amf_info_value_t *nrf_info_served_amf_info_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_amf_info_value_t));
    ogs_assert(nrf_info_served_amf_info_value_local_var);

    nrf_info_served_amf_info_value_local_var->amf_set_id = amf_set_id;
    nrf_info_served_amf_info_value_local_var->amf_region_id = amf_region_id;
    nrf_info_served_amf_info_value_local_var->guami_list = guami_list;
    nrf_info_served_amf_info_value_local_var->tai_list = tai_list;
    nrf_info_served_amf_info_value_local_var->tai_range_list = tai_range_list;
    nrf_info_served_amf_info_value_local_var->backup_info_amf_failure = backup_info_amf_failure;
    nrf_info_served_amf_info_value_local_var->backup_info_amf_removal = backup_info_amf_removal;
    nrf_info_served_amf_info_value_local_var->n2_interface_amf_info = n2_interface_amf_info;
    nrf_info_served_amf_info_value_local_var->is_amf_onboarding_capability = is_amf_onboarding_capability;
    nrf_info_served_amf_info_value_local_var->amf_onboarding_capability = amf_onboarding_capability;

    return nrf_info_served_amf_info_value_local_var;
}

void OpenAPI_nrf_info_served_amf_info_value_free(OpenAPI_nrf_info_served_amf_info_value_t *nrf_info_served_amf_info_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_amf_info_value) {
        return;
    }
    if (nrf_info_served_amf_info_value->amf_set_id) {
        ogs_free(nrf_info_served_amf_info_value->amf_set_id);
        nrf_info_served_amf_info_value->amf_set_id = NULL;
    }
    if (nrf_info_served_amf_info_value->amf_region_id) {
        ogs_free(nrf_info_served_amf_info_value->amf_region_id);
        nrf_info_served_amf_info_value->amf_region_id = NULL;
    }
    if (nrf_info_served_amf_info_value->guami_list) {
        OpenAPI_list_for_each(nrf_info_served_amf_info_value->guami_list, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_amf_info_value->guami_list);
        nrf_info_served_amf_info_value->guami_list = NULL;
    }
    if (nrf_info_served_amf_info_value->tai_list) {
        OpenAPI_list_for_each(nrf_info_served_amf_info_value->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_amf_info_value->tai_list);
        nrf_info_served_amf_info_value->tai_list = NULL;
    }
    if (nrf_info_served_amf_info_value->tai_range_list) {
        OpenAPI_list_for_each(nrf_info_served_amf_info_value->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_amf_info_value->tai_range_list);
        nrf_info_served_amf_info_value->tai_range_list = NULL;
    }
    if (nrf_info_served_amf_info_value->backup_info_amf_failure) {
        OpenAPI_list_for_each(nrf_info_served_amf_info_value->backup_info_amf_failure, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_amf_info_value->backup_info_amf_failure);
        nrf_info_served_amf_info_value->backup_info_amf_failure = NULL;
    }
    if (nrf_info_served_amf_info_value->backup_info_amf_removal) {
        OpenAPI_list_for_each(nrf_info_served_amf_info_value->backup_info_amf_removal, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_amf_info_value->backup_info_amf_removal);
        nrf_info_served_amf_info_value->backup_info_amf_removal = NULL;
    }
    if (nrf_info_served_amf_info_value->n2_interface_amf_info) {
        OpenAPI_n2_interface_amf_info_free(nrf_info_served_amf_info_value->n2_interface_amf_info);
        nrf_info_served_amf_info_value->n2_interface_amf_info = NULL;
    }
    ogs_free(nrf_info_served_amf_info_value);
}

cJSON *OpenAPI_nrf_info_served_amf_info_value_convertToJSON(OpenAPI_nrf_info_served_amf_info_value_t *nrf_info_served_amf_info_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_amf_info_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [NrfInfo_servedAmfInfo_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nrf_info_served_amf_info_value->amf_set_id) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [amf_set_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfSetId", nrf_info_served_amf_info_value->amf_set_id) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [amf_set_id]");
        goto end;
    }

    if (!nrf_info_served_amf_info_value->amf_region_id) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [amf_region_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfRegionId", nrf_info_served_amf_info_value->amf_region_id) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [amf_region_id]");
        goto end;
    }

    if (!nrf_info_served_amf_info_value->guami_list) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [guami_list]");
        return NULL;
    }
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [guami_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_amf_info_value->guami_list, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [guami_list]");
            goto end;
        }
        cJSON_AddItemToArray(guami_listList, itemLocal);
    }

    if (nrf_info_served_amf_info_value->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_amf_info_value->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nrf_info_served_amf_info_value->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_amf_info_value->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (nrf_info_served_amf_info_value->backup_info_amf_failure) {
    cJSON *backup_info_amf_failureList = cJSON_AddArrayToObject(item, "backupInfoAmfFailure");
    if (backup_info_amf_failureList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [backup_info_amf_failure]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_amf_info_value->backup_info_amf_failure, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [backup_info_amf_failure]");
            goto end;
        }
        cJSON_AddItemToArray(backup_info_amf_failureList, itemLocal);
    }
    }

    if (nrf_info_served_amf_info_value->backup_info_amf_removal) {
    cJSON *backup_info_amf_removalList = cJSON_AddArrayToObject(item, "backupInfoAmfRemoval");
    if (backup_info_amf_removalList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [backup_info_amf_removal]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_amf_info_value->backup_info_amf_removal, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [backup_info_amf_removal]");
            goto end;
        }
        cJSON_AddItemToArray(backup_info_amf_removalList, itemLocal);
    }
    }

    if (nrf_info_served_amf_info_value->n2_interface_amf_info) {
    cJSON *n2_interface_amf_info_local_JSON = OpenAPI_n2_interface_amf_info_convertToJSON(nrf_info_served_amf_info_value->n2_interface_amf_info);
    if (n2_interface_amf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [n2_interface_amf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InterfaceAmfInfo", n2_interface_amf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [n2_interface_amf_info]");
        goto end;
    }
    }

    if (nrf_info_served_amf_info_value->is_amf_onboarding_capability) {
    if (cJSON_AddBoolToObject(item, "amfOnboardingCapability", nrf_info_served_amf_info_value->amf_onboarding_capability) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed [amf_onboarding_capability]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_amf_info_value_t *OpenAPI_nrf_info_served_amf_info_value_parseFromJSON(cJSON *nrf_info_served_amf_info_valueJSON)
{
    OpenAPI_nrf_info_served_amf_info_value_t *nrf_info_served_amf_info_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_set_id = NULL;
    cJSON *amf_region_id = NULL;
    cJSON *guami_list = NULL;
    OpenAPI_list_t *guami_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *backup_info_amf_failure = NULL;
    OpenAPI_list_t *backup_info_amf_failureList = NULL;
    cJSON *backup_info_amf_removal = NULL;
    OpenAPI_list_t *backup_info_amf_removalList = NULL;
    cJSON *n2_interface_amf_info = NULL;
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info_local_nonprim = NULL;
    cJSON *amf_onboarding_capability = NULL;
    amf_set_id = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "amfSetId");
    if (!amf_set_id) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [amf_set_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_set_id)) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [amf_set_id]");
        goto end;
    }

    amf_region_id = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "amfRegionId");
    if (!amf_region_id) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [amf_region_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_region_id)) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [amf_region_id]");
        goto end;
    }

    guami_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "guamiList");
    if (!guami_list) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [guami_list]");
        goto end;
    }
        cJSON *guami_list_local = NULL;
        if (!cJSON_IsArray(guami_list)) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [guami_list]");
            goto end;
        }

        guami_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(guami_list_local, guami_list) {
            if (!cJSON_IsObject(guami_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [guami_list]");
                goto end;
            }
            OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local);
            if (!guami_listItem) {
                ogs_error("No guami_listItem");
                goto end;
            }
            OpenAPI_list_add(guami_listList, guami_listItem);
        }

    tai_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [tai_range_list]");
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

    backup_info_amf_failure = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "backupInfoAmfFailure");
    if (backup_info_amf_failure) {
        cJSON *backup_info_amf_failure_local = NULL;
        if (!cJSON_IsArray(backup_info_amf_failure)) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [backup_info_amf_failure]");
            goto end;
        }

        backup_info_amf_failureList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_info_amf_failure_local, backup_info_amf_failure) {
            if (!cJSON_IsObject(backup_info_amf_failure_local)) {
                ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [backup_info_amf_failure]");
                goto end;
            }
            OpenAPI_guami_t *backup_info_amf_failureItem = OpenAPI_guami_parseFromJSON(backup_info_amf_failure_local);
            if (!backup_info_amf_failureItem) {
                ogs_error("No backup_info_amf_failureItem");
                goto end;
            }
            OpenAPI_list_add(backup_info_amf_failureList, backup_info_amf_failureItem);
        }
    }

    backup_info_amf_removal = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "backupInfoAmfRemoval");
    if (backup_info_amf_removal) {
        cJSON *backup_info_amf_removal_local = NULL;
        if (!cJSON_IsArray(backup_info_amf_removal)) {
            ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [backup_info_amf_removal]");
            goto end;
        }

        backup_info_amf_removalList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_info_amf_removal_local, backup_info_amf_removal) {
            if (!cJSON_IsObject(backup_info_amf_removal_local)) {
                ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [backup_info_amf_removal]");
                goto end;
            }
            OpenAPI_guami_t *backup_info_amf_removalItem = OpenAPI_guami_parseFromJSON(backup_info_amf_removal_local);
            if (!backup_info_amf_removalItem) {
                ogs_error("No backup_info_amf_removalItem");
                goto end;
            }
            OpenAPI_list_add(backup_info_amf_removalList, backup_info_amf_removalItem);
        }
    }

    n2_interface_amf_info = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "n2InterfaceAmfInfo");
    if (n2_interface_amf_info) {
    n2_interface_amf_info_local_nonprim = OpenAPI_n2_interface_amf_info_parseFromJSON(n2_interface_amf_info);
    if (!n2_interface_amf_info_local_nonprim) {
        ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON failed [n2_interface_amf_info]");
        goto end;
    }
    }

    amf_onboarding_capability = cJSON_GetObjectItemCaseSensitive(nrf_info_served_amf_info_valueJSON, "amfOnboardingCapability");
    if (amf_onboarding_capability) {
    if (!cJSON_IsBool(amf_onboarding_capability)) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_parseFromJSON() failed [amf_onboarding_capability]");
        goto end;
    }
    }

    nrf_info_served_amf_info_value_local_var = OpenAPI_nrf_info_served_amf_info_value_create (
        ogs_strdup(amf_set_id->valuestring),
        ogs_strdup(amf_region_id->valuestring),
        guami_listList,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        backup_info_amf_failure ? backup_info_amf_failureList : NULL,
        backup_info_amf_removal ? backup_info_amf_removalList : NULL,
        n2_interface_amf_info ? n2_interface_amf_info_local_nonprim : NULL,
        amf_onboarding_capability ? true : false,
        amf_onboarding_capability ? amf_onboarding_capability->valueint : 0
    );

    return nrf_info_served_amf_info_value_local_var;
end:
    if (guami_listList) {
        OpenAPI_list_for_each(guami_listList, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(guami_listList);
        guami_listList = NULL;
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
    if (backup_info_amf_failureList) {
        OpenAPI_list_for_each(backup_info_amf_failureList, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(backup_info_amf_failureList);
        backup_info_amf_failureList = NULL;
    }
    if (backup_info_amf_removalList) {
        OpenAPI_list_for_each(backup_info_amf_removalList, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(backup_info_amf_removalList);
        backup_info_amf_removalList = NULL;
    }
    if (n2_interface_amf_info_local_nonprim) {
        OpenAPI_n2_interface_amf_info_free(n2_interface_amf_info_local_nonprim);
        n2_interface_amf_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_amf_info_value_t *OpenAPI_nrf_info_served_amf_info_value_copy(OpenAPI_nrf_info_served_amf_info_value_t *dst, OpenAPI_nrf_info_served_amf_info_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_amf_info_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_amf_info_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_amf_info_value_free(dst);
    dst = OpenAPI_nrf_info_served_amf_info_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

