
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ext_snssai.h"

OpenAPI_ext_snssai_t *OpenAPI_ext_snssai_create(
    int sst,
    char *sd,
    OpenAPI_list_t *sd_ranges,
    bool is_wildcard_sd,
    int wildcard_sd
)
{
    OpenAPI_ext_snssai_t *ext_snssai_local_var = ogs_malloc(sizeof(OpenAPI_ext_snssai_t));
    ogs_assert(ext_snssai_local_var);

    ext_snssai_local_var->sst = sst;
    ext_snssai_local_var->sd = sd;
    ext_snssai_local_var->sd_ranges = sd_ranges;
    ext_snssai_local_var->is_wildcard_sd = is_wildcard_sd;
    ext_snssai_local_var->wildcard_sd = wildcard_sd;

    return ext_snssai_local_var;
}

void OpenAPI_ext_snssai_free(OpenAPI_ext_snssai_t *ext_snssai)
{
    if (NULL == ext_snssai) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ext_snssai->sd);
    OpenAPI_list_for_each(ext_snssai->sd_ranges, node) {
        OpenAPI_sd_range_free(node->data);
    }
    OpenAPI_list_free(ext_snssai->sd_ranges);
    ogs_free(ext_snssai);
}

cJSON *OpenAPI_ext_snssai_convertToJSON(OpenAPI_ext_snssai_t *ext_snssai)
{
    cJSON *item = NULL;

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

    OpenAPI_lnode_t *sd_ranges_node;
    if (ext_snssai->sd_ranges) {
        OpenAPI_list_for_each(ext_snssai->sd_ranges, sd_ranges_node) {
            cJSON *itemLocal = OpenAPI_sd_range_convertToJSON(sd_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ext_snssai_convertToJSON() failed [sd_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(sd_rangesList, itemLocal);
        }
    }
    }

    if (ext_snssai->is_wildcard_sd) {
    if (cJSON_AddBoolToObject(item, "wildcardSd", ext_snssai->wildcard_sd) == NULL) {
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
    cJSON *sst = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "sst");
    if (!sst) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sst]");
        goto end;
    }

    if (!cJSON_IsNumber(sst)) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sst]");
        goto end;
    }

    cJSON *sd = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "sd");

    if (sd) {
    if (!cJSON_IsString(sd)) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sd]");
        goto end;
    }
    }

    cJSON *sd_ranges = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "sdRanges");

    OpenAPI_list_t *sd_rangesList;
    if (sd_ranges) {
    cJSON *sd_ranges_local_nonprimitive;
    if (!cJSON_IsArray(sd_ranges)){
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sd_ranges]");
        goto end;
    }

    sd_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(sd_ranges_local_nonprimitive, sd_ranges ) {
        if (!cJSON_IsObject(sd_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [sd_ranges]");
            goto end;
        }
        OpenAPI_sd_range_t *sd_rangesItem = OpenAPI_sd_range_parseFromJSON(sd_ranges_local_nonprimitive);

        if (!sd_rangesItem) {
            ogs_error("No sd_rangesItem");
            OpenAPI_list_free(sd_rangesList);
            goto end;
        }

        OpenAPI_list_add(sd_rangesList, sd_rangesItem);
    }
    }

    cJSON *wildcard_sd = cJSON_GetObjectItemCaseSensitive(ext_snssaiJSON, "wildcardSd");

    if (wildcard_sd) {
    if (!cJSON_IsBool(wildcard_sd)) {
        ogs_error("OpenAPI_ext_snssai_parseFromJSON() failed [wildcard_sd]");
        goto end;
    }
    }

    ext_snssai_local_var = OpenAPI_ext_snssai_create (
        
        sst->valuedouble,
        sd ? ogs_strdup(sd->valuestring) : NULL,
        sd_ranges ? sd_rangesList : NULL,
        wildcard_sd ? true : false,
        wildcard_sd ? wildcard_sd->valueint : 0
    );

    return ext_snssai_local_var;
end:
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

