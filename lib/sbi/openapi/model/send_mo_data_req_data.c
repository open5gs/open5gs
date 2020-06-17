
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "send_mo_data_req_data.h"

OpenAPI_send_mo_data_req_data_t *OpenAPI_send_mo_data_req_data_create(
    OpenAPI_ref_to_binary_data_t *mo_data,
    OpenAPI_mo_exception_data_flag_t *mo_exp_data_ind,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_user_location_t *ue_location
    )
{
    OpenAPI_send_mo_data_req_data_t *send_mo_data_req_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_send_mo_data_req_data_t));
    if (!send_mo_data_req_data_local_var) {
        return NULL;
    }
    send_mo_data_req_data_local_var->mo_data = mo_data;
    send_mo_data_req_data_local_var->mo_exp_data_ind = mo_exp_data_ind;
    send_mo_data_req_data_local_var->mo_exp_data_counter = mo_exp_data_counter;
    send_mo_data_req_data_local_var->ue_location = ue_location;

    return send_mo_data_req_data_local_var;
}

void OpenAPI_send_mo_data_req_data_free(OpenAPI_send_mo_data_req_data_t *send_mo_data_req_data)
{
    if (NULL == send_mo_data_req_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ref_to_binary_data_free(send_mo_data_req_data->mo_data);
    OpenAPI_mo_exception_data_flag_free(send_mo_data_req_data->mo_exp_data_ind);
    OpenAPI_mo_exp_data_counter_free(send_mo_data_req_data->mo_exp_data_counter);
    OpenAPI_user_location_free(send_mo_data_req_data->ue_location);
    ogs_free(send_mo_data_req_data);
}

cJSON *OpenAPI_send_mo_data_req_data_convertToJSON(OpenAPI_send_mo_data_req_data_t *send_mo_data_req_data)
{
    cJSON *item = NULL;

    if (send_mo_data_req_data == NULL) {
        ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [SendMoDataReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!send_mo_data_req_data->mo_data) {
        ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_data]");
        goto end;
    }
    cJSON *mo_data_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(send_mo_data_req_data->mo_data);
    if (mo_data_local_JSON == NULL) {
        ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moData", mo_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_data]");
        goto end;
    }

    if (send_mo_data_req_data->mo_exp_data_ind) {
        cJSON *mo_exp_data_ind_local_JSON = OpenAPI_mo_exception_data_flag_convertToJSON(send_mo_data_req_data->mo_exp_data_ind);
        if (mo_exp_data_ind_local_JSON == NULL) {
            ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_exp_data_ind]");
            goto end;
        }
        cJSON_AddItemToObject(item, "moExpDataInd", mo_exp_data_ind_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_exp_data_ind]");
            goto end;
        }
    }

    if (send_mo_data_req_data->mo_exp_data_counter) {
        cJSON *mo_exp_data_counter_local_JSON = OpenAPI_mo_exp_data_counter_convertToJSON(send_mo_data_req_data->mo_exp_data_counter);
        if (mo_exp_data_counter_local_JSON == NULL) {
            ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_exp_data_counter]");
            goto end;
        }
        cJSON_AddItemToObject(item, "moExpDataCounter", mo_exp_data_counter_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [mo_exp_data_counter]");
            goto end;
        }
    }

    if (send_mo_data_req_data->ue_location) {
        cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(send_mo_data_req_data->ue_location);
        if (ue_location_local_JSON == NULL) {
            ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [ue_location]");
            goto end;
        }
        cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed [ue_location]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_send_mo_data_req_data_t *OpenAPI_send_mo_data_req_data_parseFromJSON(cJSON *send_mo_data_req_dataJSON)
{
    OpenAPI_send_mo_data_req_data_t *send_mo_data_req_data_local_var = NULL;
    cJSON *mo_data = cJSON_GetObjectItemCaseSensitive(send_mo_data_req_dataJSON, "moData");
    if (!mo_data) {
        ogs_error("OpenAPI_send_mo_data_req_data_parseFromJSON() failed [mo_data]");
        goto end;
    }

    OpenAPI_ref_to_binary_data_t *mo_data_local_nonprim = NULL;

    mo_data_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(mo_data);

    cJSON *mo_exp_data_ind = cJSON_GetObjectItemCaseSensitive(send_mo_data_req_dataJSON, "moExpDataInd");

    OpenAPI_mo_exception_data_flag_t *mo_exp_data_ind_local_nonprim = NULL;
    if (mo_exp_data_ind) {
        mo_exp_data_ind_local_nonprim = OpenAPI_mo_exception_data_flag_parseFromJSON(mo_exp_data_ind);
    }

    cJSON *mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(send_mo_data_req_dataJSON, "moExpDataCounter");

    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    if (mo_exp_data_counter) {
        mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    }

    cJSON *ue_location = cJSON_GetObjectItemCaseSensitive(send_mo_data_req_dataJSON, "ueLocation");

    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    if (ue_location) {
        ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    }

    send_mo_data_req_data_local_var = OpenAPI_send_mo_data_req_data_create (
        mo_data_local_nonprim,
        mo_exp_data_ind ? mo_exp_data_ind_local_nonprim : NULL,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        ue_location ? ue_location_local_nonprim : NULL
        );

    return send_mo_data_req_data_local_var;
end:
    return NULL;
}

OpenAPI_send_mo_data_req_data_t *OpenAPI_send_mo_data_req_data_copy(OpenAPI_send_mo_data_req_data_t *dst, OpenAPI_send_mo_data_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_send_mo_data_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_send_mo_data_req_data_convertToJSON() failed");
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

    OpenAPI_send_mo_data_req_data_free(dst);
    dst = OpenAPI_send_mo_data_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

