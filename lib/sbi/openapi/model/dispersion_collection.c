
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_collection.h"

OpenAPI_dispersion_collection_t *OpenAPI_dispersion_collection_create(
    OpenAPI_user_location_t *ue_loc,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *app_volumes,
    bool is_disper_amount,
    int disper_amount,
    OpenAPI_dispersion_class_t *disper_class,
    bool is_usage_rank,
    int usage_rank,
    bool is_percentile_rank,
    int percentile_rank,
    bool is_ue_ratio,
    int ue_ratio,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_dispersion_collection_t *dispersion_collection_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_collection_t));
    ogs_assert(dispersion_collection_local_var);

    dispersion_collection_local_var->ue_loc = ue_loc;
    dispersion_collection_local_var->snssai = snssai;
    dispersion_collection_local_var->supis = supis;
    dispersion_collection_local_var->gpsis = gpsis;
    dispersion_collection_local_var->app_volumes = app_volumes;
    dispersion_collection_local_var->is_disper_amount = is_disper_amount;
    dispersion_collection_local_var->disper_amount = disper_amount;
    dispersion_collection_local_var->disper_class = disper_class;
    dispersion_collection_local_var->is_usage_rank = is_usage_rank;
    dispersion_collection_local_var->usage_rank = usage_rank;
    dispersion_collection_local_var->is_percentile_rank = is_percentile_rank;
    dispersion_collection_local_var->percentile_rank = percentile_rank;
    dispersion_collection_local_var->is_ue_ratio = is_ue_ratio;
    dispersion_collection_local_var->ue_ratio = ue_ratio;
    dispersion_collection_local_var->is_confidence = is_confidence;
    dispersion_collection_local_var->confidence = confidence;

    return dispersion_collection_local_var;
}

void OpenAPI_dispersion_collection_free(OpenAPI_dispersion_collection_t *dispersion_collection)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_collection) {
        return;
    }
    if (dispersion_collection->ue_loc) {
        OpenAPI_user_location_free(dispersion_collection->ue_loc);
        dispersion_collection->ue_loc = NULL;
    }
    if (dispersion_collection->snssai) {
        OpenAPI_snssai_free(dispersion_collection->snssai);
        dispersion_collection->snssai = NULL;
    }
    if (dispersion_collection->supis) {
        OpenAPI_list_for_each(dispersion_collection->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dispersion_collection->supis);
        dispersion_collection->supis = NULL;
    }
    if (dispersion_collection->gpsis) {
        OpenAPI_list_for_each(dispersion_collection->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dispersion_collection->gpsis);
        dispersion_collection->gpsis = NULL;
    }
    if (dispersion_collection->app_volumes) {
        OpenAPI_list_for_each(dispersion_collection->app_volumes, node) {
            OpenAPI_application_volume_free(node->data);
        }
        OpenAPI_list_free(dispersion_collection->app_volumes);
        dispersion_collection->app_volumes = NULL;
    }
    if (dispersion_collection->disper_class) {
        OpenAPI_dispersion_class_free(dispersion_collection->disper_class);
        dispersion_collection->disper_class = NULL;
    }
    ogs_free(dispersion_collection);
}

cJSON *OpenAPI_dispersion_collection_convertToJSON(OpenAPI_dispersion_collection_t *dispersion_collection)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_collection == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [DispersionCollection]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dispersion_collection->ue_loc) {
    cJSON *ue_loc_local_JSON = OpenAPI_user_location_convertToJSON(dispersion_collection->ue_loc);
    if (ue_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [ue_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLoc", ue_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [ue_loc]");
        goto end;
    }
    }

    if (dispersion_collection->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(dispersion_collection->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (dispersion_collection->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_collection->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (dispersion_collection->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_collection->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

    if (dispersion_collection->app_volumes) {
    cJSON *app_volumesList = cJSON_AddArrayToObject(item, "appVolumes");
    if (app_volumesList == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [app_volumes]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_collection->app_volumes, node) {
        cJSON *itemLocal = OpenAPI_application_volume_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [app_volumes]");
            goto end;
        }
        cJSON_AddItemToArray(app_volumesList, itemLocal);
    }
    }

    if (dispersion_collection->is_disper_amount) {
    if (cJSON_AddNumberToObject(item, "disperAmount", dispersion_collection->disper_amount) == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [disper_amount]");
        goto end;
    }
    }

    if (dispersion_collection->disper_class) {
    cJSON *disper_class_local_JSON = OpenAPI_dispersion_class_convertToJSON(dispersion_collection->disper_class);
    if (disper_class_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [disper_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "disperClass", disper_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [disper_class]");
        goto end;
    }
    }

    if (dispersion_collection->is_usage_rank) {
    if (cJSON_AddNumberToObject(item, "usageRank", dispersion_collection->usage_rank) == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [usage_rank]");
        goto end;
    }
    }

    if (dispersion_collection->is_percentile_rank) {
    if (cJSON_AddNumberToObject(item, "percentileRank", dispersion_collection->percentile_rank) == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [percentile_rank]");
        goto end;
    }
    }

    if (dispersion_collection->is_ue_ratio) {
    if (cJSON_AddNumberToObject(item, "ueRatio", dispersion_collection->ue_ratio) == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [ue_ratio]");
        goto end;
    }
    }

    if (dispersion_collection->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", dispersion_collection->confidence) == NULL) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dispersion_collection_t *OpenAPI_dispersion_collection_parseFromJSON(cJSON *dispersion_collectionJSON)
{
    OpenAPI_dispersion_collection_t *dispersion_collection_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_loc = NULL;
    OpenAPI_user_location_t *ue_loc_local_nonprim = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    cJSON *app_volumes = NULL;
    OpenAPI_list_t *app_volumesList = NULL;
    cJSON *disper_amount = NULL;
    cJSON *disper_class = NULL;
    OpenAPI_dispersion_class_t *disper_class_local_nonprim = NULL;
    cJSON *usage_rank = NULL;
    cJSON *percentile_rank = NULL;
    cJSON *ue_ratio = NULL;
    cJSON *confidence = NULL;
    ue_loc = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "ueLoc");
    if (ue_loc) {
    ue_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_loc);
    if (!ue_loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_loc]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    supis = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    app_volumes = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "appVolumes");
    if (app_volumes) {
        cJSON *app_volumes_local = NULL;
        if (!cJSON_IsArray(app_volumes)) {
            ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [app_volumes]");
            goto end;
        }

        app_volumesList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_volumes_local, app_volumes) {
            if (!cJSON_IsObject(app_volumes_local)) {
                ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [app_volumes]");
                goto end;
            }
            OpenAPI_application_volume_t *app_volumesItem = OpenAPI_application_volume_parseFromJSON(app_volumes_local);
            if (!app_volumesItem) {
                ogs_error("No app_volumesItem");
                goto end;
            }
            OpenAPI_list_add(app_volumesList, app_volumesItem);
        }
    }

    disper_amount = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "disperAmount");
    if (disper_amount) {
    if (!cJSON_IsNumber(disper_amount)) {
        ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [disper_amount]");
        goto end;
    }
    }

    disper_class = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "disperClass");
    if (disper_class) {
    disper_class_local_nonprim = OpenAPI_dispersion_class_parseFromJSON(disper_class);
    if (!disper_class_local_nonprim) {
        ogs_error("OpenAPI_dispersion_class_parseFromJSON failed [disper_class]");
        goto end;
    }
    }

    usage_rank = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "usageRank");
    if (usage_rank) {
    if (!cJSON_IsNumber(usage_rank)) {
        ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [usage_rank]");
        goto end;
    }
    }

    percentile_rank = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "percentileRank");
    if (percentile_rank) {
    if (!cJSON_IsNumber(percentile_rank)) {
        ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [percentile_rank]");
        goto end;
    }
    }

    ue_ratio = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "ueRatio");
    if (ue_ratio) {
    if (!cJSON_IsNumber(ue_ratio)) {
        ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [ue_ratio]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(dispersion_collectionJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_dispersion_collection_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    dispersion_collection_local_var = OpenAPI_dispersion_collection_create (
        ue_loc ? ue_loc_local_nonprim : NULL,
        snssai ? snssai_local_nonprim : NULL,
        supis ? supisList : NULL,
        gpsis ? gpsisList : NULL,
        app_volumes ? app_volumesList : NULL,
        disper_amount ? true : false,
        disper_amount ? disper_amount->valuedouble : 0,
        disper_class ? disper_class_local_nonprim : NULL,
        usage_rank ? true : false,
        usage_rank ? usage_rank->valuedouble : 0,
        percentile_rank ? true : false,
        percentile_rank ? percentile_rank->valuedouble : 0,
        ue_ratio ? true : false,
        ue_ratio ? ue_ratio->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return dispersion_collection_local_var;
end:
    if (ue_loc_local_nonprim) {
        OpenAPI_user_location_free(ue_loc_local_nonprim);
        ue_loc_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
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
    if (app_volumesList) {
        OpenAPI_list_for_each(app_volumesList, node) {
            OpenAPI_application_volume_free(node->data);
        }
        OpenAPI_list_free(app_volumesList);
        app_volumesList = NULL;
    }
    if (disper_class_local_nonprim) {
        OpenAPI_dispersion_class_free(disper_class_local_nonprim);
        disper_class_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_dispersion_collection_t *OpenAPI_dispersion_collection_copy(OpenAPI_dispersion_collection_t *dst, OpenAPI_dispersion_collection_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_collection_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_collection_convertToJSON() failed");
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

    OpenAPI_dispersion_collection_free(dst);
    dst = OpenAPI_dispersion_collection_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

