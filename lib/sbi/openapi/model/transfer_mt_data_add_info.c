
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_mt_data_add_info.h"

OpenAPI_transfer_mt_data_add_info_t *OpenAPI_transfer_mt_data_add_info_create(
    bool is_max_waiting_time,
    int max_waiting_time
)
{
    OpenAPI_transfer_mt_data_add_info_t *transfer_mt_data_add_info_local_var = ogs_malloc(sizeof(OpenAPI_transfer_mt_data_add_info_t));
    ogs_assert(transfer_mt_data_add_info_local_var);

    transfer_mt_data_add_info_local_var->is_max_waiting_time = is_max_waiting_time;
    transfer_mt_data_add_info_local_var->max_waiting_time = max_waiting_time;

    return transfer_mt_data_add_info_local_var;
}

void OpenAPI_transfer_mt_data_add_info_free(OpenAPI_transfer_mt_data_add_info_t *transfer_mt_data_add_info)
{
    if (NULL == transfer_mt_data_add_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(transfer_mt_data_add_info);
}

cJSON *OpenAPI_transfer_mt_data_add_info_convertToJSON(OpenAPI_transfer_mt_data_add_info_t *transfer_mt_data_add_info)
{
    cJSON *item = NULL;

    if (transfer_mt_data_add_info == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_add_info_convertToJSON() failed [TransferMtDataAddInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (transfer_mt_data_add_info->is_max_waiting_time) {
    if (cJSON_AddNumberToObject(item, "maxWaitingTime", transfer_mt_data_add_info->max_waiting_time) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_add_info_convertToJSON() failed [max_waiting_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_transfer_mt_data_add_info_t *OpenAPI_transfer_mt_data_add_info_parseFromJSON(cJSON *transfer_mt_data_add_infoJSON)
{
    OpenAPI_transfer_mt_data_add_info_t *transfer_mt_data_add_info_local_var = NULL;
    cJSON *max_waiting_time = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_add_infoJSON, "maxWaitingTime");

    if (max_waiting_time) {
    if (!cJSON_IsNumber(max_waiting_time)) {
        ogs_error("OpenAPI_transfer_mt_data_add_info_parseFromJSON() failed [max_waiting_time]");
        goto end;
    }
    }

    transfer_mt_data_add_info_local_var = OpenAPI_transfer_mt_data_add_info_create (
        max_waiting_time ? true : false,
        max_waiting_time ? max_waiting_time->valuedouble : 0
    );

    return transfer_mt_data_add_info_local_var;
end:
    return NULL;
}

OpenAPI_transfer_mt_data_add_info_t *OpenAPI_transfer_mt_data_add_info_copy(OpenAPI_transfer_mt_data_add_info_t *dst, OpenAPI_transfer_mt_data_add_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transfer_mt_data_add_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transfer_mt_data_add_info_convertToJSON() failed");
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

    OpenAPI_transfer_mt_data_add_info_free(dst);
    dst = OpenAPI_transfer_mt_data_add_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

