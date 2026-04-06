
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tss_information.h"

OpenAPI_tss_information_t *OpenAPI_tss_information_create(
    char *nf_id,
    OpenAPI_list_t *tss_container,
    OpenAPI_list_t *tss_rsp_per_ngran_list
)
{
    OpenAPI_tss_information_t *tss_information_local_var = ogs_malloc(sizeof(OpenAPI_tss_information_t));
    ogs_assert(tss_information_local_var);

    tss_information_local_var->nf_id = nf_id;
    tss_information_local_var->tss_container = tss_container;
    tss_information_local_var->tss_rsp_per_ngran_list = tss_rsp_per_ngran_list;

    return tss_information_local_var;
}

void OpenAPI_tss_information_free(OpenAPI_tss_information_t *tss_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tss_information) {
        return;
    }
    if (tss_information->nf_id) {
        ogs_free(tss_information->nf_id);
        tss_information->nf_id = NULL;
    }
    if (tss_information->tss_container) {
        OpenAPI_list_for_each(tss_information->tss_container, node) {
            OpenAPI_n2_info_content_free(node->data);
        }
        OpenAPI_list_free(tss_information->tss_container);
        tss_information->tss_container = NULL;
    }
    if (tss_information->tss_rsp_per_ngran_list) {
        OpenAPI_list_for_each(tss_information->tss_rsp_per_ngran_list, node) {
            OpenAPI_tss_rsp_per_ngran_free(node->data);
        }
        OpenAPI_list_free(tss_information->tss_rsp_per_ngran_list);
        tss_information->tss_rsp_per_ngran_list = NULL;
    }
    ogs_free(tss_information);
}

cJSON *OpenAPI_tss_information_convertToJSON(OpenAPI_tss_information_t *tss_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tss_information == NULL) {
        ogs_error("OpenAPI_tss_information_convertToJSON() failed [TssInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tss_information->nf_id) {
    if (cJSON_AddStringToObject(item, "nfId", tss_information->nf_id) == NULL) {
        ogs_error("OpenAPI_tss_information_convertToJSON() failed [nf_id]");
        goto end;
    }
    }

    if (!tss_information->tss_container) {
        ogs_error("OpenAPI_tss_information_convertToJSON() failed [tss_container]");
        return NULL;
    }
    cJSON *tss_containerList = cJSON_AddArrayToObject(item, "tssContainer");
    if (tss_containerList == NULL) {
        ogs_error("OpenAPI_tss_information_convertToJSON() failed [tss_container]");
        goto end;
    }
    OpenAPI_list_for_each(tss_information->tss_container, node) {
        cJSON *itemLocal = OpenAPI_n2_info_content_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tss_information_convertToJSON() failed [tss_container]");
            goto end;
        }
        cJSON_AddItemToArray(tss_containerList, itemLocal);
    }

    if (tss_information->tss_rsp_per_ngran_list) {
    cJSON *tss_rsp_per_ngran_listList = cJSON_AddArrayToObject(item, "tssRspPerNgranList");
    if (tss_rsp_per_ngran_listList == NULL) {
        ogs_error("OpenAPI_tss_information_convertToJSON() failed [tss_rsp_per_ngran_list]");
        goto end;
    }
    OpenAPI_list_for_each(tss_information->tss_rsp_per_ngran_list, node) {
        cJSON *itemLocal = OpenAPI_tss_rsp_per_ngran_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_tss_information_convertToJSON() failed [tss_rsp_per_ngran_list]");
            goto end;
        }
        cJSON_AddItemToArray(tss_rsp_per_ngran_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_tss_information_t *OpenAPI_tss_information_parseFromJSON(cJSON *tss_informationJSON)
{
    OpenAPI_tss_information_t *tss_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_id = NULL;
    cJSON *tss_container = NULL;
    OpenAPI_list_t *tss_containerList = NULL;
    cJSON *tss_rsp_per_ngran_list = NULL;
    OpenAPI_list_t *tss_rsp_per_ngran_listList = NULL;
    nf_id = cJSON_GetObjectItemCaseSensitive(tss_informationJSON, "nfId");
    if (nf_id) {
    if (!cJSON_IsString(nf_id) && !cJSON_IsNull(nf_id)) {
        ogs_error("OpenAPI_tss_information_parseFromJSON() failed [nf_id]");
        goto end;
    }
    }

    tss_container = cJSON_GetObjectItemCaseSensitive(tss_informationJSON, "tssContainer");
    if (!tss_container) {
        ogs_error("OpenAPI_tss_information_parseFromJSON() failed [tss_container]");
        goto end;
    }
        cJSON *tss_container_local = NULL;
        if (!cJSON_IsArray(tss_container)) {
            ogs_error("OpenAPI_tss_information_parseFromJSON() failed [tss_container]");
            goto end;
        }

        tss_containerList = OpenAPI_list_create();

        cJSON_ArrayForEach(tss_container_local, tss_container) {
            if (!cJSON_IsObject(tss_container_local)) {
                ogs_error("OpenAPI_tss_information_parseFromJSON() failed [tss_container]");
                goto end;
            }
            OpenAPI_n2_info_content_t *tss_containerItem = OpenAPI_n2_info_content_parseFromJSON(tss_container_local);
            if (!tss_containerItem) {
                ogs_error("No tss_containerItem");
                goto end;
            }
            OpenAPI_list_add(tss_containerList, tss_containerItem);
        }

    tss_rsp_per_ngran_list = cJSON_GetObjectItemCaseSensitive(tss_informationJSON, "tssRspPerNgranList");
    if (tss_rsp_per_ngran_list) {
        cJSON *tss_rsp_per_ngran_list_local = NULL;
        if (!cJSON_IsArray(tss_rsp_per_ngran_list)) {
            ogs_error("OpenAPI_tss_information_parseFromJSON() failed [tss_rsp_per_ngran_list]");
            goto end;
        }

        tss_rsp_per_ngran_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tss_rsp_per_ngran_list_local, tss_rsp_per_ngran_list) {
            if (!cJSON_IsObject(tss_rsp_per_ngran_list_local)) {
                ogs_error("OpenAPI_tss_information_parseFromJSON() failed [tss_rsp_per_ngran_list]");
                goto end;
            }
            OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran_listItem = OpenAPI_tss_rsp_per_ngran_parseFromJSON(tss_rsp_per_ngran_list_local);
            if (!tss_rsp_per_ngran_listItem) {
                ogs_error("No tss_rsp_per_ngran_listItem");
                goto end;
            }
            OpenAPI_list_add(tss_rsp_per_ngran_listList, tss_rsp_per_ngran_listItem);
        }
    }

    tss_information_local_var = OpenAPI_tss_information_create (
        nf_id && !cJSON_IsNull(nf_id) ? ogs_strdup(nf_id->valuestring) : NULL,
        tss_containerList,
        tss_rsp_per_ngran_list ? tss_rsp_per_ngran_listList : NULL
    );

    return tss_information_local_var;
end:
    if (tss_containerList) {
        OpenAPI_list_for_each(tss_containerList, node) {
            OpenAPI_n2_info_content_free(node->data);
        }
        OpenAPI_list_free(tss_containerList);
        tss_containerList = NULL;
    }
    if (tss_rsp_per_ngran_listList) {
        OpenAPI_list_for_each(tss_rsp_per_ngran_listList, node) {
            OpenAPI_tss_rsp_per_ngran_free(node->data);
        }
        OpenAPI_list_free(tss_rsp_per_ngran_listList);
        tss_rsp_per_ngran_listList = NULL;
    }
    return NULL;
}

OpenAPI_tss_information_t *OpenAPI_tss_information_copy(OpenAPI_tss_information_t *dst, OpenAPI_tss_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tss_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tss_information_convertToJSON() failed");
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

    OpenAPI_tss_information_free(dst);
    dst = OpenAPI_tss_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

