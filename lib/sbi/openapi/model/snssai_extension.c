
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_extension.h"

char *OpenAPI_wildcard_sdsnssai_extension_ToString(OpenAPI_snssai_extension_wildcard_sd_e wildcard_sd)
{
    const char *wildcard_sdArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(wildcard_sdArray) / sizeof(wildcard_sdArray[0]);
    if (wildcard_sd < sizeofArray)
        return (char *)wildcard_sdArray[wildcard_sd];
    else
        return (char *)"Unknown";
}

OpenAPI_snssai_extension_wildcard_sd_e OpenAPI_wildcard_sdsnssai_extension_FromString(char* wildcard_sd)
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
OpenAPI_snssai_extension_t *OpenAPI_snssai_extension_create(
    OpenAPI_list_t *sd_ranges,
    OpenAPI_snssai_extension_wildcard_sd_e wildcard_sd
)
{
    OpenAPI_snssai_extension_t *snssai_extension_local_var = ogs_malloc(sizeof(OpenAPI_snssai_extension_t));
    ogs_assert(snssai_extension_local_var);

    snssai_extension_local_var->sd_ranges = sd_ranges;
    snssai_extension_local_var->wildcard_sd = wildcard_sd;

    return snssai_extension_local_var;
}

void OpenAPI_snssai_extension_free(OpenAPI_snssai_extension_t *snssai_extension)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snssai_extension) {
        return;
    }
    if (snssai_extension->sd_ranges) {
        OpenAPI_list_for_each(snssai_extension->sd_ranges, node) {
            OpenAPI_sd_range_free(node->data);
        }
        OpenAPI_list_free(snssai_extension->sd_ranges);
        snssai_extension->sd_ranges = NULL;
    }
    ogs_free(snssai_extension);
}

cJSON *OpenAPI_snssai_extension_convertToJSON(OpenAPI_snssai_extension_t *snssai_extension)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snssai_extension == NULL) {
        ogs_error("OpenAPI_snssai_extension_convertToJSON() failed [SnssaiExtension]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (snssai_extension->sd_ranges) {
    cJSON *sd_rangesList = cJSON_AddArrayToObject(item, "sdRanges");
    if (sd_rangesList == NULL) {
        ogs_error("OpenAPI_snssai_extension_convertToJSON() failed [sd_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(snssai_extension->sd_ranges, node) {
        cJSON *itemLocal = OpenAPI_sd_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_snssai_extension_convertToJSON() failed [sd_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(sd_rangesList, itemLocal);
    }
    }

    if (snssai_extension->wildcard_sd != OpenAPI_snssai_extension_WILDCARDSD_NULL) {
    if (cJSON_AddStringToObject(item, "wildcardSd", OpenAPI_wildcard_sdsnssai_extension_ToString(snssai_extension->wildcard_sd)) == NULL) {
        ogs_error("OpenAPI_snssai_extension_convertToJSON() failed [wildcard_sd]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_snssai_extension_t *OpenAPI_snssai_extension_parseFromJSON(cJSON *snssai_extensionJSON)
{
    OpenAPI_snssai_extension_t *snssai_extension_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sd_ranges = NULL;
    OpenAPI_list_t *sd_rangesList = NULL;
    cJSON *wildcard_sd = NULL;
    OpenAPI_snssai_extension_wildcard_sd_e wildcard_sdVariable = 0;
    sd_ranges = cJSON_GetObjectItemCaseSensitive(snssai_extensionJSON, "sdRanges");
    if (sd_ranges) {
        cJSON *sd_ranges_local = NULL;
        if (!cJSON_IsArray(sd_ranges)) {
            ogs_error("OpenAPI_snssai_extension_parseFromJSON() failed [sd_ranges]");
            goto end;
        }

        sd_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(sd_ranges_local, sd_ranges) {
            if (!cJSON_IsObject(sd_ranges_local)) {
                ogs_error("OpenAPI_snssai_extension_parseFromJSON() failed [sd_ranges]");
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

    wildcard_sd = cJSON_GetObjectItemCaseSensitive(snssai_extensionJSON, "wildcardSd");
    if (wildcard_sd) {
    if (!cJSON_IsString(wildcard_sd)) {
        ogs_error("OpenAPI_snssai_extension_parseFromJSON() failed [wildcard_sd]");
        goto end;
    }
    wildcard_sdVariable = OpenAPI_wildcard_sdsnssai_extension_FromString(wildcard_sd->valuestring);
    }

    snssai_extension_local_var = OpenAPI_snssai_extension_create (
        sd_ranges ? sd_rangesList : NULL,
        wildcard_sd ? wildcard_sdVariable : 0
    );

    return snssai_extension_local_var;
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

OpenAPI_snssai_extension_t *OpenAPI_snssai_extension_copy(OpenAPI_snssai_extension_t *dst, OpenAPI_snssai_extension_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_extension_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_extension_convertToJSON() failed");
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

    OpenAPI_snssai_extension_free(dst);
    dst = OpenAPI_snssai_extension_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

