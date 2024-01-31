
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_record_delivery_data.h"

OpenAPI_sms_record_delivery_data_t *OpenAPI_sms_record_delivery_data_create(
    char *sms_record_id,
    OpenAPI_sms_delivery_status_e delivery_status
)
{
    OpenAPI_sms_record_delivery_data_t *sms_record_delivery_data_local_var = ogs_malloc(sizeof(OpenAPI_sms_record_delivery_data_t));
    ogs_assert(sms_record_delivery_data_local_var);

    sms_record_delivery_data_local_var->sms_record_id = sms_record_id;
    sms_record_delivery_data_local_var->delivery_status = delivery_status;

    return sms_record_delivery_data_local_var;
}

void OpenAPI_sms_record_delivery_data_free(OpenAPI_sms_record_delivery_data_t *sms_record_delivery_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_record_delivery_data) {
        return;
    }
    if (sms_record_delivery_data->sms_record_id) {
        ogs_free(sms_record_delivery_data->sms_record_id);
        sms_record_delivery_data->sms_record_id = NULL;
    }
    ogs_free(sms_record_delivery_data);
}

cJSON *OpenAPI_sms_record_delivery_data_convertToJSON(OpenAPI_sms_record_delivery_data_t *sms_record_delivery_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_record_delivery_data == NULL) {
        ogs_error("OpenAPI_sms_record_delivery_data_convertToJSON() failed [SmsRecordDeliveryData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sms_record_delivery_data->sms_record_id) {
        ogs_error("OpenAPI_sms_record_delivery_data_convertToJSON() failed [sms_record_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smsRecordId", sms_record_delivery_data->sms_record_id) == NULL) {
        ogs_error("OpenAPI_sms_record_delivery_data_convertToJSON() failed [sms_record_id]");
        goto end;
    }

    if (sms_record_delivery_data->delivery_status == OpenAPI_sms_delivery_status_NULL) {
        ogs_error("OpenAPI_sms_record_delivery_data_convertToJSON() failed [delivery_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "deliveryStatus", OpenAPI_sms_delivery_status_ToString(sms_record_delivery_data->delivery_status)) == NULL) {
        ogs_error("OpenAPI_sms_record_delivery_data_convertToJSON() failed [delivery_status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sms_record_delivery_data_t *OpenAPI_sms_record_delivery_data_parseFromJSON(cJSON *sms_record_delivery_dataJSON)
{
    OpenAPI_sms_record_delivery_data_t *sms_record_delivery_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sms_record_id = NULL;
    cJSON *delivery_status = NULL;
    OpenAPI_sms_delivery_status_e delivery_statusVariable = 0;
    sms_record_id = cJSON_GetObjectItemCaseSensitive(sms_record_delivery_dataJSON, "smsRecordId");
    if (!sms_record_id) {
        ogs_error("OpenAPI_sms_record_delivery_data_parseFromJSON() failed [sms_record_id]");
        goto end;
    }
    if (!cJSON_IsString(sms_record_id)) {
        ogs_error("OpenAPI_sms_record_delivery_data_parseFromJSON() failed [sms_record_id]");
        goto end;
    }

    delivery_status = cJSON_GetObjectItemCaseSensitive(sms_record_delivery_dataJSON, "deliveryStatus");
    if (!delivery_status) {
        ogs_error("OpenAPI_sms_record_delivery_data_parseFromJSON() failed [delivery_status]");
        goto end;
    }
    if (!cJSON_IsString(delivery_status)) {
        ogs_error("OpenAPI_sms_record_delivery_data_parseFromJSON() failed [delivery_status]");
        goto end;
    }
    delivery_statusVariable = OpenAPI_sms_delivery_status_FromString(delivery_status->valuestring);

    sms_record_delivery_data_local_var = OpenAPI_sms_record_delivery_data_create (
        ogs_strdup(sms_record_id->valuestring),
        delivery_statusVariable
    );

    return sms_record_delivery_data_local_var;
end:
    return NULL;
}

OpenAPI_sms_record_delivery_data_t *OpenAPI_sms_record_delivery_data_copy(OpenAPI_sms_record_delivery_data_t *dst, OpenAPI_sms_record_delivery_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_record_delivery_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_record_delivery_data_convertToJSON() failed");
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

    OpenAPI_sms_record_delivery_data_free(dst);
    dst = OpenAPI_sms_record_delivery_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

