
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_instance_id_cond.h"

OpenAPI_nf_instance_id_cond_t *OpenAPI_nf_instance_id_cond_create(
    char *nf_instance_id
    )
{
    OpenAPI_nf_instance_id_cond_t *nf_instance_id_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_instance_id_cond_t));
    if (!nf_instance_id_cond_local_var) {
        return NULL;
    }
    nf_instance_id_cond_local_var->nf_instance_id = nf_instance_id;

    return nf_instance_id_cond_local_var;
}

void OpenAPI_nf_instance_id_cond_free(OpenAPI_nf_instance_id_cond_t *nf_instance_id_cond)
{
    if (NULL == nf_instance_id_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_instance_id_cond->nf_instance_id);
    ogs_free(nf_instance_id_cond);
}

cJSON *OpenAPI_nf_instance_id_cond_convertToJSON(OpenAPI_nf_instance_id_cond_t *nf_instance_id_cond)
{
    cJSON *item = NULL;

    if (nf_instance_id_cond == NULL) {
        ogs_error("OpenAPI_nf_instance_id_cond_convertToJSON() failed [NfInstanceIdCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_instance_id_cond->nf_instance_id) {
        ogs_error("OpenAPI_nf_instance_id_cond_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", nf_instance_id_cond->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_instance_id_cond_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nf_instance_id_cond_t *OpenAPI_nf_instance_id_cond_parseFromJSON(cJSON *nf_instance_id_condJSON)
{
    OpenAPI_nf_instance_id_cond_t *nf_instance_id_cond_local_var = NULL;
    cJSON *nf_instance_id = cJSON_GetObjectItemCaseSensitive(nf_instance_id_condJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_nf_instance_id_cond_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_nf_instance_id_cond_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    nf_instance_id_cond_local_var = OpenAPI_nf_instance_id_cond_create (
        ogs_strdup(nf_instance_id->valuestring)
        );

    return nf_instance_id_cond_local_var;
end:
    return NULL;
}

