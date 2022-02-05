
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_data_for_app_ext.h"

OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_create(
    char *application_id,
    OpenAPI_list_t *pfds,
    char *caching_time,
    char *supp_feat
)
{
    OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext_local_var = ogs_malloc(sizeof(OpenAPI_pfd_data_for_app_ext_t));
    ogs_assert(pfd_data_for_app_ext_local_var);

    pfd_data_for_app_ext_local_var->application_id = application_id;
    pfd_data_for_app_ext_local_var->pfds = pfds;
    pfd_data_for_app_ext_local_var->caching_time = caching_time;
    pfd_data_for_app_ext_local_var->supp_feat = supp_feat;

    return pfd_data_for_app_ext_local_var;
}

void OpenAPI_pfd_data_for_app_ext_free(OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext)
{
    if (NULL == pfd_data_for_app_ext) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pfd_data_for_app_ext->application_id);
    OpenAPI_list_for_each(pfd_data_for_app_ext->pfds, node) {
        OpenAPI_pfd_content_free(node->data);
    }
    OpenAPI_list_free(pfd_data_for_app_ext->pfds);
    ogs_free(pfd_data_for_app_ext->caching_time);
    ogs_free(pfd_data_for_app_ext->supp_feat);
    ogs_free(pfd_data_for_app_ext);
}

cJSON *OpenAPI_pfd_data_for_app_ext_convertToJSON(OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext)
{
    cJSON *item = NULL;

    if (pfd_data_for_app_ext == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [PfdDataForAppExt]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "applicationId", pfd_data_for_app_ext->application_id) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [application_id]");
        goto end;
    }

    cJSON *pfdsList = cJSON_AddArrayToObject(item, "pfds");
    if (pfdsList == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [pfds]");
        goto end;
    }

    OpenAPI_lnode_t *pfds_node;
    if (pfd_data_for_app_ext->pfds) {
        OpenAPI_list_for_each(pfd_data_for_app_ext->pfds, pfds_node) {
            cJSON *itemLocal = OpenAPI_pfd_content_convertToJSON(pfds_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [pfds]");
                goto end;
            }
            cJSON_AddItemToArray(pfdsList, itemLocal);
        }
    }

    if (pfd_data_for_app_ext->caching_time) {
    if (cJSON_AddStringToObject(item, "cachingTime", pfd_data_for_app_ext->caching_time) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [caching_time]");
        goto end;
    }
    }

    if (pfd_data_for_app_ext->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pfd_data_for_app_ext->supp_feat) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_parseFromJSON(cJSON *pfd_data_for_app_extJSON)
{
    OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext_local_var = NULL;
    cJSON *application_id = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "applicationId");
    if (!application_id) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [application_id]");
        goto end;
    }

    if (!cJSON_IsString(application_id)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [application_id]");
        goto end;
    }

    cJSON *pfds = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "pfds");
    if (!pfds) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [pfds]");
        goto end;
    }

    OpenAPI_list_t *pfdsList;
    cJSON *pfds_local_nonprimitive;
    if (!cJSON_IsArray(pfds)){
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [pfds]");
        goto end;
    }

    pfdsList = OpenAPI_list_create();

    cJSON_ArrayForEach(pfds_local_nonprimitive, pfds ) {
        if (!cJSON_IsObject(pfds_local_nonprimitive)) {
            ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [pfds]");
            goto end;
        }
        OpenAPI_pfd_content_t *pfdsItem = OpenAPI_pfd_content_parseFromJSON(pfds_local_nonprimitive);

        if (!pfdsItem) {
            ogs_error("No pfdsItem");
            OpenAPI_list_free(pfdsList);
            goto end;
        }

        OpenAPI_list_add(pfdsList, pfdsItem);
    }

    cJSON *caching_time = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "cachingTime");

    if (caching_time) {
    if (!cJSON_IsString(caching_time)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [caching_time]");
        goto end;
    }
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "suppFeat");

    if (supp_feat) {
    if (!cJSON_IsString(supp_feat)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    pfd_data_for_app_ext_local_var = OpenAPI_pfd_data_for_app_ext_create (
        ogs_strdup(application_id->valuestring),
        pfdsList,
        caching_time ? ogs_strdup(caching_time->valuestring) : NULL,
        supp_feat ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return pfd_data_for_app_ext_local_var;
end:
    return NULL;
}

OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_copy(OpenAPI_pfd_data_for_app_ext_t *dst, OpenAPI_pfd_data_for_app_ext_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_data_for_app_ext_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed");
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

    OpenAPI_pfd_data_for_app_ext_free(dst);
    dst = OpenAPI_pfd_data_for_app_ext_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

