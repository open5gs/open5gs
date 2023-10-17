
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "target_ue_information.h"

OpenAPI_target_ue_information_t *OpenAPI_target_ue_information_create(
    bool is_any_ue,
    int any_ue,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *int_group_ids
)
{
    OpenAPI_target_ue_information_t *target_ue_information_local_var = ogs_malloc(sizeof(OpenAPI_target_ue_information_t));
    ogs_assert(target_ue_information_local_var);

    target_ue_information_local_var->is_any_ue = is_any_ue;
    target_ue_information_local_var->any_ue = any_ue;
    target_ue_information_local_var->supis = supis;
    target_ue_information_local_var->gpsis = gpsis;
    target_ue_information_local_var->int_group_ids = int_group_ids;

    return target_ue_information_local_var;
}

void OpenAPI_target_ue_information_free(OpenAPI_target_ue_information_t *target_ue_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == target_ue_information) {
        return;
    }
    if (target_ue_information->supis) {
        OpenAPI_list_for_each(target_ue_information->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(target_ue_information->supis);
        target_ue_information->supis = NULL;
    }
    if (target_ue_information->gpsis) {
        OpenAPI_list_for_each(target_ue_information->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(target_ue_information->gpsis);
        target_ue_information->gpsis = NULL;
    }
    if (target_ue_information->int_group_ids) {
        OpenAPI_list_for_each(target_ue_information->int_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(target_ue_information->int_group_ids);
        target_ue_information->int_group_ids = NULL;
    }
    ogs_free(target_ue_information);
}

cJSON *OpenAPI_target_ue_information_convertToJSON(OpenAPI_target_ue_information_t *target_ue_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (target_ue_information == NULL) {
        ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [TargetUeInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (target_ue_information->is_any_ue) {
    if (cJSON_AddBoolToObject(item, "anyUe", target_ue_information->any_ue) == NULL) {
        ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [any_ue]");
        goto end;
    }
    }

    if (target_ue_information->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(target_ue_information->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (target_ue_information->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(target_ue_information->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

    if (target_ue_information->int_group_ids) {
    cJSON *int_group_idsList = cJSON_AddArrayToObject(item, "intGroupIds");
    if (int_group_idsList == NULL) {
        ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [int_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(target_ue_information->int_group_ids, node) {
        if (cJSON_AddStringToObject(int_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_target_ue_information_convertToJSON() failed [int_group_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_target_ue_information_t *OpenAPI_target_ue_information_parseFromJSON(cJSON *target_ue_informationJSON)
{
    OpenAPI_target_ue_information_t *target_ue_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *any_ue = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    cJSON *int_group_ids = NULL;
    OpenAPI_list_t *int_group_idsList = NULL;
    any_ue = cJSON_GetObjectItemCaseSensitive(target_ue_informationJSON, "anyUe");
    if (any_ue) {
    if (!cJSON_IsBool(any_ue)) {
        ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [any_ue]");
        goto end;
    }
    }

    supis = cJSON_GetObjectItemCaseSensitive(target_ue_informationJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(target_ue_informationJSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    int_group_ids = cJSON_GetObjectItemCaseSensitive(target_ue_informationJSON, "intGroupIds");
    if (int_group_ids) {
        cJSON *int_group_ids_local = NULL;
        if (!cJSON_IsArray(int_group_ids)) {
            ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [int_group_ids]");
            goto end;
        }

        int_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(int_group_ids_local, int_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(int_group_ids_local)) {
                ogs_error("OpenAPI_target_ue_information_parseFromJSON() failed [int_group_ids]");
                goto end;
            }
            OpenAPI_list_add(int_group_idsList, ogs_strdup(int_group_ids_local->valuestring));
        }
    }

    target_ue_information_local_var = OpenAPI_target_ue_information_create (
        any_ue ? true : false,
        any_ue ? any_ue->valueint : 0,
        supis ? supisList : NULL,
        gpsis ? gpsisList : NULL,
        int_group_ids ? int_group_idsList : NULL
    );

    return target_ue_information_local_var;
end:
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (gpsisList) {
        OpenAPI_list_for_each(gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsisList);
        gpsisList = NULL;
    }
    if (int_group_idsList) {
        OpenAPI_list_for_each(int_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(int_group_idsList);
        int_group_idsList = NULL;
    }
    return NULL;
}

OpenAPI_target_ue_information_t *OpenAPI_target_ue_information_copy(OpenAPI_target_ue_information_t *dst, OpenAPI_target_ue_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_target_ue_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_target_ue_information_convertToJSON() failed");
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

    OpenAPI_target_ue_information_free(dst);
    dst = OpenAPI_target_ue_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

