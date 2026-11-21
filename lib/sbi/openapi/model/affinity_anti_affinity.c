
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "affinity_anti_affinity.h"

OpenAPI_affinity_anti_affinity_t *OpenAPI_affinity_anti_affinity_create(
    OpenAPI_list_t *affinity_eas,
    OpenAPI_list_t *anti_affinity_eas
)
{
    OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity_local_var = ogs_malloc(sizeof(OpenAPI_affinity_anti_affinity_t));
    ogs_assert(affinity_anti_affinity_local_var);

    affinity_anti_affinity_local_var->affinity_eas = affinity_eas;
    affinity_anti_affinity_local_var->anti_affinity_eas = anti_affinity_eas;

    return affinity_anti_affinity_local_var;
}

void OpenAPI_affinity_anti_affinity_free(OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == affinity_anti_affinity) {
        return;
    }
    if (affinity_anti_affinity->affinity_eas) {
        OpenAPI_list_for_each(affinity_anti_affinity->affinity_eas, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(affinity_anti_affinity->affinity_eas);
        affinity_anti_affinity->affinity_eas = NULL;
    }
    if (affinity_anti_affinity->anti_affinity_eas) {
        OpenAPI_list_for_each(affinity_anti_affinity->anti_affinity_eas, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(affinity_anti_affinity->anti_affinity_eas);
        affinity_anti_affinity->anti_affinity_eas = NULL;
    }
    ogs_free(affinity_anti_affinity);
}

cJSON *OpenAPI_affinity_anti_affinity_convertToJSON(OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (affinity_anti_affinity == NULL) {
        ogs_error("OpenAPI_affinity_anti_affinity_convertToJSON() failed [AffinityAntiAffinity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (affinity_anti_affinity->affinity_eas) {
    cJSON *affinity_easList = cJSON_AddArrayToObject(item, "affinityEAS");
    if (affinity_easList == NULL) {
        ogs_error("OpenAPI_affinity_anti_affinity_convertToJSON() failed [affinity_eas]");
        goto end;
    }
    OpenAPI_list_for_each(affinity_anti_affinity->affinity_eas, node) {
        if (cJSON_AddStringToObject(affinity_easList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_affinity_anti_affinity_convertToJSON() failed [affinity_eas]");
            goto end;
        }
    }
    }

    if (affinity_anti_affinity->anti_affinity_eas) {
    cJSON *anti_affinity_easList = cJSON_AddArrayToObject(item, "antiAffinityEAS");
    if (anti_affinity_easList == NULL) {
        ogs_error("OpenAPI_affinity_anti_affinity_convertToJSON() failed [anti_affinity_eas]");
        goto end;
    }
    OpenAPI_list_for_each(affinity_anti_affinity->anti_affinity_eas, node) {
        if (cJSON_AddStringToObject(anti_affinity_easList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_affinity_anti_affinity_convertToJSON() failed [anti_affinity_eas]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_affinity_anti_affinity_t *OpenAPI_affinity_anti_affinity_parseFromJSON(cJSON *affinity_anti_affinityJSON)
{
    OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *affinity_eas = NULL;
    OpenAPI_list_t *affinity_easList = NULL;
    cJSON *anti_affinity_eas = NULL;
    OpenAPI_list_t *anti_affinity_easList = NULL;
    affinity_eas = cJSON_GetObjectItemCaseSensitive(affinity_anti_affinityJSON, "affinityEAS");
    if (affinity_eas) {
        cJSON *affinity_eas_local = NULL;
        if (!cJSON_IsArray(affinity_eas)) {
            ogs_error("OpenAPI_affinity_anti_affinity_parseFromJSON() failed [affinity_eas]");
            goto end;
        }

        affinity_easList = OpenAPI_list_create();

        cJSON_ArrayForEach(affinity_eas_local, affinity_eas) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(affinity_eas_local)) {
                ogs_error("OpenAPI_affinity_anti_affinity_parseFromJSON() failed [affinity_eas]");
                goto end;
            }
            OpenAPI_list_add(affinity_easList, ogs_strdup(affinity_eas_local->valuestring));
        }
    }

    anti_affinity_eas = cJSON_GetObjectItemCaseSensitive(affinity_anti_affinityJSON, "antiAffinityEAS");
    if (anti_affinity_eas) {
        cJSON *anti_affinity_eas_local = NULL;
        if (!cJSON_IsArray(anti_affinity_eas)) {
            ogs_error("OpenAPI_affinity_anti_affinity_parseFromJSON() failed [anti_affinity_eas]");
            goto end;
        }

        anti_affinity_easList = OpenAPI_list_create();

        cJSON_ArrayForEach(anti_affinity_eas_local, anti_affinity_eas) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(anti_affinity_eas_local)) {
                ogs_error("OpenAPI_affinity_anti_affinity_parseFromJSON() failed [anti_affinity_eas]");
                goto end;
            }
            OpenAPI_list_add(anti_affinity_easList, ogs_strdup(anti_affinity_eas_local->valuestring));
        }
    }

    affinity_anti_affinity_local_var = OpenAPI_affinity_anti_affinity_create (
        affinity_eas ? affinity_easList : NULL,
        anti_affinity_eas ? anti_affinity_easList : NULL
    );

    return affinity_anti_affinity_local_var;
end:
    if (affinity_easList) {
        OpenAPI_list_for_each(affinity_easList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(affinity_easList);
        affinity_easList = NULL;
    }
    if (anti_affinity_easList) {
        OpenAPI_list_for_each(anti_affinity_easList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(anti_affinity_easList);
        anti_affinity_easList = NULL;
    }
    return NULL;
}

OpenAPI_affinity_anti_affinity_t *OpenAPI_affinity_anti_affinity_copy(OpenAPI_affinity_anti_affinity_t *dst, OpenAPI_affinity_anti_affinity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_affinity_anti_affinity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_affinity_anti_affinity_convertToJSON() failed");
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

    OpenAPI_affinity_anti_affinity_free(dst);
    dst = OpenAPI_affinity_anti_affinity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

