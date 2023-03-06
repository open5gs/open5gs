
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "area_1.h"

OpenAPI_area_1_t *OpenAPI_area_1_create(
    OpenAPI_list_t *tacs,
    char *area_code
)
{
    OpenAPI_area_1_t *area_1_local_var = ogs_malloc(sizeof(OpenAPI_area_1_t));
    ogs_assert(area_1_local_var);

    area_1_local_var->tacs = tacs;
    area_1_local_var->area_code = area_code;

    return area_1_local_var;
}

void OpenAPI_area_1_free(OpenAPI_area_1_t *area_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == area_1) {
        return;
    }
    if (area_1->tacs) {
        OpenAPI_list_for_each(area_1->tacs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(area_1->tacs);
        area_1->tacs = NULL;
    }
    if (area_1->area_code) {
        ogs_free(area_1->area_code);
        area_1->area_code = NULL;
    }
    ogs_free(area_1);
}

cJSON *OpenAPI_area_1_convertToJSON(OpenAPI_area_1_t *area_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (area_1 == NULL) {
        ogs_error("OpenAPI_area_1_convertToJSON() failed [Area_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (area_1->tacs) {
    cJSON *tacsList = cJSON_AddArrayToObject(item, "tacs");
    if (tacsList == NULL) {
        ogs_error("OpenAPI_area_1_convertToJSON() failed [tacs]");
        goto end;
    }
    OpenAPI_list_for_each(area_1->tacs, node) {
        if (cJSON_AddStringToObject(tacsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_area_1_convertToJSON() failed [tacs]");
            goto end;
        }
    }
    }

    if (area_1->area_code) {
    if (cJSON_AddStringToObject(item, "areaCode", area_1->area_code) == NULL) {
        ogs_error("OpenAPI_area_1_convertToJSON() failed [area_code]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_area_1_t *OpenAPI_area_1_parseFromJSON(cJSON *area_1JSON)
{
    OpenAPI_area_1_t *area_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tacs = NULL;
    OpenAPI_list_t *tacsList = NULL;
    cJSON *area_code = NULL;
    tacs = cJSON_GetObjectItemCaseSensitive(area_1JSON, "tacs");
    if (tacs) {
        cJSON *tacs_local = NULL;
        if (!cJSON_IsArray(tacs)) {
            ogs_error("OpenAPI_area_1_parseFromJSON() failed [tacs]");
            goto end;
        }

        tacsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tacs_local, tacs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tacs_local)) {
                ogs_error("OpenAPI_area_1_parseFromJSON() failed [tacs]");
                goto end;
            }
            OpenAPI_list_add(tacsList, ogs_strdup(tacs_local->valuestring));
        }
    }

    area_code = cJSON_GetObjectItemCaseSensitive(area_1JSON, "areaCode");
    if (area_code) {
    if (!cJSON_IsString(area_code) && !cJSON_IsNull(area_code)) {
        ogs_error("OpenAPI_area_1_parseFromJSON() failed [area_code]");
        goto end;
    }
    }

    area_1_local_var = OpenAPI_area_1_create (
        tacs ? tacsList : NULL,
        area_code && !cJSON_IsNull(area_code) ? ogs_strdup(area_code->valuestring) : NULL
    );

    return area_1_local_var;
end:
    if (tacsList) {
        OpenAPI_list_for_each(tacsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tacsList);
        tacsList = NULL;
    }
    return NULL;
}

OpenAPI_area_1_t *OpenAPI_area_1_copy(OpenAPI_area_1_t *dst, OpenAPI_area_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_area_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_area_1_convertToJSON() failed");
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

    OpenAPI_area_1_free(dst);
    dst = OpenAPI_area_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

