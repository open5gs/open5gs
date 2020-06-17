
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sponsor_connectivity_data.h"

OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_create(
    OpenAPI_list_t *asp_ids
    )
{
    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_sponsor_connectivity_data_t));
    if (!sponsor_connectivity_data_local_var) {
        return NULL;
    }
    sponsor_connectivity_data_local_var->asp_ids = asp_ids;

    return sponsor_connectivity_data_local_var;
}

void OpenAPI_sponsor_connectivity_data_free(OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data)
{
    if (NULL == sponsor_connectivity_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(sponsor_connectivity_data->asp_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sponsor_connectivity_data->asp_ids);
    ogs_free(sponsor_connectivity_data);
}

cJSON *OpenAPI_sponsor_connectivity_data_convertToJSON(OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data)
{
    cJSON *item = NULL;

    if (sponsor_connectivity_data == NULL) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [SponsorConnectivityData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sponsor_connectivity_data->asp_ids) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [asp_ids]");
        goto end;
    }
    cJSON *asp_ids = cJSON_AddArrayToObject(item, "aspIds");
    if (asp_ids == NULL) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [asp_ids]");
        goto end;
    }

    OpenAPI_lnode_t *asp_ids_node;
    OpenAPI_list_for_each(sponsor_connectivity_data->asp_ids, asp_ids_node)  {
        if (cJSON_AddStringToObject(asp_ids, "", (char*)asp_ids_node->data) == NULL) {
            ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [asp_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_parseFromJSON(cJSON *sponsor_connectivity_dataJSON)
{
    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data_local_var = NULL;
    cJSON *asp_ids = cJSON_GetObjectItemCaseSensitive(sponsor_connectivity_dataJSON, "aspIds");
    if (!asp_ids) {
        ogs_error("OpenAPI_sponsor_connectivity_data_parseFromJSON() failed [asp_ids]");
        goto end;
    }

    OpenAPI_list_t *asp_idsList;

    cJSON *asp_ids_local;
    if (!cJSON_IsArray(asp_ids)) {
        ogs_error("OpenAPI_sponsor_connectivity_data_parseFromJSON() failed [asp_ids]");
        goto end;
    }
    asp_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(asp_ids_local, asp_ids) {
        if (!cJSON_IsString(asp_ids_local)) {
            ogs_error("OpenAPI_sponsor_connectivity_data_parseFromJSON() failed [asp_ids]");
            goto end;
        }
        OpenAPI_list_add(asp_idsList, ogs_strdup(asp_ids_local->valuestring));
    }

    sponsor_connectivity_data_local_var = OpenAPI_sponsor_connectivity_data_create (
        asp_idsList
        );

    return sponsor_connectivity_data_local_var;
end:
    return NULL;
}

OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_copy(OpenAPI_sponsor_connectivity_data_t *dst, OpenAPI_sponsor_connectivity_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sponsor_connectivity_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed");
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

    OpenAPI_sponsor_connectivity_data_free(dst);
    dst = OpenAPI_sponsor_connectivity_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

