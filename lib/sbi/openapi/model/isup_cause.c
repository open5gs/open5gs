
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "isup_cause.h"

OpenAPI_isup_cause_t *OpenAPI_isup_cause_create(
    bool is_i_sup_cause_location,
    int i_sup_cause_location,
    bool is_i_sup_cause_value,
    int i_sup_cause_value,
    char *i_sup_cause_diagnostics,
    OpenAPI_list_t *enhanced_diagnostics
)
{
    OpenAPI_isup_cause_t *isup_cause_local_var = ogs_malloc(sizeof(OpenAPI_isup_cause_t));
    ogs_assert(isup_cause_local_var);

    isup_cause_local_var->is_i_sup_cause_location = is_i_sup_cause_location;
    isup_cause_local_var->i_sup_cause_location = i_sup_cause_location;
    isup_cause_local_var->is_i_sup_cause_value = is_i_sup_cause_value;
    isup_cause_local_var->i_sup_cause_value = i_sup_cause_value;
    isup_cause_local_var->i_sup_cause_diagnostics = i_sup_cause_diagnostics;
    isup_cause_local_var->enhanced_diagnostics = enhanced_diagnostics;

    return isup_cause_local_var;
}

void OpenAPI_isup_cause_free(OpenAPI_isup_cause_t *isup_cause)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == isup_cause) {
        return;
    }
    if (isup_cause->i_sup_cause_diagnostics) {
        ogs_free(isup_cause->i_sup_cause_diagnostics);
        isup_cause->i_sup_cause_diagnostics = NULL;
    }
    if (isup_cause->enhanced_diagnostics) {
        OpenAPI_list_for_each(isup_cause->enhanced_diagnostics, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(isup_cause->enhanced_diagnostics);
        isup_cause->enhanced_diagnostics = NULL;
    }
    ogs_free(isup_cause);
}

cJSON *OpenAPI_isup_cause_convertToJSON(OpenAPI_isup_cause_t *isup_cause)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (isup_cause == NULL) {
        ogs_error("OpenAPI_isup_cause_convertToJSON() failed [ISUPCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (isup_cause->is_i_sup_cause_location) {
    if (cJSON_AddNumberToObject(item, "iSUPCauseLocation", isup_cause->i_sup_cause_location) == NULL) {
        ogs_error("OpenAPI_isup_cause_convertToJSON() failed [i_sup_cause_location]");
        goto end;
    }
    }

    if (isup_cause->is_i_sup_cause_value) {
    if (cJSON_AddNumberToObject(item, "iSUPCauseValue", isup_cause->i_sup_cause_value) == NULL) {
        ogs_error("OpenAPI_isup_cause_convertToJSON() failed [i_sup_cause_value]");
        goto end;
    }
    }

    if (isup_cause->i_sup_cause_diagnostics) {
    if (cJSON_AddStringToObject(item, "iSUPCauseDiagnostics", isup_cause->i_sup_cause_diagnostics) == NULL) {
        ogs_error("OpenAPI_isup_cause_convertToJSON() failed [i_sup_cause_diagnostics]");
        goto end;
    }
    }

    if (isup_cause->enhanced_diagnostics) {
    cJSON *enhanced_diagnosticsList = cJSON_AddArrayToObject(item, "enhancedDiagnostics");
    if (enhanced_diagnosticsList == NULL) {
        ogs_error("OpenAPI_isup_cause_convertToJSON() failed [enhanced_diagnostics]");
        goto end;
    }
    OpenAPI_list_for_each(isup_cause->enhanced_diagnostics, node) {
        cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_isup_cause_convertToJSON() failed [enhanced_diagnostics]");
            goto end;
        }
        cJSON_AddItemToArray(enhanced_diagnosticsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_isup_cause_t *OpenAPI_isup_cause_parseFromJSON(cJSON *isup_causeJSON)
{
    OpenAPI_isup_cause_t *isup_cause_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *i_sup_cause_location = NULL;
    cJSON *i_sup_cause_value = NULL;
    cJSON *i_sup_cause_diagnostics = NULL;
    cJSON *enhanced_diagnostics = NULL;
    OpenAPI_list_t *enhanced_diagnosticsList = NULL;
    i_sup_cause_location = cJSON_GetObjectItemCaseSensitive(isup_causeJSON, "iSUPCauseLocation");
    if (i_sup_cause_location) {
    if (!cJSON_IsNumber(i_sup_cause_location)) {
        ogs_error("OpenAPI_isup_cause_parseFromJSON() failed [i_sup_cause_location]");
        goto end;
    }
    }

    i_sup_cause_value = cJSON_GetObjectItemCaseSensitive(isup_causeJSON, "iSUPCauseValue");
    if (i_sup_cause_value) {
    if (!cJSON_IsNumber(i_sup_cause_value)) {
        ogs_error("OpenAPI_isup_cause_parseFromJSON() failed [i_sup_cause_value]");
        goto end;
    }
    }

    i_sup_cause_diagnostics = cJSON_GetObjectItemCaseSensitive(isup_causeJSON, "iSUPCauseDiagnostics");
    if (i_sup_cause_diagnostics) {
    if (!cJSON_IsString(i_sup_cause_diagnostics) && !cJSON_IsNull(i_sup_cause_diagnostics)) {
        ogs_error("OpenAPI_isup_cause_parseFromJSON() failed [i_sup_cause_diagnostics]");
        goto end;
    }
    }

    enhanced_diagnostics = cJSON_GetObjectItemCaseSensitive(isup_causeJSON, "enhancedDiagnostics");
    if (enhanced_diagnostics) {
        cJSON *enhanced_diagnostics_local = NULL;
        if (!cJSON_IsArray(enhanced_diagnostics)) {
            ogs_error("OpenAPI_isup_cause_parseFromJSON() failed [enhanced_diagnostics]");
            goto end;
        }

        enhanced_diagnosticsList = OpenAPI_list_create();

        cJSON_ArrayForEach(enhanced_diagnostics_local, enhanced_diagnostics) {
            if (!cJSON_IsObject(enhanced_diagnostics_local)) {
                ogs_error("OpenAPI_isup_cause_parseFromJSON() failed [enhanced_diagnostics]");
                goto end;
            }
            OpenAPI_ran_nas_rel_cause_t *enhanced_diagnosticsItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(enhanced_diagnostics_local);
            if (!enhanced_diagnosticsItem) {
                ogs_error("No enhanced_diagnosticsItem");
                goto end;
            }
            OpenAPI_list_add(enhanced_diagnosticsList, enhanced_diagnosticsItem);
        }
    }

    isup_cause_local_var = OpenAPI_isup_cause_create (
        i_sup_cause_location ? true : false,
        i_sup_cause_location ? i_sup_cause_location->valuedouble : 0,
        i_sup_cause_value ? true : false,
        i_sup_cause_value ? i_sup_cause_value->valuedouble : 0,
        i_sup_cause_diagnostics && !cJSON_IsNull(i_sup_cause_diagnostics) ? ogs_strdup(i_sup_cause_diagnostics->valuestring) : NULL,
        enhanced_diagnostics ? enhanced_diagnosticsList : NULL
    );

    return isup_cause_local_var;
end:
    if (enhanced_diagnosticsList) {
        OpenAPI_list_for_each(enhanced_diagnosticsList, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(enhanced_diagnosticsList);
        enhanced_diagnosticsList = NULL;
    }
    return NULL;
}

OpenAPI_isup_cause_t *OpenAPI_isup_cause_copy(OpenAPI_isup_cause_t *dst, OpenAPI_isup_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_isup_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_isup_cause_convertToJSON() failed");
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

    OpenAPI_isup_cause_free(dst);
    dst = OpenAPI_isup_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

