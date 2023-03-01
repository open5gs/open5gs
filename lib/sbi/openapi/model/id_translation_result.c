
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "id_translation_result.h"

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_create(
    char *supported_features,
    char *supi,
    char *gpsi,
    OpenAPI_list_t *additional_supis,
    OpenAPI_list_t *additional_gpsis
)
{
    OpenAPI_id_translation_result_t *id_translation_result_local_var = ogs_malloc(sizeof(OpenAPI_id_translation_result_t));
    ogs_assert(id_translation_result_local_var);

    id_translation_result_local_var->supported_features = supported_features;
    id_translation_result_local_var->supi = supi;
    id_translation_result_local_var->gpsi = gpsi;
    id_translation_result_local_var->additional_supis = additional_supis;
    id_translation_result_local_var->additional_gpsis = additional_gpsis;

    return id_translation_result_local_var;
}

void OpenAPI_id_translation_result_free(OpenAPI_id_translation_result_t *id_translation_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == id_translation_result) {
        return;
    }
    if (id_translation_result->supported_features) {
        ogs_free(id_translation_result->supported_features);
        id_translation_result->supported_features = NULL;
    }
    if (id_translation_result->supi) {
        ogs_free(id_translation_result->supi);
        id_translation_result->supi = NULL;
    }
    if (id_translation_result->gpsi) {
        ogs_free(id_translation_result->gpsi);
        id_translation_result->gpsi = NULL;
    }
    if (id_translation_result->additional_supis) {
        OpenAPI_list_for_each(id_translation_result->additional_supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(id_translation_result->additional_supis);
        id_translation_result->additional_supis = NULL;
    }
    if (id_translation_result->additional_gpsis) {
        OpenAPI_list_for_each(id_translation_result->additional_gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(id_translation_result->additional_gpsis);
        id_translation_result->additional_gpsis = NULL;
    }
    ogs_free(id_translation_result);
}

cJSON *OpenAPI_id_translation_result_convertToJSON(OpenAPI_id_translation_result_t *id_translation_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (id_translation_result == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [IdTranslationResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (id_translation_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", id_translation_result->supported_features) == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (!id_translation_result->supi) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", id_translation_result->supi) == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [supi]");
        goto end;
    }

    if (id_translation_result->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", id_translation_result->gpsi) == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (id_translation_result->additional_supis) {
    cJSON *additional_supisList = cJSON_AddArrayToObject(item, "additionalSupis");
    if (additional_supisList == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [additional_supis]");
        goto end;
    }
    OpenAPI_list_for_each(id_translation_result->additional_supis, node) {
        if (cJSON_AddStringToObject(additional_supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [additional_supis]");
            goto end;
        }
    }
    }

    if (id_translation_result->additional_gpsis) {
    cJSON *additional_gpsisList = cJSON_AddArrayToObject(item, "additionalGpsis");
    if (additional_gpsisList == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [additional_gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(id_translation_result->additional_gpsis, node) {
        if (cJSON_AddStringToObject(additional_gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [additional_gpsis]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_parseFromJSON(cJSON *id_translation_resultJSON)
{
    OpenAPI_id_translation_result_t *id_translation_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supported_features = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *additional_supis = NULL;
    OpenAPI_list_t *additional_supisList = NULL;
    cJSON *additional_gpsis = NULL;
    OpenAPI_list_t *additional_gpsisList = NULL;
    supported_features = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [supi]");
        goto end;
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    additional_supis = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "additionalSupis");
    if (additional_supis) {
        cJSON *additional_supis_local = NULL;
        if (!cJSON_IsArray(additional_supis)) {
            ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [additional_supis]");
            goto end;
        }

        additional_supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_supis_local, additional_supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_supis_local)) {
                ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [additional_supis]");
                goto end;
            }
            OpenAPI_list_add(additional_supisList, ogs_strdup(additional_supis_local->valuestring));
        }
    }

    additional_gpsis = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "additionalGpsis");
    if (additional_gpsis) {
        cJSON *additional_gpsis_local = NULL;
        if (!cJSON_IsArray(additional_gpsis)) {
            ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [additional_gpsis]");
            goto end;
        }

        additional_gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_gpsis_local, additional_gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_gpsis_local)) {
                ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [additional_gpsis]");
                goto end;
            }
            OpenAPI_list_add(additional_gpsisList, ogs_strdup(additional_gpsis_local->valuestring));
        }
    }

    id_translation_result_local_var = OpenAPI_id_translation_result_create (
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        ogs_strdup(supi->valuestring),
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        additional_supis ? additional_supisList : NULL,
        additional_gpsis ? additional_gpsisList : NULL
    );

    return id_translation_result_local_var;
end:
    if (additional_supisList) {
        OpenAPI_list_for_each(additional_supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_supisList);
        additional_supisList = NULL;
    }
    if (additional_gpsisList) {
        OpenAPI_list_for_each(additional_gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_gpsisList);
        additional_gpsisList = NULL;
    }
    return NULL;
}

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_copy(OpenAPI_id_translation_result_t *dst, OpenAPI_id_translation_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_id_translation_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed");
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

    OpenAPI_id_translation_result_free(dst);
    dst = OpenAPI_id_translation_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

