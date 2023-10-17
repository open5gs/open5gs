
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "collocated_nf_instance.h"

OpenAPI_collocated_nf_instance_t *OpenAPI_collocated_nf_instance_create(
    char *nf_instance_id,
    OpenAPI_collocated_nf_type_e nf_type
)
{
    OpenAPI_collocated_nf_instance_t *collocated_nf_instance_local_var = ogs_malloc(sizeof(OpenAPI_collocated_nf_instance_t));
    ogs_assert(collocated_nf_instance_local_var);

    collocated_nf_instance_local_var->nf_instance_id = nf_instance_id;
    collocated_nf_instance_local_var->nf_type = nf_type;

    return collocated_nf_instance_local_var;
}

void OpenAPI_collocated_nf_instance_free(OpenAPI_collocated_nf_instance_t *collocated_nf_instance)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == collocated_nf_instance) {
        return;
    }
    if (collocated_nf_instance->nf_instance_id) {
        ogs_free(collocated_nf_instance->nf_instance_id);
        collocated_nf_instance->nf_instance_id = NULL;
    }
    ogs_free(collocated_nf_instance);
}

cJSON *OpenAPI_collocated_nf_instance_convertToJSON(OpenAPI_collocated_nf_instance_t *collocated_nf_instance)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (collocated_nf_instance == NULL) {
        ogs_error("OpenAPI_collocated_nf_instance_convertToJSON() failed [CollocatedNfInstance]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!collocated_nf_instance->nf_instance_id) {
        ogs_error("OpenAPI_collocated_nf_instance_convertToJSON() failed [nf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", collocated_nf_instance->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_collocated_nf_instance_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (collocated_nf_instance->nf_type == OpenAPI_collocated_nf_type_NULL) {
        ogs_error("OpenAPI_collocated_nf_instance_convertToJSON() failed [nf_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_collocated_nf_type_ToString(collocated_nf_instance->nf_type)) == NULL) {
        ogs_error("OpenAPI_collocated_nf_instance_convertToJSON() failed [nf_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_collocated_nf_instance_t *OpenAPI_collocated_nf_instance_parseFromJSON(cJSON *collocated_nf_instanceJSON)
{
    OpenAPI_collocated_nf_instance_t *collocated_nf_instance_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *nf_type = NULL;
    OpenAPI_collocated_nf_type_e nf_typeVariable = 0;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(collocated_nf_instanceJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_collocated_nf_instance_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_collocated_nf_instance_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    nf_type = cJSON_GetObjectItemCaseSensitive(collocated_nf_instanceJSON, "nfType");
    if (!nf_type) {
        ogs_error("OpenAPI_collocated_nf_instance_parseFromJSON() failed [nf_type]");
        goto end;
    }
    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_collocated_nf_instance_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_collocated_nf_type_FromString(nf_type->valuestring);

    collocated_nf_instance_local_var = OpenAPI_collocated_nf_instance_create (
        ogs_strdup(nf_instance_id->valuestring),
        nf_typeVariable
    );

    return collocated_nf_instance_local_var;
end:
    return NULL;
}

OpenAPI_collocated_nf_instance_t *OpenAPI_collocated_nf_instance_copy(OpenAPI_collocated_nf_instance_t *dst, OpenAPI_collocated_nf_instance_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_collocated_nf_instance_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_collocated_nf_instance_convertToJSON() failed");
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

    OpenAPI_collocated_nf_instance_free(dst);
    dst = OpenAPI_collocated_nf_instance_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

