
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsc_assistance_information.h"

OpenAPI_tsc_assistance_information_t *OpenAPI_tsc_assistance_information_create(
    bool is_periodicity,
    int periodicity,
    OpenAPI_n6_jitter_information_t *n6_jitter_information
)
{
    OpenAPI_tsc_assistance_information_t *tsc_assistance_information_local_var = ogs_malloc(sizeof(OpenAPI_tsc_assistance_information_t));
    ogs_assert(tsc_assistance_information_local_var);

    tsc_assistance_information_local_var->is_periodicity = is_periodicity;
    tsc_assistance_information_local_var->periodicity = periodicity;
    tsc_assistance_information_local_var->n6_jitter_information = n6_jitter_information;

    return tsc_assistance_information_local_var;
}

void OpenAPI_tsc_assistance_information_free(OpenAPI_tsc_assistance_information_t *tsc_assistance_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tsc_assistance_information) {
        return;
    }
    if (tsc_assistance_information->n6_jitter_information) {
        OpenAPI_n6_jitter_information_free(tsc_assistance_information->n6_jitter_information);
        tsc_assistance_information->n6_jitter_information = NULL;
    }
    ogs_free(tsc_assistance_information);
}

cJSON *OpenAPI_tsc_assistance_information_convertToJSON(OpenAPI_tsc_assistance_information_t *tsc_assistance_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tsc_assistance_information == NULL) {
        ogs_error("OpenAPI_tsc_assistance_information_convertToJSON() failed [TscAssistanceInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsc_assistance_information->is_periodicity) {
    if (cJSON_AddNumberToObject(item, "periodicity", tsc_assistance_information->periodicity) == NULL) {
        ogs_error("OpenAPI_tsc_assistance_information_convertToJSON() failed [periodicity]");
        goto end;
    }
    }

    if (tsc_assistance_information->n6_jitter_information) {
    cJSON *n6_jitter_information_local_JSON = OpenAPI_n6_jitter_information_convertToJSON(tsc_assistance_information->n6_jitter_information);
    if (n6_jitter_information_local_JSON == NULL) {
        ogs_error("OpenAPI_tsc_assistance_information_convertToJSON() failed [n6_jitter_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n6JitterInformation", n6_jitter_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tsc_assistance_information_convertToJSON() failed [n6_jitter_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tsc_assistance_information_t *OpenAPI_tsc_assistance_information_parseFromJSON(cJSON *tsc_assistance_informationJSON)
{
    OpenAPI_tsc_assistance_information_t *tsc_assistance_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *periodicity = NULL;
    cJSON *n6_jitter_information = NULL;
    OpenAPI_n6_jitter_information_t *n6_jitter_information_local_nonprim = NULL;
    periodicity = cJSON_GetObjectItemCaseSensitive(tsc_assistance_informationJSON, "periodicity");
    if (periodicity) {
    if (!cJSON_IsNumber(periodicity)) {
        ogs_error("OpenAPI_tsc_assistance_information_parseFromJSON() failed [periodicity]");
        goto end;
    }
    }

    n6_jitter_information = cJSON_GetObjectItemCaseSensitive(tsc_assistance_informationJSON, "n6JitterInformation");
    if (n6_jitter_information) {
    n6_jitter_information_local_nonprim = OpenAPI_n6_jitter_information_parseFromJSON(n6_jitter_information);
    if (!n6_jitter_information_local_nonprim) {
        ogs_error("OpenAPI_n6_jitter_information_parseFromJSON failed [n6_jitter_information]");
        goto end;
    }
    }

    tsc_assistance_information_local_var = OpenAPI_tsc_assistance_information_create (
        periodicity ? true : false,
        periodicity ? periodicity->valuedouble : 0,
        n6_jitter_information ? n6_jitter_information_local_nonprim : NULL
    );

    return tsc_assistance_information_local_var;
end:
    if (n6_jitter_information_local_nonprim) {
        OpenAPI_n6_jitter_information_free(n6_jitter_information_local_nonprim);
        n6_jitter_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tsc_assistance_information_t *OpenAPI_tsc_assistance_information_copy(OpenAPI_tsc_assistance_information_t *dst, OpenAPI_tsc_assistance_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsc_assistance_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsc_assistance_information_convertToJSON() failed");
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

    OpenAPI_tsc_assistance_information_free(dst);
    dst = OpenAPI_tsc_assistance_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

