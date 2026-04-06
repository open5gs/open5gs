
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdm_subs_modification.h"

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_create(
    char *expires,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_nf_change_filter,
    int nf_change_filter,
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter,
    OpenAPI_list_t* expected_ue_behaviour_thresholds
)
{
    OpenAPI_sdm_subs_modification_t *sdm_subs_modification_local_var = ogs_malloc(sizeof(OpenAPI_sdm_subs_modification_t));
    ogs_assert(sdm_subs_modification_local_var);

    sdm_subs_modification_local_var->expires = expires;
    sdm_subs_modification_local_var->monitored_resource_uris = monitored_resource_uris;
    sdm_subs_modification_local_var->single_nssai = single_nssai;
    sdm_subs_modification_local_var->dnn = dnn;
    sdm_subs_modification_local_var->plmn_id = plmn_id;
    sdm_subs_modification_local_var->is_nf_change_filter = is_nf_change_filter;
    sdm_subs_modification_local_var->nf_change_filter = nf_change_filter;
    sdm_subs_modification_local_var->ue_con_smf_data_sub_filter = ue_con_smf_data_sub_filter;
    sdm_subs_modification_local_var->expected_ue_behaviour_thresholds = expected_ue_behaviour_thresholds;

    return sdm_subs_modification_local_var;
}

void OpenAPI_sdm_subs_modification_free(OpenAPI_sdm_subs_modification_t *sdm_subs_modification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sdm_subs_modification) {
        return;
    }
    if (sdm_subs_modification->expires) {
        ogs_free(sdm_subs_modification->expires);
        sdm_subs_modification->expires = NULL;
    }
    if (sdm_subs_modification->monitored_resource_uris) {
        OpenAPI_list_for_each(sdm_subs_modification->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sdm_subs_modification->monitored_resource_uris);
        sdm_subs_modification->monitored_resource_uris = NULL;
    }
    if (sdm_subs_modification->single_nssai) {
        OpenAPI_snssai_free(sdm_subs_modification->single_nssai);
        sdm_subs_modification->single_nssai = NULL;
    }
    if (sdm_subs_modification->dnn) {
        ogs_free(sdm_subs_modification->dnn);
        sdm_subs_modification->dnn = NULL;
    }
    if (sdm_subs_modification->plmn_id) {
        OpenAPI_plmn_id_free(sdm_subs_modification->plmn_id);
        sdm_subs_modification->plmn_id = NULL;
    }
    if (sdm_subs_modification->ue_con_smf_data_sub_filter) {
        OpenAPI_ue_context_in_smf_data_sub_filter_free(sdm_subs_modification->ue_con_smf_data_sub_filter);
        sdm_subs_modification->ue_con_smf_data_sub_filter = NULL;
    }
    if (sdm_subs_modification->expected_ue_behaviour_thresholds) {
        OpenAPI_list_for_each(sdm_subs_modification->expected_ue_behaviour_thresholds, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_threshold_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sdm_subs_modification->expected_ue_behaviour_thresholds);
        sdm_subs_modification->expected_ue_behaviour_thresholds = NULL;
    }
    ogs_free(sdm_subs_modification);
}

cJSON *OpenAPI_sdm_subs_modification_convertToJSON(OpenAPI_sdm_subs_modification_t *sdm_subs_modification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sdm_subs_modification == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [SdmSubsModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sdm_subs_modification->expires) {
    if (cJSON_AddStringToObject(item, "expires", sdm_subs_modification->expires) == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [expires]");
        goto end;
    }
    }

    if (sdm_subs_modification->monitored_resource_uris) {
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(sdm_subs_modification->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }
    }

    if (sdm_subs_modification->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sdm_subs_modification->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (sdm_subs_modification->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", sdm_subs_modification->dnn) == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (sdm_subs_modification->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(sdm_subs_modification->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (sdm_subs_modification->is_nf_change_filter) {
    if (cJSON_AddBoolToObject(item, "nfChangeFilter", sdm_subs_modification->nf_change_filter) == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [nf_change_filter]");
        goto end;
    }
    }

    if (sdm_subs_modification->ue_con_smf_data_sub_filter) {
    cJSON *ue_con_smf_data_sub_filter_local_JSON = OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON(sdm_subs_modification->ue_con_smf_data_sub_filter);
    if (ue_con_smf_data_sub_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueConSmfDataSubFilter", ue_con_smf_data_sub_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    }

    if (sdm_subs_modification->expected_ue_behaviour_thresholds) {
    cJSON *expected_ue_behaviour_thresholds = cJSON_AddObjectToObject(item, "expectedUeBehaviourThresholds");
    if (expected_ue_behaviour_thresholds == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [expected_ue_behaviour_thresholds]");
        goto end;
    }
    cJSON *localMapObject = expected_ue_behaviour_thresholds;
    if (sdm_subs_modification->expected_ue_behaviour_thresholds) {
        OpenAPI_list_for_each(sdm_subs_modification->expected_ue_behaviour_thresholds, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [expected_ue_behaviour_thresholds]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [expected_ue_behaviour_thresholds]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_expected_ue_behaviour_threshold_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_parseFromJSON(cJSON *sdm_subs_modificationJSON)
{
    OpenAPI_sdm_subs_modification_t *sdm_subs_modification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *expires = NULL;
    cJSON *monitored_resource_uris = NULL;
    OpenAPI_list_t *monitored_resource_urisList = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *nf_change_filter = NULL;
    cJSON *ue_con_smf_data_sub_filter = NULL;
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter_local_nonprim = NULL;
    cJSON *expected_ue_behaviour_thresholds = NULL;
    OpenAPI_list_t *expected_ue_behaviour_thresholdsList = NULL;
    expires = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "expires");
    if (expires) {
    if (!cJSON_IsString(expires) && !cJSON_IsNull(expires)) {
        ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [expires]");
        goto end;
    }
    }

    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "monitoredResourceUris");
    if (monitored_resource_uris) {
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    nf_change_filter = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "nfChangeFilter");
    if (nf_change_filter) {
    if (!cJSON_IsBool(nf_change_filter)) {
        ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [nf_change_filter]");
        goto end;
    }
    }

    ue_con_smf_data_sub_filter = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "ueConSmfDataSubFilter");
    if (ue_con_smf_data_sub_filter) {
    ue_con_smf_data_sub_filter_local_nonprim = OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON(ue_con_smf_data_sub_filter);
    if (!ue_con_smf_data_sub_filter_local_nonprim) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    }

    expected_ue_behaviour_thresholds = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "expectedUeBehaviourThresholds");
    if (expected_ue_behaviour_thresholds) {
        cJSON *expected_ue_behaviour_thresholds_local_map = NULL;
        if (!cJSON_IsObject(expected_ue_behaviour_thresholds) && !cJSON_IsNull(expected_ue_behaviour_thresholds)) {
            ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [expected_ue_behaviour_thresholds]");
            goto end;
        }
        if (cJSON_IsObject(expected_ue_behaviour_thresholds)) {
            expected_ue_behaviour_thresholdsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(expected_ue_behaviour_thresholds_local_map, expected_ue_behaviour_thresholds) {
                cJSON *localMapObject = expected_ue_behaviour_thresholds_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_expected_ue_behaviour_threshold_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(expected_ue_behaviour_thresholdsList, localMapKeyPair);
            }
        }
    }

    sdm_subs_modification_local_var = OpenAPI_sdm_subs_modification_create (
        expires && !cJSON_IsNull(expires) ? ogs_strdup(expires->valuestring) : NULL,
        monitored_resource_uris ? monitored_resource_urisList : NULL,
        single_nssai ? single_nssai_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        nf_change_filter ? true : false,
        nf_change_filter ? nf_change_filter->valueint : 0,
        ue_con_smf_data_sub_filter ? ue_con_smf_data_sub_filter_local_nonprim : NULL,
        expected_ue_behaviour_thresholds ? expected_ue_behaviour_thresholdsList : NULL
    );

    return sdm_subs_modification_local_var;
end:
    if (monitored_resource_urisList) {
        OpenAPI_list_for_each(monitored_resource_urisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(monitored_resource_urisList);
        monitored_resource_urisList = NULL;
    }
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (ue_con_smf_data_sub_filter_local_nonprim) {
        OpenAPI_ue_context_in_smf_data_sub_filter_free(ue_con_smf_data_sub_filter_local_nonprim);
        ue_con_smf_data_sub_filter_local_nonprim = NULL;
    }
    if (expected_ue_behaviour_thresholdsList) {
        OpenAPI_list_for_each(expected_ue_behaviour_thresholdsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_threshold_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(expected_ue_behaviour_thresholdsList);
        expected_ue_behaviour_thresholdsList = NULL;
    }
    return NULL;
}

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_copy(OpenAPI_sdm_subs_modification_t *dst, OpenAPI_sdm_subs_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdm_subs_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed");
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

    OpenAPI_sdm_subs_modification_free(dst);
    dst = OpenAPI_sdm_subs_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

