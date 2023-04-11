
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_data_for_app_ext.h"

OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_create(
    char *application_id,
    OpenAPI_list_t *pfds,
    char *caching_time,
    char *supp_feat,
    OpenAPI_list_t *reset_ids,
    bool is_allowed_delay,
    int allowed_delay
)
{
    OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext_local_var = ogs_malloc(sizeof(OpenAPI_pfd_data_for_app_ext_t));
    ogs_assert(pfd_data_for_app_ext_local_var);

    pfd_data_for_app_ext_local_var->application_id = application_id;
    pfd_data_for_app_ext_local_var->pfds = pfds;
    pfd_data_for_app_ext_local_var->caching_time = caching_time;
    pfd_data_for_app_ext_local_var->supp_feat = supp_feat;
    pfd_data_for_app_ext_local_var->reset_ids = reset_ids;
    pfd_data_for_app_ext_local_var->is_allowed_delay = is_allowed_delay;
    pfd_data_for_app_ext_local_var->allowed_delay = allowed_delay;

    return pfd_data_for_app_ext_local_var;
}

void OpenAPI_pfd_data_for_app_ext_free(OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pfd_data_for_app_ext) {
        return;
    }
    if (pfd_data_for_app_ext->application_id) {
        ogs_free(pfd_data_for_app_ext->application_id);
        pfd_data_for_app_ext->application_id = NULL;
    }
    if (pfd_data_for_app_ext->pfds) {
        OpenAPI_list_for_each(pfd_data_for_app_ext->pfds, node) {
            OpenAPI_pfd_content_free(node->data);
        }
        OpenAPI_list_free(pfd_data_for_app_ext->pfds);
        pfd_data_for_app_ext->pfds = NULL;
    }
    if (pfd_data_for_app_ext->caching_time) {
        ogs_free(pfd_data_for_app_ext->caching_time);
        pfd_data_for_app_ext->caching_time = NULL;
    }
    if (pfd_data_for_app_ext->supp_feat) {
        ogs_free(pfd_data_for_app_ext->supp_feat);
        pfd_data_for_app_ext->supp_feat = NULL;
    }
    if (pfd_data_for_app_ext->reset_ids) {
        OpenAPI_list_for_each(pfd_data_for_app_ext->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pfd_data_for_app_ext->reset_ids);
        pfd_data_for_app_ext->reset_ids = NULL;
    }
    ogs_free(pfd_data_for_app_ext);
}

cJSON *OpenAPI_pfd_data_for_app_ext_convertToJSON(OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pfd_data_for_app_ext == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [PfdDataForAppExt]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pfd_data_for_app_ext->application_id) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [application_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "applicationId", pfd_data_for_app_ext->application_id) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [application_id]");
        goto end;
    }

    if (!pfd_data_for_app_ext->pfds) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [pfds]");
        return NULL;
    }
    cJSON *pfdsList = cJSON_AddArrayToObject(item, "pfds");
    if (pfdsList == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [pfds]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_data_for_app_ext->pfds, node) {
        cJSON *itemLocal = OpenAPI_pfd_content_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [pfds]");
            goto end;
        }
        cJSON_AddItemToArray(pfdsList, itemLocal);
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

    if (pfd_data_for_app_ext->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_data_for_app_ext->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (pfd_data_for_app_ext->is_allowed_delay) {
    if (cJSON_AddNumberToObject(item, "allowedDelay", pfd_data_for_app_ext->allowed_delay) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_convertToJSON() failed [allowed_delay]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pfd_data_for_app_ext_t *OpenAPI_pfd_data_for_app_ext_parseFromJSON(cJSON *pfd_data_for_app_extJSON)
{
    OpenAPI_pfd_data_for_app_ext_t *pfd_data_for_app_ext_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *application_id = NULL;
    cJSON *pfds = NULL;
    OpenAPI_list_t *pfdsList = NULL;
    cJSON *caching_time = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *allowed_delay = NULL;
    application_id = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "applicationId");
    if (!application_id) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [application_id]");
        goto end;
    }
    if (!cJSON_IsString(application_id)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [application_id]");
        goto end;
    }

    pfds = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "pfds");
    if (!pfds) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [pfds]");
        goto end;
    }
        cJSON *pfds_local = NULL;
        if (!cJSON_IsArray(pfds)) {
            ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [pfds]");
            goto end;
        }

        pfdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pfds_local, pfds) {
            if (!cJSON_IsObject(pfds_local)) {
                ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [pfds]");
                goto end;
            }
            OpenAPI_pfd_content_t *pfdsItem = OpenAPI_pfd_content_parseFromJSON(pfds_local);
            if (!pfdsItem) {
                ogs_error("No pfdsItem");
                goto end;
            }
            OpenAPI_list_add(pfdsList, pfdsItem);
        }

    caching_time = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "cachingTime");
    if (caching_time) {
    if (!cJSON_IsString(caching_time) && !cJSON_IsNull(caching_time)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [caching_time]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    allowed_delay = cJSON_GetObjectItemCaseSensitive(pfd_data_for_app_extJSON, "allowedDelay");
    if (allowed_delay) {
    if (!cJSON_IsNumber(allowed_delay)) {
        ogs_error("OpenAPI_pfd_data_for_app_ext_parseFromJSON() failed [allowed_delay]");
        goto end;
    }
    }

    pfd_data_for_app_ext_local_var = OpenAPI_pfd_data_for_app_ext_create (
        ogs_strdup(application_id->valuestring),
        pfdsList,
        caching_time && !cJSON_IsNull(caching_time) ? ogs_strdup(caching_time->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL,
        allowed_delay ? true : false,
        allowed_delay ? allowed_delay->valuedouble : 0
    );

    return pfd_data_for_app_ext_local_var;
end:
    if (pfdsList) {
        OpenAPI_list_for_each(pfdsList, node) {
            OpenAPI_pfd_content_free(node->data);
        }
        OpenAPI_list_free(pfdsList);
        pfdsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

