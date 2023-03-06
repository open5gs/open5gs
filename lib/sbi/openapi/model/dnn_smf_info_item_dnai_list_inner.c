
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_smf_info_item_dnai_list_inner.h"

OpenAPI_dnn_smf_info_item_dnai_list_inner_t *OpenAPI_dnn_smf_info_item_dnai_list_inner_create(
)
{
    OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnn_smf_info_item_dnai_list_inner_local_var = ogs_malloc(sizeof(OpenAPI_dnn_smf_info_item_dnai_list_inner_t));
    ogs_assert(dnn_smf_info_item_dnai_list_inner_local_var);


    return dnn_smf_info_item_dnai_list_inner_local_var;
}

void OpenAPI_dnn_smf_info_item_dnai_list_inner_free(OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnn_smf_info_item_dnai_list_inner)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_smf_info_item_dnai_list_inner) {
        return;
    }
    ogs_free(dnn_smf_info_item_dnai_list_inner);
}

cJSON *OpenAPI_dnn_smf_info_item_dnai_list_inner_convertToJSON(OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnn_smf_info_item_dnai_list_inner)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_smf_info_item_dnai_list_inner == NULL) {
        ogs_error("OpenAPI_dnn_smf_info_item_dnai_list_inner_convertToJSON() failed [DnnSmfInfoItem_dnaiList_inner]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dnn_smf_info_item_dnai_list_inner_t *OpenAPI_dnn_smf_info_item_dnai_list_inner_parseFromJSON(cJSON *dnn_smf_info_item_dnai_list_innerJSON)
{
    OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dnn_smf_info_item_dnai_list_inner_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    dnn_smf_info_item_dnai_list_inner_local_var = OpenAPI_dnn_smf_info_item_dnai_list_inner_create (
    );

    return dnn_smf_info_item_dnai_list_inner_local_var;
end:
    return NULL;
}

OpenAPI_dnn_smf_info_item_dnai_list_inner_t *OpenAPI_dnn_smf_info_item_dnai_list_inner_copy(OpenAPI_dnn_smf_info_item_dnai_list_inner_t *dst, OpenAPI_dnn_smf_info_item_dnai_list_inner_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_smf_info_item_dnai_list_inner_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_smf_info_item_dnai_list_inner_convertToJSON() failed");
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

    OpenAPI_dnn_smf_info_item_dnai_list_inner_free(dst);
    dst = OpenAPI_dnn_smf_info_item_dnai_list_inner_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

