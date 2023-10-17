
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_instance_id_list_cond.h"

OpenAPI_nf_instance_id_list_cond_t *OpenAPI_nf_instance_id_list_cond_create(
    OpenAPI_list_t *nf_instance_id_list
)
{
    OpenAPI_nf_instance_id_list_cond_t *nf_instance_id_list_cond_local_var = ogs_malloc(sizeof(OpenAPI_nf_instance_id_list_cond_t));
    ogs_assert(nf_instance_id_list_cond_local_var);

    nf_instance_id_list_cond_local_var->nf_instance_id_list = nf_instance_id_list;

    return nf_instance_id_list_cond_local_var;
}

void OpenAPI_nf_instance_id_list_cond_free(OpenAPI_nf_instance_id_list_cond_t *nf_instance_id_list_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_instance_id_list_cond) {
        return;
    }
    if (nf_instance_id_list_cond->nf_instance_id_list) {
        OpenAPI_list_for_each(nf_instance_id_list_cond->nf_instance_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_instance_id_list_cond->nf_instance_id_list);
        nf_instance_id_list_cond->nf_instance_id_list = NULL;
    }
    ogs_free(nf_instance_id_list_cond);
}

cJSON *OpenAPI_nf_instance_id_list_cond_convertToJSON(OpenAPI_nf_instance_id_list_cond_t *nf_instance_id_list_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_instance_id_list_cond == NULL) {
        ogs_error("OpenAPI_nf_instance_id_list_cond_convertToJSON() failed [NfInstanceIdListCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_instance_id_list_cond->nf_instance_id_list) {
        ogs_error("OpenAPI_nf_instance_id_list_cond_convertToJSON() failed [nf_instance_id_list]");
        return NULL;
    }
    cJSON *nf_instance_id_listList = cJSON_AddArrayToObject(item, "nfInstanceIdList");
    if (nf_instance_id_listList == NULL) {
        ogs_error("OpenAPI_nf_instance_id_list_cond_convertToJSON() failed [nf_instance_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(nf_instance_id_list_cond->nf_instance_id_list, node) {
        if (cJSON_AddStringToObject(nf_instance_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nf_instance_id_list_cond_convertToJSON() failed [nf_instance_id_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nf_instance_id_list_cond_t *OpenAPI_nf_instance_id_list_cond_parseFromJSON(cJSON *nf_instance_id_list_condJSON)
{
    OpenAPI_nf_instance_id_list_cond_t *nf_instance_id_list_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instance_id_list = NULL;
    OpenAPI_list_t *nf_instance_id_listList = NULL;
    nf_instance_id_list = cJSON_GetObjectItemCaseSensitive(nf_instance_id_list_condJSON, "nfInstanceIdList");
    if (!nf_instance_id_list) {
        ogs_error("OpenAPI_nf_instance_id_list_cond_parseFromJSON() failed [nf_instance_id_list]");
        goto end;
    }
        cJSON *nf_instance_id_list_local = NULL;
        if (!cJSON_IsArray(nf_instance_id_list)) {
            ogs_error("OpenAPI_nf_instance_id_list_cond_parseFromJSON() failed [nf_instance_id_list]");
            goto end;
        }

        nf_instance_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instance_id_list_local, nf_instance_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_instance_id_list_local)) {
                ogs_error("OpenAPI_nf_instance_id_list_cond_parseFromJSON() failed [nf_instance_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_instance_id_listList, ogs_strdup(nf_instance_id_list_local->valuestring));
        }

    nf_instance_id_list_cond_local_var = OpenAPI_nf_instance_id_list_cond_create (
        nf_instance_id_listList
    );

    return nf_instance_id_list_cond_local_var;
end:
    if (nf_instance_id_listList) {
        OpenAPI_list_for_each(nf_instance_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_instance_id_listList);
        nf_instance_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_nf_instance_id_list_cond_t *OpenAPI_nf_instance_id_list_cond_copy(OpenAPI_nf_instance_id_list_cond_t *dst, OpenAPI_nf_instance_id_list_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_instance_id_list_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_instance_id_list_cond_convertToJSON() failed");
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

    OpenAPI_nf_instance_id_list_cond_free(dst);
    dst = OpenAPI_nf_instance_id_list_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

