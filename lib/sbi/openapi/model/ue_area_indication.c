
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_area_indication.h"

OpenAPI_ue_area_indication_t *OpenAPI_ue_area_indication_create(
    char *country,
    bool is_international_area_ind,
    int international_area_ind
)
{
    OpenAPI_ue_area_indication_t *ue_area_indication_local_var = ogs_malloc(sizeof(OpenAPI_ue_area_indication_t));
    ogs_assert(ue_area_indication_local_var);

    ue_area_indication_local_var->country = country;
    ue_area_indication_local_var->is_international_area_ind = is_international_area_ind;
    ue_area_indication_local_var->international_area_ind = international_area_ind;

    return ue_area_indication_local_var;
}

void OpenAPI_ue_area_indication_free(OpenAPI_ue_area_indication_t *ue_area_indication)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_area_indication) {
        return;
    }
    if (ue_area_indication->country) {
        ogs_free(ue_area_indication->country);
        ue_area_indication->country = NULL;
    }
    ogs_free(ue_area_indication);
}

cJSON *OpenAPI_ue_area_indication_convertToJSON(OpenAPI_ue_area_indication_t *ue_area_indication)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_area_indication == NULL) {
        ogs_error("OpenAPI_ue_area_indication_convertToJSON() failed [UeAreaIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_area_indication->country) {
    if (cJSON_AddStringToObject(item, "country", ue_area_indication->country) == NULL) {
        ogs_error("OpenAPI_ue_area_indication_convertToJSON() failed [country]");
        goto end;
    }
    }

    if (ue_area_indication->is_international_area_ind) {
    if (cJSON_AddBoolToObject(item, "internationalAreaInd", ue_area_indication->international_area_ind) == NULL) {
        ogs_error("OpenAPI_ue_area_indication_convertToJSON() failed [international_area_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_area_indication_t *OpenAPI_ue_area_indication_parseFromJSON(cJSON *ue_area_indicationJSON)
{
    OpenAPI_ue_area_indication_t *ue_area_indication_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *country = NULL;
    cJSON *international_area_ind = NULL;
    country = cJSON_GetObjectItemCaseSensitive(ue_area_indicationJSON, "country");
    if (country) {
    if (!cJSON_IsString(country) && !cJSON_IsNull(country)) {
        ogs_error("OpenAPI_ue_area_indication_parseFromJSON() failed [country]");
        goto end;
    }
    }

    international_area_ind = cJSON_GetObjectItemCaseSensitive(ue_area_indicationJSON, "internationalAreaInd");
    if (international_area_ind) {
    if (!cJSON_IsBool(international_area_ind)) {
        ogs_error("OpenAPI_ue_area_indication_parseFromJSON() failed [international_area_ind]");
        goto end;
    }
    }

    ue_area_indication_local_var = OpenAPI_ue_area_indication_create (
        country && !cJSON_IsNull(country) ? ogs_strdup(country->valuestring) : NULL,
        international_area_ind ? true : false,
        international_area_ind ? international_area_ind->valueint : 0
    );

    return ue_area_indication_local_var;
end:
    return NULL;
}

OpenAPI_ue_area_indication_t *OpenAPI_ue_area_indication_copy(OpenAPI_ue_area_indication_t *dst, OpenAPI_ue_area_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_area_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_area_indication_convertToJSON() failed");
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

    OpenAPI_ue_area_indication_free(dst);
    dst = OpenAPI_ue_area_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

