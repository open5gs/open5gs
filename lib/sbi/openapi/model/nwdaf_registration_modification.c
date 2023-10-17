
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_registration_modification.h"

OpenAPI_nwdaf_registration_modification_t *OpenAPI_nwdaf_registration_modification_create(
    char *nwdaf_instance_id,
    char *nwdaf_set_id,
    OpenAPI_list_t *analytics_ids,
    char *supported_features
)
{
    OpenAPI_nwdaf_registration_modification_t *nwdaf_registration_modification_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_registration_modification_t));
    ogs_assert(nwdaf_registration_modification_local_var);

    nwdaf_registration_modification_local_var->nwdaf_instance_id = nwdaf_instance_id;
    nwdaf_registration_modification_local_var->nwdaf_set_id = nwdaf_set_id;
    nwdaf_registration_modification_local_var->analytics_ids = analytics_ids;
    nwdaf_registration_modification_local_var->supported_features = supported_features;

    return nwdaf_registration_modification_local_var;
}

void OpenAPI_nwdaf_registration_modification_free(OpenAPI_nwdaf_registration_modification_t *nwdaf_registration_modification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_registration_modification) {
        return;
    }
    if (nwdaf_registration_modification->nwdaf_instance_id) {
        ogs_free(nwdaf_registration_modification->nwdaf_instance_id);
        nwdaf_registration_modification->nwdaf_instance_id = NULL;
    }
    if (nwdaf_registration_modification->nwdaf_set_id) {
        ogs_free(nwdaf_registration_modification->nwdaf_set_id);
        nwdaf_registration_modification->nwdaf_set_id = NULL;
    }
    if (nwdaf_registration_modification->analytics_ids) {
        OpenAPI_list_for_each(nwdaf_registration_modification->analytics_ids, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(nwdaf_registration_modification->analytics_ids);
        nwdaf_registration_modification->analytics_ids = NULL;
    }
    if (nwdaf_registration_modification->supported_features) {
        ogs_free(nwdaf_registration_modification->supported_features);
        nwdaf_registration_modification->supported_features = NULL;
    }
    ogs_free(nwdaf_registration_modification);
}

cJSON *OpenAPI_nwdaf_registration_modification_convertToJSON(OpenAPI_nwdaf_registration_modification_t *nwdaf_registration_modification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nwdaf_registration_modification == NULL) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [NwdafRegistrationModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nwdaf_registration_modification->nwdaf_instance_id) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [nwdaf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nwdafInstanceId", nwdaf_registration_modification->nwdaf_instance_id) == NULL) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [nwdaf_instance_id]");
        goto end;
    }

    if (nwdaf_registration_modification->nwdaf_set_id) {
    if (cJSON_AddStringToObject(item, "nwdafSetId", nwdaf_registration_modification->nwdaf_set_id) == NULL) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [nwdaf_set_id]");
        goto end;
    }
    }

    if (nwdaf_registration_modification->analytics_ids) {
    cJSON *analytics_idsList = cJSON_AddArrayToObject(item, "analyticsIds");
    if (analytics_idsList == NULL) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_registration_modification->analytics_ids, node) {
        cJSON *itemLocal = OpenAPI_event_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [analytics_ids]");
            goto end;
        }
        cJSON_AddItemToArray(analytics_idsList, itemLocal);
    }
    }

    if (nwdaf_registration_modification->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", nwdaf_registration_modification->supported_features) == NULL) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nwdaf_registration_modification_t *OpenAPI_nwdaf_registration_modification_parseFromJSON(cJSON *nwdaf_registration_modificationJSON)
{
    OpenAPI_nwdaf_registration_modification_t *nwdaf_registration_modification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nwdaf_instance_id = NULL;
    cJSON *nwdaf_set_id = NULL;
    cJSON *analytics_ids = NULL;
    OpenAPI_list_t *analytics_idsList = NULL;
    cJSON *supported_features = NULL;
    nwdaf_instance_id = cJSON_GetObjectItemCaseSensitive(nwdaf_registration_modificationJSON, "nwdafInstanceId");
    if (!nwdaf_instance_id) {
        ogs_error("OpenAPI_nwdaf_registration_modification_parseFromJSON() failed [nwdaf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nwdaf_instance_id)) {
        ogs_error("OpenAPI_nwdaf_registration_modification_parseFromJSON() failed [nwdaf_instance_id]");
        goto end;
    }

    nwdaf_set_id = cJSON_GetObjectItemCaseSensitive(nwdaf_registration_modificationJSON, "nwdafSetId");
    if (nwdaf_set_id) {
    if (!cJSON_IsString(nwdaf_set_id) && !cJSON_IsNull(nwdaf_set_id)) {
        ogs_error("OpenAPI_nwdaf_registration_modification_parseFromJSON() failed [nwdaf_set_id]");
        goto end;
    }
    }

    analytics_ids = cJSON_GetObjectItemCaseSensitive(nwdaf_registration_modificationJSON, "analyticsIds");
    if (analytics_ids) {
        cJSON *analytics_ids_local = NULL;
        if (!cJSON_IsArray(analytics_ids)) {
            ogs_error("OpenAPI_nwdaf_registration_modification_parseFromJSON() failed [analytics_ids]");
            goto end;
        }

        analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_ids_local, analytics_ids) {
            if (!cJSON_IsObject(analytics_ids_local)) {
                ogs_error("OpenAPI_nwdaf_registration_modification_parseFromJSON() failed [analytics_ids]");
                goto end;
            }
            OpenAPI_event_id_t *analytics_idsItem = OpenAPI_event_id_parseFromJSON(analytics_ids_local);
            if (!analytics_idsItem) {
                ogs_error("No analytics_idsItem");
                goto end;
            }
            OpenAPI_list_add(analytics_idsList, analytics_idsItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(nwdaf_registration_modificationJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_nwdaf_registration_modification_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    nwdaf_registration_modification_local_var = OpenAPI_nwdaf_registration_modification_create (
        ogs_strdup(nwdaf_instance_id->valuestring),
        nwdaf_set_id && !cJSON_IsNull(nwdaf_set_id) ? ogs_strdup(nwdaf_set_id->valuestring) : NULL,
        analytics_ids ? analytics_idsList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return nwdaf_registration_modification_local_var;
end:
    if (analytics_idsList) {
        OpenAPI_list_for_each(analytics_idsList, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(analytics_idsList);
        analytics_idsList = NULL;
    }
    return NULL;
}

OpenAPI_nwdaf_registration_modification_t *OpenAPI_nwdaf_registration_modification_copy(OpenAPI_nwdaf_registration_modification_t *dst, OpenAPI_nwdaf_registration_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_registration_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_registration_modification_convertToJSON() failed");
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

    OpenAPI_nwdaf_registration_modification_free(dst);
    dst = OpenAPI_nwdaf_registration_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

