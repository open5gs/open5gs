
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "consumer_nf_information.h"

OpenAPI_consumer_nf_information_t *OpenAPI_consumer_nf_information_create(
    char *nf_id,
    char *nf_set_id,
    OpenAPI_list_t *tai_list
)
{
    OpenAPI_consumer_nf_information_t *consumer_nf_information_local_var = ogs_malloc(sizeof(OpenAPI_consumer_nf_information_t));
    ogs_assert(consumer_nf_information_local_var);

    consumer_nf_information_local_var->nf_id = nf_id;
    consumer_nf_information_local_var->nf_set_id = nf_set_id;
    consumer_nf_information_local_var->tai_list = tai_list;

    return consumer_nf_information_local_var;
}

void OpenAPI_consumer_nf_information_free(OpenAPI_consumer_nf_information_t *consumer_nf_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == consumer_nf_information) {
        return;
    }
    if (consumer_nf_information->nf_id) {
        ogs_free(consumer_nf_information->nf_id);
        consumer_nf_information->nf_id = NULL;
    }
    if (consumer_nf_information->nf_set_id) {
        ogs_free(consumer_nf_information->nf_set_id);
        consumer_nf_information->nf_set_id = NULL;
    }
    if (consumer_nf_information->tai_list) {
        OpenAPI_list_for_each(consumer_nf_information->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(consumer_nf_information->tai_list);
        consumer_nf_information->tai_list = NULL;
    }
    ogs_free(consumer_nf_information);
}

cJSON *OpenAPI_consumer_nf_information_convertToJSON(OpenAPI_consumer_nf_information_t *consumer_nf_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (consumer_nf_information == NULL) {
        ogs_error("OpenAPI_consumer_nf_information_convertToJSON() failed [ConsumerNfInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (consumer_nf_information->nf_id) {
    if (cJSON_AddStringToObject(item, "nfId", consumer_nf_information->nf_id) == NULL) {
        ogs_error("OpenAPI_consumer_nf_information_convertToJSON() failed [nf_id]");
        goto end;
    }
    }

    if (consumer_nf_information->nf_set_id) {
    if (cJSON_AddStringToObject(item, "nfSetId", consumer_nf_information->nf_set_id) == NULL) {
        ogs_error("OpenAPI_consumer_nf_information_convertToJSON() failed [nf_set_id]");
        goto end;
    }
    }

    if (consumer_nf_information->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_consumer_nf_information_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(consumer_nf_information->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_consumer_nf_information_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_consumer_nf_information_t *OpenAPI_consumer_nf_information_parseFromJSON(cJSON *consumer_nf_informationJSON)
{
    OpenAPI_consumer_nf_information_t *consumer_nf_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_id = NULL;
    cJSON *nf_set_id = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    nf_id = cJSON_GetObjectItemCaseSensitive(consumer_nf_informationJSON, "nfId");
    if (nf_id) {
    if (!cJSON_IsString(nf_id) && !cJSON_IsNull(nf_id)) {
        ogs_error("OpenAPI_consumer_nf_information_parseFromJSON() failed [nf_id]");
        goto end;
    }
    }

    nf_set_id = cJSON_GetObjectItemCaseSensitive(consumer_nf_informationJSON, "nfSetId");
    if (nf_set_id) {
    if (!cJSON_IsString(nf_set_id) && !cJSON_IsNull(nf_set_id)) {
        ogs_error("OpenAPI_consumer_nf_information_parseFromJSON() failed [nf_set_id]");
        goto end;
    }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(consumer_nf_informationJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_consumer_nf_information_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_consumer_nf_information_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    consumer_nf_information_local_var = OpenAPI_consumer_nf_information_create (
        nf_id && !cJSON_IsNull(nf_id) ? ogs_strdup(nf_id->valuestring) : NULL,
        nf_set_id && !cJSON_IsNull(nf_set_id) ? ogs_strdup(nf_set_id->valuestring) : NULL,
        tai_list ? tai_listList : NULL
    );

    return consumer_nf_information_local_var;
end:
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    return NULL;
}

OpenAPI_consumer_nf_information_t *OpenAPI_consumer_nf_information_copy(OpenAPI_consumer_nf_information_t *dst, OpenAPI_consumer_nf_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_consumer_nf_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_consumer_nf_information_convertToJSON() failed");
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

    OpenAPI_consumer_nf_information_free(dst);
    dst = OpenAPI_consumer_nf_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

