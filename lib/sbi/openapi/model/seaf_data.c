
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seaf_data.h"

OpenAPI_seaf_data_t *OpenAPI_seaf_data_create(
    OpenAPI_ng_ksi_t *ng_ksi,
    OpenAPI_key_amf_t *key_amf,
    char *nh,
    bool is_ncc,
    int ncc,
    bool is_key_amf_change_ind,
    int key_amf_change_ind,
    bool is_key_amf_h_derivation_ind,
    int key_amf_h_derivation_ind
)
{
    OpenAPI_seaf_data_t *seaf_data_local_var = ogs_malloc(sizeof(OpenAPI_seaf_data_t));
    ogs_assert(seaf_data_local_var);

    seaf_data_local_var->ng_ksi = ng_ksi;
    seaf_data_local_var->key_amf = key_amf;
    seaf_data_local_var->nh = nh;
    seaf_data_local_var->is_ncc = is_ncc;
    seaf_data_local_var->ncc = ncc;
    seaf_data_local_var->is_key_amf_change_ind = is_key_amf_change_ind;
    seaf_data_local_var->key_amf_change_ind = key_amf_change_ind;
    seaf_data_local_var->is_key_amf_h_derivation_ind = is_key_amf_h_derivation_ind;
    seaf_data_local_var->key_amf_h_derivation_ind = key_amf_h_derivation_ind;

    return seaf_data_local_var;
}

void OpenAPI_seaf_data_free(OpenAPI_seaf_data_t *seaf_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == seaf_data) {
        return;
    }
    if (seaf_data->ng_ksi) {
        OpenAPI_ng_ksi_free(seaf_data->ng_ksi);
        seaf_data->ng_ksi = NULL;
    }
    if (seaf_data->key_amf) {
        OpenAPI_key_amf_free(seaf_data->key_amf);
        seaf_data->key_amf = NULL;
    }
    if (seaf_data->nh) {
        ogs_free(seaf_data->nh);
        seaf_data->nh = NULL;
    }
    ogs_free(seaf_data);
}

cJSON *OpenAPI_seaf_data_convertToJSON(OpenAPI_seaf_data_t *seaf_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (seaf_data == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [SeafData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!seaf_data->ng_ksi) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [ng_ksi]");
        return NULL;
    }
    cJSON *ng_ksi_local_JSON = OpenAPI_ng_ksi_convertToJSON(seaf_data->ng_ksi);
    if (ng_ksi_local_JSON == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [ng_ksi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngKsi", ng_ksi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [ng_ksi]");
        goto end;
    }

    if (!seaf_data->key_amf) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [key_amf]");
        return NULL;
    }
    cJSON *key_amf_local_JSON = OpenAPI_key_amf_convertToJSON(seaf_data->key_amf);
    if (key_amf_local_JSON == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [key_amf]");
        goto end;
    }
    cJSON_AddItemToObject(item, "keyAmf", key_amf_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [key_amf]");
        goto end;
    }

    if (seaf_data->nh) {
    if (cJSON_AddStringToObject(item, "nh", seaf_data->nh) == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [nh]");
        goto end;
    }
    }

    if (seaf_data->is_ncc) {
    if (cJSON_AddNumberToObject(item, "ncc", seaf_data->ncc) == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [ncc]");
        goto end;
    }
    }

    if (seaf_data->is_key_amf_change_ind) {
    if (cJSON_AddBoolToObject(item, "keyAmfChangeInd", seaf_data->key_amf_change_ind) == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [key_amf_change_ind]");
        goto end;
    }
    }

    if (seaf_data->is_key_amf_h_derivation_ind) {
    if (cJSON_AddBoolToObject(item, "keyAmfHDerivationInd", seaf_data->key_amf_h_derivation_ind) == NULL) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed [key_amf_h_derivation_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_seaf_data_t *OpenAPI_seaf_data_parseFromJSON(cJSON *seaf_dataJSON)
{
    OpenAPI_seaf_data_t *seaf_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ng_ksi = NULL;
    OpenAPI_ng_ksi_t *ng_ksi_local_nonprim = NULL;
    cJSON *key_amf = NULL;
    OpenAPI_key_amf_t *key_amf_local_nonprim = NULL;
    cJSON *nh = NULL;
    cJSON *ncc = NULL;
    cJSON *key_amf_change_ind = NULL;
    cJSON *key_amf_h_derivation_ind = NULL;
    ng_ksi = cJSON_GetObjectItemCaseSensitive(seaf_dataJSON, "ngKsi");
    if (!ng_ksi) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON() failed [ng_ksi]");
        goto end;
    }
    ng_ksi_local_nonprim = OpenAPI_ng_ksi_parseFromJSON(ng_ksi);
    if (!ng_ksi_local_nonprim) {
        ogs_error("OpenAPI_ng_ksi_parseFromJSON failed [ng_ksi]");
        goto end;
    }

    key_amf = cJSON_GetObjectItemCaseSensitive(seaf_dataJSON, "keyAmf");
    if (!key_amf) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON() failed [key_amf]");
        goto end;
    }
    key_amf_local_nonprim = OpenAPI_key_amf_parseFromJSON(key_amf);
    if (!key_amf_local_nonprim) {
        ogs_error("OpenAPI_key_amf_parseFromJSON failed [key_amf]");
        goto end;
    }

    nh = cJSON_GetObjectItemCaseSensitive(seaf_dataJSON, "nh");
    if (nh) {
    if (!cJSON_IsString(nh) && !cJSON_IsNull(nh)) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON() failed [nh]");
        goto end;
    }
    }

    ncc = cJSON_GetObjectItemCaseSensitive(seaf_dataJSON, "ncc");
    if (ncc) {
    if (!cJSON_IsNumber(ncc)) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON() failed [ncc]");
        goto end;
    }
    }

    key_amf_change_ind = cJSON_GetObjectItemCaseSensitive(seaf_dataJSON, "keyAmfChangeInd");
    if (key_amf_change_ind) {
    if (!cJSON_IsBool(key_amf_change_ind)) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON() failed [key_amf_change_ind]");
        goto end;
    }
    }

    key_amf_h_derivation_ind = cJSON_GetObjectItemCaseSensitive(seaf_dataJSON, "keyAmfHDerivationInd");
    if (key_amf_h_derivation_ind) {
    if (!cJSON_IsBool(key_amf_h_derivation_ind)) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON() failed [key_amf_h_derivation_ind]");
        goto end;
    }
    }

    seaf_data_local_var = OpenAPI_seaf_data_create (
        ng_ksi_local_nonprim,
        key_amf_local_nonprim,
        nh && !cJSON_IsNull(nh) ? ogs_strdup(nh->valuestring) : NULL,
        ncc ? true : false,
        ncc ? ncc->valuedouble : 0,
        key_amf_change_ind ? true : false,
        key_amf_change_ind ? key_amf_change_ind->valueint : 0,
        key_amf_h_derivation_ind ? true : false,
        key_amf_h_derivation_ind ? key_amf_h_derivation_ind->valueint : 0
    );

    return seaf_data_local_var;
end:
    if (ng_ksi_local_nonprim) {
        OpenAPI_ng_ksi_free(ng_ksi_local_nonprim);
        ng_ksi_local_nonprim = NULL;
    }
    if (key_amf_local_nonprim) {
        OpenAPI_key_amf_free(key_amf_local_nonprim);
        key_amf_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_seaf_data_t *OpenAPI_seaf_data_copy(OpenAPI_seaf_data_t *dst, OpenAPI_seaf_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_seaf_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_seaf_data_convertToJSON() failed");
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

    OpenAPI_seaf_data_free(dst);
    dst = OpenAPI_seaf_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

