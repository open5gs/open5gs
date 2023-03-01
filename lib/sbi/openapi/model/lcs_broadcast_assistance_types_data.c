
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_broadcast_assistance_types_data.h"

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_create(
    OpenAPI_binary_t* location_assistance_type
)
{
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data_local_var = ogs_malloc(sizeof(OpenAPI_lcs_broadcast_assistance_types_data_t));
    ogs_assert(lcs_broadcast_assistance_types_data_local_var);

    lcs_broadcast_assistance_types_data_local_var->location_assistance_type = location_assistance_type;

    return lcs_broadcast_assistance_types_data_local_var;
}

void OpenAPI_lcs_broadcast_assistance_types_data_free(OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_broadcast_assistance_types_data) {
        return;
    }
    if (lcs_broadcast_assistance_types_data->location_assistance_type) {
        ogs_free(lcs_broadcast_assistance_types_data->location_assistance_type->data);
        lcs_broadcast_assistance_types_data->location_assistance_type = NULL;
    }
    ogs_free(lcs_broadcast_assistance_types_data);
}

cJSON *OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_broadcast_assistance_types_data == NULL) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed [LcsBroadcastAssistanceTypesData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!lcs_broadcast_assistance_types_data->location_assistance_type) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed [location_assistance_type]");
        return NULL;
    }
    char* encoded_str_location_assistance_type = OpenAPI_base64encode(lcs_broadcast_assistance_types_data->location_assistance_type->data,lcs_broadcast_assistance_types_data->location_assistance_type->len);
    if (cJSON_AddStringToObject(item, "locationAssistanceType", encoded_str_location_assistance_type) == NULL) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed [location_assistance_type]");
        goto end;
    }
    ogs_free(encoded_str_location_assistance_type);

end:
    return item;
}

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(cJSON *lcs_broadcast_assistance_types_dataJSON)
{
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location_assistance_type = NULL;
    OpenAPI_binary_t *decoded_str_location_assistance_type = NULL;
    location_assistance_type = cJSON_GetObjectItemCaseSensitive(lcs_broadcast_assistance_types_dataJSON, "locationAssistanceType");
    if (!location_assistance_type) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }
    decoded_str_location_assistance_type = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_location_assistance_type);
    if (!cJSON_IsString(location_assistance_type)) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }
    decoded_str_location_assistance_type->data = OpenAPI_base64decode(location_assistance_type->valuestring, strlen(location_assistance_type->valuestring), &decoded_str_location_assistance_type->len);
    if (!decoded_str_location_assistance_type->data) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }

    lcs_broadcast_assistance_types_data_local_var = OpenAPI_lcs_broadcast_assistance_types_data_create (
        decoded_str_location_assistance_type
    );

    return lcs_broadcast_assistance_types_data_local_var;
end:
    return NULL;
}

OpenAPI_lcs_broadcast_assistance_types_data_t *OpenAPI_lcs_broadcast_assistance_types_data_copy(OpenAPI_lcs_broadcast_assistance_types_data_t *dst, OpenAPI_lcs_broadcast_assistance_types_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON() failed");
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

    OpenAPI_lcs_broadcast_assistance_types_data_free(dst);
    dst = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

