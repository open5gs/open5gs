
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_id.h"

OpenAPI_ue_id_t *OpenAPI_ue_id_create(
    char *supi,
    OpenAPI_list_t *gpsi_list
    )
{
    OpenAPI_ue_id_t *ue_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_id_t));
    if (!ue_id_local_var) {
        return NULL;
    }
    ue_id_local_var->supi = supi;
    ue_id_local_var->gpsi_list = gpsi_list;

    return ue_id_local_var;
}

void OpenAPI_ue_id_free(OpenAPI_ue_id_t *ue_id)
{
    if (NULL == ue_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_id->supi);
    OpenAPI_list_for_each(ue_id->gpsi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_id->gpsi_list);
    ogs_free(ue_id);
}

cJSON *OpenAPI_ue_id_convertToJSON(OpenAPI_ue_id_t *ue_id)
{
    cJSON *item = NULL;

    if (ue_id == NULL) {
        ogs_error("OpenAPI_ue_id_convertToJSON() failed [UeId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_id->supi) {
        ogs_error("OpenAPI_ue_id_convertToJSON() failed [supi]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "supi", ue_id->supi) == NULL) {
        ogs_error("OpenAPI_ue_id_convertToJSON() failed [supi]");
        goto end;
    }

    if (ue_id->gpsi_list) {
        cJSON *gpsi_list = cJSON_AddArrayToObject(item, "gpsiList");
        if (gpsi_list == NULL) {
            ogs_error("OpenAPI_ue_id_convertToJSON() failed [gpsi_list]");
            goto end;
        }

        OpenAPI_lnode_t *gpsi_list_node;
        OpenAPI_list_for_each(ue_id->gpsi_list, gpsi_list_node)  {
            if (cJSON_AddStringToObject(gpsi_list, "", (char*)gpsi_list_node->data) == NULL) {
                ogs_error("OpenAPI_ue_id_convertToJSON() failed [gpsi_list]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_ue_id_t *OpenAPI_ue_id_parseFromJSON(cJSON *ue_idJSON)
{
    OpenAPI_ue_id_t *ue_id_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(ue_idJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_ue_id_parseFromJSON() failed [supi]");
        goto end;
    }


    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_ue_id_parseFromJSON() failed [supi]");
        goto end;
    }

    cJSON *gpsi_list = cJSON_GetObjectItemCaseSensitive(ue_idJSON, "gpsiList");

    OpenAPI_list_t *gpsi_listList;
    if (gpsi_list) {
        cJSON *gpsi_list_local;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_ue_id_parseFromJSON() failed [gpsi_list]");
            goto end;
        }
        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_ue_id_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }
    }

    ue_id_local_var = OpenAPI_ue_id_create (
        ogs_strdup(supi->valuestring),
        gpsi_list ? gpsi_listList : NULL
        );

    return ue_id_local_var;
end:
    return NULL;
}

OpenAPI_ue_id_t *OpenAPI_ue_id_copy(OpenAPI_ue_id_t *dst, OpenAPI_ue_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_id_convertToJSON() failed");
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

    OpenAPI_ue_id_free(dst);
    dst = OpenAPI_ue_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

