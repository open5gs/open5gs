
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_info.h"

OpenAPI_nf_info_t *OpenAPI_nf_info_create(
    OpenAPI_nf_type_e nf_type
    )
{
    OpenAPI_nf_info_t *nf_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_info_t));
    if (!nf_info_local_var) {
        return NULL;
    }
    nf_info_local_var->nf_type = nf_type;

    return nf_info_local_var;
}

void OpenAPI_nf_info_free(OpenAPI_nf_info_t *nf_info)
{
    if (NULL == nf_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_info);
}

cJSON *OpenAPI_nf_info_convertToJSON(OpenAPI_nf_info_t *nf_info)
{
    cJSON *item = NULL;

    if (nf_info == NULL) {
        ogs_error("OpenAPI_nf_info_convertToJSON() failed [NfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nf_info->nf_type) {
        if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(nf_info->nf_type)) == NULL) {
            ogs_error("OpenAPI_nf_info_convertToJSON() failed [nf_type]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nf_info_t *OpenAPI_nf_info_parseFromJSON(cJSON *nf_infoJSON)
{
    OpenAPI_nf_info_t *nf_info_local_var = NULL;
    cJSON *nf_type = cJSON_GetObjectItemCaseSensitive(nf_infoJSON, "nfType");

    OpenAPI_nf_type_e nf_typeVariable;
    if (nf_type) {
        if (!cJSON_IsString(nf_type)) {
            ogs_error("OpenAPI_nf_info_parseFromJSON() failed [nf_type]");
            goto end;
        }
        nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);
    }

    nf_info_local_var = OpenAPI_nf_info_create (
        nf_type ? nf_typeVariable : 0
        );

    return nf_info_local_var;
end:
    return NULL;
}

OpenAPI_nf_info_t *OpenAPI_nf_info_copy(OpenAPI_nf_info_t *dst, OpenAPI_nf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_info_convertToJSON() failed");
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

    OpenAPI_nf_info_free(dst);
    dst = OpenAPI_nf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

