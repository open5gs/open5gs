
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rel_prox_info.h"

OpenAPI_rel_prox_info_t *OpenAPI_rel_prox_info_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *ue_proximities,
    OpenAPI_time_to_collision_info_t *ttc_info,
    OpenAPI_list_t *int_group_ids,
    OpenAPI_list_t *exter_group_ids
)
{
    OpenAPI_rel_prox_info_t *rel_prox_info_local_var = ogs_malloc(sizeof(OpenAPI_rel_prox_info_t));
    ogs_assert(rel_prox_info_local_var);

    rel_prox_info_local_var->ts_start = ts_start;
    rel_prox_info_local_var->ts_duration = ts_duration;
    rel_prox_info_local_var->supis = supis;
    rel_prox_info_local_var->gpsis = gpsis;
    rel_prox_info_local_var->ue_proximities = ue_proximities;
    rel_prox_info_local_var->ttc_info = ttc_info;
    rel_prox_info_local_var->int_group_ids = int_group_ids;
    rel_prox_info_local_var->exter_group_ids = exter_group_ids;

    return rel_prox_info_local_var;
}

void OpenAPI_rel_prox_info_free(OpenAPI_rel_prox_info_t *rel_prox_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rel_prox_info) {
        return;
    }
    if (rel_prox_info->ts_start) {
        ogs_free(rel_prox_info->ts_start);
        rel_prox_info->ts_start = NULL;
    }
    if (rel_prox_info->supis) {
        OpenAPI_list_for_each(rel_prox_info->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rel_prox_info->supis);
        rel_prox_info->supis = NULL;
    }
    if (rel_prox_info->gpsis) {
        OpenAPI_list_for_each(rel_prox_info->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rel_prox_info->gpsis);
        rel_prox_info->gpsis = NULL;
    }
    if (rel_prox_info->ue_proximities) {
        OpenAPI_list_for_each(rel_prox_info->ue_proximities, node) {
            OpenAPI_ue_proximity_free(node->data);
        }
        OpenAPI_list_free(rel_prox_info->ue_proximities);
        rel_prox_info->ue_proximities = NULL;
    }
    if (rel_prox_info->ttc_info) {
        OpenAPI_time_to_collision_info_free(rel_prox_info->ttc_info);
        rel_prox_info->ttc_info = NULL;
    }
    if (rel_prox_info->int_group_ids) {
        OpenAPI_list_for_each(rel_prox_info->int_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rel_prox_info->int_group_ids);
        rel_prox_info->int_group_ids = NULL;
    }
    if (rel_prox_info->exter_group_ids) {
        OpenAPI_list_for_each(rel_prox_info->exter_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rel_prox_info->exter_group_ids);
        rel_prox_info->exter_group_ids = NULL;
    }
    ogs_free(rel_prox_info);
}

cJSON *OpenAPI_rel_prox_info_convertToJSON(OpenAPI_rel_prox_info_t *rel_prox_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rel_prox_info == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [RelProxInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!rel_prox_info->ts_start) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", rel_prox_info->ts_start) == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", rel_prox_info->ts_duration) == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (rel_prox_info->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_info->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (rel_prox_info->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_info->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

    if (!rel_prox_info->ue_proximities) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ue_proximities]");
        return NULL;
    }
    cJSON *ue_proximitiesList = cJSON_AddArrayToObject(item, "ueProximities");
    if (ue_proximitiesList == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ue_proximities]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_info->ue_proximities, node) {
        cJSON *itemLocal = OpenAPI_ue_proximity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ue_proximities]");
            goto end;
        }
        cJSON_AddItemToArray(ue_proximitiesList, itemLocal);
    }

    if (rel_prox_info->ttc_info) {
    cJSON *ttc_info_local_JSON = OpenAPI_time_to_collision_info_convertToJSON(rel_prox_info->ttc_info);
    if (ttc_info_local_JSON == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ttc_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ttcInfo", ttc_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [ttc_info]");
        goto end;
    }
    }

    if (rel_prox_info->int_group_ids) {
    cJSON *int_group_idsList = cJSON_AddArrayToObject(item, "intGroupIds");
    if (int_group_idsList == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [int_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_info->int_group_ids, node) {
        if (cJSON_AddStringToObject(int_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [int_group_ids]");
            goto end;
        }
    }
    }

    if (rel_prox_info->exter_group_ids) {
    cJSON *exter_group_idsList = cJSON_AddArrayToObject(item, "exterGroupIds");
    if (exter_group_idsList == NULL) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [exter_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_info->exter_group_ids, node) {
        if (cJSON_AddStringToObject(exter_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed [exter_group_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_rel_prox_info_t *OpenAPI_rel_prox_info_parseFromJSON(cJSON *rel_prox_infoJSON)
{
    OpenAPI_rel_prox_info_t *rel_prox_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    cJSON *ue_proximities = NULL;
    OpenAPI_list_t *ue_proximitiesList = NULL;
    cJSON *ttc_info = NULL;
    OpenAPI_time_to_collision_info_t *ttc_info_local_nonprim = NULL;
    cJSON *int_group_ids = NULL;
    OpenAPI_list_t *int_group_idsList = NULL;
    cJSON *exter_group_ids = NULL;
    OpenAPI_list_t *exter_group_idsList = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    supis = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    ue_proximities = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "ueProximities");
    if (!ue_proximities) {
        ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ue_proximities]");
        goto end;
    }
        cJSON *ue_proximities_local = NULL;
        if (!cJSON_IsArray(ue_proximities)) {
            ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ue_proximities]");
            goto end;
        }

        ue_proximitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_proximities_local, ue_proximities) {
            if (!cJSON_IsObject(ue_proximities_local)) {
                ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [ue_proximities]");
                goto end;
            }
            OpenAPI_ue_proximity_t *ue_proximitiesItem = OpenAPI_ue_proximity_parseFromJSON(ue_proximities_local);
            if (!ue_proximitiesItem) {
                ogs_error("No ue_proximitiesItem");
                goto end;
            }
            OpenAPI_list_add(ue_proximitiesList, ue_proximitiesItem);
        }

    ttc_info = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "ttcInfo");
    if (ttc_info) {
    ttc_info_local_nonprim = OpenAPI_time_to_collision_info_parseFromJSON(ttc_info);
    if (!ttc_info_local_nonprim) {
        ogs_error("OpenAPI_time_to_collision_info_parseFromJSON failed [ttc_info]");
        goto end;
    }
    }

    int_group_ids = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "intGroupIds");
    if (int_group_ids) {
        cJSON *int_group_ids_local = NULL;
        if (!cJSON_IsArray(int_group_ids)) {
            ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [int_group_ids]");
            goto end;
        }

        int_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(int_group_ids_local, int_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(int_group_ids_local)) {
                ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [int_group_ids]");
                goto end;
            }
            OpenAPI_list_add(int_group_idsList, ogs_strdup(int_group_ids_local->valuestring));
        }
    }

    exter_group_ids = cJSON_GetObjectItemCaseSensitive(rel_prox_infoJSON, "exterGroupIds");
    if (exter_group_ids) {
        cJSON *exter_group_ids_local = NULL;
        if (!cJSON_IsArray(exter_group_ids)) {
            ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [exter_group_ids]");
            goto end;
        }

        exter_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(exter_group_ids_local, exter_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(exter_group_ids_local)) {
                ogs_error("OpenAPI_rel_prox_info_parseFromJSON() failed [exter_group_ids]");
                goto end;
            }
            OpenAPI_list_add(exter_group_idsList, ogs_strdup(exter_group_ids_local->valuestring));
        }
    }

    rel_prox_info_local_var = OpenAPI_rel_prox_info_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        supis ? supisList : NULL,
        gpsis ? gpsisList : NULL,
        ue_proximitiesList,
        ttc_info ? ttc_info_local_nonprim : NULL,
        int_group_ids ? int_group_idsList : NULL,
        exter_group_ids ? exter_group_idsList : NULL
    );

    return rel_prox_info_local_var;
end:
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (gpsisList) {
        OpenAPI_list_for_each(gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsisList);
        gpsisList = NULL;
    }
    if (ue_proximitiesList) {
        OpenAPI_list_for_each(ue_proximitiesList, node) {
            OpenAPI_ue_proximity_free(node->data);
        }
        OpenAPI_list_free(ue_proximitiesList);
        ue_proximitiesList = NULL;
    }
    if (ttc_info_local_nonprim) {
        OpenAPI_time_to_collision_info_free(ttc_info_local_nonprim);
        ttc_info_local_nonprim = NULL;
    }
    if (int_group_idsList) {
        OpenAPI_list_for_each(int_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(int_group_idsList);
        int_group_idsList = NULL;
    }
    if (exter_group_idsList) {
        OpenAPI_list_for_each(exter_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exter_group_idsList);
        exter_group_idsList = NULL;
    }
    return NULL;
}

OpenAPI_rel_prox_info_t *OpenAPI_rel_prox_info_copy(OpenAPI_rel_prox_info_t *dst, OpenAPI_rel_prox_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rel_prox_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rel_prox_info_convertToJSON() failed");
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

    OpenAPI_rel_prox_info_free(dst);
    dst = OpenAPI_rel_prox_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

