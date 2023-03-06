
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sponsor_connectivity_data.h"

OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_create(
    OpenAPI_list_t *asp_ids
)
{
    OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data_local_var = ogs_malloc(sizeof(OpenAPI_sponsor_connectivity_data_t));
    ogs_assert(sponsor_connectivity_data_local_var);

    sponsor_connectivity_data_local_var->asp_ids = asp_ids;

    return sponsor_connectivity_data_local_var;
}

void OpenAPI_sponsor_connectivity_data_free(OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sponsor_connectivity_data) {
        return;
    }
    if (sponsor_connectivity_data->asp_ids) {
        OpenAPI_list_for_each(sponsor_connectivity_data->asp_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sponsor_connectivity_data->asp_ids);
        sponsor_connectivity_data->asp_ids = NULL;
    }
    ogs_free(sponsor_connectivity_data);
}

cJSON *OpenAPI_sponsor_connectivity_data_convertToJSON(OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sponsor_connectivity_data == NULL) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [SponsorConnectivityData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sponsor_connectivity_data->asp_ids) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [asp_ids]");
        return NULL;
    }
    cJSON *asp_idsList = cJSON_AddArrayToObject(item, "aspIds");
    if (asp_idsList == NULL) {
        ogs_error("OpenAPI_sponsor_connectivity_data_convertToJSON() failed [asp_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sponsor_connectivity_data->asp_ids, node) {
        if (cJSON_AddStringToObject(asp_idsList, "", (char*)node->data) == NULL) {
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *asp_ids = NULL;
    OpenAPI_list_t *asp_idsList = NULL;
    asp_ids = cJSON_GetObjectItemCaseSensitive(sponsor_connectivity_dataJSON, "aspIds");
    if (!asp_ids) {
        ogs_error("OpenAPI_sponsor_connectivity_data_parseFromJSON() failed [asp_ids]");
        goto end;
    }
        cJSON *asp_ids_local = NULL;
        if (!cJSON_IsArray(asp_ids)) {
            ogs_error("OpenAPI_sponsor_connectivity_data_parseFromJSON() failed [asp_ids]");
            goto end;
        }

        asp_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(asp_ids_local, asp_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
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
    if (asp_idsList) {
        OpenAPI_list_for_each(asp_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(asp_idsList);
        asp_idsList = NULL;
    }
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

