
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_reg_status_update_rsp_data.h"

OpenAPI_ue_reg_status_update_rsp_data_t *OpenAPI_ue_reg_status_update_rsp_data_create(
    int reg_status_transfer_complete
    )
{
    OpenAPI_ue_reg_status_update_rsp_data_t *ue_reg_status_update_rsp_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_reg_status_update_rsp_data_t));
    if (!ue_reg_status_update_rsp_data_local_var) {
        return NULL;
    }
    ue_reg_status_update_rsp_data_local_var->reg_status_transfer_complete = reg_status_transfer_complete;

    return ue_reg_status_update_rsp_data_local_var;
}

void OpenAPI_ue_reg_status_update_rsp_data_free(OpenAPI_ue_reg_status_update_rsp_data_t *ue_reg_status_update_rsp_data)
{
    if (NULL == ue_reg_status_update_rsp_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_reg_status_update_rsp_data);
}

cJSON *OpenAPI_ue_reg_status_update_rsp_data_convertToJSON(OpenAPI_ue_reg_status_update_rsp_data_t *ue_reg_status_update_rsp_data)
{
    cJSON *item = NULL;

    if (ue_reg_status_update_rsp_data == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_rsp_data_convertToJSON() failed [UeRegStatusUpdateRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "regStatusTransferComplete", ue_reg_status_update_rsp_data->reg_status_transfer_complete) == NULL) {
        ogs_error("OpenAPI_ue_reg_status_update_rsp_data_convertToJSON() failed [reg_status_transfer_complete]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ue_reg_status_update_rsp_data_t *OpenAPI_ue_reg_status_update_rsp_data_parseFromJSON(cJSON *ue_reg_status_update_rsp_dataJSON)
{
    OpenAPI_ue_reg_status_update_rsp_data_t *ue_reg_status_update_rsp_data_local_var = NULL;
    cJSON *reg_status_transfer_complete = cJSON_GetObjectItemCaseSensitive(ue_reg_status_update_rsp_dataJSON, "regStatusTransferComplete");
    if (!reg_status_transfer_complete) {
        ogs_error("OpenAPI_ue_reg_status_update_rsp_data_parseFromJSON() failed [reg_status_transfer_complete]");
        goto end;
    }


    if (!cJSON_IsBool(reg_status_transfer_complete)) {
        ogs_error("OpenAPI_ue_reg_status_update_rsp_data_parseFromJSON() failed [reg_status_transfer_complete]");
        goto end;
    }

    ue_reg_status_update_rsp_data_local_var = OpenAPI_ue_reg_status_update_rsp_data_create (
        reg_status_transfer_complete->valueint
        );

    return ue_reg_status_update_rsp_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_reg_status_update_rsp_data_t *OpenAPI_ue_reg_status_update_rsp_data_copy(OpenAPI_ue_reg_status_update_rsp_data_t *dst, OpenAPI_ue_reg_status_update_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_reg_status_update_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_reg_status_update_rsp_data_convertToJSON() failed");
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

    OpenAPI_ue_reg_status_update_rsp_data_free(dst);
    dst = OpenAPI_ue_reg_status_update_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

