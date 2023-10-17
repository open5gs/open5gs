
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smcce_ue_list.h"

OpenAPI_smcce_ue_list_t *OpenAPI_smcce_ue_list_create(
    OpenAPI_list_t *high_level,
    OpenAPI_list_t *medium_level,
    OpenAPI_list_t *low_level
)
{
    OpenAPI_smcce_ue_list_t *smcce_ue_list_local_var = ogs_malloc(sizeof(OpenAPI_smcce_ue_list_t));
    ogs_assert(smcce_ue_list_local_var);

    smcce_ue_list_local_var->high_level = high_level;
    smcce_ue_list_local_var->medium_level = medium_level;
    smcce_ue_list_local_var->low_level = low_level;

    return smcce_ue_list_local_var;
}

void OpenAPI_smcce_ue_list_free(OpenAPI_smcce_ue_list_t *smcce_ue_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smcce_ue_list) {
        return;
    }
    if (smcce_ue_list->high_level) {
        OpenAPI_list_for_each(smcce_ue_list->high_level, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smcce_ue_list->high_level);
        smcce_ue_list->high_level = NULL;
    }
    if (smcce_ue_list->medium_level) {
        OpenAPI_list_for_each(smcce_ue_list->medium_level, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smcce_ue_list->medium_level);
        smcce_ue_list->medium_level = NULL;
    }
    if (smcce_ue_list->low_level) {
        OpenAPI_list_for_each(smcce_ue_list->low_level, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smcce_ue_list->low_level);
        smcce_ue_list->low_level = NULL;
    }
    ogs_free(smcce_ue_list);
}

cJSON *OpenAPI_smcce_ue_list_convertToJSON(OpenAPI_smcce_ue_list_t *smcce_ue_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smcce_ue_list == NULL) {
        ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [SmcceUeList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smcce_ue_list->high_level) {
    cJSON *high_levelList = cJSON_AddArrayToObject(item, "highLevel");
    if (high_levelList == NULL) {
        ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [high_level]");
        goto end;
    }
    OpenAPI_list_for_each(smcce_ue_list->high_level, node) {
        if (cJSON_AddStringToObject(high_levelList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [high_level]");
            goto end;
        }
    }
    }

    if (smcce_ue_list->medium_level) {
    cJSON *medium_levelList = cJSON_AddArrayToObject(item, "mediumLevel");
    if (medium_levelList == NULL) {
        ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [medium_level]");
        goto end;
    }
    OpenAPI_list_for_each(smcce_ue_list->medium_level, node) {
        if (cJSON_AddStringToObject(medium_levelList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [medium_level]");
            goto end;
        }
    }
    }

    if (smcce_ue_list->low_level) {
    cJSON *low_levelList = cJSON_AddArrayToObject(item, "lowLevel");
    if (low_levelList == NULL) {
        ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [low_level]");
        goto end;
    }
    OpenAPI_list_for_each(smcce_ue_list->low_level, node) {
        if (cJSON_AddStringToObject(low_levelList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed [low_level]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_smcce_ue_list_t *OpenAPI_smcce_ue_list_parseFromJSON(cJSON *smcce_ue_listJSON)
{
    OpenAPI_smcce_ue_list_t *smcce_ue_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *high_level = NULL;
    OpenAPI_list_t *high_levelList = NULL;
    cJSON *medium_level = NULL;
    OpenAPI_list_t *medium_levelList = NULL;
    cJSON *low_level = NULL;
    OpenAPI_list_t *low_levelList = NULL;
    high_level = cJSON_GetObjectItemCaseSensitive(smcce_ue_listJSON, "highLevel");
    if (high_level) {
        cJSON *high_level_local = NULL;
        if (!cJSON_IsArray(high_level)) {
            ogs_error("OpenAPI_smcce_ue_list_parseFromJSON() failed [high_level]");
            goto end;
        }

        high_levelList = OpenAPI_list_create();

        cJSON_ArrayForEach(high_level_local, high_level) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(high_level_local)) {
                ogs_error("OpenAPI_smcce_ue_list_parseFromJSON() failed [high_level]");
                goto end;
            }
            OpenAPI_list_add(high_levelList, ogs_strdup(high_level_local->valuestring));
        }
    }

    medium_level = cJSON_GetObjectItemCaseSensitive(smcce_ue_listJSON, "mediumLevel");
    if (medium_level) {
        cJSON *medium_level_local = NULL;
        if (!cJSON_IsArray(medium_level)) {
            ogs_error("OpenAPI_smcce_ue_list_parseFromJSON() failed [medium_level]");
            goto end;
        }

        medium_levelList = OpenAPI_list_create();

        cJSON_ArrayForEach(medium_level_local, medium_level) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(medium_level_local)) {
                ogs_error("OpenAPI_smcce_ue_list_parseFromJSON() failed [medium_level]");
                goto end;
            }
            OpenAPI_list_add(medium_levelList, ogs_strdup(medium_level_local->valuestring));
        }
    }

    low_level = cJSON_GetObjectItemCaseSensitive(smcce_ue_listJSON, "lowLevel");
    if (low_level) {
        cJSON *low_level_local = NULL;
        if (!cJSON_IsArray(low_level)) {
            ogs_error("OpenAPI_smcce_ue_list_parseFromJSON() failed [low_level]");
            goto end;
        }

        low_levelList = OpenAPI_list_create();

        cJSON_ArrayForEach(low_level_local, low_level) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(low_level_local)) {
                ogs_error("OpenAPI_smcce_ue_list_parseFromJSON() failed [low_level]");
                goto end;
            }
            OpenAPI_list_add(low_levelList, ogs_strdup(low_level_local->valuestring));
        }
    }

    smcce_ue_list_local_var = OpenAPI_smcce_ue_list_create (
        high_level ? high_levelList : NULL,
        medium_level ? medium_levelList : NULL,
        low_level ? low_levelList : NULL
    );

    return smcce_ue_list_local_var;
end:
    if (high_levelList) {
        OpenAPI_list_for_each(high_levelList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(high_levelList);
        high_levelList = NULL;
    }
    if (medium_levelList) {
        OpenAPI_list_for_each(medium_levelList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(medium_levelList);
        medium_levelList = NULL;
    }
    if (low_levelList) {
        OpenAPI_list_for_each(low_levelList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(low_levelList);
        low_levelList = NULL;
    }
    return NULL;
}

OpenAPI_smcce_ue_list_t *OpenAPI_smcce_ue_list_copy(OpenAPI_smcce_ue_list_t *dst, OpenAPI_smcce_ue_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smcce_ue_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smcce_ue_list_convertToJSON() failed");
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

    OpenAPI_smcce_ue_list_free(dst);
    dst = OpenAPI_smcce_ue_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

