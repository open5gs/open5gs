
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_type_cond.h"

OpenAPI_nf_type_cond_t *OpenAPI_nf_type_cond_create(
    OpenAPI_nf_type_e nf_type
    )
{
    OpenAPI_nf_type_cond_t *nf_type_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_type_cond_t));
    if (!nf_type_cond_local_var) {
        return NULL;
    }
    nf_type_cond_local_var->nf_type = nf_type;

    return nf_type_cond_local_var;
}

void OpenAPI_nf_type_cond_free(OpenAPI_nf_type_cond_t *nf_type_cond)
{
    if (NULL == nf_type_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_type_cond);
}

cJSON *OpenAPI_nf_type_cond_convertToJSON(OpenAPI_nf_type_cond_t *nf_type_cond)
{
    cJSON *item = NULL;

    if (nf_type_cond == NULL) {
        ogs_error("OpenAPI_nf_type_cond_convertToJSON() failed [NfTypeCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_type_cond->nf_type) {
        ogs_error("OpenAPI_nf_type_cond_convertToJSON() failed [nf_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(nf_type_cond->nf_type)) == NULL) {
        ogs_error("OpenAPI_nf_type_cond_convertToJSON() failed [nf_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nf_type_cond_t *OpenAPI_nf_type_cond_parseFromJSON(cJSON *nf_type_condJSON)
{
    OpenAPI_nf_type_cond_t *nf_type_cond_local_var = NULL;
    cJSON *nf_type = cJSON_GetObjectItemCaseSensitive(nf_type_condJSON, "nfType");
    if (!nf_type) {
        ogs_error("OpenAPI_nf_type_cond_parseFromJSON() failed [nf_type]");
        goto end;
    }

    OpenAPI_nf_type_e nf_typeVariable;

    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_nf_type_cond_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);

    nf_type_cond_local_var = OpenAPI_nf_type_cond_create (
        nf_typeVariable
        );

    return nf_type_cond_local_var;
end:
    return NULL;
}

