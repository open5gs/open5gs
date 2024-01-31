
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_data.h"

OpenAPI_sms_data_t *OpenAPI_sms_data_create(
    OpenAPI_ref_to_binary_data_t *sms_payload
)
{
    OpenAPI_sms_data_t *sms_data_local_var = ogs_malloc(sizeof(OpenAPI_sms_data_t));
    ogs_assert(sms_data_local_var);

    sms_data_local_var->sms_payload = sms_payload;

    return sms_data_local_var;
}

void OpenAPI_sms_data_free(OpenAPI_sms_data_t *sms_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_data) {
        return;
    }
    if (sms_data->sms_payload) {
        OpenAPI_ref_to_binary_data_free(sms_data->sms_payload);
        sms_data->sms_payload = NULL;
    }
    ogs_free(sms_data);
}

cJSON *OpenAPI_sms_data_convertToJSON(OpenAPI_sms_data_t *sms_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_data == NULL) {
        ogs_error("OpenAPI_sms_data_convertToJSON() failed [SmsData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sms_data->sms_payload) {
        ogs_error("OpenAPI_sms_data_convertToJSON() failed [sms_payload]");
        return NULL;
    }
    cJSON *sms_payload_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sms_data->sms_payload);
    if (sms_payload_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_data_convertToJSON() failed [sms_payload]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsPayload", sms_payload_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_data_convertToJSON() failed [sms_payload]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sms_data_t *OpenAPI_sms_data_parseFromJSON(cJSON *sms_dataJSON)
{
    OpenAPI_sms_data_t *sms_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sms_payload = NULL;
    OpenAPI_ref_to_binary_data_t *sms_payload_local_nonprim = NULL;
    sms_payload = cJSON_GetObjectItemCaseSensitive(sms_dataJSON, "smsPayload");
    if (!sms_payload) {
        ogs_error("OpenAPI_sms_data_parseFromJSON() failed [sms_payload]");
        goto end;
    }
    sms_payload_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(sms_payload);
    if (!sms_payload_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [sms_payload]");
        goto end;
    }

    sms_data_local_var = OpenAPI_sms_data_create (
        sms_payload_local_nonprim
    );

    return sms_data_local_var;
end:
    if (sms_payload_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(sms_payload_local_nonprim);
        sms_payload_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sms_data_t *OpenAPI_sms_data_copy(OpenAPI_sms_data_t *dst, OpenAPI_sms_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_data_convertToJSON() failed");
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

    OpenAPI_sms_data_free(dst);
    dst = OpenAPI_sms_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

