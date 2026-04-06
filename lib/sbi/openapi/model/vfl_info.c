
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vfl_info.h"

OpenAPI_vfl_info_t *OpenAPI_vfl_info_create(
    OpenAPI_list_t *ml_analytics_ids,
    OpenAPI_vfl_capability_type_e vfl_capability_type,
    bool is_vfl_client_aggr_cap,
    int vfl_client_aggr_cap,
    OpenAPI_time_window_t *vfl_time_interval,
    OpenAPI_ml_model_inter_info_t *vfl_inter_info,
    OpenAPI_list_t *feature_ids
)
{
    OpenAPI_vfl_info_t *vfl_info_local_var = ogs_malloc(sizeof(OpenAPI_vfl_info_t));
    ogs_assert(vfl_info_local_var);

    vfl_info_local_var->ml_analytics_ids = ml_analytics_ids;
    vfl_info_local_var->vfl_capability_type = vfl_capability_type;
    vfl_info_local_var->is_vfl_client_aggr_cap = is_vfl_client_aggr_cap;
    vfl_info_local_var->vfl_client_aggr_cap = vfl_client_aggr_cap;
    vfl_info_local_var->vfl_time_interval = vfl_time_interval;
    vfl_info_local_var->vfl_inter_info = vfl_inter_info;
    vfl_info_local_var->feature_ids = feature_ids;

    return vfl_info_local_var;
}

void OpenAPI_vfl_info_free(OpenAPI_vfl_info_t *vfl_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vfl_info) {
        return;
    }
    if (vfl_info->ml_analytics_ids) {
        OpenAPI_list_free(vfl_info->ml_analytics_ids);
        vfl_info->ml_analytics_ids = NULL;
    }
    if (vfl_info->vfl_time_interval) {
        OpenAPI_time_window_free(vfl_info->vfl_time_interval);
        vfl_info->vfl_time_interval = NULL;
    }
    if (vfl_info->vfl_inter_info) {
        OpenAPI_ml_model_inter_info_free(vfl_info->vfl_inter_info);
        vfl_info->vfl_inter_info = NULL;
    }
    if (vfl_info->feature_ids) {
        OpenAPI_list_for_each(vfl_info->feature_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(vfl_info->feature_ids);
        vfl_info->feature_ids = NULL;
    }
    ogs_free(vfl_info);
}

cJSON *OpenAPI_vfl_info_convertToJSON(OpenAPI_vfl_info_t *vfl_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vfl_info == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [VflInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vfl_info->ml_analytics_ids == OpenAPI_nwdaf_event_NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [ml_analytics_ids]");
        return NULL;
    }
    cJSON *ml_analytics_idsList = cJSON_AddArrayToObject(item, "mlAnalyticsIds");
    if (ml_analytics_idsList == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [ml_analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(vfl_info->ml_analytics_ids, node) {
        if (cJSON_AddStringToObject(ml_analytics_idsList, "", OpenAPI_nwdaf_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_vfl_info_convertToJSON() failed [ml_analytics_ids]");
            goto end;
        }
    }

    if (vfl_info->vfl_capability_type == OpenAPI_vfl_capability_type_NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_capability_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "vflCapabilityType", OpenAPI_vfl_capability_type_ToString(vfl_info->vfl_capability_type)) == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_capability_type]");
        goto end;
    }

    if (vfl_info->is_vfl_client_aggr_cap) {
    if (cJSON_AddBoolToObject(item, "vflClientAggrCap", vfl_info->vfl_client_aggr_cap) == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_client_aggr_cap]");
        goto end;
    }
    }

    if (vfl_info->vfl_time_interval) {
    cJSON *vfl_time_interval_local_JSON = OpenAPI_time_window_convertToJSON(vfl_info->vfl_time_interval);
    if (vfl_time_interval_local_JSON == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_time_interval]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vflTimeInterval", vfl_time_interval_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_time_interval]");
        goto end;
    }
    }

    if (vfl_info->vfl_inter_info) {
    cJSON *vfl_inter_info_local_JSON = OpenAPI_ml_model_inter_info_convertToJSON(vfl_info->vfl_inter_info);
    if (vfl_inter_info_local_JSON == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_inter_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vflInterInfo", vfl_inter_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [vfl_inter_info]");
        goto end;
    }
    }

    if (vfl_info->feature_ids) {
    cJSON *feature_idsList = cJSON_AddArrayToObject(item, "featureIds");
    if (feature_idsList == NULL) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed [feature_ids]");
        goto end;
    }
    OpenAPI_list_for_each(vfl_info->feature_ids, node) {
        if (cJSON_AddStringToObject(feature_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_vfl_info_convertToJSON() failed [feature_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_vfl_info_t *OpenAPI_vfl_info_parseFromJSON(cJSON *vfl_infoJSON)
{
    OpenAPI_vfl_info_t *vfl_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ml_analytics_ids = NULL;
    OpenAPI_list_t *ml_analytics_idsList = NULL;
    cJSON *vfl_capability_type = NULL;
    OpenAPI_vfl_capability_type_e vfl_capability_typeVariable = 0;
    cJSON *vfl_client_aggr_cap = NULL;
    cJSON *vfl_time_interval = NULL;
    OpenAPI_time_window_t *vfl_time_interval_local_nonprim = NULL;
    cJSON *vfl_inter_info = NULL;
    OpenAPI_ml_model_inter_info_t *vfl_inter_info_local_nonprim = NULL;
    cJSON *feature_ids = NULL;
    OpenAPI_list_t *feature_idsList = NULL;
    ml_analytics_ids = cJSON_GetObjectItemCaseSensitive(vfl_infoJSON, "mlAnalyticsIds");
    if (!ml_analytics_ids) {
        ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [ml_analytics_ids]");
        goto end;
    }
        cJSON *ml_analytics_ids_local = NULL;
        if (!cJSON_IsArray(ml_analytics_ids)) {
            ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [ml_analytics_ids]");
            goto end;
        }

        ml_analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_ids_local, ml_analytics_ids) {
            OpenAPI_nwdaf_event_e localEnum = OpenAPI_nwdaf_event_NULL;
            if (!cJSON_IsString(ml_analytics_ids_local)) {
                ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [ml_analytics_ids]");
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
            ogs_error("OpenAPI_vfl_info_parseFromJSON() failed: Expected ml_analytics_idsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    vfl_capability_type = cJSON_GetObjectItemCaseSensitive(vfl_infoJSON, "vflCapabilityType");
    if (!vfl_capability_type) {
        ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [vfl_capability_type]");
        goto end;
    }
    if (!cJSON_IsString(vfl_capability_type)) {
        ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [vfl_capability_type]");
        goto end;
    }
    vfl_capability_typeVariable = OpenAPI_vfl_capability_type_FromString(vfl_capability_type->valuestring);

    vfl_client_aggr_cap = cJSON_GetObjectItemCaseSensitive(vfl_infoJSON, "vflClientAggrCap");
    if (vfl_client_aggr_cap) {
    if (!cJSON_IsBool(vfl_client_aggr_cap)) {
        ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [vfl_client_aggr_cap]");
        goto end;
    }
    }

    vfl_time_interval = cJSON_GetObjectItemCaseSensitive(vfl_infoJSON, "vflTimeInterval");
    if (vfl_time_interval) {
    vfl_time_interval_local_nonprim = OpenAPI_time_window_parseFromJSON(vfl_time_interval);
    if (!vfl_time_interval_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [vfl_time_interval]");
        goto end;
    }
    }

    vfl_inter_info = cJSON_GetObjectItemCaseSensitive(vfl_infoJSON, "vflInterInfo");
    if (vfl_inter_info) {
    vfl_inter_info_local_nonprim = OpenAPI_ml_model_inter_info_parseFromJSON(vfl_inter_info);
    if (!vfl_inter_info_local_nonprim) {
        ogs_error("OpenAPI_ml_model_inter_info_parseFromJSON failed [vfl_inter_info]");
        goto end;
    }
    }

    feature_ids = cJSON_GetObjectItemCaseSensitive(vfl_infoJSON, "featureIds");
    if (feature_ids) {
        cJSON *feature_ids_local = NULL;
        if (!cJSON_IsArray(feature_ids)) {
            ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [feature_ids]");
            goto end;
        }

        feature_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(feature_ids_local, feature_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(feature_ids_local)) {
                ogs_error("OpenAPI_vfl_info_parseFromJSON() failed [feature_ids]");
                goto end;
            }
            OpenAPI_list_add(feature_idsList, ogs_strdup(feature_ids_local->valuestring));
        }
    }

    vfl_info_local_var = OpenAPI_vfl_info_create (
        ml_analytics_idsList,
        vfl_capability_typeVariable,
        vfl_client_aggr_cap ? true : false,
        vfl_client_aggr_cap ? vfl_client_aggr_cap->valueint : 0,
        vfl_time_interval ? vfl_time_interval_local_nonprim : NULL,
        vfl_inter_info ? vfl_inter_info_local_nonprim : NULL,
        feature_ids ? feature_idsList : NULL
    );

    return vfl_info_local_var;
end:
    if (ml_analytics_idsList) {
        OpenAPI_list_free(ml_analytics_idsList);
        ml_analytics_idsList = NULL;
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

OpenAPI_vfl_info_t *OpenAPI_vfl_info_copy(OpenAPI_vfl_info_t *dst, OpenAPI_vfl_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vfl_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vfl_info_convertToJSON() failed");
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

    OpenAPI_vfl_info_free(dst);
    dst = OpenAPI_vfl_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

