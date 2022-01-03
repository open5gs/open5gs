
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_ran_information.h"

OpenAPI_n2_ran_information_t *OpenAPI_n2_ran_information_create(
    OpenAPI_n2_info_content_t *n2_info_content
)
{
    OpenAPI_n2_ran_information_t *n2_ran_information_local_var = ogs_malloc(sizeof(OpenAPI_n2_ran_information_t));
    ogs_assert(n2_ran_information_local_var);

    n2_ran_information_local_var->n2_info_content = n2_info_content;

    return n2_ran_information_local_var;
}

void OpenAPI_n2_ran_information_free(OpenAPI_n2_ran_information_t *n2_ran_information)
{
    if (NULL == n2_ran_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_n2_info_content_free(n2_ran_information->n2_info_content);
    ogs_free(n2_ran_information);
}

cJSON *OpenAPI_n2_ran_information_convertToJSON(OpenAPI_n2_ran_information_t *n2_ran_information)
{
    cJSON *item = NULL;

    if (n2_ran_information == NULL) {
        ogs_error("OpenAPI_n2_ran_information_convertToJSON() failed [N2RanInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *n2_info_content_local_JSON = OpenAPI_n2_info_content_convertToJSON(n2_ran_information->n2_info_content);
    if (n2_info_content_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_ran_information_convertToJSON() failed [n2_info_content]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InfoContent", n2_info_content_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_ran_information_convertToJSON() failed [n2_info_content]");
        goto end;
    }

end:
    return item;
}

OpenAPI_n2_ran_information_t *OpenAPI_n2_ran_information_parseFromJSON(cJSON *n2_ran_informationJSON)
{
    OpenAPI_n2_ran_information_t *n2_ran_information_local_var = NULL;
    cJSON *n2_info_content = cJSON_GetObjectItemCaseSensitive(n2_ran_informationJSON, "n2InfoContent");
    if (!n2_info_content) {
        ogs_error("OpenAPI_n2_ran_information_parseFromJSON() failed [n2_info_content]");
        goto end;
    }

    OpenAPI_n2_info_content_t *n2_info_content_local_nonprim = NULL;
    n2_info_content_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(n2_info_content);

    n2_ran_information_local_var = OpenAPI_n2_ran_information_create (
        n2_info_content_local_nonprim
    );

    return n2_ran_information_local_var;
end:
    return NULL;
}

OpenAPI_n2_ran_information_t *OpenAPI_n2_ran_information_copy(OpenAPI_n2_ran_information_t *dst, OpenAPI_n2_ran_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_ran_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_ran_information_convertToJSON() failed");
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

    OpenAPI_n2_ran_information_free(dst);
    dst = OpenAPI_n2_ran_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

