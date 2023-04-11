
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_analytics_context_descriptor.h"

OpenAPI_ue_analytics_context_descriptor_t *OpenAPI_ue_analytics_context_descriptor_create(
    char *supi,
    OpenAPI_list_t *ana_types
)
{
    OpenAPI_ue_analytics_context_descriptor_t *ue_analytics_context_descriptor_local_var = ogs_malloc(sizeof(OpenAPI_ue_analytics_context_descriptor_t));
    ogs_assert(ue_analytics_context_descriptor_local_var);

    ue_analytics_context_descriptor_local_var->supi = supi;
    ue_analytics_context_descriptor_local_var->ana_types = ana_types;

    return ue_analytics_context_descriptor_local_var;
}

void OpenAPI_ue_analytics_context_descriptor_free(OpenAPI_ue_analytics_context_descriptor_t *ue_analytics_context_descriptor)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_analytics_context_descriptor) {
        return;
    }
    if (ue_analytics_context_descriptor->supi) {
        ogs_free(ue_analytics_context_descriptor->supi);
        ue_analytics_context_descriptor->supi = NULL;
    }
    if (ue_analytics_context_descriptor->ana_types) {
        OpenAPI_list_for_each(ue_analytics_context_descriptor->ana_types, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(ue_analytics_context_descriptor->ana_types);
        ue_analytics_context_descriptor->ana_types = NULL;
    }
    ogs_free(ue_analytics_context_descriptor);
}

cJSON *OpenAPI_ue_analytics_context_descriptor_convertToJSON(OpenAPI_ue_analytics_context_descriptor_t *ue_analytics_context_descriptor)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_analytics_context_descriptor == NULL) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed [UeAnalyticsContextDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_analytics_context_descriptor->supi) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", ue_analytics_context_descriptor->supi) == NULL) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed [supi]");
        goto end;
    }

    if (!ue_analytics_context_descriptor->ana_types) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed [ana_types]");
        return NULL;
    }
    cJSON *ana_typesList = cJSON_AddArrayToObject(item, "anaTypes");
    if (ana_typesList == NULL) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed [ana_types]");
        goto end;
    }
    OpenAPI_list_for_each(ue_analytics_context_descriptor->ana_types, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed [ana_types]");
            goto end;
        }
        cJSON_AddItemToArray(ana_typesList, itemLocal);
    }

end:
    return item;
}

OpenAPI_ue_analytics_context_descriptor_t *OpenAPI_ue_analytics_context_descriptor_parseFromJSON(cJSON *ue_analytics_context_descriptorJSON)
{
    OpenAPI_ue_analytics_context_descriptor_t *ue_analytics_context_descriptor_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *ana_types = NULL;
    OpenAPI_list_t *ana_typesList = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(ue_analytics_context_descriptorJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_parseFromJSON() failed [supi]");
        goto end;
    }

    ana_types = cJSON_GetObjectItemCaseSensitive(ue_analytics_context_descriptorJSON, "anaTypes");
    if (!ana_types) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_parseFromJSON() failed [ana_types]");
        goto end;
    }
        cJSON *ana_types_local = NULL;
        if (!cJSON_IsArray(ana_types)) {
            ogs_error("OpenAPI_ue_analytics_context_descriptor_parseFromJSON() failed [ana_types]");
            goto end;
        }

        ana_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ana_types_local, ana_types) {
            if (!cJSON_IsObject(ana_types_local)) {
                ogs_error("OpenAPI_ue_analytics_context_descriptor_parseFromJSON() failed [ana_types]");
                goto end;
            }
            OpenAPI_nwdaf_event_t *ana_typesItem = OpenAPI_nwdaf_event_parseFromJSON(ana_types_local);
            if (!ana_typesItem) {
                ogs_error("No ana_typesItem");
                goto end;
            }
            OpenAPI_list_add(ana_typesList, ana_typesItem);
        }

    ue_analytics_context_descriptor_local_var = OpenAPI_ue_analytics_context_descriptor_create (
        ogs_strdup(supi->valuestring),
        ana_typesList
    );

    return ue_analytics_context_descriptor_local_var;
end:
    if (ana_typesList) {
        OpenAPI_list_for_each(ana_typesList, node) {
            OpenAPI_nwdaf_event_free(node->data);
        }
        OpenAPI_list_free(ana_typesList);
        ana_typesList = NULL;
    }
    return NULL;
}

OpenAPI_ue_analytics_context_descriptor_t *OpenAPI_ue_analytics_context_descriptor_copy(OpenAPI_ue_analytics_context_descriptor_t *dst, OpenAPI_ue_analytics_context_descriptor_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_analytics_context_descriptor_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_analytics_context_descriptor_convertToJSON() failed");
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

    OpenAPI_ue_analytics_context_descriptor_free(dst);
    dst = OpenAPI_ue_analytics_context_descriptor_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

