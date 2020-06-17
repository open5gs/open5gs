
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "identity_data.h"

OpenAPI_identity_data_t *OpenAPI_identity_data_create(
    OpenAPI_list_t *supi_list,
    OpenAPI_list_t *gpsi_list
    )
{
    OpenAPI_identity_data_t *identity_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_identity_data_t));
    if (!identity_data_local_var) {
        return NULL;
    }
    identity_data_local_var->supi_list = supi_list;
    identity_data_local_var->gpsi_list = gpsi_list;

    return identity_data_local_var;
}

void OpenAPI_identity_data_free(OpenAPI_identity_data_t *identity_data)
{
    if (NULL == identity_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(identity_data->supi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(identity_data->supi_list);
    OpenAPI_list_for_each(identity_data->gpsi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(identity_data->gpsi_list);
    ogs_free(identity_data);
}

cJSON *OpenAPI_identity_data_convertToJSON(OpenAPI_identity_data_t *identity_data)
{
    cJSON *item = NULL;

    if (identity_data == NULL) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed [IdentityData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (identity_data->supi_list) {
        cJSON *supi_list = cJSON_AddArrayToObject(item, "supiList");
        if (supi_list == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [supi_list]");
            goto end;
        }

        OpenAPI_lnode_t *supi_list_node;
        OpenAPI_list_for_each(identity_data->supi_list, supi_list_node)  {
            if (cJSON_AddStringToObject(supi_list, "", (char*)supi_list_node->data) == NULL) {
                ogs_error("OpenAPI_identity_data_convertToJSON() failed [supi_list]");
                goto end;
            }
        }
    }

    if (identity_data->gpsi_list) {
        cJSON *gpsi_list = cJSON_AddArrayToObject(item, "gpsiList");
        if (gpsi_list == NULL) {
            ogs_error("OpenAPI_identity_data_convertToJSON() failed [gpsi_list]");
            goto end;
        }

        OpenAPI_lnode_t *gpsi_list_node;
        OpenAPI_list_for_each(identity_data->gpsi_list, gpsi_list_node)  {
            if (cJSON_AddStringToObject(gpsi_list, "", (char*)gpsi_list_node->data) == NULL) {
                ogs_error("OpenAPI_identity_data_convertToJSON() failed [gpsi_list]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_identity_data_t *OpenAPI_identity_data_parseFromJSON(cJSON *identity_dataJSON)
{
    OpenAPI_identity_data_t *identity_data_local_var = NULL;
    cJSON *supi_list = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "supiList");

    OpenAPI_list_t *supi_listList;
    if (supi_list) {
        cJSON *supi_list_local;
        if (!cJSON_IsArray(supi_list)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [supi_list]");
            goto end;
        }
        supi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_list_local, supi_list) {
            if (!cJSON_IsString(supi_list_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [supi_list]");
                goto end;
            }
            OpenAPI_list_add(supi_listList, ogs_strdup(supi_list_local->valuestring));
        }
    }

    cJSON *gpsi_list = cJSON_GetObjectItemCaseSensitive(identity_dataJSON, "gpsiList");

    OpenAPI_list_t *gpsi_listList;
    if (gpsi_list) {
        cJSON *gpsi_list_local;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_identity_data_parseFromJSON() failed [gpsi_list]");
            goto end;
        }
        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_identity_data_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }
    }

    identity_data_local_var = OpenAPI_identity_data_create (
        supi_list ? supi_listList : NULL,
        gpsi_list ? gpsi_listList : NULL
        );

    return identity_data_local_var;
end:
    return NULL;
}

OpenAPI_identity_data_t *OpenAPI_identity_data_copy(OpenAPI_identity_data_t *dst, OpenAPI_identity_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_identity_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_identity_data_convertToJSON() failed");
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

    OpenAPI_identity_data_free(dst);
    dst = OpenAPI_identity_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

