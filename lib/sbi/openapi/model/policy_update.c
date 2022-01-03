
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_update.h"

OpenAPI_policy_update_t *OpenAPI_policy_update_create(
    char *resource_uri,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    bool is_rfsp,
    int rfsp,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_list_t* pras
)
{
    OpenAPI_policy_update_t *policy_update_local_var = ogs_malloc(sizeof(OpenAPI_policy_update_t));
    ogs_assert(policy_update_local_var);

    policy_update_local_var->resource_uri = resource_uri;
    policy_update_local_var->triggers = triggers;
    policy_update_local_var->serv_area_res = serv_area_res;
    policy_update_local_var->wl_serv_area_res = wl_serv_area_res;
    policy_update_local_var->is_rfsp = is_rfsp;
    policy_update_local_var->rfsp = rfsp;
    policy_update_local_var->smf_sel_info = smf_sel_info;
    policy_update_local_var->ue_ambr = ue_ambr;
    policy_update_local_var->pras = pras;

    return policy_update_local_var;
}

void OpenAPI_policy_update_free(OpenAPI_policy_update_t *policy_update)
{
    if (NULL == policy_update) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(policy_update->resource_uri);
    OpenAPI_list_free(policy_update->triggers);
    OpenAPI_service_area_restriction_free(policy_update->serv_area_res);
    OpenAPI_wireline_service_area_restriction_free(policy_update->wl_serv_area_res);
    OpenAPI_smf_selection_data_free(policy_update->smf_sel_info);
    OpenAPI_ambr_free(policy_update->ue_ambr);
    OpenAPI_list_for_each(policy_update->pras, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_presence_info_rm_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(policy_update->pras);
    ogs_free(policy_update);
}

cJSON *OpenAPI_policy_update_convertToJSON(OpenAPI_policy_update_t *policy_update)
{
    cJSON *item = NULL;

    if (policy_update == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [PolicyUpdate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "resourceUri", policy_update->resource_uri) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [resource_uri]");
        goto end;
    }

    if (policy_update->triggers) {
    cJSON *triggers = cJSON_AddArrayToObject(item, "triggers");
    if (triggers == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_lnode_t *triggers_node;
    OpenAPI_list_for_each(policy_update->triggers, triggers_node) {
        if (cJSON_AddStringToObject(triggers, "", OpenAPI_request_trigger_ToString((intptr_t)triggers_node->data)) == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [triggers]");
            goto end;
        }
    }
    }

    if (policy_update->serv_area_res) {
    cJSON *serv_area_res_local_JSON = OpenAPI_service_area_restriction_convertToJSON(policy_update->serv_area_res);
    if (serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servAreaRes", serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    }

    if (policy_update->wl_serv_area_res) {
    cJSON *wl_serv_area_res_local_JSON = OpenAPI_wireline_service_area_restriction_convertToJSON(policy_update->wl_serv_area_res);
    if (wl_serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wlServAreaRes", wl_serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    }

    if (policy_update->is_rfsp) {
    if (cJSON_AddNumberToObject(item, "rfsp", policy_update->rfsp) == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [rfsp]");
        goto end;
    }
    }

    if (policy_update->smf_sel_info) {
    cJSON *smf_sel_info_local_JSON = OpenAPI_smf_selection_data_convertToJSON(policy_update->smf_sel_info);
    if (smf_sel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelInfo", smf_sel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    }

    if (policy_update->ue_ambr) {
    cJSON *ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(policy_update->ue_ambr);
    if (ue_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueAmbr", ue_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    }

    if (policy_update->pras) {
    cJSON *pras = cJSON_AddObjectToObject(item, "pras");
    if (pras == NULL) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed [pras]");
        goto end;
    }
    cJSON *localMapObject = pras;
    OpenAPI_lnode_t *pras_node;
    if (policy_update->pras) {
        OpenAPI_list_for_each(policy_update->pras, pras_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pras_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_presence_info_rm_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_update_convertToJSON() failed [pras]");
            goto end;
        }
        cJSON_AddItemToObject(pras, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_policy_update_t *OpenAPI_policy_update_parseFromJSON(cJSON *policy_updateJSON)
{
    OpenAPI_policy_update_t *policy_update_local_var = NULL;
    cJSON *resource_uri = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "resourceUri");
    if (!resource_uri) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [resource_uri]");
        goto end;
    }

    if (!cJSON_IsString(resource_uri)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [resource_uri]");
        goto end;
    }

    cJSON *triggers = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "triggers");

    OpenAPI_list_t *triggersList;
    if (triggers) {
    cJSON *triggers_local_nonprimitive;
    if (!cJSON_IsArray(triggers)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [triggers]");
        goto end;
    }

    triggersList = OpenAPI_list_create();

    cJSON_ArrayForEach(triggers_local_nonprimitive, triggers ) {
        if (!cJSON_IsString(triggers_local_nonprimitive)){
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [triggers]");
            goto end;
        }

        OpenAPI_list_add(triggersList, (void *)OpenAPI_request_trigger_FromString(triggers_local_nonprimitive->valuestring));
    }
    }

    cJSON *serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "servAreaRes");

    OpenAPI_service_area_restriction_t *serv_area_res_local_nonprim = NULL;
    if (serv_area_res) {
    serv_area_res_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(serv_area_res);
    }

    cJSON *wl_serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "wlServAreaRes");

    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res_local_nonprim = NULL;
    if (wl_serv_area_res) {
    wl_serv_area_res_local_nonprim = OpenAPI_wireline_service_area_restriction_parseFromJSON(wl_serv_area_res);
    }

    cJSON *rfsp = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "rfsp");

    if (rfsp) {
    if (!cJSON_IsNumber(rfsp)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [rfsp]");
        goto end;
    }
    }

    cJSON *smf_sel_info = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "smfSelInfo");

    OpenAPI_smf_selection_data_t *smf_sel_info_local_nonprim = NULL;
    if (smf_sel_info) {
    smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    }

    cJSON *ue_ambr = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "ueAmbr");

    OpenAPI_ambr_t *ue_ambr_local_nonprim = NULL;
    if (ue_ambr) {
    ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(ue_ambr);
    }

    cJSON *pras = cJSON_GetObjectItemCaseSensitive(policy_updateJSON, "pras");

    OpenAPI_list_t *prasList;
    if (pras) {
    cJSON *pras_local_map;
    if (!cJSON_IsObject(pras)) {
        ogs_error("OpenAPI_policy_update_parseFromJSON() failed [pras]");
        goto end;
    }
    prasList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(pras_local_map, pras) {
        cJSON *localMapObject = pras_local_map;
        if (cJSON_IsObject(pras_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_presence_info_rm_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(pras_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_policy_update_parseFromJSON() failed [pras]");
            goto end;
        }
        OpenAPI_list_add(prasList , localMapKeyPair);
    }
    }

    policy_update_local_var = OpenAPI_policy_update_create (
        ogs_strdup(resource_uri->valuestring),
        triggers ? triggersList : NULL,
        serv_area_res ? serv_area_res_local_nonprim : NULL,
        wl_serv_area_res ? wl_serv_area_res_local_nonprim : NULL,
        rfsp ? true : false,
        rfsp ? rfsp->valuedouble : 0,
        smf_sel_info ? smf_sel_info_local_nonprim : NULL,
        ue_ambr ? ue_ambr_local_nonprim : NULL,
        pras ? prasList : NULL
    );

    return policy_update_local_var;
end:
    return NULL;
}

OpenAPI_policy_update_t *OpenAPI_policy_update_copy(OpenAPI_policy_update_t *dst, OpenAPI_policy_update_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_update_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_update_convertToJSON() failed");
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

    OpenAPI_policy_update_free(dst);
    dst = OpenAPI_policy_update_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

