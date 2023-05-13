
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_data_change_notification.h"

OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_create(
    OpenAPI_am_policy_data_t *am_policy_data,
    OpenAPI_ue_policy_set_t *ue_policy_set,
    OpenAPI_ue_policy_set_t *plmn_ue_policy_set,
    OpenAPI_sm_policy_data_t *sm_policy_data,
    OpenAPI_usage_mon_data_t *usage_mon_data,
    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data,
    OpenAPI_bdt_data_t *bdt_data,
    OpenAPI_operator_specific_data_container_t *op_spec_data,
    OpenAPI_list_t* op_spec_data_map,
    char *ue_id,
    char *sponsor_id,
    char *bdt_ref_id,
    char *usage_mon_id,
    OpenAPI_plmn_id_1_t *plmn_id,
    OpenAPI_list_t *del_resources,
    char *notif_id,
    OpenAPI_list_t *reported_fragments,
    OpenAPI_slice_policy_data_t *slice_policy_data,
    OpenAPI_snssai_t *snssai
)
{
    OpenAPI_policy_data_change_notification_t *policy_data_change_notification_local_var = ogs_malloc(sizeof(OpenAPI_policy_data_change_notification_t));
    ogs_assert(policy_data_change_notification_local_var);

    policy_data_change_notification_local_var->am_policy_data = am_policy_data;
    policy_data_change_notification_local_var->ue_policy_set = ue_policy_set;
    policy_data_change_notification_local_var->plmn_ue_policy_set = plmn_ue_policy_set;
    policy_data_change_notification_local_var->sm_policy_data = sm_policy_data;
    policy_data_change_notification_local_var->usage_mon_data = usage_mon_data;
    policy_data_change_notification_local_var->sponsor_connectivity_data = sponsor_connectivity_data;
    policy_data_change_notification_local_var->bdt_data = bdt_data;
    policy_data_change_notification_local_var->op_spec_data = op_spec_data;
    policy_data_change_notification_local_var->op_spec_data_map = op_spec_data_map;
    policy_data_change_notification_local_var->ue_id = ue_id;
    policy_data_change_notification_local_var->sponsor_id = sponsor_id;
    policy_data_change_notification_local_var->bdt_ref_id = bdt_ref_id;
    policy_data_change_notification_local_var->usage_mon_id = usage_mon_id;
    policy_data_change_notification_local_var->plmn_id = plmn_id;
    policy_data_change_notification_local_var->del_resources = del_resources;
    policy_data_change_notification_local_var->notif_id = notif_id;
    policy_data_change_notification_local_var->reported_fragments = reported_fragments;
    policy_data_change_notification_local_var->slice_policy_data = slice_policy_data;
    policy_data_change_notification_local_var->snssai = snssai;

    return policy_data_change_notification_local_var;
}

void OpenAPI_policy_data_change_notification_free(OpenAPI_policy_data_change_notification_t *policy_data_change_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_data_change_notification) {
        return;
    }
    if (policy_data_change_notification->am_policy_data) {
        OpenAPI_am_policy_data_free(policy_data_change_notification->am_policy_data);
        policy_data_change_notification->am_policy_data = NULL;
    }
    if (policy_data_change_notification->ue_policy_set) {
        OpenAPI_ue_policy_set_free(policy_data_change_notification->ue_policy_set);
        policy_data_change_notification->ue_policy_set = NULL;
    }
    if (policy_data_change_notification->plmn_ue_policy_set) {
        OpenAPI_ue_policy_set_free(policy_data_change_notification->plmn_ue_policy_set);
        policy_data_change_notification->plmn_ue_policy_set = NULL;
    }
    if (policy_data_change_notification->sm_policy_data) {
        OpenAPI_sm_policy_data_free(policy_data_change_notification->sm_policy_data);
        policy_data_change_notification->sm_policy_data = NULL;
    }
    if (policy_data_change_notification->usage_mon_data) {
        OpenAPI_usage_mon_data_free(policy_data_change_notification->usage_mon_data);
        policy_data_change_notification->usage_mon_data = NULL;
    }
    if (policy_data_change_notification->sponsor_connectivity_data) {
        OpenAPI_sponsor_connectivity_data_free(policy_data_change_notification->sponsor_connectivity_data);
        policy_data_change_notification->sponsor_connectivity_data = NULL;
    }
    if (policy_data_change_notification->bdt_data) {
        OpenAPI_bdt_data_free(policy_data_change_notification->bdt_data);
        policy_data_change_notification->bdt_data = NULL;
    }
    if (policy_data_change_notification->op_spec_data) {
        OpenAPI_operator_specific_data_container_free(policy_data_change_notification->op_spec_data);
        policy_data_change_notification->op_spec_data = NULL;
    }
    if (policy_data_change_notification->op_spec_data_map) {
        OpenAPI_list_for_each(policy_data_change_notification->op_spec_data_map, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_operator_specific_data_container_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_data_change_notification->op_spec_data_map);
        policy_data_change_notification->op_spec_data_map = NULL;
    }
    if (policy_data_change_notification->ue_id) {
        ogs_free(policy_data_change_notification->ue_id);
        policy_data_change_notification->ue_id = NULL;
    }
    if (policy_data_change_notification->sponsor_id) {
        ogs_free(policy_data_change_notification->sponsor_id);
        policy_data_change_notification->sponsor_id = NULL;
    }
    if (policy_data_change_notification->bdt_ref_id) {
        ogs_free(policy_data_change_notification->bdt_ref_id);
        policy_data_change_notification->bdt_ref_id = NULL;
    }
    if (policy_data_change_notification->usage_mon_id) {
        ogs_free(policy_data_change_notification->usage_mon_id);
        policy_data_change_notification->usage_mon_id = NULL;
    }
    if (policy_data_change_notification->plmn_id) {
        OpenAPI_plmn_id_1_free(policy_data_change_notification->plmn_id);
        policy_data_change_notification->plmn_id = NULL;
    }
    if (policy_data_change_notification->del_resources) {
        OpenAPI_list_for_each(policy_data_change_notification->del_resources, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(policy_data_change_notification->del_resources);
        policy_data_change_notification->del_resources = NULL;
    }
    if (policy_data_change_notification->notif_id) {
        ogs_free(policy_data_change_notification->notif_id);
        policy_data_change_notification->notif_id = NULL;
    }
    if (policy_data_change_notification->reported_fragments) {
        OpenAPI_list_for_each(policy_data_change_notification->reported_fragments, node) {
            OpenAPI_notification_item_free(node->data);
        }
        OpenAPI_list_free(policy_data_change_notification->reported_fragments);
        policy_data_change_notification->reported_fragments = NULL;
    }
    if (policy_data_change_notification->slice_policy_data) {
        OpenAPI_slice_policy_data_free(policy_data_change_notification->slice_policy_data);
        policy_data_change_notification->slice_policy_data = NULL;
    }
    if (policy_data_change_notification->snssai) {
        OpenAPI_snssai_free(policy_data_change_notification->snssai);
        policy_data_change_notification->snssai = NULL;
    }
    ogs_free(policy_data_change_notification);
}

cJSON *OpenAPI_policy_data_change_notification_convertToJSON(OpenAPI_policy_data_change_notification_t *policy_data_change_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (policy_data_change_notification == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [PolicyDataChangeNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (policy_data_change_notification->am_policy_data) {
    cJSON *am_policy_data_local_JSON = OpenAPI_am_policy_data_convertToJSON(policy_data_change_notification->am_policy_data);
    if (am_policy_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [am_policy_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "amPolicyData", am_policy_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [am_policy_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->ue_policy_set) {
    cJSON *ue_policy_set_local_JSON = OpenAPI_ue_policy_set_convertToJSON(policy_data_change_notification->ue_policy_set);
    if (ue_policy_set_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [ue_policy_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uePolicySet", ue_policy_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [ue_policy_set]");
        goto end;
    }
    }

    if (policy_data_change_notification->plmn_ue_policy_set) {
    cJSON *plmn_ue_policy_set_local_JSON = OpenAPI_ue_policy_set_convertToJSON(policy_data_change_notification->plmn_ue_policy_set);
    if (plmn_ue_policy_set_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [plmn_ue_policy_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnUePolicySet", plmn_ue_policy_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [plmn_ue_policy_set]");
        goto end;
    }
    }

    if (policy_data_change_notification->sm_policy_data) {
    cJSON *sm_policy_data_local_JSON = OpenAPI_sm_policy_data_convertToJSON(policy_data_change_notification->sm_policy_data);
    if (sm_policy_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [sm_policy_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smPolicyData", sm_policy_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [sm_policy_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->usage_mon_data) {
    cJSON *usage_mon_data_local_JSON = OpenAPI_usage_mon_data_convertToJSON(policy_data_change_notification->usage_mon_data);
    if (usage_mon_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [usage_mon_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usageMonData", usage_mon_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [usage_mon_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->sponsor_connectivity_data) {
    cJSON *sponsor_connectivity_data_local_JSON = OpenAPI_sponsor_connectivity_data_convertToJSON(policy_data_change_notification->sponsor_connectivity_data);
    if (sponsor_connectivity_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [sponsor_connectivity_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "SponsorConnectivityData", sponsor_connectivity_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [sponsor_connectivity_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->bdt_data) {
    cJSON *bdt_data_local_JSON = OpenAPI_bdt_data_convertToJSON(policy_data_change_notification->bdt_data);
    if (bdt_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [bdt_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "bdtData", bdt_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [bdt_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->op_spec_data) {
    cJSON *op_spec_data_local_JSON = OpenAPI_operator_specific_data_container_convertToJSON(policy_data_change_notification->op_spec_data);
    if (op_spec_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [op_spec_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "opSpecData", op_spec_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [op_spec_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->op_spec_data_map) {
    cJSON *op_spec_data_map = cJSON_AddObjectToObject(item, "opSpecDataMap");
    if (op_spec_data_map == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [op_spec_data_map]");
        goto end;
    }
    cJSON *localMapObject = op_spec_data_map;
    if (policy_data_change_notification->op_spec_data_map) {
        OpenAPI_list_for_each(policy_data_change_notification->op_spec_data_map, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [op_spec_data_map]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [op_spec_data_map]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_operator_specific_data_container_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (policy_data_change_notification->ue_id) {
    if (cJSON_AddStringToObject(item, "ueId", policy_data_change_notification->ue_id) == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [ue_id]");
        goto end;
    }
    }

    if (policy_data_change_notification->sponsor_id) {
    if (cJSON_AddStringToObject(item, "sponsorId", policy_data_change_notification->sponsor_id) == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [sponsor_id]");
        goto end;
    }
    }

    if (policy_data_change_notification->bdt_ref_id) {
    if (cJSON_AddStringToObject(item, "bdtRefId", policy_data_change_notification->bdt_ref_id) == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    if (policy_data_change_notification->usage_mon_id) {
    if (cJSON_AddStringToObject(item, "usageMonId", policy_data_change_notification->usage_mon_id) == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [usage_mon_id]");
        goto end;
    }
    }

    if (policy_data_change_notification->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(policy_data_change_notification->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (policy_data_change_notification->del_resources) {
    cJSON *del_resourcesList = cJSON_AddArrayToObject(item, "delResources");
    if (del_resourcesList == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [del_resources]");
        goto end;
    }
    OpenAPI_list_for_each(policy_data_change_notification->del_resources, node) {
        if (cJSON_AddStringToObject(del_resourcesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [del_resources]");
            goto end;
        }
    }
    }

    if (policy_data_change_notification->notif_id) {
    if (cJSON_AddStringToObject(item, "notifId", policy_data_change_notification->notif_id) == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [notif_id]");
        goto end;
    }
    }

    if (policy_data_change_notification->reported_fragments) {
    cJSON *reported_fragmentsList = cJSON_AddArrayToObject(item, "reportedFragments");
    if (reported_fragmentsList == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [reported_fragments]");
        goto end;
    }
    OpenAPI_list_for_each(policy_data_change_notification->reported_fragments, node) {
        cJSON *itemLocal = OpenAPI_notification_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [reported_fragments]");
            goto end;
        }
        cJSON_AddItemToArray(reported_fragmentsList, itemLocal);
    }
    }

    if (policy_data_change_notification->slice_policy_data) {
    cJSON *slice_policy_data_local_JSON = OpenAPI_slice_policy_data_convertToJSON(policy_data_change_notification->slice_policy_data);
    if (slice_policy_data_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [slice_policy_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "slicePolicyData", slice_policy_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [slice_policy_data]");
        goto end;
    }
    }

    if (policy_data_change_notification->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(policy_data_change_notification->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_parseFromJSON(cJSON *policy_data_change_notificationJSON)
{
    OpenAPI_policy_data_change_notification_t *policy_data_change_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *am_policy_data = NULL;
    OpenAPI_am_policy_data_t *am_policy_data_local_nonprim = NULL;
    cJSON *ue_policy_set = NULL;
    OpenAPI_ue_policy_set_t *ue_policy_set_local_nonprim = NULL;
    cJSON *plmn_ue_policy_set = NULL;
    OpenAPI_ue_policy_set_t *plmn_ue_policy_set_local_nonprim = NULL;
    cJSON *sm_policy_data = NULL;
    OpenAPI_sm_policy_data_t *sm_policy_data_local_nonprim = NULL;
    cJSON *usage_mon_data = NULL;
    OpenAPI_usage_mon_data_t *usage_mon_data_local_nonprim = NULL;
    cJSON *sponsor_connectivity_data = NULL;
    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data_local_nonprim = NULL;
    cJSON *bdt_data = NULL;
    OpenAPI_bdt_data_t *bdt_data_local_nonprim = NULL;
    cJSON *op_spec_data = NULL;
    OpenAPI_operator_specific_data_container_t *op_spec_data_local_nonprim = NULL;
    cJSON *op_spec_data_map = NULL;
    OpenAPI_list_t *op_spec_data_mapList = NULL;
    cJSON *ue_id = NULL;
    cJSON *sponsor_id = NULL;
    cJSON *bdt_ref_id = NULL;
    cJSON *usage_mon_id = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    cJSON *del_resources = NULL;
    OpenAPI_list_t *del_resourcesList = NULL;
    cJSON *notif_id = NULL;
    cJSON *reported_fragments = NULL;
    OpenAPI_list_t *reported_fragmentsList = NULL;
    cJSON *slice_policy_data = NULL;
    OpenAPI_slice_policy_data_t *slice_policy_data_local_nonprim = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    am_policy_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "amPolicyData");
    if (am_policy_data) {
    am_policy_data_local_nonprim = OpenAPI_am_policy_data_parseFromJSON(am_policy_data);
    if (!am_policy_data_local_nonprim) {
        ogs_error("OpenAPI_am_policy_data_parseFromJSON failed [am_policy_data]");
        goto end;
    }
    }

    ue_policy_set = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "uePolicySet");
    if (ue_policy_set) {
    ue_policy_set_local_nonprim = OpenAPI_ue_policy_set_parseFromJSON(ue_policy_set);
    if (!ue_policy_set_local_nonprim) {
        ogs_error("OpenAPI_ue_policy_set_parseFromJSON failed [ue_policy_set]");
        goto end;
    }
    }

    plmn_ue_policy_set = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "plmnUePolicySet");
    if (plmn_ue_policy_set) {
    plmn_ue_policy_set_local_nonprim = OpenAPI_ue_policy_set_parseFromJSON(plmn_ue_policy_set);
    if (!plmn_ue_policy_set_local_nonprim) {
        ogs_error("OpenAPI_ue_policy_set_parseFromJSON failed [plmn_ue_policy_set]");
        goto end;
    }
    }

    sm_policy_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "smPolicyData");
    if (sm_policy_data) {
    sm_policy_data_local_nonprim = OpenAPI_sm_policy_data_parseFromJSON(sm_policy_data);
    if (!sm_policy_data_local_nonprim) {
        ogs_error("OpenAPI_sm_policy_data_parseFromJSON failed [sm_policy_data]");
        goto end;
    }
    }

    usage_mon_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "usageMonData");
    if (usage_mon_data) {
    usage_mon_data_local_nonprim = OpenAPI_usage_mon_data_parseFromJSON(usage_mon_data);
    if (!usage_mon_data_local_nonprim) {
        ogs_error("OpenAPI_usage_mon_data_parseFromJSON failed [usage_mon_data]");
        goto end;
    }
    }

    sponsor_connectivity_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "SponsorConnectivityData");
    if (sponsor_connectivity_data) {
    sponsor_connectivity_data_local_nonprim = OpenAPI_sponsor_connectivity_data_parseFromJSON(sponsor_connectivity_data);
    if (!sponsor_connectivity_data_local_nonprim) {
        ogs_error("OpenAPI_sponsor_connectivity_data_parseFromJSON failed [sponsor_connectivity_data]");
        goto end;
    }
    }

    bdt_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "bdtData");
    if (bdt_data) {
    bdt_data_local_nonprim = OpenAPI_bdt_data_parseFromJSON(bdt_data);
    if (!bdt_data_local_nonprim) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON failed [bdt_data]");
        goto end;
    }
    }

    op_spec_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "opSpecData");
    if (op_spec_data) {
    op_spec_data_local_nonprim = OpenAPI_operator_specific_data_container_parseFromJSON(op_spec_data);
    if (!op_spec_data_local_nonprim) {
        ogs_error("OpenAPI_operator_specific_data_container_parseFromJSON failed [op_spec_data]");
        goto end;
    }
    }

    op_spec_data_map = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "opSpecDataMap");
    if (op_spec_data_map) {
        cJSON *op_spec_data_map_local_map = NULL;
        if (!cJSON_IsObject(op_spec_data_map) && !cJSON_IsNull(op_spec_data_map)) {
            ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [op_spec_data_map]");
            goto end;
        }
        if (cJSON_IsObject(op_spec_data_map)) {
            op_spec_data_mapList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(op_spec_data_map_local_map, op_spec_data_map) {
                cJSON *localMapObject = op_spec_data_map_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_operator_specific_data_container_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(op_spec_data_mapList, localMapKeyPair);
            }
        }
    }

    ue_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "ueId");
    if (ue_id) {
    if (!cJSON_IsString(ue_id) && !cJSON_IsNull(ue_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [ue_id]");
        goto end;
    }
    }

    sponsor_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "sponsorId");
    if (sponsor_id) {
    if (!cJSON_IsString(sponsor_id) && !cJSON_IsNull(sponsor_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [sponsor_id]");
        goto end;
    }
    }

    bdt_ref_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "bdtRefId");
    if (bdt_ref_id) {
    if (!cJSON_IsString(bdt_ref_id) && !cJSON_IsNull(bdt_ref_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    usage_mon_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "usageMonId");
    if (usage_mon_id) {
    if (!cJSON_IsString(usage_mon_id) && !cJSON_IsNull(usage_mon_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [usage_mon_id]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    del_resources = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "delResources");
    if (del_resources) {
        cJSON *del_resources_local = NULL;
        if (!cJSON_IsArray(del_resources)) {
            ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [del_resources]");
            goto end;
        }

        del_resourcesList = OpenAPI_list_create();

        cJSON_ArrayForEach(del_resources_local, del_resources) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(del_resources_local)) {
                ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [del_resources]");
                goto end;
            }
            OpenAPI_list_add(del_resourcesList, ogs_strdup(del_resources_local->valuestring));
        }
    }

    notif_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "notifId");
    if (notif_id) {
    if (!cJSON_IsString(notif_id) && !cJSON_IsNull(notif_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [notif_id]");
        goto end;
    }
    }

    reported_fragments = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "reportedFragments");
    if (reported_fragments) {
        cJSON *reported_fragments_local = NULL;
        if (!cJSON_IsArray(reported_fragments)) {
            ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [reported_fragments]");
            goto end;
        }

        reported_fragmentsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reported_fragments_local, reported_fragments) {
            if (!cJSON_IsObject(reported_fragments_local)) {
                ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [reported_fragments]");
                goto end;
            }
            OpenAPI_notification_item_t *reported_fragmentsItem = OpenAPI_notification_item_parseFromJSON(reported_fragments_local);
            if (!reported_fragmentsItem) {
                ogs_error("No reported_fragmentsItem");
                goto end;
            }
            OpenAPI_list_add(reported_fragmentsList, reported_fragmentsItem);
        }
    }

    slice_policy_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "slicePolicyData");
    if (slice_policy_data) {
    slice_policy_data_local_nonprim = OpenAPI_slice_policy_data_parseFromJSON(slice_policy_data);
    if (!slice_policy_data_local_nonprim) {
        ogs_error("OpenAPI_slice_policy_data_parseFromJSON failed [slice_policy_data]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    policy_data_change_notification_local_var = OpenAPI_policy_data_change_notification_create (
        am_policy_data ? am_policy_data_local_nonprim : NULL,
        ue_policy_set ? ue_policy_set_local_nonprim : NULL,
        plmn_ue_policy_set ? plmn_ue_policy_set_local_nonprim : NULL,
        sm_policy_data ? sm_policy_data_local_nonprim : NULL,
        usage_mon_data ? usage_mon_data_local_nonprim : NULL,
        sponsor_connectivity_data ? sponsor_connectivity_data_local_nonprim : NULL,
        bdt_data ? bdt_data_local_nonprim : NULL,
        op_spec_data ? op_spec_data_local_nonprim : NULL,
        op_spec_data_map ? op_spec_data_mapList : NULL,
        ue_id && !cJSON_IsNull(ue_id) ? ogs_strdup(ue_id->valuestring) : NULL,
        sponsor_id && !cJSON_IsNull(sponsor_id) ? ogs_strdup(sponsor_id->valuestring) : NULL,
        bdt_ref_id && !cJSON_IsNull(bdt_ref_id) ? ogs_strdup(bdt_ref_id->valuestring) : NULL,
        usage_mon_id && !cJSON_IsNull(usage_mon_id) ? ogs_strdup(usage_mon_id->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        del_resources ? del_resourcesList : NULL,
        notif_id && !cJSON_IsNull(notif_id) ? ogs_strdup(notif_id->valuestring) : NULL,
        reported_fragments ? reported_fragmentsList : NULL,
        slice_policy_data ? slice_policy_data_local_nonprim : NULL,
        snssai ? snssai_local_nonprim : NULL
    );

    return policy_data_change_notification_local_var;
end:
    if (am_policy_data_local_nonprim) {
        OpenAPI_am_policy_data_free(am_policy_data_local_nonprim);
        am_policy_data_local_nonprim = NULL;
    }
    if (ue_policy_set_local_nonprim) {
        OpenAPI_ue_policy_set_free(ue_policy_set_local_nonprim);
        ue_policy_set_local_nonprim = NULL;
    }
    if (plmn_ue_policy_set_local_nonprim) {
        OpenAPI_ue_policy_set_free(plmn_ue_policy_set_local_nonprim);
        plmn_ue_policy_set_local_nonprim = NULL;
    }
    if (sm_policy_data_local_nonprim) {
        OpenAPI_sm_policy_data_free(sm_policy_data_local_nonprim);
        sm_policy_data_local_nonprim = NULL;
    }
    if (usage_mon_data_local_nonprim) {
        OpenAPI_usage_mon_data_free(usage_mon_data_local_nonprim);
        usage_mon_data_local_nonprim = NULL;
    }
    if (sponsor_connectivity_data_local_nonprim) {
        OpenAPI_sponsor_connectivity_data_free(sponsor_connectivity_data_local_nonprim);
        sponsor_connectivity_data_local_nonprim = NULL;
    }
    if (bdt_data_local_nonprim) {
        OpenAPI_bdt_data_free(bdt_data_local_nonprim);
        bdt_data_local_nonprim = NULL;
    }
    if (op_spec_data_local_nonprim) {
        OpenAPI_operator_specific_data_container_free(op_spec_data_local_nonprim);
        op_spec_data_local_nonprim = NULL;
    }
    if (op_spec_data_mapList) {
        OpenAPI_list_for_each(op_spec_data_mapList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_operator_specific_data_container_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(op_spec_data_mapList);
        op_spec_data_mapList = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (del_resourcesList) {
        OpenAPI_list_for_each(del_resourcesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(del_resourcesList);
        del_resourcesList = NULL;
    }
    if (reported_fragmentsList) {
        OpenAPI_list_for_each(reported_fragmentsList, node) {
            OpenAPI_notification_item_free(node->data);
        }
        OpenAPI_list_free(reported_fragmentsList);
        reported_fragmentsList = NULL;
    }
    if (slice_policy_data_local_nonprim) {
        OpenAPI_slice_policy_data_free(slice_policy_data_local_nonprim);
        slice_policy_data_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_copy(OpenAPI_policy_data_change_notification_t *dst, OpenAPI_policy_data_change_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_data_change_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed");
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

    OpenAPI_policy_data_change_notification_free(dst);
    dst = OpenAPI_policy_data_change_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

