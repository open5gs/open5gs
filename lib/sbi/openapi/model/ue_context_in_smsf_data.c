
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_smsf_data.h"

OpenAPI_ue_context_in_smsf_data_t *OpenAPI_ue_context_in_smsf_data_create(
    OpenAPI_smsf_info_t *smsf_info3_gpp_access,
    OpenAPI_smsf_info_t *smsf_info_non3_gpp_access
)
{
    OpenAPI_ue_context_in_smsf_data_t *ue_context_in_smsf_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_in_smsf_data_t));
    ogs_assert(ue_context_in_smsf_data_local_var);

    ue_context_in_smsf_data_local_var->smsf_info3_gpp_access = smsf_info3_gpp_access;
    ue_context_in_smsf_data_local_var->smsf_info_non3_gpp_access = smsf_info_non3_gpp_access;

    return ue_context_in_smsf_data_local_var;
}

void OpenAPI_ue_context_in_smsf_data_free(OpenAPI_ue_context_in_smsf_data_t *ue_context_in_smsf_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_in_smsf_data) {
        return;
    }
    if (ue_context_in_smsf_data->smsf_info3_gpp_access) {
        OpenAPI_smsf_info_free(ue_context_in_smsf_data->smsf_info3_gpp_access);
        ue_context_in_smsf_data->smsf_info3_gpp_access = NULL;
    }
    if (ue_context_in_smsf_data->smsf_info_non3_gpp_access) {
        OpenAPI_smsf_info_free(ue_context_in_smsf_data->smsf_info_non3_gpp_access);
        ue_context_in_smsf_data->smsf_info_non3_gpp_access = NULL;
    }
    ogs_free(ue_context_in_smsf_data);
}

cJSON *OpenAPI_ue_context_in_smsf_data_convertToJSON(OpenAPI_ue_context_in_smsf_data_t *ue_context_in_smsf_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_in_smsf_data == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_convertToJSON() failed [UeContextInSmsfData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_smsf_data->smsf_info3_gpp_access) {
    cJSON *smsf_info3_gpp_access_local_JSON = OpenAPI_smsf_info_convertToJSON(ue_context_in_smsf_data->smsf_info3_gpp_access);
    if (smsf_info3_gpp_access_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_convertToJSON() failed [smsf_info3_gpp_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfInfo3GppAccess", smsf_info3_gpp_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_convertToJSON() failed [smsf_info3_gpp_access]");
        goto end;
    }
    }

    if (ue_context_in_smsf_data->smsf_info_non3_gpp_access) {
    cJSON *smsf_info_non3_gpp_access_local_JSON = OpenAPI_smsf_info_convertToJSON(ue_context_in_smsf_data->smsf_info_non3_gpp_access);
    if (smsf_info_non3_gpp_access_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_convertToJSON() failed [smsf_info_non3_gpp_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfInfoNon3GppAccess", smsf_info_non3_gpp_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_convertToJSON() failed [smsf_info_non3_gpp_access]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_in_smsf_data_t *OpenAPI_ue_context_in_smsf_data_parseFromJSON(cJSON *ue_context_in_smsf_dataJSON)
{
    OpenAPI_ue_context_in_smsf_data_t *ue_context_in_smsf_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smsf_info3_gpp_access = NULL;
    OpenAPI_smsf_info_t *smsf_info3_gpp_access_local_nonprim = NULL;
    cJSON *smsf_info_non3_gpp_access = NULL;
    OpenAPI_smsf_info_t *smsf_info_non3_gpp_access_local_nonprim = NULL;
    smsf_info3_gpp_access = cJSON_GetObjectItemCaseSensitive(ue_context_in_smsf_dataJSON, "smsfInfo3GppAccess");
    if (smsf_info3_gpp_access) {
    smsf_info3_gpp_access_local_nonprim = OpenAPI_smsf_info_parseFromJSON(smsf_info3_gpp_access);
    if (!smsf_info3_gpp_access_local_nonprim) {
        ogs_error("OpenAPI_smsf_info_parseFromJSON failed [smsf_info3_gpp_access]");
        goto end;
    }
    }

    smsf_info_non3_gpp_access = cJSON_GetObjectItemCaseSensitive(ue_context_in_smsf_dataJSON, "smsfInfoNon3GppAccess");
    if (smsf_info_non3_gpp_access) {
    smsf_info_non3_gpp_access_local_nonprim = OpenAPI_smsf_info_parseFromJSON(smsf_info_non3_gpp_access);
    if (!smsf_info_non3_gpp_access_local_nonprim) {
        ogs_error("OpenAPI_smsf_info_parseFromJSON failed [smsf_info_non3_gpp_access]");
        goto end;
    }
    }

    ue_context_in_smsf_data_local_var = OpenAPI_ue_context_in_smsf_data_create (
        smsf_info3_gpp_access ? smsf_info3_gpp_access_local_nonprim : NULL,
        smsf_info_non3_gpp_access ? smsf_info_non3_gpp_access_local_nonprim : NULL
    );

    return ue_context_in_smsf_data_local_var;
end:
    if (smsf_info3_gpp_access_local_nonprim) {
        OpenAPI_smsf_info_free(smsf_info3_gpp_access_local_nonprim);
        smsf_info3_gpp_access_local_nonprim = NULL;
    }
    if (smsf_info_non3_gpp_access_local_nonprim) {
        OpenAPI_smsf_info_free(smsf_info_non3_gpp_access_local_nonprim);
        smsf_info_non3_gpp_access_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_in_smsf_data_t *OpenAPI_ue_context_in_smsf_data_copy(OpenAPI_ue_context_in_smsf_data_t *dst, OpenAPI_ue_context_in_smsf_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_smsf_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_convertToJSON() failed");
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

    OpenAPI_ue_context_in_smsf_data_free(dst);
    dst = OpenAPI_ue_context_in_smsf_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

