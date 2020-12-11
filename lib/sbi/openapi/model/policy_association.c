
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_association.h"

OpenAPI_policy_association_t *OpenAPI_policy_association_create(
    OpenAPI_policy_association_request_t *request,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    int rfsp,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_ambr_t *rg_tmbr,
    OpenAPI_list_t* pras,
    char *supp_feat
    )
{
    OpenAPI_policy_association_t *policy_association_local_var = OpenAPI_malloc(sizeof(OpenAPI_policy_association_t));
    if (!policy_association_local_var) {
        return NULL;
    }
    policy_association_local_var->request = request;
    policy_association_local_var->triggers = triggers;
    policy_association_local_var->serv_area_res = serv_area_res;
    policy_association_local_var->wl_serv_area_res = wl_serv_area_res;
    policy_association_local_var->rfsp = rfsp;
    policy_association_local_var->smf_sel_info = smf_sel_info;
    policy_association_local_var->ue_ambr = ue_ambr;
    policy_association_local_var->rg_tmbr = rg_tmbr;
    policy_association_local_var->pras = pras;
    policy_association_local_var->supp_feat = supp_feat;

    return policy_association_local_var;
}

void OpenAPI_policy_association_free(OpenAPI_policy_association_t *policy_association)
{
    if (NULL == policy_association) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_policy_association_request_free(policy_association->request);
    OpenAPI_list_free(policy_association->triggers);
    OpenAPI_service_area_restriction_free(policy_association->serv_area_res);
    OpenAPI_wireline_service_area_restriction_free(policy_association->wl_serv_area_res);
    OpenAPI_smf_selection_data_free(policy_association->smf_sel_info);
    OpenAPI_ambr_free(policy_association->ue_ambr);
    OpenAPI_ambr_free(policy_association->rg_tmbr);
    OpenAPI_list_for_each(policy_association->pras, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_presence_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(policy_association->pras);
    ogs_free(policy_association->supp_feat);
    ogs_free(policy_association);
}

cJSON *OpenAPI_policy_association_convertToJSON(OpenAPI_policy_association_t *policy_association)
{
    cJSON *item = NULL;

    if (policy_association == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [PolicyAssociation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (policy_association->request) {
        cJSON *request_local_JSON = OpenAPI_policy_association_request_convertToJSON(policy_association->request);
        if (request_local_JSON == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [request]");
            goto end;
        }
        cJSON_AddItemToObject(item, "request", request_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [request]");
            goto end;
        }
    }

    if (policy_association->triggers) {
        cJSON *triggers = cJSON_AddArrayToObject(item, "triggers");
        if (triggers == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [triggers]");
            goto end;
        }
        OpenAPI_lnode_t *triggers_node;
        OpenAPI_list_for_each(policy_association->triggers, triggers_node) {
            if (cJSON_AddStringToObject(triggers, "", OpenAPI_request_trigger_ToString((OpenAPI_request_trigger_e)triggers_node->data)) == NULL) {
                ogs_error("OpenAPI_policy_association_convertToJSON() failed [triggers]");
                goto end;
            }
        }
    }

    if (policy_association->serv_area_res) {
        cJSON *serv_area_res_local_JSON = OpenAPI_service_area_restriction_convertToJSON(policy_association->serv_area_res);
        if (serv_area_res_local_JSON == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [serv_area_res]");
            goto end;
        }
        cJSON_AddItemToObject(item, "servAreaRes", serv_area_res_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [serv_area_res]");
            goto end;
        }
    }

    if (policy_association->wl_serv_area_res) {
        cJSON *wl_serv_area_res_local_JSON = OpenAPI_wireline_service_area_restriction_convertToJSON(policy_association->wl_serv_area_res);
        if (wl_serv_area_res_local_JSON == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [wl_serv_area_res]");
            goto end;
        }
        cJSON_AddItemToObject(item, "wlServAreaRes", wl_serv_area_res_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [wl_serv_area_res]");
            goto end;
        }
    }

    if (policy_association->rfsp) {
        if (cJSON_AddNumberToObject(item, "rfsp", policy_association->rfsp) == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [rfsp]");
            goto end;
        }
    }

    if (policy_association->smf_sel_info) {
        cJSON *smf_sel_info_local_JSON = OpenAPI_smf_selection_data_convertToJSON(policy_association->smf_sel_info);
        if (smf_sel_info_local_JSON == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [smf_sel_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "smfSelInfo", smf_sel_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [smf_sel_info]");
            goto end;
        }
    }

    if (policy_association->ue_ambr) {
        cJSON *ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(policy_association->ue_ambr);
        if (ue_ambr_local_JSON == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [ue_ambr]");
            goto end;
        }
        cJSON_AddItemToObject(item, "ueAmbr", ue_ambr_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [ue_ambr]");
            goto end;
        }
    }

    if (policy_association->rg_tmbr) {
        cJSON *rg_tmbr_local_JSON = OpenAPI_ambr_convertToJSON(policy_association->rg_tmbr);
        if (rg_tmbr_local_JSON == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [rg_tmbr]");
            goto end;
        }
        cJSON_AddItemToObject(item, "rgTmbr", rg_tmbr_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [rg_tmbr]");
            goto end;
        }
    }

    if (policy_association->pras) {
        cJSON *pras = cJSON_AddObjectToObject(item, "pras");
        if (pras == NULL) {
            ogs_error("OpenAPI_policy_association_convertToJSON() failed [pras]");
            goto end;
        }
        cJSON *localMapObject = pras;
        OpenAPI_lnode_t *pras_node;
        if (policy_association->pras) {
            OpenAPI_list_for_each(policy_association->pras, pras_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pras_node->data;
                cJSON *itemLocal = OpenAPI_presence_info_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_policy_association_convertToJSON() failed [pras]");
                    goto end;
                }
                cJSON_AddItemToObject(pras, localKeyValue->key, itemLocal);
            }
        }
    }

    if (!policy_association->supp_feat) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [supp_feat]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "suppFeat", policy_association->supp_feat) == NULL) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed [supp_feat]");
        goto end;
    }

end:
    return item;
}

OpenAPI_policy_association_t *OpenAPI_policy_association_parseFromJSON(cJSON *policy_associationJSON)
{
    OpenAPI_policy_association_t *policy_association_local_var = NULL;
    cJSON *request = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "request");

    OpenAPI_policy_association_request_t *request_local_nonprim = NULL;
    if (request) {
        request_local_nonprim = OpenAPI_policy_association_request_parseFromJSON(request);
    }

    cJSON *triggers = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "triggers");

    OpenAPI_list_t *triggersList;
    if (triggers) {
        cJSON *triggers_local_nonprimitive;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local_nonprimitive, triggers ) {
            if (!cJSON_IsString(triggers_local_nonprimitive)) {
                ogs_error("OpenAPI_policy_association_parseFromJSON() failed [triggers]");
                goto end;
            }

            OpenAPI_list_add(triggersList, (void *)OpenAPI_request_trigger_FromString(triggers_local_nonprimitive->valuestring));
        }
    }

    cJSON *serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "servAreaRes");

    OpenAPI_service_area_restriction_t *serv_area_res_local_nonprim = NULL;
    if (serv_area_res) {
        serv_area_res_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(serv_area_res);
    }

    cJSON *wl_serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "wlServAreaRes");

    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res_local_nonprim = NULL;
    if (wl_serv_area_res) {
        wl_serv_area_res_local_nonprim = OpenAPI_wireline_service_area_restriction_parseFromJSON(wl_serv_area_res);
    }

    cJSON *rfsp = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "rfsp");

    if (rfsp) {
        if (!cJSON_IsNumber(rfsp)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [rfsp]");
            goto end;
        }
    }

    cJSON *smf_sel_info = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "smfSelInfo");

    OpenAPI_smf_selection_data_t *smf_sel_info_local_nonprim = NULL;
    if (smf_sel_info) {
        smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    }

    cJSON *ue_ambr = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "ueAmbr");

    OpenAPI_ambr_t *ue_ambr_local_nonprim = NULL;
    if (ue_ambr) {
        ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(ue_ambr);
    }

    cJSON *rg_tmbr = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "rgTmbr");

    OpenAPI_ambr_t *rg_tmbr_local_nonprim = NULL;
    if (rg_tmbr) {
        rg_tmbr_local_nonprim = OpenAPI_ambr_parseFromJSON(rg_tmbr);
    }

    cJSON *pras = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "pras");

    OpenAPI_list_t *prasList;
    if (pras) {
        cJSON *pras_local_map;
        if (!cJSON_IsObject(pras)) {
            ogs_error("OpenAPI_policy_association_parseFromJSON() failed [pras]");
            goto end;
        }
        prasList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(pras_local_map, pras) {
            cJSON *localMapObject = pras_local_map;
            if (!cJSON_IsObject(pras_local_map)) {
                ogs_error("OpenAPI_policy_association_parseFromJSON() failed [pras]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_presence_info_parseFromJSON(localMapObject));
            OpenAPI_list_add(prasList, localMapKeyPair);
        }
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(policy_associationJSON, "suppFeat");
    if (!supp_feat) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [supp_feat]");
        goto end;
    }


    if (!cJSON_IsString(supp_feat)) {
        ogs_error("OpenAPI_policy_association_parseFromJSON() failed [supp_feat]");
        goto end;
    }

    policy_association_local_var = OpenAPI_policy_association_create (
        request ? request_local_nonprim : NULL,
        triggers ? triggersList : NULL,
        serv_area_res ? serv_area_res_local_nonprim : NULL,
        wl_serv_area_res ? wl_serv_area_res_local_nonprim : NULL,
        rfsp ? rfsp->valuedouble : 0,
        smf_sel_info ? smf_sel_info_local_nonprim : NULL,
        ue_ambr ? ue_ambr_local_nonprim : NULL,
        rg_tmbr ? rg_tmbr_local_nonprim : NULL,
        pras ? prasList : NULL,
        ogs_strdup(supp_feat->valuestring)
        );

    return policy_association_local_var;
end:
    return NULL;
}

OpenAPI_policy_association_t *OpenAPI_policy_association_copy(OpenAPI_policy_association_t *dst, OpenAPI_policy_association_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_association_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_association_convertToJSON() failed");
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

    OpenAPI_policy_association_free(dst);
    dst = OpenAPI_policy_association_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

