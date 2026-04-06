
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "a2x_information.h"

OpenAPI_a2x_information_t *OpenAPI_a2x_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_pol
)
{
    OpenAPI_a2x_information_t *a2x_information_local_var = ogs_malloc(sizeof(OpenAPI_a2x_information_t));
    ogs_assert(a2x_information_local_var);

    a2x_information_local_var->n2_pc5_pol = n2_pc5_pol;

    return a2x_information_local_var;
}

void OpenAPI_a2x_information_free(OpenAPI_a2x_information_t *a2x_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == a2x_information) {
        return;
    }
    if (a2x_information->n2_pc5_pol) {
        OpenAPI_n2_info_content_free(a2x_information->n2_pc5_pol);
        a2x_information->n2_pc5_pol = NULL;
    }
    ogs_free(a2x_information);
}

cJSON *OpenAPI_a2x_information_convertToJSON(OpenAPI_a2x_information_t *a2x_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (a2x_information == NULL) {
        ogs_error("OpenAPI_a2x_information_convertToJSON() failed [A2xInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (a2x_information->n2_pc5_pol) {
    cJSON *n2_pc5_pol_local_JSON = OpenAPI_n2_info_content_convertToJSON(a2x_information->n2_pc5_pol);
    if (n2_pc5_pol_local_JSON == NULL) {
        ogs_error("OpenAPI_a2x_information_convertToJSON() failed [n2_pc5_pol]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2Pc5Pol", n2_pc5_pol_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_a2x_information_convertToJSON() failed [n2_pc5_pol]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_a2x_information_t *OpenAPI_a2x_information_parseFromJSON(cJSON *a2x_informationJSON)
{
    OpenAPI_a2x_information_t *a2x_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n2_pc5_pol = NULL;
    OpenAPI_n2_info_content_t *n2_pc5_pol_local_nonprim = NULL;
    n2_pc5_pol = cJSON_GetObjectItemCaseSensitive(a2x_informationJSON, "n2Pc5Pol");
    if (n2_pc5_pol) {
    n2_pc5_pol_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(n2_pc5_pol);
    if (!n2_pc5_pol_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [n2_pc5_pol]");
        goto end;
    }
    }

    a2x_information_local_var = OpenAPI_a2x_information_create (
        n2_pc5_pol ? n2_pc5_pol_local_nonprim : NULL
    );

    return a2x_information_local_var;
end:
    if (n2_pc5_pol_local_nonprim) {
        OpenAPI_n2_info_content_free(n2_pc5_pol_local_nonprim);
        n2_pc5_pol_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_a2x_information_t *OpenAPI_a2x_information_copy(OpenAPI_a2x_information_t *dst, OpenAPI_a2x_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_a2x_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_a2x_information_convertToJSON() failed");
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

    OpenAPI_a2x_information_free(dst);
    dst = OpenAPI_a2x_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

