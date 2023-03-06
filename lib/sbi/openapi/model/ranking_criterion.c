
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranking_criterion.h"

OpenAPI_ranking_criterion_t *OpenAPI_ranking_criterion_create(
    int high_base,
    int low_base
)
{
    OpenAPI_ranking_criterion_t *ranking_criterion_local_var = ogs_malloc(sizeof(OpenAPI_ranking_criterion_t));
    ogs_assert(ranking_criterion_local_var);

    ranking_criterion_local_var->high_base = high_base;
    ranking_criterion_local_var->low_base = low_base;

    return ranking_criterion_local_var;
}

void OpenAPI_ranking_criterion_free(OpenAPI_ranking_criterion_t *ranking_criterion)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranking_criterion) {
        return;
    }
    ogs_free(ranking_criterion);
}

cJSON *OpenAPI_ranking_criterion_convertToJSON(OpenAPI_ranking_criterion_t *ranking_criterion)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranking_criterion == NULL) {
        ogs_error("OpenAPI_ranking_criterion_convertToJSON() failed [RankingCriterion]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "highBase", ranking_criterion->high_base) == NULL) {
        ogs_error("OpenAPI_ranking_criterion_convertToJSON() failed [high_base]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "lowBase", ranking_criterion->low_base) == NULL) {
        ogs_error("OpenAPI_ranking_criterion_convertToJSON() failed [low_base]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ranking_criterion_t *OpenAPI_ranking_criterion_parseFromJSON(cJSON *ranking_criterionJSON)
{
    OpenAPI_ranking_criterion_t *ranking_criterion_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *high_base = NULL;
    cJSON *low_base = NULL;
    high_base = cJSON_GetObjectItemCaseSensitive(ranking_criterionJSON, "highBase");
    if (!high_base) {
        ogs_error("OpenAPI_ranking_criterion_parseFromJSON() failed [high_base]");
        goto end;
    }
    if (!cJSON_IsNumber(high_base)) {
        ogs_error("OpenAPI_ranking_criterion_parseFromJSON() failed [high_base]");
        goto end;
    }

    low_base = cJSON_GetObjectItemCaseSensitive(ranking_criterionJSON, "lowBase");
    if (!low_base) {
        ogs_error("OpenAPI_ranking_criterion_parseFromJSON() failed [low_base]");
        goto end;
    }
    if (!cJSON_IsNumber(low_base)) {
        ogs_error("OpenAPI_ranking_criterion_parseFromJSON() failed [low_base]");
        goto end;
    }

    ranking_criterion_local_var = OpenAPI_ranking_criterion_create (
        
        high_base->valuedouble,
        
        low_base->valuedouble
    );

    return ranking_criterion_local_var;
end:
    return NULL;
}

OpenAPI_ranking_criterion_t *OpenAPI_ranking_criterion_copy(OpenAPI_ranking_criterion_t *dst, OpenAPI_ranking_criterion_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranking_criterion_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranking_criterion_convertToJSON() failed");
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

    OpenAPI_ranking_criterion_free(dst);
    dst = OpenAPI_ranking_criterion_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

