
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_load_level_information.h"

OpenAPI_slice_load_level_information_t *OpenAPI_slice_load_level_information_create(
    int load_level_information,
    OpenAPI_list_t *snssais
)
{
    OpenAPI_slice_load_level_information_t *slice_load_level_information_local_var = ogs_malloc(sizeof(OpenAPI_slice_load_level_information_t));
    ogs_assert(slice_load_level_information_local_var);

    slice_load_level_information_local_var->load_level_information = load_level_information;
    slice_load_level_information_local_var->snssais = snssais;

    return slice_load_level_information_local_var;
}

void OpenAPI_slice_load_level_information_free(OpenAPI_slice_load_level_information_t *slice_load_level_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_load_level_information) {
        return;
    }
    if (slice_load_level_information->snssais) {
        OpenAPI_list_for_each(slice_load_level_information->snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_load_level_information->snssais);
        slice_load_level_information->snssais = NULL;
    }
    ogs_free(slice_load_level_information);
}

cJSON *OpenAPI_slice_load_level_information_convertToJSON(OpenAPI_slice_load_level_information_t *slice_load_level_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_load_level_information == NULL) {
        ogs_error("OpenAPI_slice_load_level_information_convertToJSON() failed [SliceLoadLevelInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "loadLevelInformation", slice_load_level_information->load_level_information) == NULL) {
        ogs_error("OpenAPI_slice_load_level_information_convertToJSON() failed [load_level_information]");
        goto end;
    }

    if (!slice_load_level_information->snssais) {
        ogs_error("OpenAPI_slice_load_level_information_convertToJSON() failed [snssais]");
        return NULL;
    }
    cJSON *snssaisList = cJSON_AddArrayToObject(item, "snssais");
    if (snssaisList == NULL) {
        ogs_error("OpenAPI_slice_load_level_information_convertToJSON() failed [snssais]");
        goto end;
    }
    OpenAPI_list_for_each(slice_load_level_information->snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_slice_load_level_information_convertToJSON() failed [snssais]");
            goto end;
        }
        cJSON_AddItemToArray(snssaisList, itemLocal);
    }

end:
    return item;
}

OpenAPI_slice_load_level_information_t *OpenAPI_slice_load_level_information_parseFromJSON(cJSON *slice_load_level_informationJSON)
{
    OpenAPI_slice_load_level_information_t *slice_load_level_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *load_level_information = NULL;
    cJSON *snssais = NULL;
    OpenAPI_list_t *snssaisList = NULL;
    load_level_information = cJSON_GetObjectItemCaseSensitive(slice_load_level_informationJSON, "loadLevelInformation");
    if (!load_level_information) {
        ogs_error("OpenAPI_slice_load_level_information_parseFromJSON() failed [load_level_information]");
        goto end;
    }
    if (!cJSON_IsNumber(load_level_information)) {
        ogs_error("OpenAPI_slice_load_level_information_parseFromJSON() failed [load_level_information]");
        goto end;
    }

    snssais = cJSON_GetObjectItemCaseSensitive(slice_load_level_informationJSON, "snssais");
    if (!snssais) {
        ogs_error("OpenAPI_slice_load_level_information_parseFromJSON() failed [snssais]");
        goto end;
    }
        cJSON *snssais_local = NULL;
        if (!cJSON_IsArray(snssais)) {
            ogs_error("OpenAPI_slice_load_level_information_parseFromJSON() failed [snssais]");
            goto end;
        }

        snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssais_local, snssais) {
            if (!cJSON_IsObject(snssais_local)) {
                ogs_error("OpenAPI_slice_load_level_information_parseFromJSON() failed [snssais]");
                goto end;
            }
            OpenAPI_snssai_t *snssaisItem = OpenAPI_snssai_parseFromJSON(snssais_local);
            if (!snssaisItem) {
                ogs_error("No snssaisItem");
                goto end;
            }
            OpenAPI_list_add(snssaisList, snssaisItem);
        }

    slice_load_level_information_local_var = OpenAPI_slice_load_level_information_create (
        
        load_level_information->valuedouble,
        snssaisList
    );

    return slice_load_level_information_local_var;
end:
    if (snssaisList) {
        OpenAPI_list_for_each(snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssaisList);
        snssaisList = NULL;
    }
    return NULL;
}

OpenAPI_slice_load_level_information_t *OpenAPI_slice_load_level_information_copy(OpenAPI_slice_load_level_information_t *dst, OpenAPI_slice_load_level_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_load_level_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_load_level_information_convertToJSON() failed");
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

    OpenAPI_slice_load_level_information_free(dst);
    dst = OpenAPI_slice_load_level_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

