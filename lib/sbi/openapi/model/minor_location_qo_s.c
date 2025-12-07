
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "minor_location_qo_s.h"

OpenAPI_minor_location_qo_s_t *OpenAPI_minor_location_qo_s_create(
    bool is_h_accuracy,
    float h_accuracy,
    bool is_v_accuracy,
    float v_accuracy
)
{
    OpenAPI_minor_location_qo_s_t *minor_location_qo_s_local_var = ogs_malloc(sizeof(OpenAPI_minor_location_qo_s_t));
    ogs_assert(minor_location_qo_s_local_var);

    minor_location_qo_s_local_var->is_h_accuracy = is_h_accuracy;
    minor_location_qo_s_local_var->h_accuracy = h_accuracy;
    minor_location_qo_s_local_var->is_v_accuracy = is_v_accuracy;
    minor_location_qo_s_local_var->v_accuracy = v_accuracy;

    return minor_location_qo_s_local_var;
}

void OpenAPI_minor_location_qo_s_free(OpenAPI_minor_location_qo_s_t *minor_location_qo_s)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == minor_location_qo_s) {
        return;
    }
    ogs_free(minor_location_qo_s);
}

cJSON *OpenAPI_minor_location_qo_s_convertToJSON(OpenAPI_minor_location_qo_s_t *minor_location_qo_s)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (minor_location_qo_s == NULL) {
        ogs_error("OpenAPI_minor_location_qo_s_convertToJSON() failed [MinorLocationQoS]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (minor_location_qo_s->is_h_accuracy) {
    if (cJSON_AddNumberToObject(item, "hAccuracy", minor_location_qo_s->h_accuracy) == NULL) {
        ogs_error("OpenAPI_minor_location_qo_s_convertToJSON() failed [h_accuracy]");
        goto end;
    }
    }

    if (minor_location_qo_s->is_v_accuracy) {
    if (cJSON_AddNumberToObject(item, "vAccuracy", minor_location_qo_s->v_accuracy) == NULL) {
        ogs_error("OpenAPI_minor_location_qo_s_convertToJSON() failed [v_accuracy]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_minor_location_qo_s_t *OpenAPI_minor_location_qo_s_parseFromJSON(cJSON *minor_location_qo_sJSON)
{
    OpenAPI_minor_location_qo_s_t *minor_location_qo_s_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_accuracy = NULL;
    cJSON *v_accuracy = NULL;
    h_accuracy = cJSON_GetObjectItemCaseSensitive(minor_location_qo_sJSON, "hAccuracy");
    if (h_accuracy) {
    if (!cJSON_IsNumber(h_accuracy)) {
        ogs_error("OpenAPI_minor_location_qo_s_parseFromJSON() failed [h_accuracy]");
        goto end;
    }
    }

    v_accuracy = cJSON_GetObjectItemCaseSensitive(minor_location_qo_sJSON, "vAccuracy");
    if (v_accuracy) {
    if (!cJSON_IsNumber(v_accuracy)) {
        ogs_error("OpenAPI_minor_location_qo_s_parseFromJSON() failed [v_accuracy]");
        goto end;
    }
    }

    minor_location_qo_s_local_var = OpenAPI_minor_location_qo_s_create (
        h_accuracy ? true : false,
        h_accuracy ? h_accuracy->valuedouble : 0,
        v_accuracy ? true : false,
        v_accuracy ? v_accuracy->valuedouble : 0
    );

    return minor_location_qo_s_local_var;
end:
    return NULL;
}

OpenAPI_minor_location_qo_s_t *OpenAPI_minor_location_qo_s_copy(OpenAPI_minor_location_qo_s_t *dst, OpenAPI_minor_location_qo_s_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_minor_location_qo_s_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_minor_location_qo_s_convertToJSON() failed");
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

    OpenAPI_minor_location_qo_s_free(dst);
    dst = OpenAPI_minor_location_qo_s_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

