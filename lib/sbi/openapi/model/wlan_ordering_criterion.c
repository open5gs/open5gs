
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wlan_ordering_criterion.h"

OpenAPI_wlan_ordering_criterion_t *OpenAPI_wlan_ordering_criterion_create(
)
{
    OpenAPI_wlan_ordering_criterion_t *wlan_ordering_criterion_local_var = ogs_malloc(sizeof(OpenAPI_wlan_ordering_criterion_t));
    ogs_assert(wlan_ordering_criterion_local_var);


    return wlan_ordering_criterion_local_var;
}

void OpenAPI_wlan_ordering_criterion_free(OpenAPI_wlan_ordering_criterion_t *wlan_ordering_criterion)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wlan_ordering_criterion) {
        return;
    }
    ogs_free(wlan_ordering_criterion);
}

cJSON *OpenAPI_wlan_ordering_criterion_convertToJSON(OpenAPI_wlan_ordering_criterion_t *wlan_ordering_criterion)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wlan_ordering_criterion == NULL) {
        ogs_error("OpenAPI_wlan_ordering_criterion_convertToJSON() failed [WlanOrderingCriterion]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_wlan_ordering_criterion_t *OpenAPI_wlan_ordering_criterion_parseFromJSON(cJSON *wlan_ordering_criterionJSON)
{
    OpenAPI_wlan_ordering_criterion_t *wlan_ordering_criterion_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    wlan_ordering_criterion_local_var = OpenAPI_wlan_ordering_criterion_create (
    );

    return wlan_ordering_criterion_local_var;
end:
    return NULL;
}

OpenAPI_wlan_ordering_criterion_t *OpenAPI_wlan_ordering_criterion_copy(OpenAPI_wlan_ordering_criterion_t *dst, OpenAPI_wlan_ordering_criterion_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wlan_ordering_criterion_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wlan_ordering_criterion_convertToJSON() failed");
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

    OpenAPI_wlan_ordering_criterion_free(dst);
    dst = OpenAPI_wlan_ordering_criterion_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

