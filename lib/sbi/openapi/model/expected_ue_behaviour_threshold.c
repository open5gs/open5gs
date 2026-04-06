
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_ue_behaviour_threshold.h"

OpenAPI_expected_ue_behaviour_threshold_t *OpenAPI_expected_ue_behaviour_threshold_create(
    OpenAPI_list_t *expected_ue_behaviour_datasets,
    OpenAPI_list_t *single_nssais,
    OpenAPI_list_t *dnns,
    char *confidence_level,
    char *accuracy_level
)
{
    OpenAPI_expected_ue_behaviour_threshold_t *expected_ue_behaviour_threshold_local_var = ogs_malloc(sizeof(OpenAPI_expected_ue_behaviour_threshold_t));
    ogs_assert(expected_ue_behaviour_threshold_local_var);

    expected_ue_behaviour_threshold_local_var->expected_ue_behaviour_datasets = expected_ue_behaviour_datasets;
    expected_ue_behaviour_threshold_local_var->single_nssais = single_nssais;
    expected_ue_behaviour_threshold_local_var->dnns = dnns;
    expected_ue_behaviour_threshold_local_var->confidence_level = confidence_level;
    expected_ue_behaviour_threshold_local_var->accuracy_level = accuracy_level;

    return expected_ue_behaviour_threshold_local_var;
}

void OpenAPI_expected_ue_behaviour_threshold_free(OpenAPI_expected_ue_behaviour_threshold_t *expected_ue_behaviour_threshold)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == expected_ue_behaviour_threshold) {
        return;
    }
    if (expected_ue_behaviour_threshold->expected_ue_behaviour_datasets) {
        OpenAPI_list_free(expected_ue_behaviour_threshold->expected_ue_behaviour_datasets);
        expected_ue_behaviour_threshold->expected_ue_behaviour_datasets = NULL;
    }
    if (expected_ue_behaviour_threshold->single_nssais) {
        OpenAPI_list_for_each(expected_ue_behaviour_threshold->single_nssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(expected_ue_behaviour_threshold->single_nssais);
        expected_ue_behaviour_threshold->single_nssais = NULL;
    }
    if (expected_ue_behaviour_threshold->dnns) {
        OpenAPI_list_for_each(expected_ue_behaviour_threshold->dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(expected_ue_behaviour_threshold->dnns);
        expected_ue_behaviour_threshold->dnns = NULL;
    }
    if (expected_ue_behaviour_threshold->confidence_level) {
        ogs_free(expected_ue_behaviour_threshold->confidence_level);
        expected_ue_behaviour_threshold->confidence_level = NULL;
    }
    if (expected_ue_behaviour_threshold->accuracy_level) {
        ogs_free(expected_ue_behaviour_threshold->accuracy_level);
        expected_ue_behaviour_threshold->accuracy_level = NULL;
    }
    ogs_free(expected_ue_behaviour_threshold);
}

cJSON *OpenAPI_expected_ue_behaviour_threshold_convertToJSON(OpenAPI_expected_ue_behaviour_threshold_t *expected_ue_behaviour_threshold)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (expected_ue_behaviour_threshold == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [ExpectedUeBehaviourThreshold]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (expected_ue_behaviour_threshold->expected_ue_behaviour_datasets != OpenAPI_expected_ue_behaviour_dataset_NULL) {
    cJSON *expected_ue_behaviour_datasetsList = cJSON_AddArrayToObject(item, "expectedUeBehaviourDatasets");
    if (expected_ue_behaviour_datasetsList == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [expected_ue_behaviour_datasets]");
        goto end;
    }
    OpenAPI_list_for_each(expected_ue_behaviour_threshold->expected_ue_behaviour_datasets, node) {
        if (cJSON_AddStringToObject(expected_ue_behaviour_datasetsList, "", OpenAPI_expected_ue_behaviour_dataset_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [expected_ue_behaviour_datasets]");
            goto end;
        }
    }
    }

    if (expected_ue_behaviour_threshold->single_nssais) {
    cJSON *single_nssaisList = cJSON_AddArrayToObject(item, "singleNssais");
    if (single_nssaisList == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [single_nssais]");
        goto end;
    }
    OpenAPI_list_for_each(expected_ue_behaviour_threshold->single_nssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [single_nssais]");
            goto end;
        }
        cJSON_AddItemToArray(single_nssaisList, itemLocal);
    }
    }

    if (expected_ue_behaviour_threshold->dnns) {
    cJSON *dnnsList = cJSON_AddArrayToObject(item, "dnns");
    if (dnnsList == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_for_each(expected_ue_behaviour_threshold->dnns, node) {
        if (cJSON_AddStringToObject(dnnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [dnns]");
            goto end;
        }
    }
    }

    if (expected_ue_behaviour_threshold->confidence_level) {
    if (cJSON_AddStringToObject(item, "confidenceLevel", expected_ue_behaviour_threshold->confidence_level) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [confidence_level]");
        goto end;
    }
    }

    if (expected_ue_behaviour_threshold->accuracy_level) {
    if (cJSON_AddStringToObject(item, "accuracyLevel", expected_ue_behaviour_threshold->accuracy_level) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed [accuracy_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_expected_ue_behaviour_threshold_t *OpenAPI_expected_ue_behaviour_threshold_parseFromJSON(cJSON *expected_ue_behaviour_thresholdJSON)
{
    OpenAPI_expected_ue_behaviour_threshold_t *expected_ue_behaviour_threshold_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *expected_ue_behaviour_datasets = NULL;
    OpenAPI_list_t *expected_ue_behaviour_datasetsList = NULL;
    cJSON *single_nssais = NULL;
    OpenAPI_list_t *single_nssaisList = NULL;
    cJSON *dnns = NULL;
    OpenAPI_list_t *dnnsList = NULL;
    cJSON *confidence_level = NULL;
    cJSON *accuracy_level = NULL;
    expected_ue_behaviour_datasets = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_thresholdJSON, "expectedUeBehaviourDatasets");
    if (expected_ue_behaviour_datasets) {
        cJSON *expected_ue_behaviour_datasets_local = NULL;
        if (!cJSON_IsArray(expected_ue_behaviour_datasets)) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [expected_ue_behaviour_datasets]");
            goto end;
        }

        expected_ue_behaviour_datasetsList = OpenAPI_list_create();

        cJSON_ArrayForEach(expected_ue_behaviour_datasets_local, expected_ue_behaviour_datasets) {
            OpenAPI_expected_ue_behaviour_dataset_e localEnum = OpenAPI_expected_ue_behaviour_dataset_NULL;
            if (!cJSON_IsString(expected_ue_behaviour_datasets_local)) {
                ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [expected_ue_behaviour_datasets]");
                goto end;
            }
            localEnum = OpenAPI_expected_ue_behaviour_dataset_FromString(expected_ue_behaviour_datasets_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"expected_ue_behaviour_datasets\" is not supported. Ignoring it ...",
                         expected_ue_behaviour_datasets_local->valuestring);
            } else {
                OpenAPI_list_add(expected_ue_behaviour_datasetsList, (void *)localEnum);
            }
        }
        if (expected_ue_behaviour_datasetsList->count == 0) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed: Expected expected_ue_behaviour_datasetsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    single_nssais = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_thresholdJSON, "singleNssais");
    if (single_nssais) {
        cJSON *single_nssais_local = NULL;
        if (!cJSON_IsArray(single_nssais)) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [single_nssais]");
            goto end;
        }

        single_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(single_nssais_local, single_nssais) {
            if (!cJSON_IsObject(single_nssais_local)) {
                ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [single_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *single_nssaisItem = OpenAPI_snssai_parseFromJSON(single_nssais_local);
            if (!single_nssaisItem) {
                ogs_error("No single_nssaisItem");
                goto end;
            }
            OpenAPI_list_add(single_nssaisList, single_nssaisItem);
        }
    }

    dnns = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_thresholdJSON, "dnns");
    if (dnns) {
        cJSON *dnns_local = NULL;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [dnns]");
            goto end;
        }

        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    confidence_level = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_thresholdJSON, "confidenceLevel");
    if (confidence_level) {
    if (!cJSON_IsString(confidence_level) && !cJSON_IsNull(confidence_level)) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [confidence_level]");
        goto end;
    }
    }

    accuracy_level = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_thresholdJSON, "accuracyLevel");
    if (accuracy_level) {
    if (!cJSON_IsString(accuracy_level) && !cJSON_IsNull(accuracy_level)) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_parseFromJSON() failed [accuracy_level]");
        goto end;
    }
    }

    expected_ue_behaviour_threshold_local_var = OpenAPI_expected_ue_behaviour_threshold_create (
        expected_ue_behaviour_datasets ? expected_ue_behaviour_datasetsList : NULL,
        single_nssais ? single_nssaisList : NULL,
        dnns ? dnnsList : NULL,
        confidence_level && !cJSON_IsNull(confidence_level) ? ogs_strdup(confidence_level->valuestring) : NULL,
        accuracy_level && !cJSON_IsNull(accuracy_level) ? ogs_strdup(accuracy_level->valuestring) : NULL
    );

    return expected_ue_behaviour_threshold_local_var;
end:
    if (expected_ue_behaviour_datasetsList) {
        OpenAPI_list_free(expected_ue_behaviour_datasetsList);
        expected_ue_behaviour_datasetsList = NULL;
    }
    if (single_nssaisList) {
        OpenAPI_list_for_each(single_nssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(single_nssaisList);
        single_nssaisList = NULL;
    }
    if (dnnsList) {
        OpenAPI_list_for_each(dnnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnnsList);
        dnnsList = NULL;
    }
    return NULL;
}

OpenAPI_expected_ue_behaviour_threshold_t *OpenAPI_expected_ue_behaviour_threshold_copy(OpenAPI_expected_ue_behaviour_threshold_t *dst, OpenAPI_expected_ue_behaviour_threshold_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_expected_ue_behaviour_threshold_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_expected_ue_behaviour_threshold_convertToJSON() failed");
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

    OpenAPI_expected_ue_behaviour_threshold_free(dst);
    dst = OpenAPI_expected_ue_behaviour_threshold_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

