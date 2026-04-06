
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ml_analytics_info.h"

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_create(
    OpenAPI_list_t *ml_analytics_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_ml_model_inter_info_t *ml_model_inter_info,
    OpenAPI_fl_capability_type_e fl_capability_type,
    OpenAPI_time_window_t *fl_time_interval,
    OpenAPI_list_t *nf_type_list,
    OpenAPI_list_t *nf_set_id_list,
    OpenAPI_vfl_capability_type_e vfl_capability_type,
    bool is_vfl_client_aggr_cap,
    int vfl_client_aggr_cap,
    OpenAPI_time_window_t *vfl_time_interval,
    OpenAPI_ml_model_inter_info_t *vfl_inter_info,
    OpenAPI_list_t *feature_ids
)
{
    OpenAPI_ml_analytics_info_t *ml_analytics_info_local_var = ogs_malloc(sizeof(OpenAPI_ml_analytics_info_t));
    ogs_assert(ml_analytics_info_local_var);

    ml_analytics_info_local_var->ml_analytics_ids = ml_analytics_ids;
    ml_analytics_info_local_var->snssai_list = snssai_list;
    ml_analytics_info_local_var->tracking_area_list = tracking_area_list;
    ml_analytics_info_local_var->ml_model_inter_info = ml_model_inter_info;
    ml_analytics_info_local_var->fl_capability_type = fl_capability_type;
    ml_analytics_info_local_var->fl_time_interval = fl_time_interval;
    ml_analytics_info_local_var->nf_type_list = nf_type_list;
    ml_analytics_info_local_var->nf_set_id_list = nf_set_id_list;
    ml_analytics_info_local_var->vfl_capability_type = vfl_capability_type;
    ml_analytics_info_local_var->is_vfl_client_aggr_cap = is_vfl_client_aggr_cap;
    ml_analytics_info_local_var->vfl_client_aggr_cap = vfl_client_aggr_cap;
    ml_analytics_info_local_var->vfl_time_interval = vfl_time_interval;
    ml_analytics_info_local_var->vfl_inter_info = vfl_inter_info;
    ml_analytics_info_local_var->feature_ids = feature_ids;

    return ml_analytics_info_local_var;
}

void OpenAPI_ml_analytics_info_free(OpenAPI_ml_analytics_info_t *ml_analytics_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ml_analytics_info) {
        return;
    }
    if (ml_analytics_info->ml_analytics_ids) {
        OpenAPI_list_free(ml_analytics_info->ml_analytics_ids);
        ml_analytics_info->ml_analytics_ids = NULL;
    }
    if (ml_analytics_info->snssai_list) {
        OpenAPI_list_for_each(ml_analytics_info->snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->snssai_list);
        ml_analytics_info->snssai_list = NULL;
    }
    if (ml_analytics_info->tracking_area_list) {
        OpenAPI_list_for_each(ml_analytics_info->tracking_area_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->tracking_area_list);
        ml_analytics_info->tracking_area_list = NULL;
    }
    if (ml_analytics_info->ml_model_inter_info) {
        OpenAPI_ml_model_inter_info_free(ml_analytics_info->ml_model_inter_info);
        ml_analytics_info->ml_model_inter_info = NULL;
    }
    if (ml_analytics_info->fl_time_interval) {
        OpenAPI_time_window_free(ml_analytics_info->fl_time_interval);
        ml_analytics_info->fl_time_interval = NULL;
    }
    if (ml_analytics_info->nf_type_list) {
        OpenAPI_list_free(ml_analytics_info->nf_type_list);
        ml_analytics_info->nf_type_list = NULL;
    }
    if (ml_analytics_info->nf_set_id_list) {
        OpenAPI_list_for_each(ml_analytics_info->nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->nf_set_id_list);
        ml_analytics_info->nf_set_id_list = NULL;
    }
    if (ml_analytics_info->vfl_time_interval) {
        OpenAPI_time_window_free(ml_analytics_info->vfl_time_interval);
        ml_analytics_info->vfl_time_interval = NULL;
    }
    if (ml_analytics_info->vfl_inter_info) {
        OpenAPI_ml_model_inter_info_free(ml_analytics_info->vfl_inter_info);
        ml_analytics_info->vfl_inter_info = NULL;
    }
    if (ml_analytics_info->feature_ids) {
        OpenAPI_list_for_each(ml_analytics_info->feature_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_info->feature_ids);
        ml_analytics_info->feature_ids = NULL;
    }
    ogs_free(ml_analytics_info);
}

cJSON *OpenAPI_ml_analytics_info_convertToJSON(OpenAPI_ml_analytics_info_t *ml_analytics_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ml_analytics_info == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [MlAnalyticsInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ml_analytics_info->ml_analytics_ids != OpenAPI_nwdaf_event_NULL) {
    cJSON *ml_analytics_idsList = cJSON_AddArrayToObject(item, "mlAnalyticsIds");
    if (ml_analytics_idsList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [ml_analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->ml_analytics_ids, node) {
        if (cJSON_AddStringToObject(ml_analytics_idsList, "", OpenAPI_nwdaf_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [ml_analytics_ids]");
            goto end;
        }
    }
    }

    if (ml_analytics_info->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }
    }

    if (ml_analytics_info->tracking_area_list) {
    cJSON *tracking_area_listList = cJSON_AddArrayToObject(item, "trackingAreaList");
    if (tracking_area_listList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [tracking_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->tracking_area_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [tracking_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(tracking_area_listList, itemLocal);
    }
    }

    if (ml_analytics_info->ml_model_inter_info) {
    cJSON *ml_model_inter_info_local_JSON = OpenAPI_ml_model_inter_info_convertToJSON(ml_analytics_info->ml_model_inter_info);
    if (ml_model_inter_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [ml_model_inter_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mlModelInterInfo", ml_model_inter_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [ml_model_inter_info]");
        goto end;
    }
    }

    if (ml_analytics_info->fl_capability_type != OpenAPI_fl_capability_type_NULL) {
    if (cJSON_AddStringToObject(item, "flCapabilityType", OpenAPI_fl_capability_type_ToString(ml_analytics_info->fl_capability_type)) == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [fl_capability_type]");
        goto end;
    }
    }

    if (ml_analytics_info->fl_time_interval) {
    cJSON *fl_time_interval_local_JSON = OpenAPI_time_window_convertToJSON(ml_analytics_info->fl_time_interval);
    if (fl_time_interval_local_JSON == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [fl_time_interval]");
        goto end;
    }
    cJSON_AddItemToObject(item, "flTimeInterval", fl_time_interval_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [fl_time_interval]");
        goto end;
    }
    }

    if (ml_analytics_info->nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *nf_type_listList = cJSON_AddArrayToObject(item, "nfTypeList");
    if (nf_type_listList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->nf_type_list, node) {
        if (cJSON_AddStringToObject(nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [nf_type_list]");
            goto end;
        }
    }
    }

    if (ml_analytics_info->nf_set_id_list) {
    cJSON *nf_set_id_listList = cJSON_AddArrayToObject(item, "nfSetIdList");
    if (nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->nf_set_id_list, node) {
        if (cJSON_AddStringToObject(nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [nf_set_id_list]");
            goto end;
        }
    }
    }

    if (ml_analytics_info->vfl_capability_type != OpenAPI_vfl_capability_type_NULL) {
    if (cJSON_AddStringToObject(item, "vflCapabilityType", OpenAPI_vfl_capability_type_ToString(ml_analytics_info->vfl_capability_type)) == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [vfl_capability_type]");
        goto end;
    }
    }

    if (ml_analytics_info->is_vfl_client_aggr_cap) {
    if (cJSON_AddBoolToObject(item, "vflClientAggrCap", ml_analytics_info->vfl_client_aggr_cap) == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [vfl_client_aggr_cap]");
        goto end;
    }
    }

    if (ml_analytics_info->vfl_time_interval) {
    cJSON *vfl_time_interval_local_JSON = OpenAPI_time_window_convertToJSON(ml_analytics_info->vfl_time_interval);
    if (vfl_time_interval_local_JSON == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [vfl_time_interval]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vflTimeInterval", vfl_time_interval_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [vfl_time_interval]");
        goto end;
    }
    }

    if (ml_analytics_info->vfl_inter_info) {
    cJSON *vfl_inter_info_local_JSON = OpenAPI_ml_model_inter_info_convertToJSON(ml_analytics_info->vfl_inter_info);
    if (vfl_inter_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [vfl_inter_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vflInterInfo", vfl_inter_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [vfl_inter_info]");
        goto end;
    }
    }

    if (ml_analytics_info->feature_ids) {
    cJSON *feature_idsList = cJSON_AddArrayToObject(item, "featureIds");
    if (feature_idsList == NULL) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [feature_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ml_analytics_info->feature_ids, node) {
        if (cJSON_AddStringToObject(feature_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed [feature_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_parseFromJSON(cJSON *ml_analytics_infoJSON)
{
    OpenAPI_ml_analytics_info_t *ml_analytics_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ml_analytics_ids = NULL;
    OpenAPI_list_t *ml_analytics_idsList = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *tracking_area_list = NULL;
    OpenAPI_list_t *tracking_area_listList = NULL;
    cJSON *ml_model_inter_info = NULL;
    OpenAPI_ml_model_inter_info_t *ml_model_inter_info_local_nonprim = NULL;
    cJSON *fl_capability_type = NULL;
    OpenAPI_fl_capability_type_e fl_capability_typeVariable = 0;
    cJSON *fl_time_interval = NULL;
    OpenAPI_time_window_t *fl_time_interval_local_nonprim = NULL;
    cJSON *nf_type_list = NULL;
    OpenAPI_list_t *nf_type_listList = NULL;
    cJSON *nf_set_id_list = NULL;
    OpenAPI_list_t *nf_set_id_listList = NULL;
    cJSON *vfl_capability_type = NULL;
    OpenAPI_vfl_capability_type_e vfl_capability_typeVariable = 0;
    cJSON *vfl_client_aggr_cap = NULL;
    cJSON *vfl_time_interval = NULL;
    OpenAPI_time_window_t *vfl_time_interval_local_nonprim = NULL;
    cJSON *vfl_inter_info = NULL;
    OpenAPI_ml_model_inter_info_t *vfl_inter_info_local_nonprim = NULL;
    cJSON *feature_ids = NULL;
    OpenAPI_list_t *feature_idsList = NULL;
    ml_analytics_ids = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "mlAnalyticsIds");
    if (ml_analytics_ids) {
        cJSON *ml_analytics_ids_local = NULL;
        if (!cJSON_IsArray(ml_analytics_ids)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [ml_analytics_ids]");
            goto end;
        }

        ml_analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_ids_local, ml_analytics_ids) {
            OpenAPI_nwdaf_event_e localEnum = OpenAPI_nwdaf_event_NULL;
            if (!cJSON_IsString(ml_analytics_ids_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [ml_analytics_ids]");
                goto end;
            }
            localEnum = OpenAPI_nwdaf_event_FromString(ml_analytics_ids_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ml_analytics_ids\" is not supported. Ignoring it ...",
                         ml_analytics_ids_local->valuestring);
            } else {
                OpenAPI_list_add(ml_analytics_idsList, (void *)localEnum);
            }
        }
        if (ml_analytics_idsList->count == 0) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed: Expected ml_analytics_idsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    snssai_list = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "snssaiList");
    if (snssai_list) {
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }
    }

    tracking_area_list = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "trackingAreaList");
    if (tracking_area_list) {
        cJSON *tracking_area_list_local = NULL;
        if (!cJSON_IsArray(tracking_area_list)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [tracking_area_list]");
            goto end;
        }

        tracking_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tracking_area_list_local, tracking_area_list) {
            if (!cJSON_IsObject(tracking_area_list_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [tracking_area_list]");
                goto end;
            }
            OpenAPI_tai_t *tracking_area_listItem = OpenAPI_tai_parseFromJSON(tracking_area_list_local);
            if (!tracking_area_listItem) {
                ogs_error("No tracking_area_listItem");
                goto end;
            }
            OpenAPI_list_add(tracking_area_listList, tracking_area_listItem);
        }
    }

    ml_model_inter_info = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "mlModelInterInfo");
    if (ml_model_inter_info) {
    ml_model_inter_info_local_nonprim = OpenAPI_ml_model_inter_info_parseFromJSON(ml_model_inter_info);
    if (!ml_model_inter_info_local_nonprim) {
        ogs_error("OpenAPI_ml_model_inter_info_parseFromJSON failed [ml_model_inter_info]");
        goto end;
    }
    }

    fl_capability_type = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "flCapabilityType");
    if (fl_capability_type) {
    if (!cJSON_IsString(fl_capability_type)) {
        ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [fl_capability_type]");
        goto end;
    }
    fl_capability_typeVariable = OpenAPI_fl_capability_type_FromString(fl_capability_type->valuestring);
    }

    fl_time_interval = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "flTimeInterval");
    if (fl_time_interval) {
    fl_time_interval_local_nonprim = OpenAPI_time_window_parseFromJSON(fl_time_interval);
    if (!fl_time_interval_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [fl_time_interval]");
        goto end;
    }
    }

    nf_type_list = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "nfTypeList");
    if (nf_type_list) {
        cJSON *nf_type_list_local = NULL;
        if (!cJSON_IsArray(nf_type_list)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [nf_type_list]");
            goto end;
        }

        nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_type_list_local, nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(nf_type_list_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [nf_type_list]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(nf_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"nf_type_list\" is not supported. Ignoring it ...",
                         nf_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(nf_type_listList, (void *)localEnum);
            }
        }
        if (nf_type_listList->count == 0) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed: Expected nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    nf_set_id_list = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "nfSetIdList");
    if (nf_set_id_list) {
        cJSON *nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(nf_set_id_list)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [nf_set_id_list]");
            goto end;
        }

        nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_set_id_list_local, nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_set_id_list_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_set_id_listList, ogs_strdup(nf_set_id_list_local->valuestring));
        }
    }

    vfl_capability_type = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "vflCapabilityType");
    if (vfl_capability_type) {
    if (!cJSON_IsString(vfl_capability_type)) {
        ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [vfl_capability_type]");
        goto end;
    }
    vfl_capability_typeVariable = OpenAPI_vfl_capability_type_FromString(vfl_capability_type->valuestring);
    }

    vfl_client_aggr_cap = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "vflClientAggrCap");
    if (vfl_client_aggr_cap) {
    if (!cJSON_IsBool(vfl_client_aggr_cap)) {
        ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [vfl_client_aggr_cap]");
        goto end;
    }
    }

    vfl_time_interval = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "vflTimeInterval");
    if (vfl_time_interval) {
    vfl_time_interval_local_nonprim = OpenAPI_time_window_parseFromJSON(vfl_time_interval);
    if (!vfl_time_interval_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [vfl_time_interval]");
        goto end;
    }
    }

    vfl_inter_info = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "vflInterInfo");
    if (vfl_inter_info) {
    vfl_inter_info_local_nonprim = OpenAPI_ml_model_inter_info_parseFromJSON(vfl_inter_info);
    if (!vfl_inter_info_local_nonprim) {
        ogs_error("OpenAPI_ml_model_inter_info_parseFromJSON failed [vfl_inter_info]");
        goto end;
    }
    }

    feature_ids = cJSON_GetObjectItemCaseSensitive(ml_analytics_infoJSON, "featureIds");
    if (feature_ids) {
        cJSON *feature_ids_local = NULL;
        if (!cJSON_IsArray(feature_ids)) {
            ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [feature_ids]");
            goto end;
        }

        feature_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(feature_ids_local, feature_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(feature_ids_local)) {
                ogs_error("OpenAPI_ml_analytics_info_parseFromJSON() failed [feature_ids]");
                goto end;
            }
            OpenAPI_list_add(feature_idsList, ogs_strdup(feature_ids_local->valuestring));
        }
    }

    ml_analytics_info_local_var = OpenAPI_ml_analytics_info_create (
        ml_analytics_ids ? ml_analytics_idsList : NULL,
        snssai_list ? snssai_listList : NULL,
        tracking_area_list ? tracking_area_listList : NULL,
        ml_model_inter_info ? ml_model_inter_info_local_nonprim : NULL,
        fl_capability_type ? fl_capability_typeVariable : 0,
        fl_time_interval ? fl_time_interval_local_nonprim : NULL,
        nf_type_list ? nf_type_listList : NULL,
        nf_set_id_list ? nf_set_id_listList : NULL,
        vfl_capability_type ? vfl_capability_typeVariable : 0,
        vfl_client_aggr_cap ? true : false,
        vfl_client_aggr_cap ? vfl_client_aggr_cap->valueint : 0,
        vfl_time_interval ? vfl_time_interval_local_nonprim : NULL,
        vfl_inter_info ? vfl_inter_info_local_nonprim : NULL,
        feature_ids ? feature_idsList : NULL
    );

    return ml_analytics_info_local_var;
end:
    if (ml_analytics_idsList) {
        OpenAPI_list_free(ml_analytics_idsList);
        ml_analytics_idsList = NULL;
    }
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
    }
    if (tracking_area_listList) {
        OpenAPI_list_for_each(tracking_area_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tracking_area_listList);
        tracking_area_listList = NULL;
    }
    if (ml_model_inter_info_local_nonprim) {
        OpenAPI_ml_model_inter_info_free(ml_model_inter_info_local_nonprim);
        ml_model_inter_info_local_nonprim = NULL;
    }
    if (fl_time_interval_local_nonprim) {
        OpenAPI_time_window_free(fl_time_interval_local_nonprim);
        fl_time_interval_local_nonprim = NULL;
    }
    if (nf_type_listList) {
        OpenAPI_list_free(nf_type_listList);
        nf_type_listList = NULL;
    }
    if (nf_set_id_listList) {
        OpenAPI_list_for_each(nf_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_set_id_listList);
        nf_set_id_listList = NULL;
    }
    if (vfl_time_interval_local_nonprim) {
        OpenAPI_time_window_free(vfl_time_interval_local_nonprim);
        vfl_time_interval_local_nonprim = NULL;
    }
    if (vfl_inter_info_local_nonprim) {
        OpenAPI_ml_model_inter_info_free(vfl_inter_info_local_nonprim);
        vfl_inter_info_local_nonprim = NULL;
    }
    if (feature_idsList) {
        OpenAPI_list_for_each(feature_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(feature_idsList);
        feature_idsList = NULL;
    }
    return NULL;
}

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_copy(OpenAPI_ml_analytics_info_t *dst, OpenAPI_ml_analytics_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ml_analytics_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ml_analytics_info_convertToJSON() failed");
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

    OpenAPI_ml_analytics_info_free(dst);
    dst = OpenAPI_ml_analytics_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

