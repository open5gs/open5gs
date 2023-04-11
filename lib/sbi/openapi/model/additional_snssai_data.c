
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_snssai_data.h"

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_create(
    bool is_required_authn_authz,
    int required_authn_authz,
    OpenAPI_slice_mbr_rm_t *subscribed_ue_slice_mbr,
    OpenAPI_list_t *subscribed_ns_srg_list
)
{
    OpenAPI_additional_snssai_data_t *additional_snssai_data_local_var = ogs_malloc(sizeof(OpenAPI_additional_snssai_data_t));
    ogs_assert(additional_snssai_data_local_var);

    additional_snssai_data_local_var->is_required_authn_authz = is_required_authn_authz;
    additional_snssai_data_local_var->required_authn_authz = required_authn_authz;
    additional_snssai_data_local_var->subscribed_ue_slice_mbr = subscribed_ue_slice_mbr;
    additional_snssai_data_local_var->subscribed_ns_srg_list = subscribed_ns_srg_list;

    return additional_snssai_data_local_var;
}

void OpenAPI_additional_snssai_data_free(OpenAPI_additional_snssai_data_t *additional_snssai_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == additional_snssai_data) {
        return;
    }
    if (additional_snssai_data->subscribed_ue_slice_mbr) {
        OpenAPI_slice_mbr_rm_free(additional_snssai_data->subscribed_ue_slice_mbr);
        additional_snssai_data->subscribed_ue_slice_mbr = NULL;
    }
    if (additional_snssai_data->subscribed_ns_srg_list) {
        OpenAPI_list_for_each(additional_snssai_data->subscribed_ns_srg_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_snssai_data->subscribed_ns_srg_list);
        additional_snssai_data->subscribed_ns_srg_list = NULL;
    }
    ogs_free(additional_snssai_data);
}

cJSON *OpenAPI_additional_snssai_data_convertToJSON(OpenAPI_additional_snssai_data_t *additional_snssai_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (additional_snssai_data == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [AdditionalSnssaiData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (additional_snssai_data->is_required_authn_authz) {
    if (cJSON_AddBoolToObject(item, "requiredAuthnAuthz", additional_snssai_data->required_authn_authz) == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [required_authn_authz]");
        goto end;
    }
    }

    if (additional_snssai_data->subscribed_ue_slice_mbr) {
    cJSON *subscribed_ue_slice_mbr_local_JSON = OpenAPI_slice_mbr_rm_convertToJSON(additional_snssai_data->subscribed_ue_slice_mbr);
    if (subscribed_ue_slice_mbr_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [subscribed_ue_slice_mbr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscribedUeSliceMbr", subscribed_ue_slice_mbr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [subscribed_ue_slice_mbr]");
        goto end;
    }
    }

    if (additional_snssai_data->subscribed_ns_srg_list) {
    cJSON *subscribed_ns_srg_listList = cJSON_AddArrayToObject(item, "subscribedNsSrgList");
    if (subscribed_ns_srg_listList == NULL) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [subscribed_ns_srg_list]");
        goto end;
    }
    OpenAPI_list_for_each(additional_snssai_data->subscribed_ns_srg_list, node) {
        if (cJSON_AddStringToObject(subscribed_ns_srg_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed [subscribed_ns_srg_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_parseFromJSON(cJSON *additional_snssai_dataJSON)
{
    OpenAPI_additional_snssai_data_t *additional_snssai_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *required_authn_authz = NULL;
    cJSON *subscribed_ue_slice_mbr = NULL;
    OpenAPI_slice_mbr_rm_t *subscribed_ue_slice_mbr_local_nonprim = NULL;
    cJSON *subscribed_ns_srg_list = NULL;
    OpenAPI_list_t *subscribed_ns_srg_listList = NULL;
    required_authn_authz = cJSON_GetObjectItemCaseSensitive(additional_snssai_dataJSON, "requiredAuthnAuthz");
    if (required_authn_authz) {
    if (!cJSON_IsBool(required_authn_authz)) {
        ogs_error("OpenAPI_additional_snssai_data_parseFromJSON() failed [required_authn_authz]");
        goto end;
    }
    }

    subscribed_ue_slice_mbr = cJSON_GetObjectItemCaseSensitive(additional_snssai_dataJSON, "subscribedUeSliceMbr");
    if (subscribed_ue_slice_mbr) {
    subscribed_ue_slice_mbr_local_nonprim = OpenAPI_slice_mbr_rm_parseFromJSON(subscribed_ue_slice_mbr);
    if (!subscribed_ue_slice_mbr_local_nonprim) {
        ogs_error("OpenAPI_slice_mbr_rm_parseFromJSON failed [subscribed_ue_slice_mbr]");
        goto end;
    }
    }

    subscribed_ns_srg_list = cJSON_GetObjectItemCaseSensitive(additional_snssai_dataJSON, "subscribedNsSrgList");
    if (subscribed_ns_srg_list) {
        cJSON *subscribed_ns_srg_list_local = NULL;
        if (!cJSON_IsArray(subscribed_ns_srg_list)) {
            ogs_error("OpenAPI_additional_snssai_data_parseFromJSON() failed [subscribed_ns_srg_list]");
            goto end;
        }

        subscribed_ns_srg_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_ns_srg_list_local, subscribed_ns_srg_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subscribed_ns_srg_list_local)) {
                ogs_error("OpenAPI_additional_snssai_data_parseFromJSON() failed [subscribed_ns_srg_list]");
                goto end;
            }
            OpenAPI_list_add(subscribed_ns_srg_listList, ogs_strdup(subscribed_ns_srg_list_local->valuestring));
        }
    }

    additional_snssai_data_local_var = OpenAPI_additional_snssai_data_create (
        required_authn_authz ? true : false,
        required_authn_authz ? required_authn_authz->valueint : 0,
        subscribed_ue_slice_mbr ? subscribed_ue_slice_mbr_local_nonprim : NULL,
        subscribed_ns_srg_list ? subscribed_ns_srg_listList : NULL
    );

    return additional_snssai_data_local_var;
end:
    if (subscribed_ue_slice_mbr_local_nonprim) {
        OpenAPI_slice_mbr_rm_free(subscribed_ue_slice_mbr_local_nonprim);
        subscribed_ue_slice_mbr_local_nonprim = NULL;
    }
    if (subscribed_ns_srg_listList) {
        OpenAPI_list_for_each(subscribed_ns_srg_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscribed_ns_srg_listList);
        subscribed_ns_srg_listList = NULL;
    }
    return NULL;
}

OpenAPI_additional_snssai_data_t *OpenAPI_additional_snssai_data_copy(OpenAPI_additional_snssai_data_t *dst, OpenAPI_additional_snssai_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_snssai_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_snssai_data_convertToJSON() failed");
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

    OpenAPI_additional_snssai_data_free(dst);
    dst = OpenAPI_additional_snssai_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

