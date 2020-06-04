
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_set_cond.h"

OpenAPI_nf_set_cond_t *OpenAPI_nf_set_cond_create(
    char *nf_set_id
    )
{
    OpenAPI_nf_set_cond_t *nf_set_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_set_cond_t));
    if (!nf_set_cond_local_var) {
        return NULL;
    }
    nf_set_cond_local_var->nf_set_id = nf_set_id;

    return nf_set_cond_local_var;
}

void OpenAPI_nf_set_cond_free(OpenAPI_nf_set_cond_t *nf_set_cond)
{
    if (NULL == nf_set_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_set_cond->nf_set_id);
    ogs_free(nf_set_cond);
}

cJSON *OpenAPI_nf_set_cond_convertToJSON(OpenAPI_nf_set_cond_t *nf_set_cond)
{
    cJSON *item = NULL;

    if (nf_set_cond == NULL) {
        ogs_error("OpenAPI_nf_set_cond_convertToJSON() failed [NfSetCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_set_cond->nf_set_id) {
        ogs_error("OpenAPI_nf_set_cond_convertToJSON() failed [nf_set_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfSetId", nf_set_cond->nf_set_id) == NULL) {
        ogs_error("OpenAPI_nf_set_cond_convertToJSON() failed [nf_set_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nf_set_cond_t *OpenAPI_nf_set_cond_parseFromJSON(cJSON *nf_set_condJSON)
{
    OpenAPI_nf_set_cond_t *nf_set_cond_local_var = NULL;
    cJSON *nf_set_id = cJSON_GetObjectItemCaseSensitive(nf_set_condJSON, "nfSetId");
    if (!nf_set_id) {
        ogs_error("OpenAPI_nf_set_cond_parseFromJSON() failed [nf_set_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_set_id)) {
        ogs_error("OpenAPI_nf_set_cond_parseFromJSON() failed [nf_set_id]");
        goto end;
    }

    nf_set_cond_local_var = OpenAPI_nf_set_cond_create (
        ogs_strdup(nf_set_id->valuestring)
        );

    return nf_set_cond_local_var;
end:
    return NULL;
}

