
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_retrieve_data.h"

OpenAPI_sm_context_retrieve_data_t *OpenAPI_sm_context_retrieve_data_create(
    OpenAPI_mme_capabilities_t *target_mme_cap,
    OpenAPI_sm_context_type_e sm_context_type,
    OpenAPI_plmn_id_t *serving_network,
    OpenAPI_list_t *not_to_transfer_ebi_list,
    bool is_ran_unchanged_ind,
    int ran_unchanged_ind
)
{
    OpenAPI_sm_context_retrieve_data_t *sm_context_retrieve_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_retrieve_data_t));
    ogs_assert(sm_context_retrieve_data_local_var);

    sm_context_retrieve_data_local_var->target_mme_cap = target_mme_cap;
    sm_context_retrieve_data_local_var->sm_context_type = sm_context_type;
    sm_context_retrieve_data_local_var->serving_network = serving_network;
    sm_context_retrieve_data_local_var->not_to_transfer_ebi_list = not_to_transfer_ebi_list;
    sm_context_retrieve_data_local_var->is_ran_unchanged_ind = is_ran_unchanged_ind;
    sm_context_retrieve_data_local_var->ran_unchanged_ind = ran_unchanged_ind;

    return sm_context_retrieve_data_local_var;
}

void OpenAPI_sm_context_retrieve_data_free(OpenAPI_sm_context_retrieve_data_t *sm_context_retrieve_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_retrieve_data) {
        return;
    }
    if (sm_context_retrieve_data->target_mme_cap) {
        OpenAPI_mme_capabilities_free(sm_context_retrieve_data->target_mme_cap);
        sm_context_retrieve_data->target_mme_cap = NULL;
    }
    if (sm_context_retrieve_data->serving_network) {
        OpenAPI_plmn_id_free(sm_context_retrieve_data->serving_network);
        sm_context_retrieve_data->serving_network = NULL;
    }
    if (sm_context_retrieve_data->not_to_transfer_ebi_list) {
        OpenAPI_list_for_each(sm_context_retrieve_data->not_to_transfer_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_retrieve_data->not_to_transfer_ebi_list);
        sm_context_retrieve_data->not_to_transfer_ebi_list = NULL;
    }
    ogs_free(sm_context_retrieve_data);
}

cJSON *OpenAPI_sm_context_retrieve_data_convertToJSON(OpenAPI_sm_context_retrieve_data_t *sm_context_retrieve_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_retrieve_data == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [SmContextRetrieveData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_retrieve_data->target_mme_cap) {
    cJSON *target_mme_cap_local_JSON = OpenAPI_mme_capabilities_convertToJSON(sm_context_retrieve_data->target_mme_cap);
    if (target_mme_cap_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [target_mme_cap]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetMmeCap", target_mme_cap_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [target_mme_cap]");
        goto end;
    }
    }

    if (sm_context_retrieve_data->sm_context_type != OpenAPI_sm_context_type_NULL) {
    if (cJSON_AddStringToObject(item, "smContextType", OpenAPI_sm_context_type_ToString(sm_context_retrieve_data->sm_context_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [sm_context_type]");
        goto end;
    }
    }

    if (sm_context_retrieve_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_convertToJSON(sm_context_retrieve_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (sm_context_retrieve_data->not_to_transfer_ebi_list) {
    cJSON *not_to_transfer_ebi_listList = cJSON_AddArrayToObject(item, "notToTransferEbiList");
    if (not_to_transfer_ebi_listList == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [not_to_transfer_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_retrieve_data->not_to_transfer_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [not_to_transfer_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(not_to_transfer_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [not_to_transfer_ebi_list]");
            goto end;
        }
    }
    }

    if (sm_context_retrieve_data->is_ran_unchanged_ind) {
    if (cJSON_AddBoolToObject(item, "ranUnchangedInd", sm_context_retrieve_data->ran_unchanged_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed [ran_unchanged_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_retrieve_data_t *OpenAPI_sm_context_retrieve_data_parseFromJSON(cJSON *sm_context_retrieve_dataJSON)
{
    OpenAPI_sm_context_retrieve_data_t *sm_context_retrieve_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *target_mme_cap = NULL;
    OpenAPI_mme_capabilities_t *target_mme_cap_local_nonprim = NULL;
    cJSON *sm_context_type = NULL;
    OpenAPI_sm_context_type_e sm_context_typeVariable = 0;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_t *serving_network_local_nonprim = NULL;
    cJSON *not_to_transfer_ebi_list = NULL;
    OpenAPI_list_t *not_to_transfer_ebi_listList = NULL;
    cJSON *ran_unchanged_ind = NULL;
    target_mme_cap = cJSON_GetObjectItemCaseSensitive(sm_context_retrieve_dataJSON, "targetMmeCap");
    if (target_mme_cap) {
    target_mme_cap_local_nonprim = OpenAPI_mme_capabilities_parseFromJSON(target_mme_cap);
    if (!target_mme_cap_local_nonprim) {
        ogs_error("OpenAPI_mme_capabilities_parseFromJSON failed [target_mme_cap]");
        goto end;
    }
    }

    sm_context_type = cJSON_GetObjectItemCaseSensitive(sm_context_retrieve_dataJSON, "smContextType");
    if (sm_context_type) {
    if (!cJSON_IsString(sm_context_type)) {
        ogs_error("OpenAPI_sm_context_retrieve_data_parseFromJSON() failed [sm_context_type]");
        goto end;
    }
    sm_context_typeVariable = OpenAPI_sm_context_type_FromString(sm_context_type->valuestring);
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(sm_context_retrieve_dataJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    not_to_transfer_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_retrieve_dataJSON, "notToTransferEbiList");
    if (not_to_transfer_ebi_list) {
        cJSON *not_to_transfer_ebi_list_local = NULL;
        if (!cJSON_IsArray(not_to_transfer_ebi_list)) {
            ogs_error("OpenAPI_sm_context_retrieve_data_parseFromJSON() failed [not_to_transfer_ebi_list]");
            goto end;
        }

        not_to_transfer_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(not_to_transfer_ebi_list_local, not_to_transfer_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(not_to_transfer_ebi_list_local)) {
                ogs_error("OpenAPI_sm_context_retrieve_data_parseFromJSON() failed [not_to_transfer_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_sm_context_retrieve_data_parseFromJSON() failed [not_to_transfer_ebi_list]");
                goto end;
            }
            *localDouble = not_to_transfer_ebi_list_local->valuedouble;
            OpenAPI_list_add(not_to_transfer_ebi_listList, localDouble);
        }
    }

    ran_unchanged_ind = cJSON_GetObjectItemCaseSensitive(sm_context_retrieve_dataJSON, "ranUnchangedInd");
    if (ran_unchanged_ind) {
    if (!cJSON_IsBool(ran_unchanged_ind)) {
        ogs_error("OpenAPI_sm_context_retrieve_data_parseFromJSON() failed [ran_unchanged_ind]");
        goto end;
    }
    }

    sm_context_retrieve_data_local_var = OpenAPI_sm_context_retrieve_data_create (
        target_mme_cap ? target_mme_cap_local_nonprim : NULL,
        sm_context_type ? sm_context_typeVariable : 0,
        serving_network ? serving_network_local_nonprim : NULL,
        not_to_transfer_ebi_list ? not_to_transfer_ebi_listList : NULL,
        ran_unchanged_ind ? true : false,
        ran_unchanged_ind ? ran_unchanged_ind->valueint : 0
    );

    return sm_context_retrieve_data_local_var;
end:
    if (target_mme_cap_local_nonprim) {
        OpenAPI_mme_capabilities_free(target_mme_cap_local_nonprim);
        target_mme_cap_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (not_to_transfer_ebi_listList) {
        OpenAPI_list_for_each(not_to_transfer_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(not_to_transfer_ebi_listList);
        not_to_transfer_ebi_listList = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_retrieve_data_t *OpenAPI_sm_context_retrieve_data_copy(OpenAPI_sm_context_retrieve_data_t *dst, OpenAPI_sm_context_retrieve_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_retrieve_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_retrieve_data_convertToJSON() failed");
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

    OpenAPI_sm_context_retrieve_data_free(dst);
    dst = OpenAPI_sm_context_retrieve_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

