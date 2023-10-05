
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_selection_data.h"

OpenAPI_smf_selection_data_t *OpenAPI_smf_selection_data_create(
    bool is_unsupp_dnn,
    int unsupp_dnn,
    bool is_candidates_null,
    OpenAPI_list_t* candidates,
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_t *mapping_snssai,
    char *dnn
)
{
    OpenAPI_smf_selection_data_t *smf_selection_data_local_var = ogs_malloc(sizeof(OpenAPI_smf_selection_data_t));
    ogs_assert(smf_selection_data_local_var);

    smf_selection_data_local_var->is_unsupp_dnn = is_unsupp_dnn;
    smf_selection_data_local_var->unsupp_dnn = unsupp_dnn;
    smf_selection_data_local_var->is_candidates_null = is_candidates_null;
    smf_selection_data_local_var->candidates = candidates;
    smf_selection_data_local_var->snssai = snssai;
    smf_selection_data_local_var->mapping_snssai = mapping_snssai;
    smf_selection_data_local_var->dnn = dnn;

    return smf_selection_data_local_var;
}

void OpenAPI_smf_selection_data_free(OpenAPI_smf_selection_data_t *smf_selection_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_selection_data) {
        return;
    }
    if (smf_selection_data->candidates) {
        OpenAPI_list_for_each(smf_selection_data->candidates, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_candidate_for_replacement_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(smf_selection_data->candidates);
        smf_selection_data->candidates = NULL;
    }
    if (smf_selection_data->snssai) {
        OpenAPI_snssai_free(smf_selection_data->snssai);
        smf_selection_data->snssai = NULL;
    }
    if (smf_selection_data->mapping_snssai) {
        OpenAPI_snssai_free(smf_selection_data->mapping_snssai);
        smf_selection_data->mapping_snssai = NULL;
    }
    if (smf_selection_data->dnn) {
        ogs_free(smf_selection_data->dnn);
        smf_selection_data->dnn = NULL;
    }
    ogs_free(smf_selection_data);
}

cJSON *OpenAPI_smf_selection_data_convertToJSON(OpenAPI_smf_selection_data_t *smf_selection_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_selection_data == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [SmfSelectionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smf_selection_data->is_unsupp_dnn) {
    if (cJSON_AddBoolToObject(item, "unsuppDnn", smf_selection_data->unsupp_dnn) == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [unsupp_dnn]");
        goto end;
    }
    }

    if (smf_selection_data->candidates) {
    cJSON *candidates = cJSON_AddObjectToObject(item, "candidates");
    if (candidates == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [candidates]");
        goto end;
    }
    cJSON *localMapObject = candidates;
    if (smf_selection_data->candidates) {
        OpenAPI_list_for_each(smf_selection_data->candidates, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [candidates]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [candidates]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_candidate_for_replacement_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (smf_selection_data->is_candidates_null) {
        if (cJSON_AddNullToObject(item, "candidates") == NULL) {
            ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [candidates]");
            goto end;
        }
    }

    if (smf_selection_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(smf_selection_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (smf_selection_data->mapping_snssai) {
    cJSON *mapping_snssai_local_JSON = OpenAPI_snssai_convertToJSON(smf_selection_data->mapping_snssai);
    if (mapping_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [mapping_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mappingSnssai", mapping_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [mapping_snssai]");
        goto end;
    }
    }

    if (smf_selection_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", smf_selection_data->dnn) == NULL) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_selection_data_t *OpenAPI_smf_selection_data_parseFromJSON(cJSON *smf_selection_dataJSON)
{
    OpenAPI_smf_selection_data_t *smf_selection_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *unsupp_dnn = NULL;
    cJSON *candidates = NULL;
    OpenAPI_list_t *candidatesList = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *mapping_snssai = NULL;
    OpenAPI_snssai_t *mapping_snssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    unsupp_dnn = cJSON_GetObjectItemCaseSensitive(smf_selection_dataJSON, "unsuppDnn");
    if (unsupp_dnn) {
    if (!cJSON_IsBool(unsupp_dnn)) {
        ogs_error("OpenAPI_smf_selection_data_parseFromJSON() failed [unsupp_dnn]");
        goto end;
    }
    }

    candidates = cJSON_GetObjectItemCaseSensitive(smf_selection_dataJSON, "candidates");
    if (candidates) {
    if (!cJSON_IsNull(candidates)) {
        cJSON *candidates_local_map = NULL;
        if (!cJSON_IsObject(candidates) && !cJSON_IsNull(candidates)) {
            ogs_error("OpenAPI_smf_selection_data_parseFromJSON() failed [candidates]");
            goto end;
        }
        if (cJSON_IsObject(candidates)) {
            candidatesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(candidates_local_map, candidates) {
                cJSON *localMapObject = candidates_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_candidate_for_replacement_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_smf_selection_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(candidatesList, localMapKeyPair);
            }
        }
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(smf_selection_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    mapping_snssai = cJSON_GetObjectItemCaseSensitive(smf_selection_dataJSON, "mappingSnssai");
    if (mapping_snssai) {
    mapping_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(mapping_snssai);
    if (!mapping_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [mapping_snssai]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(smf_selection_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_smf_selection_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    smf_selection_data_local_var = OpenAPI_smf_selection_data_create (
        unsupp_dnn ? true : false,
        unsupp_dnn ? unsupp_dnn->valueint : 0,
        candidates && cJSON_IsNull(candidates) ? true : false,
        candidates ? candidatesList : NULL,
        snssai ? snssai_local_nonprim : NULL,
        mapping_snssai ? mapping_snssai_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL
    );

    return smf_selection_data_local_var;
end:
    if (candidatesList) {
        OpenAPI_list_for_each(candidatesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_candidate_for_replacement_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(candidatesList);
        candidatesList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (mapping_snssai_local_nonprim) {
        OpenAPI_snssai_free(mapping_snssai_local_nonprim);
        mapping_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_smf_selection_data_t *OpenAPI_smf_selection_data_copy(OpenAPI_smf_selection_data_t *dst, OpenAPI_smf_selection_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_selection_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_selection_data_convertToJSON() failed");
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

    OpenAPI_smf_selection_data_free(dst);
    dst = OpenAPI_smf_selection_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

