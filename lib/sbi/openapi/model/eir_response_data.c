
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eir_response_data.h"

OpenAPI_eir_response_data_t *OpenAPI_eir_response_data_create(
    OpenAPI_equipment_status_e status
)
{
    OpenAPI_eir_response_data_t *eir_response_data_local_var = ogs_malloc(sizeof(OpenAPI_eir_response_data_t));
    ogs_assert(eir_response_data_local_var);

    eir_response_data_local_var->status = status;

    return eir_response_data_local_var;
}

void OpenAPI_eir_response_data_free(OpenAPI_eir_response_data_t *eir_response_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eir_response_data) {
        return;
    }
    ogs_free(eir_response_data);
}

cJSON *OpenAPI_eir_response_data_convertToJSON(OpenAPI_eir_response_data_t *eir_response_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eir_response_data == NULL) {
        ogs_error("OpenAPI_eir_response_data_convertToJSON() failed [EirResponseData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eir_response_data->status == OpenAPI_equipment_status_NULL) {
        ogs_error("OpenAPI_eir_response_data_convertToJSON() failed [status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "status", OpenAPI_equipment_status_ToString(eir_response_data->status)) == NULL) {
        ogs_error("OpenAPI_eir_response_data_convertToJSON() failed [status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_eir_response_data_t *OpenAPI_eir_response_data_parseFromJSON(cJSON *eir_response_dataJSON)
{
    OpenAPI_eir_response_data_t *eir_response_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *status = NULL;
    OpenAPI_equipment_status_e statusVariable = 0;
    status = cJSON_GetObjectItemCaseSensitive(eir_response_dataJSON, "status");
    if (!status) {
        ogs_error("OpenAPI_eir_response_data_parseFromJSON() failed [status]");
        goto end;
    }
    if (!cJSON_IsString(status)) {
        ogs_error("OpenAPI_eir_response_data_parseFromJSON() failed [status]");
        goto end;
    }
    statusVariable = OpenAPI_equipment_status_FromString(status->valuestring);

    eir_response_data_local_var = OpenAPI_eir_response_data_create (
        statusVariable
    );

    return eir_response_data_local_var;
end:
    return NULL;
}

OpenAPI_eir_response_data_t *OpenAPI_eir_response_data_copy(OpenAPI_eir_response_data_t *dst, OpenAPI_eir_response_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eir_response_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eir_response_data_convertToJSON() failed");
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

    OpenAPI_eir_response_data_free(dst);
    dst = OpenAPI_eir_response_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

