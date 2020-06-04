
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "preferred_search.h"

OpenAPI_preferred_search_t *OpenAPI_preferred_search_create(
    int preferred_tai_match_ind
    )
{
    OpenAPI_preferred_search_t *preferred_search_local_var = OpenAPI_malloc(sizeof(OpenAPI_preferred_search_t));
    if (!preferred_search_local_var) {
        return NULL;
    }
    preferred_search_local_var->preferred_tai_match_ind = preferred_tai_match_ind;

    return preferred_search_local_var;
}

void OpenAPI_preferred_search_free(OpenAPI_preferred_search_t *preferred_search)
{
    if (NULL == preferred_search) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(preferred_search);
}

cJSON *OpenAPI_preferred_search_convertToJSON(OpenAPI_preferred_search_t *preferred_search)
{
    cJSON *item = NULL;

    if (preferred_search == NULL) {
        ogs_error("OpenAPI_preferred_search_convertToJSON() failed [PreferredSearch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (preferred_search->preferred_tai_match_ind >= 0) {
        if (cJSON_AddBoolToObject(item, "preferredTaiMatchInd", preferred_search->preferred_tai_match_ind) == NULL) {
            ogs_error("OpenAPI_preferred_search_convertToJSON() failed [preferred_tai_match_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_preferred_search_t *OpenAPI_preferred_search_parseFromJSON(cJSON *preferred_searchJSON)
{
    OpenAPI_preferred_search_t *preferred_search_local_var = NULL;
    cJSON *preferred_tai_match_ind = cJSON_GetObjectItemCaseSensitive(preferred_searchJSON, "preferredTaiMatchInd");

    if (preferred_tai_match_ind) {
        if (!cJSON_IsBool(preferred_tai_match_ind)) {
            ogs_error("OpenAPI_preferred_search_parseFromJSON() failed [preferred_tai_match_ind]");
            goto end;
        }
    }

    preferred_search_local_var = OpenAPI_preferred_search_create (
        preferred_tai_match_ind ? preferred_tai_match_ind->valueint : 0
        );

    return preferred_search_local_var;
end:
    return NULL;
}

