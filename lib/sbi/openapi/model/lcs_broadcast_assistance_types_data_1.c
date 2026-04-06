
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_broadcast_assistance_types_data_1.h"

OpenAPI_lcs_broadcast_assistance_types_data_1_t *OpenAPI_lcs_broadcast_assistance_types_data_1_create(
    char *location_assistance_type
)
{
    OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data_1_local_var = ogs_malloc(sizeof(OpenAPI_lcs_broadcast_assistance_types_data_1_t));
    ogs_assert(lcs_broadcast_assistance_types_data_1_local_var);

    lcs_broadcast_assistance_types_data_1_local_var->location_assistance_type = location_assistance_type;

    return lcs_broadcast_assistance_types_data_1_local_var;
}

void OpenAPI_lcs_broadcast_assistance_types_data_1_free(OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_broadcast_assistance_types_data_1) {
        return;
    }
    if (lcs_broadcast_assistance_types_data_1->location_assistance_type) {
        ogs_free(lcs_broadcast_assistance_types_data_1->location_assistance_type);
        lcs_broadcast_assistance_types_data_1->location_assistance_type = NULL;
    }
    ogs_free(lcs_broadcast_assistance_types_data_1);
}

cJSON *OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON(OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_broadcast_assistance_types_data_1 == NULL) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON() failed [LcsBroadcastAssistanceTypesData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!lcs_broadcast_assistance_types_data_1->location_assistance_type) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON() failed [location_assistance_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "locationAssistanceType", lcs_broadcast_assistance_types_data_1->location_assistance_type) == NULL) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON() failed [location_assistance_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_lcs_broadcast_assistance_types_data_1_t *OpenAPI_lcs_broadcast_assistance_types_data_1_parseFromJSON(cJSON *lcs_broadcast_assistance_types_data_1JSON)
{
    OpenAPI_lcs_broadcast_assistance_types_data_1_t *lcs_broadcast_assistance_types_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location_assistance_type = NULL;
    location_assistance_type = cJSON_GetObjectItemCaseSensitive(lcs_broadcast_assistance_types_data_1JSON, "locationAssistanceType");
    if (!location_assistance_type) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_1_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }
    if (!cJSON_IsString(location_assistance_type)) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_1_parseFromJSON() failed [location_assistance_type]");
        goto end;
    }

    lcs_broadcast_assistance_types_data_1_local_var = OpenAPI_lcs_broadcast_assistance_types_data_1_create (
        ogs_strdup(location_assistance_type->valuestring)
    );

    return lcs_broadcast_assistance_types_data_1_local_var;
end:
    return NULL;
}

OpenAPI_lcs_broadcast_assistance_types_data_1_t *OpenAPI_lcs_broadcast_assistance_types_data_1_copy(OpenAPI_lcs_broadcast_assistance_types_data_1_t *dst, OpenAPI_lcs_broadcast_assistance_types_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_1_convertToJSON() failed");
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

    OpenAPI_lcs_broadcast_assistance_types_data_1_free(dst);
    dst = OpenAPI_lcs_broadcast_assistance_types_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

