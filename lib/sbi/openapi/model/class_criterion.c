
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "class_criterion.h"

OpenAPI_class_criterion_t *OpenAPI_class_criterion_create(
    OpenAPI_dispersion_class_t *disper_class,
    int class_threshold,
    OpenAPI_matching_direction_t *thres_match
)
{
    OpenAPI_class_criterion_t *class_criterion_local_var = ogs_malloc(sizeof(OpenAPI_class_criterion_t));
    ogs_assert(class_criterion_local_var);

    class_criterion_local_var->disper_class = disper_class;
    class_criterion_local_var->class_threshold = class_threshold;
    class_criterion_local_var->thres_match = thres_match;

    return class_criterion_local_var;
}

void OpenAPI_class_criterion_free(OpenAPI_class_criterion_t *class_criterion)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == class_criterion) {
        return;
    }
    if (class_criterion->disper_class) {
        OpenAPI_dispersion_class_free(class_criterion->disper_class);
        class_criterion->disper_class = NULL;
    }
    if (class_criterion->thres_match) {
        OpenAPI_matching_direction_free(class_criterion->thres_match);
        class_criterion->thres_match = NULL;
    }
    ogs_free(class_criterion);
}

cJSON *OpenAPI_class_criterion_convertToJSON(OpenAPI_class_criterion_t *class_criterion)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (class_criterion == NULL) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [ClassCriterion]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!class_criterion->disper_class) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [disper_class]");
        return NULL;
    }
    cJSON *disper_class_local_JSON = OpenAPI_dispersion_class_convertToJSON(class_criterion->disper_class);
    if (disper_class_local_JSON == NULL) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [disper_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "disperClass", disper_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [disper_class]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "classThreshold", class_criterion->class_threshold) == NULL) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [class_threshold]");
        goto end;
    }

    if (!class_criterion->thres_match) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [thres_match]");
        return NULL;
    }
    cJSON *thres_match_local_JSON = OpenAPI_matching_direction_convertToJSON(class_criterion->thres_match);
    if (thres_match_local_JSON == NULL) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [thres_match]");
        goto end;
    }
    cJSON_AddItemToObject(item, "thresMatch", thres_match_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed [thres_match]");
        goto end;
    }

end:
    return item;
}

OpenAPI_class_criterion_t *OpenAPI_class_criterion_parseFromJSON(cJSON *class_criterionJSON)
{
    OpenAPI_class_criterion_t *class_criterion_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *disper_class = NULL;
    OpenAPI_dispersion_class_t *disper_class_local_nonprim = NULL;
    cJSON *class_threshold = NULL;
    cJSON *thres_match = NULL;
    OpenAPI_matching_direction_t *thres_match_local_nonprim = NULL;
    disper_class = cJSON_GetObjectItemCaseSensitive(class_criterionJSON, "disperClass");
    if (!disper_class) {
        ogs_error("OpenAPI_class_criterion_parseFromJSON() failed [disper_class]");
        goto end;
    }
    disper_class_local_nonprim = OpenAPI_dispersion_class_parseFromJSON(disper_class);
    if (!disper_class_local_nonprim) {
        ogs_error("OpenAPI_dispersion_class_parseFromJSON failed [disper_class]");
        goto end;
    }

    class_threshold = cJSON_GetObjectItemCaseSensitive(class_criterionJSON, "classThreshold");
    if (!class_threshold) {
        ogs_error("OpenAPI_class_criterion_parseFromJSON() failed [class_threshold]");
        goto end;
    }
    if (!cJSON_IsNumber(class_threshold)) {
        ogs_error("OpenAPI_class_criterion_parseFromJSON() failed [class_threshold]");
        goto end;
    }

    thres_match = cJSON_GetObjectItemCaseSensitive(class_criterionJSON, "thresMatch");
    if (!thres_match) {
        ogs_error("OpenAPI_class_criterion_parseFromJSON() failed [thres_match]");
        goto end;
    }
    thres_match_local_nonprim = OpenAPI_matching_direction_parseFromJSON(thres_match);
    if (!thres_match_local_nonprim) {
        ogs_error("OpenAPI_matching_direction_parseFromJSON failed [thres_match]");
        goto end;
    }

    class_criterion_local_var = OpenAPI_class_criterion_create (
        disper_class_local_nonprim,
        
        class_threshold->valuedouble,
        thres_match_local_nonprim
    );

    return class_criterion_local_var;
end:
    if (disper_class_local_nonprim) {
        OpenAPI_dispersion_class_free(disper_class_local_nonprim);
        disper_class_local_nonprim = NULL;
    }
    if (thres_match_local_nonprim) {
        OpenAPI_matching_direction_free(thres_match_local_nonprim);
        thres_match_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_class_criterion_t *OpenAPI_class_criterion_copy(OpenAPI_class_criterion_t *dst, OpenAPI_class_criterion_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_class_criterion_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_class_criterion_convertToJSON() failed");
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

    OpenAPI_class_criterion_free(dst);
    dst = OpenAPI_class_criterion_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

