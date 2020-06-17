
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "area.h"

OpenAPI_area_t *OpenAPI_area_create(
    OpenAPI_list_t *tacs,
    char *area_code
    )
{
    OpenAPI_area_t *area_local_var = OpenAPI_malloc(sizeof(OpenAPI_area_t));
    if (!area_local_var) {
        return NULL;
    }
    area_local_var->tacs = tacs;
    area_local_var->area_code = area_code;

    return area_local_var;
}

void OpenAPI_area_free(OpenAPI_area_t *area)
{
    if (NULL == area) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(area->tacs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(area->tacs);
    ogs_free(area->area_code);
    ogs_free(area);
}

cJSON *OpenAPI_area_convertToJSON(OpenAPI_area_t *area)
{
    cJSON *item = NULL;

    if (area == NULL) {
        ogs_error("OpenAPI_area_convertToJSON() failed [Area]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (area->tacs) {
        cJSON *tacs = cJSON_AddArrayToObject(item, "tacs");
        if (tacs == NULL) {
            ogs_error("OpenAPI_area_convertToJSON() failed [tacs]");
            goto end;
        }

        OpenAPI_lnode_t *tacs_node;
        OpenAPI_list_for_each(area->tacs, tacs_node)  {
            if (cJSON_AddStringToObject(tacs, "", (char*)tacs_node->data) == NULL) {
                ogs_error("OpenAPI_area_convertToJSON() failed [tacs]");
                goto end;
            }
        }
    }

    if (area->area_code) {
        if (cJSON_AddStringToObject(item, "areaCode", area->area_code) == NULL) {
            ogs_error("OpenAPI_area_convertToJSON() failed [area_code]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_area_t *OpenAPI_area_parseFromJSON(cJSON *areaJSON)
{
    OpenAPI_area_t *area_local_var = NULL;
    cJSON *tacs = cJSON_GetObjectItemCaseSensitive(areaJSON, "tacs");

    OpenAPI_list_t *tacsList;
    if (tacs) {
        cJSON *tacs_local;
        if (!cJSON_IsArray(tacs)) {
            ogs_error("OpenAPI_area_parseFromJSON() failed [tacs]");
            goto end;
        }
        tacsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tacs_local, tacs) {
            if (!cJSON_IsString(tacs_local)) {
                ogs_error("OpenAPI_area_parseFromJSON() failed [tacs]");
                goto end;
            }
            OpenAPI_list_add(tacsList, ogs_strdup(tacs_local->valuestring));
        }
    }

    cJSON *area_code = cJSON_GetObjectItemCaseSensitive(areaJSON, "areaCode");

    if (area_code) {
        if (!cJSON_IsString(area_code)) {
            ogs_error("OpenAPI_area_parseFromJSON() failed [area_code]");
            goto end;
        }
    }

    area_local_var = OpenAPI_area_create (
        tacs ? tacsList : NULL,
        area_code ? ogs_strdup(area_code->valuestring) : NULL
        );

    return area_local_var;
end:
    return NULL;
}

OpenAPI_area_t *OpenAPI_area_copy(OpenAPI_area_t *dst, OpenAPI_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_area_convertToJSON() failed");
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

    OpenAPI_area_free(dst);
    dst = OpenAPI_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

