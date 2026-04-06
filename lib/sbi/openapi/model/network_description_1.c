
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_description_1.h"

OpenAPI_network_description_1_t *OpenAPI_network_description_1_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *mcc,
    OpenAPI_list_t *mncs,
    bool is_any_plmn_ind,
    int any_plmn_ind
)
{
    OpenAPI_network_description_1_t *network_description_1_local_var = ogs_malloc(sizeof(OpenAPI_network_description_1_t));
    ogs_assert(network_description_1_local_var);

    network_description_1_local_var->plmn_id = plmn_id;
    network_description_1_local_var->mcc = mcc;
    network_description_1_local_var->mncs = mncs;
    network_description_1_local_var->is_any_plmn_ind = is_any_plmn_ind;
    network_description_1_local_var->any_plmn_ind = any_plmn_ind;

    return network_description_1_local_var;
}

void OpenAPI_network_description_1_free(OpenAPI_network_description_1_t *network_description_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_description_1) {
        return;
    }
    if (network_description_1->plmn_id) {
        OpenAPI_plmn_id_free(network_description_1->plmn_id);
        network_description_1->plmn_id = NULL;
    }
    if (network_description_1->mcc) {
        ogs_free(network_description_1->mcc);
        network_description_1->mcc = NULL;
    }
    if (network_description_1->mncs) {
        OpenAPI_list_for_each(network_description_1->mncs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(network_description_1->mncs);
        network_description_1->mncs = NULL;
    }
    ogs_free(network_description_1);
}

cJSON *OpenAPI_network_description_1_convertToJSON(OpenAPI_network_description_1_t *network_description_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_description_1 == NULL) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed [NetworkDescription_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (network_description_1->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(network_description_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (network_description_1->mcc) {
    if (cJSON_AddStringToObject(item, "mcc", network_description_1->mcc) == NULL) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed [mcc]");
        goto end;
    }
    }

    if (network_description_1->mncs) {
    cJSON *mncsList = cJSON_AddArrayToObject(item, "mncs");
    if (mncsList == NULL) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed [mncs]");
        goto end;
    }
    OpenAPI_list_for_each(network_description_1->mncs, node) {
        if (cJSON_AddStringToObject(mncsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_network_description_1_convertToJSON() failed [mncs]");
            goto end;
        }
    }
    }

    if (network_description_1->is_any_plmn_ind) {
    if (cJSON_AddBoolToObject(item, "anyPlmnInd", network_description_1->any_plmn_ind) == NULL) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed [any_plmn_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_network_description_1_t *OpenAPI_network_description_1_parseFromJSON(cJSON *network_description_1JSON)
{
    OpenAPI_network_description_1_t *network_description_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *mcc = NULL;
    cJSON *mncs = NULL;
    OpenAPI_list_t *mncsList = NULL;
    cJSON *any_plmn_ind = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(network_description_1JSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    mcc = cJSON_GetObjectItemCaseSensitive(network_description_1JSON, "mcc");
    if (mcc) {
    if (!cJSON_IsString(mcc) && !cJSON_IsNull(mcc)) {
        ogs_error("OpenAPI_network_description_1_parseFromJSON() failed [mcc]");
        goto end;
    }
    }

    mncs = cJSON_GetObjectItemCaseSensitive(network_description_1JSON, "mncs");
    if (mncs) {
        cJSON *mncs_local = NULL;
        if (!cJSON_IsArray(mncs)) {
            ogs_error("OpenAPI_network_description_1_parseFromJSON() failed [mncs]");
            goto end;
        }

        mncsList = OpenAPI_list_create();

        cJSON_ArrayForEach(mncs_local, mncs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(mncs_local)) {
                ogs_error("OpenAPI_network_description_1_parseFromJSON() failed [mncs]");
                goto end;
            }
            OpenAPI_list_add(mncsList, ogs_strdup(mncs_local->valuestring));
        }
    }

    any_plmn_ind = cJSON_GetObjectItemCaseSensitive(network_description_1JSON, "anyPlmnInd");
    if (any_plmn_ind) {
    if (!cJSON_IsBool(any_plmn_ind)) {
        ogs_error("OpenAPI_network_description_1_parseFromJSON() failed [any_plmn_ind]");
        goto end;
    }
    }

    network_description_1_local_var = OpenAPI_network_description_1_create (
        plmn_id ? plmn_id_local_nonprim : NULL,
        mcc && !cJSON_IsNull(mcc) ? ogs_strdup(mcc->valuestring) : NULL,
        mncs ? mncsList : NULL,
        any_plmn_ind ? true : false,
        any_plmn_ind ? any_plmn_ind->valueint : 0
    );

    return network_description_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (mncsList) {
        OpenAPI_list_for_each(mncsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mncsList);
        mncsList = NULL;
    }
    return NULL;
}

OpenAPI_network_description_1_t *OpenAPI_network_description_1_copy(OpenAPI_network_description_1_t *dst, OpenAPI_network_description_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_description_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_description_1_convertToJSON() failed");
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

    OpenAPI_network_description_1_free(dst);
    dst = OpenAPI_network_description_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

