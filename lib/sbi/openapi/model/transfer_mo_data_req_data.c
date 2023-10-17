
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_mo_data_req_data.h"

OpenAPI_transfer_mo_data_req_data_t *OpenAPI_transfer_mo_data_req_data_create(
    OpenAPI_ref_to_binary_data_t *mo_data,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_user_location_t *ue_location
)
{
    OpenAPI_transfer_mo_data_req_data_t *transfer_mo_data_req_data_local_var = ogs_malloc(sizeof(OpenAPI_transfer_mo_data_req_data_t));
    ogs_assert(transfer_mo_data_req_data_local_var);

    transfer_mo_data_req_data_local_var->mo_data = mo_data;
    transfer_mo_data_req_data_local_var->mo_exp_data_counter = mo_exp_data_counter;
    transfer_mo_data_req_data_local_var->ue_location = ue_location;

    return transfer_mo_data_req_data_local_var;
}

void OpenAPI_transfer_mo_data_req_data_free(OpenAPI_transfer_mo_data_req_data_t *transfer_mo_data_req_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == transfer_mo_data_req_data) {
        return;
    }
    if (transfer_mo_data_req_data->mo_data) {
        OpenAPI_ref_to_binary_data_free(transfer_mo_data_req_data->mo_data);
        transfer_mo_data_req_data->mo_data = NULL;
    }
    if (transfer_mo_data_req_data->mo_exp_data_counter) {
        OpenAPI_mo_exp_data_counter_free(transfer_mo_data_req_data->mo_exp_data_counter);
        transfer_mo_data_req_data->mo_exp_data_counter = NULL;
    }
    if (transfer_mo_data_req_data->ue_location) {
        OpenAPI_user_location_free(transfer_mo_data_req_data->ue_location);
        transfer_mo_data_req_data->ue_location = NULL;
    }
    ogs_free(transfer_mo_data_req_data);
}

cJSON *OpenAPI_transfer_mo_data_req_data_convertToJSON(OpenAPI_transfer_mo_data_req_data_t *transfer_mo_data_req_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (transfer_mo_data_req_data == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [TransferMoDataReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!transfer_mo_data_req_data->mo_data) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [mo_data]");
        return NULL;
    }
    cJSON *mo_data_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(transfer_mo_data_req_data->mo_data);
    if (mo_data_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [mo_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moData", mo_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [mo_data]");
        goto end;
    }

    if (transfer_mo_data_req_data->mo_exp_data_counter) {
    cJSON *mo_exp_data_counter_local_JSON = OpenAPI_mo_exp_data_counter_convertToJSON(transfer_mo_data_req_data->mo_exp_data_counter);
    if (mo_exp_data_counter_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moExpDataCounter", mo_exp_data_counter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    }

    if (transfer_mo_data_req_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(transfer_mo_data_req_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_transfer_mo_data_req_data_t *OpenAPI_transfer_mo_data_req_data_parseFromJSON(cJSON *transfer_mo_data_req_dataJSON)
{
    OpenAPI_transfer_mo_data_req_data_t *transfer_mo_data_req_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mo_data = NULL;
    OpenAPI_ref_to_binary_data_t *mo_data_local_nonprim = NULL;
    cJSON *mo_exp_data_counter = NULL;
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    mo_data = cJSON_GetObjectItemCaseSensitive(transfer_mo_data_req_dataJSON, "moData");
    if (!mo_data) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_parseFromJSON() failed [mo_data]");
        goto end;
    }
    mo_data_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(mo_data);
    if (!mo_data_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [mo_data]");
        goto end;
    }

    mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(transfer_mo_data_req_dataJSON, "moExpDataCounter");
    if (mo_exp_data_counter) {
    mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    if (!mo_exp_data_counter_local_nonprim) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON failed [mo_exp_data_counter]");
        goto end;
    }
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(transfer_mo_data_req_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    transfer_mo_data_req_data_local_var = OpenAPI_transfer_mo_data_req_data_create (
        mo_data_local_nonprim,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        ue_location ? ue_location_local_nonprim : NULL
    );

    return transfer_mo_data_req_data_local_var;
end:
    if (mo_data_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(mo_data_local_nonprim);
        mo_data_local_nonprim = NULL;
    }
    if (mo_exp_data_counter_local_nonprim) {
        OpenAPI_mo_exp_data_counter_free(mo_exp_data_counter_local_nonprim);
        mo_exp_data_counter_local_nonprim = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_transfer_mo_data_req_data_t *OpenAPI_transfer_mo_data_req_data_copy(OpenAPI_transfer_mo_data_req_data_t *dst, OpenAPI_transfer_mo_data_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transfer_mo_data_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_convertToJSON() failed");
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

    OpenAPI_transfer_mo_data_req_data_free(dst);
    dst = OpenAPI_transfer_mo_data_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

