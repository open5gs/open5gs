
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allowed_snssai.h"

OpenAPI_allowed_snssai_t *OpenAPI_allowed_snssai_create(
    OpenAPI_snssai_t *allowed_snssai,
    OpenAPI_list_t *nsi_information_list,
    OpenAPI_snssai_t *mapped_home_snssai
    )
{
    OpenAPI_allowed_snssai_t *allowed_snssai_local_var = OpenAPI_malloc(sizeof(OpenAPI_allowed_snssai_t));
    if (!allowed_snssai_local_var) {
        return NULL;
    }
    allowed_snssai_local_var->allowed_snssai = allowed_snssai;
    allowed_snssai_local_var->nsi_information_list = nsi_information_list;
    allowed_snssai_local_var->mapped_home_snssai = mapped_home_snssai;

    return allowed_snssai_local_var;
}

void OpenAPI_allowed_snssai_free(OpenAPI_allowed_snssai_t *allowed_snssai)
{
    if (NULL == allowed_snssai) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(allowed_snssai->allowed_snssai);
    OpenAPI_list_for_each(allowed_snssai->nsi_information_list, node) {
        OpenAPI_nsi_information_free(node->data);
    }
    OpenAPI_list_free(allowed_snssai->nsi_information_list);
    OpenAPI_snssai_free(allowed_snssai->mapped_home_snssai);
    ogs_free(allowed_snssai);
}

cJSON *OpenAPI_allowed_snssai_convertToJSON(OpenAPI_allowed_snssai_t *allowed_snssai)
{
    cJSON *item = NULL;

    if (allowed_snssai == NULL) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [AllowedSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!allowed_snssai->allowed_snssai) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [allowed_snssai]");
        goto end;
    }
    cJSON *allowed_snssai_local_JSON = OpenAPI_snssai_convertToJSON(allowed_snssai->allowed_snssai);
    if (allowed_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [allowed_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedSnssai", allowed_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [allowed_snssai]");
        goto end;
    }

    if (allowed_snssai->nsi_information_list) {
        cJSON *nsi_information_listList = cJSON_AddArrayToObject(item, "nsiInformationList");
        if (nsi_information_listList == NULL) {
            ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [nsi_information_list]");
            goto end;
        }

        OpenAPI_lnode_t *nsi_information_list_node;
        if (allowed_snssai->nsi_information_list) {
            OpenAPI_list_for_each(allowed_snssai->nsi_information_list, nsi_information_list_node) {
                cJSON *itemLocal = OpenAPI_nsi_information_convertToJSON(nsi_information_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [nsi_information_list]");
                    goto end;
                }
                cJSON_AddItemToArray(nsi_information_listList, itemLocal);
            }
        }
    }

    if (allowed_snssai->mapped_home_snssai) {
        cJSON *mapped_home_snssai_local_JSON = OpenAPI_snssai_convertToJSON(allowed_snssai->mapped_home_snssai);
        if (mapped_home_snssai_local_JSON == NULL) {
            ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [mapped_home_snssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "mappedHomeSnssai", mapped_home_snssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [mapped_home_snssai]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_allowed_snssai_t *OpenAPI_allowed_snssai_parseFromJSON(cJSON *allowed_snssaiJSON)
{
    OpenAPI_allowed_snssai_t *allowed_snssai_local_var = NULL;
    cJSON *allowed_snssai = cJSON_GetObjectItemCaseSensitive(allowed_snssaiJSON, "allowedSnssai");
    if (!allowed_snssai) {
        ogs_error("OpenAPI_allowed_snssai_parseFromJSON() failed [allowed_snssai]");
        goto end;
    }

    OpenAPI_snssai_t *allowed_snssai_local_nonprim = NULL;

    allowed_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(allowed_snssai);

    cJSON *nsi_information_list = cJSON_GetObjectItemCaseSensitive(allowed_snssaiJSON, "nsiInformationList");

    OpenAPI_list_t *nsi_information_listList;
    if (nsi_information_list) {
        cJSON *nsi_information_list_local_nonprimitive;
        if (!cJSON_IsArray(nsi_information_list)) {
            ogs_error("OpenAPI_allowed_snssai_parseFromJSON() failed [nsi_information_list]");
            goto end;
        }

        nsi_information_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_information_list_local_nonprimitive, nsi_information_list ) {
            if (!cJSON_IsObject(nsi_information_list_local_nonprimitive)) {
                ogs_error("OpenAPI_allowed_snssai_parseFromJSON() failed [nsi_information_list]");
                goto end;
            }
            OpenAPI_nsi_information_t *nsi_information_listItem = OpenAPI_nsi_information_parseFromJSON(nsi_information_list_local_nonprimitive);

            OpenAPI_list_add(nsi_information_listList, nsi_information_listItem);
        }
    }

    cJSON *mapped_home_snssai = cJSON_GetObjectItemCaseSensitive(allowed_snssaiJSON, "mappedHomeSnssai");

    OpenAPI_snssai_t *mapped_home_snssai_local_nonprim = NULL;
    if (mapped_home_snssai) {
        mapped_home_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(mapped_home_snssai);
    }

    allowed_snssai_local_var = OpenAPI_allowed_snssai_create (
        allowed_snssai_local_nonprim,
        nsi_information_list ? nsi_information_listList : NULL,
        mapped_home_snssai ? mapped_home_snssai_local_nonprim : NULL
        );

    return allowed_snssai_local_var;
end:
    return NULL;
}

OpenAPI_allowed_snssai_t *OpenAPI_allowed_snssai_copy(OpenAPI_allowed_snssai_t *dst, OpenAPI_allowed_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_allowed_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed");
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

    OpenAPI_allowed_snssai_free(dst);
    dst = OpenAPI_allowed_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

