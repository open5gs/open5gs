
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_data_ref.h"

OpenAPI_additional_data_ref_t *OpenAPI_additional_data_ref_create(
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_list_t *additional_data_uris
)
{
    OpenAPI_additional_data_ref_t *additional_data_ref_local_var = ogs_malloc(sizeof(OpenAPI_additional_data_ref_t));
    ogs_assert(additional_data_ref_local_var);

    additional_data_ref_local_var->monitored_resource_uris = monitored_resource_uris;
    additional_data_ref_local_var->additional_data_uris = additional_data_uris;

    return additional_data_ref_local_var;
}

void OpenAPI_additional_data_ref_free(OpenAPI_additional_data_ref_t *additional_data_ref)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == additional_data_ref) {
        return;
    }
    if (additional_data_ref->monitored_resource_uris) {
        OpenAPI_list_for_each(additional_data_ref->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_data_ref->monitored_resource_uris);
        additional_data_ref->monitored_resource_uris = NULL;
    }
    if (additional_data_ref->additional_data_uris) {
        OpenAPI_list_for_each(additional_data_ref->additional_data_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_data_ref->additional_data_uris);
        additional_data_ref->additional_data_uris = NULL;
    }
    ogs_free(additional_data_ref);
}

cJSON *OpenAPI_additional_data_ref_convertToJSON(OpenAPI_additional_data_ref_t *additional_data_ref)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (additional_data_ref == NULL) {
        ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [AdditionalDataRef]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!additional_data_ref->monitored_resource_uris) {
        ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [monitored_resource_uris]");
        return NULL;
    }
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(additional_data_ref->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (!additional_data_ref->additional_data_uris) {
        ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [additional_data_uris]");
        return NULL;
    }
    cJSON *additional_data_urisList = cJSON_AddArrayToObject(item, "additionalDataUris");
    if (additional_data_urisList == NULL) {
        ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [additional_data_uris]");
        goto end;
    }
    OpenAPI_list_for_each(additional_data_ref->additional_data_uris, node) {
        if (cJSON_AddStringToObject(additional_data_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed [additional_data_uris]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_additional_data_ref_t *OpenAPI_additional_data_ref_parseFromJSON(cJSON *additional_data_refJSON)
{
    OpenAPI_additional_data_ref_t *additional_data_ref_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *monitored_resource_uris = NULL;
    OpenAPI_list_t *monitored_resource_urisList = NULL;
    cJSON *additional_data_uris = NULL;
    OpenAPI_list_t *additional_data_urisList = NULL;
    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(additional_data_refJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_additional_data_ref_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_additional_data_ref_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_additional_data_ref_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }

    additional_data_uris = cJSON_GetObjectItemCaseSensitive(additional_data_refJSON, "additionalDataUris");
    if (!additional_data_uris) {
        ogs_error("OpenAPI_additional_data_ref_parseFromJSON() failed [additional_data_uris]");
        goto end;
    }
        cJSON *additional_data_uris_local = NULL;
        if (!cJSON_IsArray(additional_data_uris)) {
            ogs_error("OpenAPI_additional_data_ref_parseFromJSON() failed [additional_data_uris]");
            goto end;
        }

        additional_data_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_data_uris_local, additional_data_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_data_uris_local)) {
                ogs_error("OpenAPI_additional_data_ref_parseFromJSON() failed [additional_data_uris]");
                goto end;
            }
            OpenAPI_list_add(additional_data_urisList, ogs_strdup(additional_data_uris_local->valuestring));
        }

    additional_data_ref_local_var = OpenAPI_additional_data_ref_create (
        monitored_resource_urisList,
        additional_data_urisList
    );

    return additional_data_ref_local_var;
end:
    if (monitored_resource_urisList) {
        OpenAPI_list_for_each(monitored_resource_urisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(monitored_resource_urisList);
        monitored_resource_urisList = NULL;
    }
    if (additional_data_urisList) {
        OpenAPI_list_for_each(additional_data_urisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_data_urisList);
        additional_data_urisList = NULL;
    }
    return NULL;
}

OpenAPI_additional_data_ref_t *OpenAPI_additional_data_ref_copy(OpenAPI_additional_data_ref_t *dst, OpenAPI_additional_data_ref_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_data_ref_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_data_ref_convertToJSON() failed");
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

    OpenAPI_additional_data_ref_free(dst);
    dst = OpenAPI_additional_data_ref_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

