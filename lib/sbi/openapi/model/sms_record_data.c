
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_record_data.h"

OpenAPI_sms_record_data_t *OpenAPI_sms_record_data_create(
    char *sms_record_id,
    OpenAPI_ref_to_binary_data_t *sms_payload,
    OpenAPI_access_type_e access_type,
    char *gpsi,
    char *pei,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone
)
{
    OpenAPI_sms_record_data_t *sms_record_data_local_var = ogs_malloc(sizeof(OpenAPI_sms_record_data_t));
    ogs_assert(sms_record_data_local_var);

    sms_record_data_local_var->sms_record_id = sms_record_id;
    sms_record_data_local_var->sms_payload = sms_payload;
    sms_record_data_local_var->access_type = access_type;
    sms_record_data_local_var->gpsi = gpsi;
    sms_record_data_local_var->pei = pei;
    sms_record_data_local_var->ue_location = ue_location;
    sms_record_data_local_var->ue_time_zone = ue_time_zone;

    return sms_record_data_local_var;
}

void OpenAPI_sms_record_data_free(OpenAPI_sms_record_data_t *sms_record_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_record_data) {
        return;
    }
    if (sms_record_data->sms_record_id) {
        ogs_free(sms_record_data->sms_record_id);
        sms_record_data->sms_record_id = NULL;
    }
    if (sms_record_data->sms_payload) {
        OpenAPI_ref_to_binary_data_free(sms_record_data->sms_payload);
        sms_record_data->sms_payload = NULL;
    }
    if (sms_record_data->gpsi) {
        ogs_free(sms_record_data->gpsi);
        sms_record_data->gpsi = NULL;
    }
    if (sms_record_data->pei) {
        ogs_free(sms_record_data->pei);
        sms_record_data->pei = NULL;
    }
    if (sms_record_data->ue_location) {
        OpenAPI_user_location_free(sms_record_data->ue_location);
        sms_record_data->ue_location = NULL;
    }
    if (sms_record_data->ue_time_zone) {
        ogs_free(sms_record_data->ue_time_zone);
        sms_record_data->ue_time_zone = NULL;
    }
    ogs_free(sms_record_data);
}

cJSON *OpenAPI_sms_record_data_convertToJSON(OpenAPI_sms_record_data_t *sms_record_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_record_data == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [SmsRecordData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sms_record_data->sms_record_id) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [sms_record_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smsRecordId", sms_record_data->sms_record_id) == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [sms_record_id]");
        goto end;
    }

    if (!sms_record_data->sms_payload) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [sms_payload]");
        return NULL;
    }
    cJSON *sms_payload_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sms_record_data->sms_payload);
    if (sms_payload_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [sms_payload]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsPayload", sms_payload_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [sms_payload]");
        goto end;
    }

    if (sms_record_data->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(sms_record_data->access_type)) == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (sms_record_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", sms_record_data->gpsi) == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (sms_record_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", sms_record_data->pei) == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (sms_record_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sms_record_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (sms_record_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sms_record_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sms_record_data_t *OpenAPI_sms_record_data_parseFromJSON(cJSON *sms_record_dataJSON)
{
    OpenAPI_sms_record_data_t *sms_record_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sms_record_id = NULL;
    cJSON *sms_payload = NULL;
    OpenAPI_ref_to_binary_data_t *sms_payload_local_nonprim = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *gpsi = NULL;
    cJSON *pei = NULL;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    sms_record_id = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "smsRecordId");
    if (!sms_record_id) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [sms_record_id]");
        goto end;
    }
    if (!cJSON_IsString(sms_record_id)) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [sms_record_id]");
        goto end;
    }

    sms_payload = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "smsPayload");
    if (!sms_payload) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [sms_payload]");
        goto end;
    }
    sms_payload_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(sms_payload);
    if (!sms_payload_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [sms_payload]");
        goto end;
    }

    access_type = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sms_record_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sms_record_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    sms_record_data_local_var = OpenAPI_sms_record_data_create (
        ogs_strdup(sms_record_id->valuestring),
        sms_payload_local_nonprim,
        access_type ? access_typeVariable : 0,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL
    );

    return sms_record_data_local_var;
end:
    if (sms_payload_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(sms_payload_local_nonprim);
        sms_payload_local_nonprim = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sms_record_data_t *OpenAPI_sms_record_data_copy(OpenAPI_sms_record_data_t *dst, OpenAPI_sms_record_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_record_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_record_data_convertToJSON() failed");
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

    OpenAPI_sms_record_data_free(dst);
    dst = OpenAPI_sms_record_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

