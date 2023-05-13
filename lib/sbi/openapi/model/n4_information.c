
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n4_information.h"

OpenAPI_n4_information_t *OpenAPI_n4_information_create(
    OpenAPI_n4_message_type_e n4_message_type,
    OpenAPI_ref_to_binary_data_t *n4_message_payload,
    OpenAPI_dnai_information_t *n4_dnai_info,
    char *psa_upf_id,
    char *ul_cl_bp_id,
    OpenAPI_list_t *n9_ul_pdr_id_list
)
{
    OpenAPI_n4_information_t *n4_information_local_var = ogs_malloc(sizeof(OpenAPI_n4_information_t));
    ogs_assert(n4_information_local_var);

    n4_information_local_var->n4_message_type = n4_message_type;
    n4_information_local_var->n4_message_payload = n4_message_payload;
    n4_information_local_var->n4_dnai_info = n4_dnai_info;
    n4_information_local_var->psa_upf_id = psa_upf_id;
    n4_information_local_var->ul_cl_bp_id = ul_cl_bp_id;
    n4_information_local_var->n9_ul_pdr_id_list = n9_ul_pdr_id_list;

    return n4_information_local_var;
}

void OpenAPI_n4_information_free(OpenAPI_n4_information_t *n4_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n4_information) {
        return;
    }
    if (n4_information->n4_message_payload) {
        OpenAPI_ref_to_binary_data_free(n4_information->n4_message_payload);
        n4_information->n4_message_payload = NULL;
    }
    if (n4_information->n4_dnai_info) {
        OpenAPI_dnai_information_free(n4_information->n4_dnai_info);
        n4_information->n4_dnai_info = NULL;
    }
    if (n4_information->psa_upf_id) {
        ogs_free(n4_information->psa_upf_id);
        n4_information->psa_upf_id = NULL;
    }
    if (n4_information->ul_cl_bp_id) {
        ogs_free(n4_information->ul_cl_bp_id);
        n4_information->ul_cl_bp_id = NULL;
    }
    if (n4_information->n9_ul_pdr_id_list) {
        OpenAPI_list_for_each(n4_information->n9_ul_pdr_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(n4_information->n9_ul_pdr_id_list);
        n4_information->n9_ul_pdr_id_list = NULL;
    }
    ogs_free(n4_information);
}

cJSON *OpenAPI_n4_information_convertToJSON(OpenAPI_n4_information_t *n4_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n4_information == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [N4Information]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n4_information->n4_message_type == OpenAPI_n4_message_type_NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n4MessageType", OpenAPI_n4_message_type_ToString(n4_information->n4_message_type)) == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_type]");
        goto end;
    }

    if (!n4_information->n4_message_payload) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_payload]");
        return NULL;
    }
    cJSON *n4_message_payload_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(n4_information->n4_message_payload);
    if (n4_message_payload_local_JSON == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_payload]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4MessagePayload", n4_message_payload_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_payload]");
        goto end;
    }

    if (n4_information->n4_dnai_info) {
    cJSON *n4_dnai_info_local_JSON = OpenAPI_dnai_information_convertToJSON(n4_information->n4_dnai_info);
    if (n4_dnai_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_dnai_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4DnaiInfo", n4_dnai_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_dnai_info]");
        goto end;
    }
    }

    if (n4_information->psa_upf_id) {
    if (cJSON_AddStringToObject(item, "psaUpfId", n4_information->psa_upf_id) == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [psa_upf_id]");
        goto end;
    }
    }

    if (n4_information->ul_cl_bp_id) {
    if (cJSON_AddStringToObject(item, "ulClBpId", n4_information->ul_cl_bp_id) == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [ul_cl_bp_id]");
        goto end;
    }
    }

    if (n4_information->n9_ul_pdr_id_list) {
    cJSON *n9_ul_pdr_id_listList = cJSON_AddArrayToObject(item, "n9UlPdrIdList");
    if (n9_ul_pdr_id_listList == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n9_ul_pdr_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(n4_information->n9_ul_pdr_id_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_n4_information_convertToJSON() failed [n9_ul_pdr_id_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(n9_ul_pdr_id_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_n4_information_convertToJSON() failed [n9_ul_pdr_id_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_n4_information_t *OpenAPI_n4_information_parseFromJSON(cJSON *n4_informationJSON)
{
    OpenAPI_n4_information_t *n4_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n4_message_type = NULL;
    OpenAPI_n4_message_type_e n4_message_typeVariable = 0;
    cJSON *n4_message_payload = NULL;
    OpenAPI_ref_to_binary_data_t *n4_message_payload_local_nonprim = NULL;
    cJSON *n4_dnai_info = NULL;
    OpenAPI_dnai_information_t *n4_dnai_info_local_nonprim = NULL;
    cJSON *psa_upf_id = NULL;
    cJSON *ul_cl_bp_id = NULL;
    cJSON *n9_ul_pdr_id_list = NULL;
    OpenAPI_list_t *n9_ul_pdr_id_listList = NULL;
    n4_message_type = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n4MessageType");
    if (!n4_message_type) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n4_message_type]");
        goto end;
    }
    if (!cJSON_IsString(n4_message_type)) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n4_message_type]");
        goto end;
    }
    n4_message_typeVariable = OpenAPI_n4_message_type_FromString(n4_message_type->valuestring);

    n4_message_payload = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n4MessagePayload");
    if (!n4_message_payload) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n4_message_payload]");
        goto end;
    }
    n4_message_payload_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n4_message_payload);
    if (!n4_message_payload_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n4_message_payload]");
        goto end;
    }

    n4_dnai_info = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n4DnaiInfo");
    if (n4_dnai_info) {
    n4_dnai_info_local_nonprim = OpenAPI_dnai_information_parseFromJSON(n4_dnai_info);
    if (!n4_dnai_info_local_nonprim) {
        ogs_error("OpenAPI_dnai_information_parseFromJSON failed [n4_dnai_info]");
        goto end;
    }
    }

    psa_upf_id = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "psaUpfId");
    if (psa_upf_id) {
    if (!cJSON_IsString(psa_upf_id) && !cJSON_IsNull(psa_upf_id)) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [psa_upf_id]");
        goto end;
    }
    }

    ul_cl_bp_id = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "ulClBpId");
    if (ul_cl_bp_id) {
    if (!cJSON_IsString(ul_cl_bp_id) && !cJSON_IsNull(ul_cl_bp_id)) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [ul_cl_bp_id]");
        goto end;
    }
    }

    n9_ul_pdr_id_list = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n9UlPdrIdList");
    if (n9_ul_pdr_id_list) {
        cJSON *n9_ul_pdr_id_list_local = NULL;
        if (!cJSON_IsArray(n9_ul_pdr_id_list)) {
            ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n9_ul_pdr_id_list]");
            goto end;
        }

        n9_ul_pdr_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n9_ul_pdr_id_list_local, n9_ul_pdr_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(n9_ul_pdr_id_list_local)) {
                ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n9_ul_pdr_id_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n9_ul_pdr_id_list]");
                goto end;
            }
            *localDouble = n9_ul_pdr_id_list_local->valuedouble;
            OpenAPI_list_add(n9_ul_pdr_id_listList, localDouble);
        }
    }

    n4_information_local_var = OpenAPI_n4_information_create (
        n4_message_typeVariable,
        n4_message_payload_local_nonprim,
        n4_dnai_info ? n4_dnai_info_local_nonprim : NULL,
        psa_upf_id && !cJSON_IsNull(psa_upf_id) ? ogs_strdup(psa_upf_id->valuestring) : NULL,
        ul_cl_bp_id && !cJSON_IsNull(ul_cl_bp_id) ? ogs_strdup(ul_cl_bp_id->valuestring) : NULL,
        n9_ul_pdr_id_list ? n9_ul_pdr_id_listList : NULL
    );

    return n4_information_local_var;
end:
    if (n4_message_payload_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n4_message_payload_local_nonprim);
        n4_message_payload_local_nonprim = NULL;
    }
    if (n4_dnai_info_local_nonprim) {
        OpenAPI_dnai_information_free(n4_dnai_info_local_nonprim);
        n4_dnai_info_local_nonprim = NULL;
    }
    if (n9_ul_pdr_id_listList) {
        OpenAPI_list_for_each(n9_ul_pdr_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(n9_ul_pdr_id_listList);
        n9_ul_pdr_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_n4_information_t *OpenAPI_n4_information_copy(OpenAPI_n4_information_t *dst, OpenAPI_n4_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n4_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed");
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

    OpenAPI_n4_information_free(dst);
    dst = OpenAPI_n4_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

