
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_info.h"

OpenAPI_amf_info_t *OpenAPI_amf_info_create(
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *backup_info_amf_failure,
    OpenAPI_list_t *backup_info_amf_removal,
    bool is_n2_interface_amf_info_null,
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info,
    bool is_amf_onboarding_capability,
    int amf_onboarding_capability,
    bool is_high_latency_com,
    int high_latency_com,
    OpenAPI_list_t *amf_events,
    OpenAPI_list_t *pra_id_list,
    bool is_mobile_iab_ind_null,
    bool is_mobile_iab_ind,
    int mobile_iab_ind
)
{
    OpenAPI_amf_info_t *amf_info_local_var = ogs_malloc(sizeof(OpenAPI_amf_info_t));
    ogs_assert(amf_info_local_var);

    amf_info_local_var->amf_set_id = amf_set_id;
    amf_info_local_var->amf_region_id = amf_region_id;
    amf_info_local_var->guami_list = guami_list;
    amf_info_local_var->tai_list = tai_list;
    amf_info_local_var->tai_range_list = tai_range_list;
    amf_info_local_var->backup_info_amf_failure = backup_info_amf_failure;
    amf_info_local_var->backup_info_amf_removal = backup_info_amf_removal;
    amf_info_local_var->is_n2_interface_amf_info_null = is_n2_interface_amf_info_null;
    amf_info_local_var->n2_interface_amf_info = n2_interface_amf_info;
    amf_info_local_var->is_amf_onboarding_capability = is_amf_onboarding_capability;
    amf_info_local_var->amf_onboarding_capability = amf_onboarding_capability;
    amf_info_local_var->is_high_latency_com = is_high_latency_com;
    amf_info_local_var->high_latency_com = high_latency_com;
    amf_info_local_var->amf_events = amf_events;
    amf_info_local_var->pra_id_list = pra_id_list;
    amf_info_local_var->is_mobile_iab_ind_null = is_mobile_iab_ind_null;
    amf_info_local_var->is_mobile_iab_ind = is_mobile_iab_ind;
    amf_info_local_var->mobile_iab_ind = mobile_iab_ind;

    return amf_info_local_var;
}

void OpenAPI_amf_info_free(OpenAPI_amf_info_t *amf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_info) {
        return;
    }
    if (amf_info->amf_set_id) {
        ogs_free(amf_info->amf_set_id);
        amf_info->amf_set_id = NULL;
    }
    if (amf_info->amf_region_id) {
        ogs_free(amf_info->amf_region_id);
        amf_info->amf_region_id = NULL;
    }
    if (amf_info->guami_list) {
        OpenAPI_list_for_each(amf_info->guami_list, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(amf_info->guami_list);
        amf_info->guami_list = NULL;
    }
    if (amf_info->tai_list) {
        OpenAPI_list_for_each(amf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(amf_info->tai_list);
        amf_info->tai_list = NULL;
    }
    if (amf_info->tai_range_list) {
        OpenAPI_list_for_each(amf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(amf_info->tai_range_list);
        amf_info->tai_range_list = NULL;
    }
    if (amf_info->backup_info_amf_failure) {
        OpenAPI_list_for_each(amf_info->backup_info_amf_failure, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(amf_info->backup_info_amf_failure);
        amf_info->backup_info_amf_failure = NULL;
    }
    if (amf_info->backup_info_amf_removal) {
        OpenAPI_list_for_each(amf_info->backup_info_amf_removal, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(amf_info->backup_info_amf_removal);
        amf_info->backup_info_amf_removal = NULL;
    }
    if (amf_info->n2_interface_amf_info) {
        OpenAPI_n2_interface_amf_info_free(amf_info->n2_interface_amf_info);
        amf_info->n2_interface_amf_info = NULL;
    }
    if (amf_info->amf_events) {
        OpenAPI_list_free(amf_info->amf_events);
        amf_info->amf_events = NULL;
    }
    if (amf_info->pra_id_list) {
        OpenAPI_list_for_each(amf_info->pra_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(amf_info->pra_id_list);
        amf_info->pra_id_list = NULL;
    }
    ogs_free(amf_info);
}

cJSON *OpenAPI_amf_info_convertToJSON(OpenAPI_amf_info_t *amf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_info == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [AmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_info->amf_set_id) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_set_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfSetId", amf_info->amf_set_id) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_set_id]");
        goto end;
    }

    if (!amf_info->amf_region_id) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_region_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfRegionId", amf_info->amf_region_id) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_region_id]");
        goto end;
    }

    if (!amf_info->guami_list) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [guami_list]");
        return NULL;
    }
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [guami_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->guami_list, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [guami_list]");
            goto end;
        }
        cJSON_AddItemToArray(guami_listList, itemLocal);
    }

    if (amf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (amf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (amf_info->backup_info_amf_failure) {
    cJSON *backup_info_amf_failureList = cJSON_AddArrayToObject(item, "backupInfoAmfFailure");
    if (backup_info_amf_failureList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_failure]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->backup_info_amf_failure, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_failure]");
            goto end;
        }
        cJSON_AddItemToArray(backup_info_amf_failureList, itemLocal);
    }
    }

    if (amf_info->backup_info_amf_removal) {
    cJSON *backup_info_amf_removalList = cJSON_AddArrayToObject(item, "backupInfoAmfRemoval");
    if (backup_info_amf_removalList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_removal]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->backup_info_amf_removal, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [backup_info_amf_removal]");
            goto end;
        }
        cJSON_AddItemToArray(backup_info_amf_removalList, itemLocal);
    }
    }

    if (amf_info->n2_interface_amf_info) {
    cJSON *n2_interface_amf_info_local_JSON = OpenAPI_n2_interface_amf_info_convertToJSON(amf_info->n2_interface_amf_info);
    if (n2_interface_amf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [n2_interface_amf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InterfaceAmfInfo", n2_interface_amf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [n2_interface_amf_info]");
        goto end;
    }
    } else if (amf_info->is_n2_interface_amf_info_null) {
        if (cJSON_AddNullToObject(item, "n2InterfaceAmfInfo") == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [n2_interface_amf_info]");
            goto end;
        }
    }

    if (amf_info->is_amf_onboarding_capability) {
    if (cJSON_AddBoolToObject(item, "amfOnboardingCapability", amf_info->amf_onboarding_capability) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_onboarding_capability]");
        goto end;
    }
    }

    if (amf_info->is_high_latency_com) {
    if (cJSON_AddBoolToObject(item, "highLatencyCom", amf_info->high_latency_com) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [high_latency_com]");
        goto end;
    }
    }

    if (amf_info->amf_events != OpenAPI_amf_event_type_NULL) {
    cJSON *amf_eventsList = cJSON_AddArrayToObject(item, "amfEvents");
    if (amf_eventsList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_events]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->amf_events, node) {
        if (cJSON_AddStringToObject(amf_eventsList, "", OpenAPI_amf_event_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [amf_events]");
            goto end;
        }
    }
    }

    if (amf_info->pra_id_list) {
    cJSON *pra_id_listList = cJSON_AddArrayToObject(item, "praIdList");
    if (pra_id_listList == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [pra_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(amf_info->pra_id_list, node) {
        if (cJSON_AddStringToObject(pra_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [pra_id_list]");
            goto end;
        }
    }
    }

    if (amf_info->is_mobile_iab_ind) {
    if (cJSON_AddBoolToObject(item, "mobileIabInd", amf_info->mobile_iab_ind) == NULL) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed [mobile_iab_ind]");
        goto end;
    }
    } else if (amf_info->is_mobile_iab_ind_null) {
        if (cJSON_AddNullToObject(item, "mobileIabInd") == NULL) {
            ogs_error("OpenAPI_amf_info_convertToJSON() failed [mobile_iab_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_amf_info_t *OpenAPI_amf_info_parseFromJSON(cJSON *amf_infoJSON)
{
    OpenAPI_amf_info_t *amf_info_local_var = NULL;
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
    cJSON *high_latency_com = NULL;
    cJSON *amf_events = NULL;
    OpenAPI_list_t *amf_eventsList = NULL;
    cJSON *pra_id_list = NULL;
    OpenAPI_list_t *pra_id_listList = NULL;
    cJSON *mobile_iab_ind = NULL;
    amf_set_id = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "amfSetId");
    if (!amf_set_id) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_set_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_set_id)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_set_id]");
        goto end;
    }

    amf_region_id = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "amfRegionId");
    if (!amf_region_id) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_region_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_region_id)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_region_id]");
        goto end;
    }

    guami_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "guamiList");
    if (!guami_list) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [guami_list]");
        goto end;
    }
        cJSON *guami_list_local = NULL;
        if (!cJSON_IsArray(guami_list)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [guami_list]");
            goto end;
        }

        guami_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(guami_list_local, guami_list) {
            if (!cJSON_IsObject(guami_list_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [guami_list]");
                goto end;
            }
            OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local);
            if (!guami_listItem) {
                ogs_error("No guami_listItem");
                goto end;
            }
            OpenAPI_list_add(guami_listList, guami_listItem);
        }

    tai_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [tai_range_list]");
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

    backup_info_amf_failure = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "backupInfoAmfFailure");
    if (backup_info_amf_failure) {
        cJSON *backup_info_amf_failure_local = NULL;
        if (!cJSON_IsArray(backup_info_amf_failure)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_failure]");
            goto end;
        }

        backup_info_amf_failureList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_info_amf_failure_local, backup_info_amf_failure) {
            if (!cJSON_IsObject(backup_info_amf_failure_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_failure]");
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

    backup_info_amf_removal = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "backupInfoAmfRemoval");
    if (backup_info_amf_removal) {
        cJSON *backup_info_amf_removal_local = NULL;
        if (!cJSON_IsArray(backup_info_amf_removal)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_removal]");
            goto end;
        }

        backup_info_amf_removalList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_info_amf_removal_local, backup_info_amf_removal) {
            if (!cJSON_IsObject(backup_info_amf_removal_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [backup_info_amf_removal]");
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

    n2_interface_amf_info = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "n2InterfaceAmfInfo");
    if (n2_interface_amf_info) {
    if (!cJSON_IsNull(n2_interface_amf_info)) {
    n2_interface_amf_info_local_nonprim = OpenAPI_n2_interface_amf_info_parseFromJSON(n2_interface_amf_info);
    if (!n2_interface_amf_info_local_nonprim) {
        ogs_error("OpenAPI_n2_interface_amf_info_parseFromJSON failed [n2_interface_amf_info]");
        goto end;
    }
    }
    }

    amf_onboarding_capability = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "amfOnboardingCapability");
    if (amf_onboarding_capability) {
    if (!cJSON_IsBool(amf_onboarding_capability)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_onboarding_capability]");
        goto end;
    }
    }

    high_latency_com = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "highLatencyCom");
    if (high_latency_com) {
    if (!cJSON_IsBool(high_latency_com)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [high_latency_com]");
        goto end;
    }
    }

    amf_events = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "amfEvents");
    if (amf_events) {
        cJSON *amf_events_local = NULL;
        if (!cJSON_IsArray(amf_events)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_events]");
            goto end;
        }

        amf_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(amf_events_local, amf_events) {
            OpenAPI_amf_event_type_e localEnum = OpenAPI_amf_event_type_NULL;
            if (!cJSON_IsString(amf_events_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [amf_events]");
                goto end;
            }
            localEnum = OpenAPI_amf_event_type_FromString(amf_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"amf_events\" is not supported. Ignoring it ...",
                         amf_events_local->valuestring);
            } else {
                OpenAPI_list_add(amf_eventsList, (void *)localEnum);
            }
        }
        if (amf_eventsList->count == 0) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed: Expected amf_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pra_id_list = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "praIdList");
    if (pra_id_list) {
        cJSON *pra_id_list_local = NULL;
        if (!cJSON_IsArray(pra_id_list)) {
            ogs_error("OpenAPI_amf_info_parseFromJSON() failed [pra_id_list]");
            goto end;
        }

        pra_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pra_id_list_local, pra_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pra_id_list_local)) {
                ogs_error("OpenAPI_amf_info_parseFromJSON() failed [pra_id_list]");
                goto end;
            }
            OpenAPI_list_add(pra_id_listList, ogs_strdup(pra_id_list_local->valuestring));
        }
    }

    mobile_iab_ind = cJSON_GetObjectItemCaseSensitive(amf_infoJSON, "mobileIabInd");
    if (mobile_iab_ind) {
    if (!cJSON_IsNull(mobile_iab_ind)) {
    if (!cJSON_IsBool(mobile_iab_ind)) {
        ogs_error("OpenAPI_amf_info_parseFromJSON() failed [mobile_iab_ind]");
        goto end;
    }
    }
    }

    amf_info_local_var = OpenAPI_amf_info_create (
        ogs_strdup(amf_set_id->valuestring),
        ogs_strdup(amf_region_id->valuestring),
        guami_listList,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        backup_info_amf_failure ? backup_info_amf_failureList : NULL,
        backup_info_amf_removal ? backup_info_amf_removalList : NULL,
        n2_interface_amf_info && cJSON_IsNull(n2_interface_amf_info) ? true : false,
        n2_interface_amf_info ? n2_interface_amf_info_local_nonprim : NULL,
        amf_onboarding_capability ? true : false,
        amf_onboarding_capability ? amf_onboarding_capability->valueint : 0,
        high_latency_com ? true : false,
        high_latency_com ? high_latency_com->valueint : 0,
        amf_events ? amf_eventsList : NULL,
        pra_id_list ? pra_id_listList : NULL,
        mobile_iab_ind && cJSON_IsNull(mobile_iab_ind) ? true : false,
        mobile_iab_ind ? true : false,
        mobile_iab_ind ? mobile_iab_ind->valueint : 0
    );

    return amf_info_local_var;
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
    if (amf_eventsList) {
        OpenAPI_list_free(amf_eventsList);
        amf_eventsList = NULL;
    }
    if (pra_id_listList) {
        OpenAPI_list_for_each(pra_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pra_id_listList);
        pra_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_amf_info_t *OpenAPI_amf_info_copy(OpenAPI_amf_info_t *dst, OpenAPI_amf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_info_convertToJSON() failed");
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

    OpenAPI_amf_info_free(dst);
    dst = OpenAPI_amf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

