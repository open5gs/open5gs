
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_characterization.h"

OpenAPI_traffic_characterization_t *OpenAPI_traffic_characterization_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *app_id,
    OpenAPI_list_t *f_descs,
    bool is_ul_vol,
    long ul_vol,
    bool is_ul_vol_variance,
    float ul_vol_variance,
    bool is_dl_vol,
    long dl_vol,
    bool is_dl_vol_variance,
    float dl_vol_variance
)
{
    OpenAPI_traffic_characterization_t *traffic_characterization_local_var = ogs_malloc(sizeof(OpenAPI_traffic_characterization_t));
    ogs_assert(traffic_characterization_local_var);

    traffic_characterization_local_var->dnn = dnn;
    traffic_characterization_local_var->snssai = snssai;
    traffic_characterization_local_var->app_id = app_id;
    traffic_characterization_local_var->f_descs = f_descs;
    traffic_characterization_local_var->is_ul_vol = is_ul_vol;
    traffic_characterization_local_var->ul_vol = ul_vol;
    traffic_characterization_local_var->is_ul_vol_variance = is_ul_vol_variance;
    traffic_characterization_local_var->ul_vol_variance = ul_vol_variance;
    traffic_characterization_local_var->is_dl_vol = is_dl_vol;
    traffic_characterization_local_var->dl_vol = dl_vol;
    traffic_characterization_local_var->is_dl_vol_variance = is_dl_vol_variance;
    traffic_characterization_local_var->dl_vol_variance = dl_vol_variance;

    return traffic_characterization_local_var;
}

void OpenAPI_traffic_characterization_free(OpenAPI_traffic_characterization_t *traffic_characterization)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_characterization) {
        return;
    }
    if (traffic_characterization->dnn) {
        ogs_free(traffic_characterization->dnn);
        traffic_characterization->dnn = NULL;
    }
    if (traffic_characterization->snssai) {
        OpenAPI_snssai_free(traffic_characterization->snssai);
        traffic_characterization->snssai = NULL;
    }
    if (traffic_characterization->app_id) {
        ogs_free(traffic_characterization->app_id);
        traffic_characterization->app_id = NULL;
    }
    if (traffic_characterization->f_descs) {
        OpenAPI_list_for_each(traffic_characterization->f_descs, node) {
            OpenAPI_ip_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(traffic_characterization->f_descs);
        traffic_characterization->f_descs = NULL;
    }
    ogs_free(traffic_characterization);
}

cJSON *OpenAPI_traffic_characterization_convertToJSON(OpenAPI_traffic_characterization_t *traffic_characterization)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_characterization == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [TrafficCharacterization]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_characterization->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", traffic_characterization->dnn) == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (traffic_characterization->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(traffic_characterization->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (traffic_characterization->app_id) {
    if (cJSON_AddStringToObject(item, "appId", traffic_characterization->app_id) == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (traffic_characterization->f_descs) {
    cJSON *f_descsList = cJSON_AddArrayToObject(item, "fDescs");
    if (f_descsList == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [f_descs]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_characterization->f_descs, node) {
        cJSON *itemLocal = OpenAPI_ip_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [f_descs]");
            goto end;
        }
        cJSON_AddItemToArray(f_descsList, itemLocal);
    }
    }

    if (traffic_characterization->is_ul_vol) {
    if (cJSON_AddNumberToObject(item, "ulVol", traffic_characterization->ul_vol) == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [ul_vol]");
        goto end;
    }
    }

    if (traffic_characterization->is_ul_vol_variance) {
    if (cJSON_AddNumberToObject(item, "ulVolVariance", traffic_characterization->ul_vol_variance) == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [ul_vol_variance]");
        goto end;
    }
    }

    if (traffic_characterization->is_dl_vol) {
    if (cJSON_AddNumberToObject(item, "dlVol", traffic_characterization->dl_vol) == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [dl_vol]");
        goto end;
    }
    }

    if (traffic_characterization->is_dl_vol_variance) {
    if (cJSON_AddNumberToObject(item, "dlVolVariance", traffic_characterization->dl_vol_variance) == NULL) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed [dl_vol_variance]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_traffic_characterization_t *OpenAPI_traffic_characterization_parseFromJSON(cJSON *traffic_characterizationJSON)
{
    OpenAPI_traffic_characterization_t *traffic_characterization_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *app_id = NULL;
    cJSON *f_descs = NULL;
    OpenAPI_list_t *f_descsList = NULL;
    cJSON *ul_vol = NULL;
    cJSON *ul_vol_variance = NULL;
    cJSON *dl_vol = NULL;
    cJSON *dl_vol_variance = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    f_descs = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "fDescs");
    if (f_descs) {
        cJSON *f_descs_local = NULL;
        if (!cJSON_IsArray(f_descs)) {
            ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [f_descs]");
            goto end;
        }

        f_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(f_descs_local, f_descs) {
            if (!cJSON_IsObject(f_descs_local)) {
                ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [f_descs]");
                goto end;
            }
            OpenAPI_ip_eth_flow_description_t *f_descsItem = OpenAPI_ip_eth_flow_description_parseFromJSON(f_descs_local);
            if (!f_descsItem) {
                ogs_error("No f_descsItem");
                goto end;
            }
            OpenAPI_list_add(f_descsList, f_descsItem);
        }
    }

    ul_vol = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "ulVol");
    if (ul_vol) {
    if (!cJSON_IsNumber(ul_vol)) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [ul_vol]");
        goto end;
    }
    }

    ul_vol_variance = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "ulVolVariance");
    if (ul_vol_variance) {
    if (!cJSON_IsNumber(ul_vol_variance)) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [ul_vol_variance]");
        goto end;
    }
    }

    dl_vol = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "dlVol");
    if (dl_vol) {
    if (!cJSON_IsNumber(dl_vol)) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [dl_vol]");
        goto end;
    }
    }

    dl_vol_variance = cJSON_GetObjectItemCaseSensitive(traffic_characterizationJSON, "dlVolVariance");
    if (dl_vol_variance) {
    if (!cJSON_IsNumber(dl_vol_variance)) {
        ogs_error("OpenAPI_traffic_characterization_parseFromJSON() failed [dl_vol_variance]");
        goto end;
    }
    }

    traffic_characterization_local_var = OpenAPI_traffic_characterization_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        f_descs ? f_descsList : NULL,
        ul_vol ? true : false,
        ul_vol ? ul_vol->valuedouble : 0,
        ul_vol_variance ? true : false,
        ul_vol_variance ? ul_vol_variance->valuedouble : 0,
        dl_vol ? true : false,
        dl_vol ? dl_vol->valuedouble : 0,
        dl_vol_variance ? true : false,
        dl_vol_variance ? dl_vol_variance->valuedouble : 0
    );

    return traffic_characterization_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (f_descsList) {
        OpenAPI_list_for_each(f_descsList, node) {
            OpenAPI_ip_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(f_descsList);
        f_descsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_characterization_t *OpenAPI_traffic_characterization_copy(OpenAPI_traffic_characterization_t *dst, OpenAPI_traffic_characterization_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_characterization_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_characterization_convertToJSON() failed");
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

    OpenAPI_traffic_characterization_free(dst);
    dst = OpenAPI_traffic_characterization_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

