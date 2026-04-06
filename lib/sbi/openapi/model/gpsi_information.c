
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gpsi_information.h"

OpenAPI_gpsi_information_t *OpenAPI_gpsi_information_create(
    OpenAPI_list_t *gpsi_list
)
{
    OpenAPI_gpsi_information_t *gpsi_information_local_var = ogs_malloc(sizeof(OpenAPI_gpsi_information_t));
    ogs_assert(gpsi_information_local_var);

    gpsi_information_local_var->gpsi_list = gpsi_list;

    return gpsi_information_local_var;
}

void OpenAPI_gpsi_information_free(OpenAPI_gpsi_information_t *gpsi_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gpsi_information) {
        return;
    }
    if (gpsi_information->gpsi_list) {
        OpenAPI_list_for_each(gpsi_information->gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsi_information->gpsi_list);
        gpsi_information->gpsi_list = NULL;
    }
    ogs_free(gpsi_information);
}

cJSON *OpenAPI_gpsi_information_convertToJSON(OpenAPI_gpsi_information_t *gpsi_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gpsi_information == NULL) {
        ogs_error("OpenAPI_gpsi_information_convertToJSON() failed [GpsiInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!gpsi_information->gpsi_list) {
        ogs_error("OpenAPI_gpsi_information_convertToJSON() failed [gpsi_list]");
        return NULL;
    }
    cJSON *gpsi_listList = cJSON_AddArrayToObject(item, "gpsiList");
    if (gpsi_listList == NULL) {
        ogs_error("OpenAPI_gpsi_information_convertToJSON() failed [gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(gpsi_information->gpsi_list, node) {
        if (cJSON_AddStringToObject(gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_gpsi_information_convertToJSON() failed [gpsi_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_gpsi_information_t *OpenAPI_gpsi_information_parseFromJSON(cJSON *gpsi_informationJSON)
{
    OpenAPI_gpsi_information_t *gpsi_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *gpsi_list = NULL;
    OpenAPI_list_t *gpsi_listList = NULL;
    gpsi_list = cJSON_GetObjectItemCaseSensitive(gpsi_informationJSON, "gpsiList");
    if (!gpsi_list) {
        ogs_error("OpenAPI_gpsi_information_parseFromJSON() failed [gpsi_list]");
        goto end;
    }
        cJSON *gpsi_list_local = NULL;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_gpsi_information_parseFromJSON() failed [gpsi_list]");
            goto end;
        }

        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_gpsi_information_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }

    gpsi_information_local_var = OpenAPI_gpsi_information_create (
        gpsi_listList
    );

    return gpsi_information_local_var;
end:
    if (gpsi_listList) {
        OpenAPI_list_for_each(gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsi_listList);
        gpsi_listList = NULL;
    }
    return NULL;
}

OpenAPI_gpsi_information_t *OpenAPI_gpsi_information_copy(OpenAPI_gpsi_information_t *dst, OpenAPI_gpsi_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gpsi_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gpsi_information_convertToJSON() failed");
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

    OpenAPI_gpsi_information_free(dst);
    dst = OpenAPI_gpsi_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

