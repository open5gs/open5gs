
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "e2e_data_vol_trans_time_ue_list.h"

OpenAPI_e2e_data_vol_trans_time_ue_list_t *OpenAPI_e2e_data_vol_trans_time_ue_list_create(
    OpenAPI_list_t *high_level,
    OpenAPI_list_t *medium_level,
    OpenAPI_list_t *low_level,
    bool is_low_ratio,
    int low_ratio,
    bool is_medium_ratio,
    int medium_ratio,
    bool is_high_ratio,
    int high_ratio,
    OpenAPI_network_area_info_t *spatial_validity,
    OpenAPI_time_window_t *validity_period
)
{
    OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_list_local_var = ogs_malloc(sizeof(OpenAPI_e2e_data_vol_trans_time_ue_list_t));
    ogs_assert(e2e_data_vol_trans_time_ue_list_local_var);

    e2e_data_vol_trans_time_ue_list_local_var->high_level = high_level;
    e2e_data_vol_trans_time_ue_list_local_var->medium_level = medium_level;
    e2e_data_vol_trans_time_ue_list_local_var->low_level = low_level;
    e2e_data_vol_trans_time_ue_list_local_var->is_low_ratio = is_low_ratio;
    e2e_data_vol_trans_time_ue_list_local_var->low_ratio = low_ratio;
    e2e_data_vol_trans_time_ue_list_local_var->is_medium_ratio = is_medium_ratio;
    e2e_data_vol_trans_time_ue_list_local_var->medium_ratio = medium_ratio;
    e2e_data_vol_trans_time_ue_list_local_var->is_high_ratio = is_high_ratio;
    e2e_data_vol_trans_time_ue_list_local_var->high_ratio = high_ratio;
    e2e_data_vol_trans_time_ue_list_local_var->spatial_validity = spatial_validity;
    e2e_data_vol_trans_time_ue_list_local_var->validity_period = validity_period;

    return e2e_data_vol_trans_time_ue_list_local_var;
}

void OpenAPI_e2e_data_vol_trans_time_ue_list_free(OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == e2e_data_vol_trans_time_ue_list) {
        return;
    }
    if (e2e_data_vol_trans_time_ue_list->high_level) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_list->high_level, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_ue_list->high_level);
        e2e_data_vol_trans_time_ue_list->high_level = NULL;
    }
    if (e2e_data_vol_trans_time_ue_list->medium_level) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_list->medium_level, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_ue_list->medium_level);
        e2e_data_vol_trans_time_ue_list->medium_level = NULL;
    }
    if (e2e_data_vol_trans_time_ue_list->low_level) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_list->low_level, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_ue_list->low_level);
        e2e_data_vol_trans_time_ue_list->low_level = NULL;
    }
    if (e2e_data_vol_trans_time_ue_list->spatial_validity) {
        OpenAPI_network_area_info_free(e2e_data_vol_trans_time_ue_list->spatial_validity);
        e2e_data_vol_trans_time_ue_list->spatial_validity = NULL;
    }
    if (e2e_data_vol_trans_time_ue_list->validity_period) {
        OpenAPI_time_window_free(e2e_data_vol_trans_time_ue_list->validity_period);
        e2e_data_vol_trans_time_ue_list->validity_period = NULL;
    }
    ogs_free(e2e_data_vol_trans_time_ue_list);
}

cJSON *OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON(OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (e2e_data_vol_trans_time_ue_list == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [E2eDataVolTransTimeUeList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (e2e_data_vol_trans_time_ue_list->high_level) {
    cJSON *high_levelList = cJSON_AddArrayToObject(item, "highLevel");
    if (high_levelList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [high_level]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_list->high_level, node) {
        if (cJSON_AddStringToObject(high_levelList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [high_level]");
            goto end;
        }
    }
    }

    if (e2e_data_vol_trans_time_ue_list->medium_level) {
    cJSON *medium_levelList = cJSON_AddArrayToObject(item, "mediumLevel");
    if (medium_levelList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [medium_level]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_list->medium_level, node) {
        if (cJSON_AddStringToObject(medium_levelList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [medium_level]");
            goto end;
        }
    }
    }

    if (e2e_data_vol_trans_time_ue_list->low_level) {
    cJSON *low_levelList = cJSON_AddArrayToObject(item, "lowLevel");
    if (low_levelList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [low_level]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_ue_list->low_level, node) {
        if (cJSON_AddStringToObject(low_levelList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [low_level]");
            goto end;
        }
    }
    }

    if (e2e_data_vol_trans_time_ue_list->is_low_ratio) {
    if (cJSON_AddNumberToObject(item, "lowRatio", e2e_data_vol_trans_time_ue_list->low_ratio) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [low_ratio]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_ue_list->is_medium_ratio) {
    if (cJSON_AddNumberToObject(item, "mediumRatio", e2e_data_vol_trans_time_ue_list->medium_ratio) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [medium_ratio]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_ue_list->is_high_ratio) {
    if (cJSON_AddNumberToObject(item, "highRatio", e2e_data_vol_trans_time_ue_list->high_ratio) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [high_ratio]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_ue_list->spatial_validity) {
    cJSON *spatial_validity_local_JSON = OpenAPI_network_area_info_convertToJSON(e2e_data_vol_trans_time_ue_list->spatial_validity);
    if (spatial_validity_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [spatial_validity]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidity", spatial_validity_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [spatial_validity]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_ue_list->validity_period) {
    cJSON *validity_period_local_JSON = OpenAPI_time_window_convertToJSON(e2e_data_vol_trans_time_ue_list->validity_period);
    if (validity_period_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [validity_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validityPeriod", validity_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed [validity_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_e2e_data_vol_trans_time_ue_list_t *OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON(cJSON *e2e_data_vol_trans_time_ue_listJSON)
{
    OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *high_level = NULL;
    OpenAPI_list_t *high_levelList = NULL;
    cJSON *medium_level = NULL;
    OpenAPI_list_t *medium_levelList = NULL;
    cJSON *low_level = NULL;
    OpenAPI_list_t *low_levelList = NULL;
    cJSON *low_ratio = NULL;
    cJSON *medium_ratio = NULL;
    cJSON *high_ratio = NULL;
    cJSON *spatial_validity = NULL;
    OpenAPI_network_area_info_t *spatial_validity_local_nonprim = NULL;
    cJSON *validity_period = NULL;
    OpenAPI_time_window_t *validity_period_local_nonprim = NULL;
    high_level = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "highLevel");
    if (high_level) {
        cJSON *high_level_local = NULL;
        if (!cJSON_IsArray(high_level)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [high_level]");
            goto end;
        }

        high_levelList = OpenAPI_list_create();

        cJSON_ArrayForEach(high_level_local, high_level) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(high_level_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [high_level]");
                goto end;
            }
            OpenAPI_list_add(high_levelList, ogs_strdup(high_level_local->valuestring));
        }
    }

    medium_level = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "mediumLevel");
    if (medium_level) {
        cJSON *medium_level_local = NULL;
        if (!cJSON_IsArray(medium_level)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [medium_level]");
            goto end;
        }

        medium_levelList = OpenAPI_list_create();

        cJSON_ArrayForEach(medium_level_local, medium_level) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(medium_level_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [medium_level]");
                goto end;
            }
            OpenAPI_list_add(medium_levelList, ogs_strdup(medium_level_local->valuestring));
        }
    }

    low_level = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "lowLevel");
    if (low_level) {
        cJSON *low_level_local = NULL;
        if (!cJSON_IsArray(low_level)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [low_level]");
            goto end;
        }

        low_levelList = OpenAPI_list_create();

        cJSON_ArrayForEach(low_level_local, low_level) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(low_level_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [low_level]");
                goto end;
            }
            OpenAPI_list_add(low_levelList, ogs_strdup(low_level_local->valuestring));
        }
    }

    low_ratio = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "lowRatio");
    if (low_ratio) {
    if (!cJSON_IsNumber(low_ratio)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [low_ratio]");
        goto end;
    }
    }

    medium_ratio = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "mediumRatio");
    if (medium_ratio) {
    if (!cJSON_IsNumber(medium_ratio)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [medium_ratio]");
        goto end;
    }
    }

    high_ratio = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "highRatio");
    if (high_ratio) {
    if (!cJSON_IsNumber(high_ratio)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON() failed [high_ratio]");
        goto end;
    }
    }

    spatial_validity = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "spatialValidity");
    if (spatial_validity) {
    spatial_validity_local_nonprim = OpenAPI_network_area_info_parseFromJSON(spatial_validity);
    if (!spatial_validity_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [spatial_validity]");
        goto end;
    }
    }

    validity_period = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_ue_listJSON, "validityPeriod");
    if (validity_period) {
    validity_period_local_nonprim = OpenAPI_time_window_parseFromJSON(validity_period);
    if (!validity_period_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [validity_period]");
        goto end;
    }
    }

    e2e_data_vol_trans_time_ue_list_local_var = OpenAPI_e2e_data_vol_trans_time_ue_list_create (
        high_level ? high_levelList : NULL,
        medium_level ? medium_levelList : NULL,
        low_level ? low_levelList : NULL,
        low_ratio ? true : false,
        low_ratio ? low_ratio->valuedouble : 0,
        medium_ratio ? true : false,
        medium_ratio ? medium_ratio->valuedouble : 0,
        high_ratio ? true : false,
        high_ratio ? high_ratio->valuedouble : 0,
        spatial_validity ? spatial_validity_local_nonprim : NULL,
        validity_period ? validity_period_local_nonprim : NULL
    );

    return e2e_data_vol_trans_time_ue_list_local_var;
end:
    if (high_levelList) {
        OpenAPI_list_for_each(high_levelList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(high_levelList);
        high_levelList = NULL;
    }
    if (medium_levelList) {
        OpenAPI_list_for_each(medium_levelList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(medium_levelList);
        medium_levelList = NULL;
    }
    if (low_levelList) {
        OpenAPI_list_for_each(low_levelList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(low_levelList);
        low_levelList = NULL;
    }
    if (spatial_validity_local_nonprim) {
        OpenAPI_network_area_info_free(spatial_validity_local_nonprim);
        spatial_validity_local_nonprim = NULL;
    }
    if (validity_period_local_nonprim) {
        OpenAPI_time_window_free(validity_period_local_nonprim);
        validity_period_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_e2e_data_vol_trans_time_ue_list_t *OpenAPI_e2e_data_vol_trans_time_ue_list_copy(OpenAPI_e2e_data_vol_trans_time_ue_list_t *dst, OpenAPI_e2e_data_vol_trans_time_ue_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON() failed");
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

    OpenAPI_e2e_data_vol_trans_time_ue_list_free(dst);
    dst = OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

