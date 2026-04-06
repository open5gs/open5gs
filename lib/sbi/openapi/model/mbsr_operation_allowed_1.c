
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbsr_operation_allowed_1.h"

OpenAPI_mbsr_operation_allowed_1_t *OpenAPI_mbsr_operation_allowed_1_create(
    bool is_mbsr_operation_allowed_ind,
    int mbsr_operation_allowed_ind,
    OpenAPI_mbsr_location_info_1_t *mbsr_location_info,
    OpenAPI_mbsr_time_info_t *mbsr_time_info
)
{
    OpenAPI_mbsr_operation_allowed_1_t *mbsr_operation_allowed_1_local_var = ogs_malloc(sizeof(OpenAPI_mbsr_operation_allowed_1_t));
    ogs_assert(mbsr_operation_allowed_1_local_var);

    mbsr_operation_allowed_1_local_var->is_mbsr_operation_allowed_ind = is_mbsr_operation_allowed_ind;
    mbsr_operation_allowed_1_local_var->mbsr_operation_allowed_ind = mbsr_operation_allowed_ind;
    mbsr_operation_allowed_1_local_var->mbsr_location_info = mbsr_location_info;
    mbsr_operation_allowed_1_local_var->mbsr_time_info = mbsr_time_info;

    return mbsr_operation_allowed_1_local_var;
}

void OpenAPI_mbsr_operation_allowed_1_free(OpenAPI_mbsr_operation_allowed_1_t *mbsr_operation_allowed_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbsr_operation_allowed_1) {
        return;
    }
    if (mbsr_operation_allowed_1->mbsr_location_info) {
        OpenAPI_mbsr_location_info_1_free(mbsr_operation_allowed_1->mbsr_location_info);
        mbsr_operation_allowed_1->mbsr_location_info = NULL;
    }
    if (mbsr_operation_allowed_1->mbsr_time_info) {
        OpenAPI_mbsr_time_info_free(mbsr_operation_allowed_1->mbsr_time_info);
        mbsr_operation_allowed_1->mbsr_time_info = NULL;
    }
    ogs_free(mbsr_operation_allowed_1);
}

cJSON *OpenAPI_mbsr_operation_allowed_1_convertToJSON(OpenAPI_mbsr_operation_allowed_1_t *mbsr_operation_allowed_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbsr_operation_allowed_1 == NULL) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed [MbsrOperationAllowed_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbsr_operation_allowed_1->is_mbsr_operation_allowed_ind) {
    if (cJSON_AddBoolToObject(item, "mbsrOperationAllowedInd", mbsr_operation_allowed_1->mbsr_operation_allowed_ind) == NULL) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed [mbsr_operation_allowed_ind]");
        goto end;
    }
    }

    if (mbsr_operation_allowed_1->mbsr_location_info) {
    cJSON *mbsr_location_info_local_JSON = OpenAPI_mbsr_location_info_1_convertToJSON(mbsr_operation_allowed_1->mbsr_location_info);
    if (mbsr_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed [mbsr_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsrLocationInfo", mbsr_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed [mbsr_location_info]");
        goto end;
    }
    }

    if (mbsr_operation_allowed_1->mbsr_time_info) {
    cJSON *mbsr_time_info_local_JSON = OpenAPI_mbsr_time_info_convertToJSON(mbsr_operation_allowed_1->mbsr_time_info);
    if (mbsr_time_info_local_JSON == NULL) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed [mbsr_time_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsrTimeInfo", mbsr_time_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed [mbsr_time_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mbsr_operation_allowed_1_t *OpenAPI_mbsr_operation_allowed_1_parseFromJSON(cJSON *mbsr_operation_allowed_1JSON)
{
    OpenAPI_mbsr_operation_allowed_1_t *mbsr_operation_allowed_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbsr_operation_allowed_ind = NULL;
    cJSON *mbsr_location_info = NULL;
    OpenAPI_mbsr_location_info_1_t *mbsr_location_info_local_nonprim = NULL;
    cJSON *mbsr_time_info = NULL;
    OpenAPI_mbsr_time_info_t *mbsr_time_info_local_nonprim = NULL;
    mbsr_operation_allowed_ind = cJSON_GetObjectItemCaseSensitive(mbsr_operation_allowed_1JSON, "mbsrOperationAllowedInd");
    if (mbsr_operation_allowed_ind) {
    if (!cJSON_IsBool(mbsr_operation_allowed_ind)) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_parseFromJSON() failed [mbsr_operation_allowed_ind]");
        goto end;
    }
    }

    mbsr_location_info = cJSON_GetObjectItemCaseSensitive(mbsr_operation_allowed_1JSON, "mbsrLocationInfo");
    if (mbsr_location_info) {
    mbsr_location_info_local_nonprim = OpenAPI_mbsr_location_info_1_parseFromJSON(mbsr_location_info);
    if (!mbsr_location_info_local_nonprim) {
        ogs_error("OpenAPI_mbsr_location_info_1_parseFromJSON failed [mbsr_location_info]");
        goto end;
    }
    }

    mbsr_time_info = cJSON_GetObjectItemCaseSensitive(mbsr_operation_allowed_1JSON, "mbsrTimeInfo");
    if (mbsr_time_info) {
    mbsr_time_info_local_nonprim = OpenAPI_mbsr_time_info_parseFromJSON(mbsr_time_info);
    if (!mbsr_time_info_local_nonprim) {
        ogs_error("OpenAPI_mbsr_time_info_parseFromJSON failed [mbsr_time_info]");
        goto end;
    }
    }

    mbsr_operation_allowed_1_local_var = OpenAPI_mbsr_operation_allowed_1_create (
        mbsr_operation_allowed_ind ? true : false,
        mbsr_operation_allowed_ind ? mbsr_operation_allowed_ind->valueint : 0,
        mbsr_location_info ? mbsr_location_info_local_nonprim : NULL,
        mbsr_time_info ? mbsr_time_info_local_nonprim : NULL
    );

    return mbsr_operation_allowed_1_local_var;
end:
    if (mbsr_location_info_local_nonprim) {
        OpenAPI_mbsr_location_info_1_free(mbsr_location_info_local_nonprim);
        mbsr_location_info_local_nonprim = NULL;
    }
    if (mbsr_time_info_local_nonprim) {
        OpenAPI_mbsr_time_info_free(mbsr_time_info_local_nonprim);
        mbsr_time_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mbsr_operation_allowed_1_t *OpenAPI_mbsr_operation_allowed_1_copy(OpenAPI_mbsr_operation_allowed_1_t *dst, OpenAPI_mbsr_operation_allowed_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbsr_operation_allowed_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbsr_operation_allowed_1_convertToJSON() failed");
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

    OpenAPI_mbsr_operation_allowed_1_free(dst);
    dst = OpenAPI_mbsr_operation_allowed_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

