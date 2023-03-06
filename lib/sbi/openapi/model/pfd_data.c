
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_data.h"

OpenAPI_pfd_data_t *OpenAPI_pfd_data_create(
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *af_ids
)
{
    OpenAPI_pfd_data_t *pfd_data_local_var = ogs_malloc(sizeof(OpenAPI_pfd_data_t));
    ogs_assert(pfd_data_local_var);

    pfd_data_local_var->app_ids = app_ids;
    pfd_data_local_var->af_ids = af_ids;

    return pfd_data_local_var;
}

void OpenAPI_pfd_data_free(OpenAPI_pfd_data_t *pfd_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pfd_data) {
        return;
    }
    if (pfd_data->app_ids) {
        OpenAPI_list_for_each(pfd_data->app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pfd_data->app_ids);
        pfd_data->app_ids = NULL;
    }
    if (pfd_data->af_ids) {
        OpenAPI_list_for_each(pfd_data->af_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pfd_data->af_ids);
        pfd_data->af_ids = NULL;
    }
    ogs_free(pfd_data);
}

cJSON *OpenAPI_pfd_data_convertToJSON(OpenAPI_pfd_data_t *pfd_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pfd_data == NULL) {
        ogs_error("OpenAPI_pfd_data_convertToJSON() failed [PfdData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pfd_data->app_ids) {
    cJSON *app_idsList = cJSON_AddArrayToObject(item, "appIds");
    if (app_idsList == NULL) {
        ogs_error("OpenAPI_pfd_data_convertToJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_data->app_ids, node) {
        if (cJSON_AddStringToObject(app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pfd_data_convertToJSON() failed [app_ids]");
            goto end;
        }
    }
    }

    if (pfd_data->af_ids) {
    cJSON *af_idsList = cJSON_AddArrayToObject(item, "afIds");
    if (af_idsList == NULL) {
        ogs_error("OpenAPI_pfd_data_convertToJSON() failed [af_ids]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_data->af_ids, node) {
        if (cJSON_AddStringToObject(af_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pfd_data_convertToJSON() failed [af_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pfd_data_t *OpenAPI_pfd_data_parseFromJSON(cJSON *pfd_dataJSON)
{
    OpenAPI_pfd_data_t *pfd_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    cJSON *af_ids = NULL;
    OpenAPI_list_t *af_idsList = NULL;
    app_ids = cJSON_GetObjectItemCaseSensitive(pfd_dataJSON, "appIds");
    if (app_ids) {
        cJSON *app_ids_local = NULL;
        if (!cJSON_IsArray(app_ids)) {
            ogs_error("OpenAPI_pfd_data_parseFromJSON() failed [app_ids]");
            goto end;
        }

        app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_ids_local, app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(app_ids_local)) {
                ogs_error("OpenAPI_pfd_data_parseFromJSON() failed [app_ids]");
                goto end;
            }
            OpenAPI_list_add(app_idsList, ogs_strdup(app_ids_local->valuestring));
        }
    }

    af_ids = cJSON_GetObjectItemCaseSensitive(pfd_dataJSON, "afIds");
    if (af_ids) {
        cJSON *af_ids_local = NULL;
        if (!cJSON_IsArray(af_ids)) {
            ogs_error("OpenAPI_pfd_data_parseFromJSON() failed [af_ids]");
            goto end;
        }

        af_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_ids_local, af_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(af_ids_local)) {
                ogs_error("OpenAPI_pfd_data_parseFromJSON() failed [af_ids]");
                goto end;
            }
            OpenAPI_list_add(af_idsList, ogs_strdup(af_ids_local->valuestring));
        }
    }

    pfd_data_local_var = OpenAPI_pfd_data_create (
        app_ids ? app_idsList : NULL,
        af_ids ? af_idsList : NULL
    );

    return pfd_data_local_var;
end:
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    if (af_idsList) {
        OpenAPI_list_for_each(af_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_idsList);
        af_idsList = NULL;
    }
    return NULL;
}

OpenAPI_pfd_data_t *OpenAPI_pfd_data_copy(OpenAPI_pfd_data_t *dst, OpenAPI_pfd_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_data_convertToJSON() failed");
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

    OpenAPI_pfd_data_free(dst);
    dst = OpenAPI_pfd_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

