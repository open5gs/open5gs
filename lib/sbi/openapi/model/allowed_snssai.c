
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
    OpenAPI_allowed_snssai_t *allowed_snssai_local_var = ogs_malloc(sizeof(OpenAPI_allowed_snssai_t));
    ogs_assert(allowed_snssai_local_var);

    allowed_snssai_local_var->allowed_snssai = allowed_snssai;
    allowed_snssai_local_var->nsi_information_list = nsi_information_list;
    allowed_snssai_local_var->mapped_home_snssai = mapped_home_snssai;

    return allowed_snssai_local_var;
}

void OpenAPI_allowed_snssai_free(OpenAPI_allowed_snssai_t *allowed_snssai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == allowed_snssai) {
        return;
    }
    if (allowed_snssai->allowed_snssai) {
        OpenAPI_snssai_free(allowed_snssai->allowed_snssai);
        allowed_snssai->allowed_snssai = NULL;
    }
    if (allowed_snssai->nsi_information_list) {
        OpenAPI_list_for_each(allowed_snssai->nsi_information_list, node) {
            OpenAPI_nsi_information_free(node->data);
        }
        OpenAPI_list_free(allowed_snssai->nsi_information_list);
        allowed_snssai->nsi_information_list = NULL;
    }
    if (allowed_snssai->mapped_home_snssai) {
        OpenAPI_snssai_free(allowed_snssai->mapped_home_snssai);
        allowed_snssai->mapped_home_snssai = NULL;
    }
    ogs_free(allowed_snssai);
}

cJSON *OpenAPI_allowed_snssai_convertToJSON(OpenAPI_allowed_snssai_t *allowed_snssai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (allowed_snssai == NULL) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [AllowedSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!allowed_snssai->allowed_snssai) {
        ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [allowed_snssai]");
        return NULL;
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
    OpenAPI_list_for_each(allowed_snssai->nsi_information_list, node) {
        cJSON *itemLocal = OpenAPI_nsi_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_allowed_snssai_convertToJSON() failed [nsi_information_list]");
            goto end;
        }
        cJSON_AddItemToArray(nsi_information_listList, itemLocal);
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_snssai = NULL;
    OpenAPI_snssai_t *allowed_snssai_local_nonprim = NULL;
    cJSON *nsi_information_list = NULL;
    OpenAPI_list_t *nsi_information_listList = NULL;
    cJSON *mapped_home_snssai = NULL;
    OpenAPI_snssai_t *mapped_home_snssai_local_nonprim = NULL;
    allowed_snssai = cJSON_GetObjectItemCaseSensitive(allowed_snssaiJSON, "allowedSnssai");
    if (!allowed_snssai) {
        ogs_error("OpenAPI_allowed_snssai_parseFromJSON() failed [allowed_snssai]");
        goto end;
    }
    allowed_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(allowed_snssai);
    if (!allowed_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [allowed_snssai]");
        goto end;
    }

    nsi_information_list = cJSON_GetObjectItemCaseSensitive(allowed_snssaiJSON, "nsiInformationList");
    if (nsi_information_list) {
        cJSON *nsi_information_list_local = NULL;
        if (!cJSON_IsArray(nsi_information_list)) {
            ogs_error("OpenAPI_allowed_snssai_parseFromJSON() failed [nsi_information_list]");
            goto end;
        }

        nsi_information_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_information_list_local, nsi_information_list) {
            if (!cJSON_IsObject(nsi_information_list_local)) {
                ogs_error("OpenAPI_allowed_snssai_parseFromJSON() failed [nsi_information_list]");
                goto end;
            }
            OpenAPI_nsi_information_t *nsi_information_listItem = OpenAPI_nsi_information_parseFromJSON(nsi_information_list_local);
            if (!nsi_information_listItem) {
                ogs_error("No nsi_information_listItem");
                goto end;
            }
            OpenAPI_list_add(nsi_information_listList, nsi_information_listItem);
        }
    }

    mapped_home_snssai = cJSON_GetObjectItemCaseSensitive(allowed_snssaiJSON, "mappedHomeSnssai");
    if (mapped_home_snssai) {
    mapped_home_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(mapped_home_snssai);
    if (!mapped_home_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [mapped_home_snssai]");
        goto end;
    }
    }

    allowed_snssai_local_var = OpenAPI_allowed_snssai_create (
        allowed_snssai_local_nonprim,
        nsi_information_list ? nsi_information_listList : NULL,
        mapped_home_snssai ? mapped_home_snssai_local_nonprim : NULL
    );

    return allowed_snssai_local_var;
end:
    if (allowed_snssai_local_nonprim) {
        OpenAPI_snssai_free(allowed_snssai_local_nonprim);
        allowed_snssai_local_nonprim = NULL;
    }
    if (nsi_information_listList) {
        OpenAPI_list_for_each(nsi_information_listList, node) {
            OpenAPI_nsi_information_free(node->data);
        }
        OpenAPI_list_free(nsi_information_listList);
        nsi_information_listList = NULL;
    }
    if (mapped_home_snssai_local_nonprim) {
        OpenAPI_snssai_free(mapped_home_snssai_local_nonprim);
        mapped_home_snssai_local_nonprim = NULL;
    }
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

