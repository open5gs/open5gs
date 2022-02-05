
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
    OpenAPI_list_t *reported_fragments
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

    return policy_data_change_notification_local_var;
}

void OpenAPI_policy_data_change_notification_free(OpenAPI_policy_data_change_notification_t *policy_data_change_notification)
{
    if (NULL == policy_data_change_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_am_policy_data_free(policy_data_change_notification->am_policy_data);
    OpenAPI_ue_policy_set_free(policy_data_change_notification->ue_policy_set);
    OpenAPI_ue_policy_set_free(policy_data_change_notification->plmn_ue_policy_set);
    OpenAPI_sm_policy_data_free(policy_data_change_notification->sm_policy_data);
    OpenAPI_usage_mon_data_free(policy_data_change_notification->usage_mon_data);
    OpenAPI_sponsor_connectivity_data_free(policy_data_change_notification->sponsor_connectivity_data);
    OpenAPI_bdt_data_free(policy_data_change_notification->bdt_data);
    OpenAPI_operator_specific_data_container_free(policy_data_change_notification->op_spec_data);
    OpenAPI_list_for_each(policy_data_change_notification->op_spec_data_map, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_operator_specific_data_container_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(policy_data_change_notification->op_spec_data_map);
    ogs_free(policy_data_change_notification->ue_id);
    ogs_free(policy_data_change_notification->sponsor_id);
    ogs_free(policy_data_change_notification->bdt_ref_id);
    ogs_free(policy_data_change_notification->usage_mon_id);
    OpenAPI_plmn_id_1_free(policy_data_change_notification->plmn_id);
    OpenAPI_list_for_each(policy_data_change_notification->del_resources, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(policy_data_change_notification->del_resources);
    ogs_free(policy_data_change_notification->notif_id);
    OpenAPI_list_for_each(policy_data_change_notification->reported_fragments, node) {
        OpenAPI_notification_item_free(node->data);
    }
    OpenAPI_list_free(policy_data_change_notification->reported_fragments);
    ogs_free(policy_data_change_notification);
}

cJSON *OpenAPI_policy_data_change_notification_convertToJSON(OpenAPI_policy_data_change_notification_t *policy_data_change_notification)
{
    cJSON *item = NULL;

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
    OpenAPI_lnode_t *op_spec_data_map_node;
    if (policy_data_change_notification->op_spec_data_map) {
        OpenAPI_list_for_each(policy_data_change_notification->op_spec_data_map, op_spec_data_map_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)op_spec_data_map_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_operator_specific_data_container_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [op_spec_data_map]");
            goto end;
        }
        cJSON_AddItemToObject(op_spec_data_map, localKeyValue->key, itemLocal);
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
    cJSON *del_resources = cJSON_AddArrayToObject(item, "delResources");
    if (del_resources == NULL) {
        ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [del_resources]");
        goto end;
    }

    OpenAPI_lnode_t *del_resources_node;
    OpenAPI_list_for_each(policy_data_change_notification->del_resources, del_resources_node)  {
    if (cJSON_AddStringToObject(del_resources, "", (char*)del_resources_node->data) == NULL) {
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

    OpenAPI_lnode_t *reported_fragments_node;
    if (policy_data_change_notification->reported_fragments) {
        OpenAPI_list_for_each(policy_data_change_notification->reported_fragments, reported_fragments_node) {
            cJSON *itemLocal = OpenAPI_notification_item_convertToJSON(reported_fragments_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_data_change_notification_convertToJSON() failed [reported_fragments]");
                goto end;
            }
            cJSON_AddItemToArray(reported_fragmentsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_policy_data_change_notification_t *OpenAPI_policy_data_change_notification_parseFromJSON(cJSON *policy_data_change_notificationJSON)
{
    OpenAPI_policy_data_change_notification_t *policy_data_change_notification_local_var = NULL;
    cJSON *am_policy_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "amPolicyData");

    OpenAPI_am_policy_data_t *am_policy_data_local_nonprim = NULL;
    if (am_policy_data) {
    am_policy_data_local_nonprim = OpenAPI_am_policy_data_parseFromJSON(am_policy_data);
    }

    cJSON *ue_policy_set = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "uePolicySet");

    OpenAPI_ue_policy_set_t *ue_policy_set_local_nonprim = NULL;
    if (ue_policy_set) {
    ue_policy_set_local_nonprim = OpenAPI_ue_policy_set_parseFromJSON(ue_policy_set);
    }

    cJSON *plmn_ue_policy_set = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "plmnUePolicySet");

    OpenAPI_ue_policy_set_t *plmn_ue_policy_set_local_nonprim = NULL;
    if (plmn_ue_policy_set) {
    plmn_ue_policy_set_local_nonprim = OpenAPI_ue_policy_set_parseFromJSON(plmn_ue_policy_set);
    }

    cJSON *sm_policy_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "smPolicyData");

    OpenAPI_sm_policy_data_t *sm_policy_data_local_nonprim = NULL;
    if (sm_policy_data) {
    sm_policy_data_local_nonprim = OpenAPI_sm_policy_data_parseFromJSON(sm_policy_data);
    }

    cJSON *usage_mon_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "usageMonData");

    OpenAPI_usage_mon_data_t *usage_mon_data_local_nonprim = NULL;
    if (usage_mon_data) {
    usage_mon_data_local_nonprim = OpenAPI_usage_mon_data_parseFromJSON(usage_mon_data);
    }

    cJSON *sponsor_connectivity_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "SponsorConnectivityData");

    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data_local_nonprim = NULL;
    if (sponsor_connectivity_data) {
    sponsor_connectivity_data_local_nonprim = OpenAPI_sponsor_connectivity_data_parseFromJSON(sponsor_connectivity_data);
    }

    cJSON *bdt_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "bdtData");

    OpenAPI_bdt_data_t *bdt_data_local_nonprim = NULL;
    if (bdt_data) {
    bdt_data_local_nonprim = OpenAPI_bdt_data_parseFromJSON(bdt_data);
    }

    cJSON *op_spec_data = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "opSpecData");

    OpenAPI_operator_specific_data_container_t *op_spec_data_local_nonprim = NULL;
    if (op_spec_data) {
    op_spec_data_local_nonprim = OpenAPI_operator_specific_data_container_parseFromJSON(op_spec_data);
    }

    cJSON *op_spec_data_map = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "opSpecDataMap");

    OpenAPI_list_t *op_spec_data_mapList;
    if (op_spec_data_map) {
    cJSON *op_spec_data_map_local_map;
    if (!cJSON_IsObject(op_spec_data_map)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [op_spec_data_map]");
        goto end;
    }
    op_spec_data_mapList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(op_spec_data_map_local_map, op_spec_data_map) {
        cJSON *localMapObject = op_spec_data_map_local_map;
        if (cJSON_IsObject(op_spec_data_map_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_operator_specific_data_container_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(op_spec_data_map_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [op_spec_data_map]");
            goto end;
        }
        OpenAPI_list_add(op_spec_data_mapList , localMapKeyPair);
    }
    }

    cJSON *ue_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "ueId");

    if (ue_id) {
    if (!cJSON_IsString(ue_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [ue_id]");
        goto end;
    }
    }

    cJSON *sponsor_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "sponsorId");

    if (sponsor_id) {
    if (!cJSON_IsString(sponsor_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [sponsor_id]");
        goto end;
    }
    }

    cJSON *bdt_ref_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "bdtRefId");

    if (bdt_ref_id) {
    if (!cJSON_IsString(bdt_ref_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    cJSON *usage_mon_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "usageMonId");

    if (usage_mon_id) {
    if (!cJSON_IsString(usage_mon_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [usage_mon_id]");
        goto end;
    }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "plmnId");

    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    }

    cJSON *del_resources = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "delResources");

    OpenAPI_list_t *del_resourcesList;
    if (del_resources) {
    cJSON *del_resources_local;
    if (!cJSON_IsArray(del_resources)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [del_resources]");
        goto end;
    }
    del_resourcesList = OpenAPI_list_create();

    cJSON_ArrayForEach(del_resources_local, del_resources) {
    if (!cJSON_IsString(del_resources_local)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [del_resources]");
        goto end;
    }
    OpenAPI_list_add(del_resourcesList , ogs_strdup(del_resources_local->valuestring));
    }
    }

    cJSON *notif_id = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "notifId");

    if (notif_id) {
    if (!cJSON_IsString(notif_id)) {
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [notif_id]");
        goto end;
    }
    }

    cJSON *reported_fragments = cJSON_GetObjectItemCaseSensitive(policy_data_change_notificationJSON, "reportedFragments");

    OpenAPI_list_t *reported_fragmentsList;
    if (reported_fragments) {
    cJSON *reported_fragments_local_nonprimitive;
    if (!cJSON_IsArray(reported_fragments)){
        ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [reported_fragments]");
        goto end;
    }

    reported_fragmentsList = OpenAPI_list_create();

    cJSON_ArrayForEach(reported_fragments_local_nonprimitive, reported_fragments ) {
        if (!cJSON_IsObject(reported_fragments_local_nonprimitive)) {
            ogs_error("OpenAPI_policy_data_change_notification_parseFromJSON() failed [reported_fragments]");
            goto end;
        }
        OpenAPI_notification_item_t *reported_fragmentsItem = OpenAPI_notification_item_parseFromJSON(reported_fragments_local_nonprimitive);

        if (!reported_fragmentsItem) {
            ogs_error("No reported_fragmentsItem");
            OpenAPI_list_free(reported_fragmentsList);
            goto end;
        }

        OpenAPI_list_add(reported_fragmentsList, reported_fragmentsItem);
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
        ue_id ? ogs_strdup(ue_id->valuestring) : NULL,
        sponsor_id ? ogs_strdup(sponsor_id->valuestring) : NULL,
        bdt_ref_id ? ogs_strdup(bdt_ref_id->valuestring) : NULL,
        usage_mon_id ? ogs_strdup(usage_mon_id->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        del_resources ? del_resourcesList : NULL,
        notif_id ? ogs_strdup(notif_id->valuestring) : NULL,
        reported_fragments ? reported_fragmentsList : NULL
    );

    return policy_data_change_notification_local_var;
end:
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

