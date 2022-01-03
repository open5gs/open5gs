
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_smsf_data_1.h"

OpenAPI_ue_context_in_smsf_data_1_t *OpenAPI_ue_context_in_smsf_data_1_create(
    OpenAPI_smsf_info_1_t *smsf_info3_gpp_access,
    OpenAPI_smsf_info_1_t *smsf_info_non3_gpp_access
)
{
    OpenAPI_ue_context_in_smsf_data_1_t *ue_context_in_smsf_data_1_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_in_smsf_data_1_t));
    ogs_assert(ue_context_in_smsf_data_1_local_var);

    ue_context_in_smsf_data_1_local_var->smsf_info3_gpp_access = smsf_info3_gpp_access;
    ue_context_in_smsf_data_1_local_var->smsf_info_non3_gpp_access = smsf_info_non3_gpp_access;

    return ue_context_in_smsf_data_1_local_var;
}

void OpenAPI_ue_context_in_smsf_data_1_free(OpenAPI_ue_context_in_smsf_data_1_t *ue_context_in_smsf_data_1)
{
    if (NULL == ue_context_in_smsf_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_smsf_info_1_free(ue_context_in_smsf_data_1->smsf_info3_gpp_access);
    OpenAPI_smsf_info_1_free(ue_context_in_smsf_data_1->smsf_info_non3_gpp_access);
    ogs_free(ue_context_in_smsf_data_1);
}

cJSON *OpenAPI_ue_context_in_smsf_data_1_convertToJSON(OpenAPI_ue_context_in_smsf_data_1_t *ue_context_in_smsf_data_1)
{
    cJSON *item = NULL;

    if (ue_context_in_smsf_data_1 == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_1_convertToJSON() failed [UeContextInSmsfData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_smsf_data_1->smsf_info3_gpp_access) {
    cJSON *smsf_info3_gpp_access_local_JSON = OpenAPI_smsf_info_1_convertToJSON(ue_context_in_smsf_data_1->smsf_info3_gpp_access);
    if (smsf_info3_gpp_access_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_1_convertToJSON() failed [smsf_info3_gpp_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfInfo3GppAccess", smsf_info3_gpp_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_1_convertToJSON() failed [smsf_info3_gpp_access]");
        goto end;
    }
    }

    if (ue_context_in_smsf_data_1->smsf_info_non3_gpp_access) {
    cJSON *smsf_info_non3_gpp_access_local_JSON = OpenAPI_smsf_info_1_convertToJSON(ue_context_in_smsf_data_1->smsf_info_non3_gpp_access);
    if (smsf_info_non3_gpp_access_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_1_convertToJSON() failed [smsf_info_non3_gpp_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfInfoNon3GppAccess", smsf_info_non3_gpp_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_1_convertToJSON() failed [smsf_info_non3_gpp_access]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_in_smsf_data_1_t *OpenAPI_ue_context_in_smsf_data_1_parseFromJSON(cJSON *ue_context_in_smsf_data_1JSON)
{
    OpenAPI_ue_context_in_smsf_data_1_t *ue_context_in_smsf_data_1_local_var = NULL;
    cJSON *smsf_info3_gpp_access = cJSON_GetObjectItemCaseSensitive(ue_context_in_smsf_data_1JSON, "smsfInfo3GppAccess");

    OpenAPI_smsf_info_1_t *smsf_info3_gpp_access_local_nonprim = NULL;
    if (smsf_info3_gpp_access) {
    smsf_info3_gpp_access_local_nonprim = OpenAPI_smsf_info_1_parseFromJSON(smsf_info3_gpp_access);
    }

    cJSON *smsf_info_non3_gpp_access = cJSON_GetObjectItemCaseSensitive(ue_context_in_smsf_data_1JSON, "smsfInfoNon3GppAccess");

    OpenAPI_smsf_info_1_t *smsf_info_non3_gpp_access_local_nonprim = NULL;
    if (smsf_info_non3_gpp_access) {
    smsf_info_non3_gpp_access_local_nonprim = OpenAPI_smsf_info_1_parseFromJSON(smsf_info_non3_gpp_access);
    }

    ue_context_in_smsf_data_1_local_var = OpenAPI_ue_context_in_smsf_data_1_create (
        smsf_info3_gpp_access ? smsf_info3_gpp_access_local_nonprim : NULL,
        smsf_info_non3_gpp_access ? smsf_info_non3_gpp_access_local_nonprim : NULL
    );

    return ue_context_in_smsf_data_1_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_in_smsf_data_1_t *OpenAPI_ue_context_in_smsf_data_1_copy(OpenAPI_ue_context_in_smsf_data_1_t *dst, OpenAPI_ue_context_in_smsf_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_smsf_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_1_convertToJSON() failed");
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

    OpenAPI_ue_context_in_smsf_data_1_free(dst);
    dst = OpenAPI_ue_context_in_smsf_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

