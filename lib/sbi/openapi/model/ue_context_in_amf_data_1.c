
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_amf_data_1.h"

OpenAPI_ue_context_in_amf_data_1_t *OpenAPI_ue_context_in_amf_data_1_create(
    OpenAPI_eps_interworking_info_t *eps_interworking_info
)
{
    OpenAPI_ue_context_in_amf_data_1_t *ue_context_in_amf_data_1_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_in_amf_data_1_t));
    ogs_assert(ue_context_in_amf_data_1_local_var);

    ue_context_in_amf_data_1_local_var->eps_interworking_info = eps_interworking_info;

    return ue_context_in_amf_data_1_local_var;
}

void OpenAPI_ue_context_in_amf_data_1_free(OpenAPI_ue_context_in_amf_data_1_t *ue_context_in_amf_data_1)
{
    if (NULL == ue_context_in_amf_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_eps_interworking_info_free(ue_context_in_amf_data_1->eps_interworking_info);
    ogs_free(ue_context_in_amf_data_1);
}

cJSON *OpenAPI_ue_context_in_amf_data_1_convertToJSON(OpenAPI_ue_context_in_amf_data_1_t *ue_context_in_amf_data_1)
{
    cJSON *item = NULL;

    if (ue_context_in_amf_data_1 == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_1_convertToJSON() failed [UeContextInAmfData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_amf_data_1->eps_interworking_info) {
    cJSON *eps_interworking_info_local_JSON = OpenAPI_eps_interworking_info_convertToJSON(ue_context_in_amf_data_1->eps_interworking_info);
    if (eps_interworking_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_1_convertToJSON() failed [eps_interworking_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsInterworkingInfo", eps_interworking_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_amf_data_1_convertToJSON() failed [eps_interworking_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_in_amf_data_1_t *OpenAPI_ue_context_in_amf_data_1_parseFromJSON(cJSON *ue_context_in_amf_data_1JSON)
{
    OpenAPI_ue_context_in_amf_data_1_t *ue_context_in_amf_data_1_local_var = NULL;
    cJSON *eps_interworking_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_amf_data_1JSON, "epsInterworkingInfo");

    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_nonprim = NULL;
    if (eps_interworking_info) {
    eps_interworking_info_local_nonprim = OpenAPI_eps_interworking_info_parseFromJSON(eps_interworking_info);
    }

    ue_context_in_amf_data_1_local_var = OpenAPI_ue_context_in_amf_data_1_create (
        eps_interworking_info ? eps_interworking_info_local_nonprim : NULL
    );

    return ue_context_in_amf_data_1_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_in_amf_data_1_t *OpenAPI_ue_context_in_amf_data_1_copy(OpenAPI_ue_context_in_amf_data_1_t *dst, OpenAPI_ue_context_in_amf_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_amf_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_amf_data_1_convertToJSON() failed");
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

    OpenAPI_ue_context_in_amf_data_1_free(dst);
    dst = OpenAPI_ue_context_in_amf_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

