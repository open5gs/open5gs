
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "e2e_data_vol_trans_time_per_ue.h"

OpenAPI_e2e_data_vol_trans_time_per_ue_t *OpenAPI_e2e_data_vol_trans_time_per_ue_create(
    char *supi,
    char *gpsi,
    OpenAPI_snssai_t *snssai,
    OpenAPI_access_type_e access_type,
    OpenAPI_list_t *rat_types,
    char *app_id,
    OpenAPI_user_location_t *ue_loc,
    char *dnn,
    OpenAPI_network_area_info_t *spatial_validity,
    OpenAPI_time_window_t *validity_period,
    OpenAPI_data_volume_transfer_time_t *data_vol_trans_time
)
{
    OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ue_local_var = ogs_malloc(sizeof(OpenAPI_e2e_data_vol_trans_time_per_ue_t));
    ogs_assert(e2e_data_vol_trans_time_per_ue_local_var);

    e2e_data_vol_trans_time_per_ue_local_var->supi = supi;
    e2e_data_vol_trans_time_per_ue_local_var->gpsi = gpsi;
    e2e_data_vol_trans_time_per_ue_local_var->snssai = snssai;
    e2e_data_vol_trans_time_per_ue_local_var->access_type = access_type;
    e2e_data_vol_trans_time_per_ue_local_var->rat_types = rat_types;
    e2e_data_vol_trans_time_per_ue_local_var->app_id = app_id;
    e2e_data_vol_trans_time_per_ue_local_var->ue_loc = ue_loc;
    e2e_data_vol_trans_time_per_ue_local_var->dnn = dnn;
    e2e_data_vol_trans_time_per_ue_local_var->spatial_validity = spatial_validity;
    e2e_data_vol_trans_time_per_ue_local_var->validity_period = validity_period;
    e2e_data_vol_trans_time_per_ue_local_var->data_vol_trans_time = data_vol_trans_time;

    return e2e_data_vol_trans_time_per_ue_local_var;
}

void OpenAPI_e2e_data_vol_trans_time_per_ue_free(OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ue)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == e2e_data_vol_trans_time_per_ue) {
        return;
    }
    if (e2e_data_vol_trans_time_per_ue->supi) {
        ogs_free(e2e_data_vol_trans_time_per_ue->supi);
        e2e_data_vol_trans_time_per_ue->supi = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->gpsi) {
        ogs_free(e2e_data_vol_trans_time_per_ue->gpsi);
        e2e_data_vol_trans_time_per_ue->gpsi = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->snssai) {
        OpenAPI_snssai_free(e2e_data_vol_trans_time_per_ue->snssai);
        e2e_data_vol_trans_time_per_ue->snssai = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->rat_types) {
        OpenAPI_list_free(e2e_data_vol_trans_time_per_ue->rat_types);
        e2e_data_vol_trans_time_per_ue->rat_types = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->app_id) {
        ogs_free(e2e_data_vol_trans_time_per_ue->app_id);
        e2e_data_vol_trans_time_per_ue->app_id = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->ue_loc) {
        OpenAPI_user_location_free(e2e_data_vol_trans_time_per_ue->ue_loc);
        e2e_data_vol_trans_time_per_ue->ue_loc = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->dnn) {
        ogs_free(e2e_data_vol_trans_time_per_ue->dnn);
        e2e_data_vol_trans_time_per_ue->dnn = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->spatial_validity) {
        OpenAPI_network_area_info_free(e2e_data_vol_trans_time_per_ue->spatial_validity);
        e2e_data_vol_trans_time_per_ue->spatial_validity = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->validity_period) {
        OpenAPI_time_window_free(e2e_data_vol_trans_time_per_ue->validity_period);
        e2e_data_vol_trans_time_per_ue->validity_period = NULL;
    }
    if (e2e_data_vol_trans_time_per_ue->data_vol_trans_time) {
        OpenAPI_data_volume_transfer_time_free(e2e_data_vol_trans_time_per_ue->data_vol_trans_time);
        e2e_data_vol_trans_time_per_ue->data_vol_trans_time = NULL;
    }
    ogs_free(e2e_data_vol_trans_time_per_ue);
}

cJSON *OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON(OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ue)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (e2e_data_vol_trans_time_per_ue == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [E2eDataVolTransTimePerUe]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (e2e_data_vol_trans_time_per_ue->supi) {
    if (cJSON_AddStringToObject(item, "supi", e2e_data_vol_trans_time_per_ue->supi) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", e2e_data_vol_trans_time_per_ue->gpsi) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(e2e_data_vol_trans_time_per_ue->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(e2e_data_vol_trans_time_per_ue->access_type)) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->rat_types != OpenAPI_rat_type_NULL) {
    cJSON *rat_typesList = cJSON_AddArrayToObject(item, "ratTypes");
    if (rat_typesList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_per_ue->rat_types, node) {
        if (cJSON_AddStringToObject(rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [rat_types]");
            goto end;
        }
    }
    }

    if (e2e_data_vol_trans_time_per_ue->app_id) {
    if (cJSON_AddStringToObject(item, "appId", e2e_data_vol_trans_time_per_ue->app_id) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->ue_loc) {
    cJSON *ue_loc_local_JSON = OpenAPI_user_location_convertToJSON(e2e_data_vol_trans_time_per_ue->ue_loc);
    if (ue_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [ue_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLoc", ue_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [ue_loc]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", e2e_data_vol_trans_time_per_ue->dnn) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->spatial_validity) {
    cJSON *spatial_validity_local_JSON = OpenAPI_network_area_info_convertToJSON(e2e_data_vol_trans_time_per_ue->spatial_validity);
    if (spatial_validity_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [spatial_validity]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidity", spatial_validity_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [spatial_validity]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->validity_period) {
    cJSON *validity_period_local_JSON = OpenAPI_time_window_convertToJSON(e2e_data_vol_trans_time_per_ue->validity_period);
    if (validity_period_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [validity_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validityPeriod", validity_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [validity_period]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_per_ue->data_vol_trans_time) {
    cJSON *data_vol_trans_time_local_JSON = OpenAPI_data_volume_transfer_time_convertToJSON(e2e_data_vol_trans_time_per_ue->data_vol_trans_time);
    if (data_vol_trans_time_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [data_vol_trans_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dataVolTransTime", data_vol_trans_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed [data_vol_trans_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_e2e_data_vol_trans_time_per_ue_t *OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON(cJSON *e2e_data_vol_trans_time_per_ueJSON)
{
    OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ue_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *rat_types = NULL;
    OpenAPI_list_t *rat_typesList = NULL;
    cJSON *app_id = NULL;
    cJSON *ue_loc = NULL;
    OpenAPI_user_location_t *ue_loc_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *spatial_validity = NULL;
    OpenAPI_network_area_info_t *spatial_validity_local_nonprim = NULL;
    cJSON *validity_period = NULL;
    OpenAPI_time_window_t *validity_period_local_nonprim = NULL;
    cJSON *data_vol_trans_time = NULL;
    OpenAPI_data_volume_transfer_time_t *data_vol_trans_time_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    rat_types = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "ratTypes");
    if (rat_types) {
        cJSON *rat_types_local = NULL;
        if (!cJSON_IsArray(rat_types)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [rat_types]");
            goto end;
        }

        rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_types_local, rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_types_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_types\" is not supported. Ignoring it ...",
                         rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(rat_typesList, (void *)localEnum);
            }
        }
        if (rat_typesList->count == 0) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed: Expected rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    ue_loc = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "ueLoc");
    if (ue_loc) {
    ue_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_loc);
    if (!ue_loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_loc]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    spatial_validity = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "spatialValidity");
    if (spatial_validity) {
    spatial_validity_local_nonprim = OpenAPI_network_area_info_parseFromJSON(spatial_validity);
    if (!spatial_validity_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [spatial_validity]");
        goto end;
    }
    }

    validity_period = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "validityPeriod");
    if (validity_period) {
    validity_period_local_nonprim = OpenAPI_time_window_parseFromJSON(validity_period);
    if (!validity_period_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [validity_period]");
        goto end;
    }
    }

    data_vol_trans_time = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_ueJSON, "dataVolTransTime");
    if (data_vol_trans_time) {
    data_vol_trans_time_local_nonprim = OpenAPI_data_volume_transfer_time_parseFromJSON(data_vol_trans_time);
    if (!data_vol_trans_time_local_nonprim) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON failed [data_vol_trans_time]");
        goto end;
    }
    }

    e2e_data_vol_trans_time_per_ue_local_var = OpenAPI_e2e_data_vol_trans_time_per_ue_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        access_type ? access_typeVariable : 0,
        rat_types ? rat_typesList : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        ue_loc ? ue_loc_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        spatial_validity ? spatial_validity_local_nonprim : NULL,
        validity_period ? validity_period_local_nonprim : NULL,
        data_vol_trans_time ? data_vol_trans_time_local_nonprim : NULL
    );

    return e2e_data_vol_trans_time_per_ue_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (rat_typesList) {
        OpenAPI_list_free(rat_typesList);
        rat_typesList = NULL;
    }
    if (ue_loc_local_nonprim) {
        OpenAPI_user_location_free(ue_loc_local_nonprim);
        ue_loc_local_nonprim = NULL;
    }
    if (spatial_validity_local_nonprim) {
        OpenAPI_network_area_info_free(spatial_validity_local_nonprim);
        spatial_validity_local_nonprim = NULL;
    }
    if (validity_period_local_nonprim) {
        OpenAPI_time_window_free(validity_period_local_nonprim);
        validity_period_local_nonprim = NULL;
    }
    if (data_vol_trans_time_local_nonprim) {
        OpenAPI_data_volume_transfer_time_free(data_vol_trans_time_local_nonprim);
        data_vol_trans_time_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_e2e_data_vol_trans_time_per_ue_t *OpenAPI_e2e_data_vol_trans_time_per_ue_copy(OpenAPI_e2e_data_vol_trans_time_per_ue_t *dst, OpenAPI_e2e_data_vol_trans_time_per_ue_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON() failed");
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

    OpenAPI_e2e_data_vol_trans_time_per_ue_free(dst);
    dst = OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

