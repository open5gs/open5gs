
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ext_snssai.h"

char *OpenAPI_wildcard_sdext_snssai_ToString(OpenAPI_ext_snssai_wildcard_sd_e wildcard_sd)
{
    const char *wildcard_sdArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wildcard_sdArray) / sizeof(wildcard_sdArray[0]);
    if (wildcard_sd < sizeofArray)
        return (char *)wildcard_sdArray[wildcard_sd];
    else
        return (char *)"Unknown";
}

OpenAPI_ext_snssai_wildcard_sd_e OpenAPI_wildcard_sdext_snssai_FromString(char* wildcard_sd)
{
    int stringToReturn = 0;
    const char *wildcard_sdArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wildcard_sdArray) / sizeof(wildcard_sdArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(wildcard_sd, wildcard_sdArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_create(
    int sst,
    char *sd,
    OpenAPI_list_t *sd_ranges,
    OpenAPI_ext_snssai_wildcard_sd_e wildcard_sd
)
{
    OpenAPI_ext_snssai_t *ext_snssai_local_var = ogs_malloc(sizeof(OpenAPI_ext_snssai_t));
    ogs_assert(ext_snssai_local_var);

    ext_snssai_local_var->sst = sst;
    ext_snssai_local_var->sd = sd;
    ext_snssai_local_var->sd_ranges = sd_ranges;
    ext_snssai_local_var->wildcard_sd = wildcard_sd;

    return ext_snssai_local_var;
}

void OpenAPI_ext_snssai_free(OpenAPI_ext_snssai_t *ext_snssai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ext_snssai) {
        return;
    }
    if (ext_snssai->sd) {
        ogs_free(ext_snssai->sd);
        ext_snssai->sd = NULL;
    }
    if (ext_snssai->sd_ranges) {
        OpenAPI_list_for_each(ext_snssai->sd_ranges, node) {
            OpenAPI_sd_range_free(node->data);
        }
        OpenAPI_list_free(ext_snssai->sd_ranges);
        ext_snssai->sd_ranges = NULL;
    }
    ogs_free(ext_snssai);
}

cJSON *OpenAPI_ext_snssai_convertToJSON(OpenAPI_ext_snssai_t *ext_snssai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ext_snssai == NULL) {
        ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [ExtSnssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "sst", ext_snssai->sst) == NULL) {
        ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [sst]");
        goto end;
    }

    if (ext_snssai->sd) {
    if (cJSON_AddStringToObject(item, "sd", ext_snssai->sd) == NULL) {
        ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [sd]");
        goto end;
    }
    }

    if (ext_snssai->sd_ranges) {
    cJSON *sd_rangesList = cJSON_AddArrayToObject(item, "sdRanges");
    if (sd_rangesList == NULL) {
        ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [sd_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(ext_snssai->sd_ranges, node) {
        cJSON *itemLocal = OpenAPI_sd_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [sd_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(sd_rangesList, itemLocal);
    }
    }

    if (ext_snssai->wildcard_sd != OpenAPI_ext_snssai_WILDCARDSD_NULL) {
    if (cJSON_AddStringToObject(item, "wildcardSd", OpenAPI_wildcard_sdext_snssai_ToString(ext_snssai->wildcard_sd)) == NULL) {
        ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [wildcard_sd]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_parseFromJSON(cJSON *ext_snssaiJSON)
{
    OpenAPI_ext_snssai_t *ext_snssai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sst = NULL;
    cJSON *sd = NULL;
    cJSON *sd_ranges = NULL;
    OpenAPI_list_t *sd_rangesList = NULL;
    cJSON *wildcard_sd = NULL;
    OpenAPI_ext_snssai_wildcard_sd_e wildcard_sdVariable = 0;
    sst = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "sst");
    if (!sst) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sst]");
        goto end;
    }
    if (!cJSON_IsNumber(sst)) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sst]");
        goto end;
    }

    sd = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "sd");
    if (sd) {
    if (!cJSON_IsString(sd) && !cJSON_IsNull(sd)) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sd]");
        goto end;
    }
    }

    sd_ranges = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "sdRanges");
    if (sd_ranges) {
        cJSON *sd_ranges_local = NULL;
        if (!cJSON_IsArray(sd_ranges)) {
            ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sd_ranges]");
            goto end;
        }

        sd_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(sd_ranges_local, sd_ranges) {
            if (!cJSON_IsObject(sd_ranges_local)) {
                ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sd_ranges]");
                goto end;
            }
            OpenAPI_sd_range_t *sd_rangesItem = OpenAPI_sd_range_parseFromJSON(sd_ranges_local);
            if (!sd_rangesItem) {
                ogs_error("No sd_rangesItem");
                goto end;
            }
            OpenAPI_list_add(sd_rangesList, sd_rangesItem);
        }
    }

    wildcard_sd = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "wildcardSd");
    if (wildcard_sd) {
    if (!cJSON_IsString(wildcard_sd)) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [wildcard_sd]");
        goto end;
    }
    wildcard_sdVariable = OpenAPI_wildcard_sdext_snssai_FromString(wildcard_sd->valuestring);
    }

    ext_snssai_local_var = OpenAPI_ext_snssai_create (
        
        sst->valuedouble,
        sd && !cJSON_IsNull(sd) ? ogs_strdup(sd->valuestring) : NULL,
        sd_ranges ? sd_rangesList : NULL,
        wildcard_sd ? wildcard_sdVariable : 0
    );

    return ext_snssai_local_var;
end:
    if (sd_rangesList) {
        OpenAPI_list_for_each(sd_rangesList, node) {
            OpenAPI_sd_range_free(node->data);
        }
        OpenAPI_list_free(sd_rangesList);
        sd_rangesList = NULL;
    }
    return NULL;
}

OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_copy(OpenAPI_ext_snssai_t *dst, OpenAPI_ext_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ext_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ext_snssai_convertToJSON() failed");
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

    OpenAPI_ext_snssai_free(dst);
    dst = OpenAPI_ext_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

