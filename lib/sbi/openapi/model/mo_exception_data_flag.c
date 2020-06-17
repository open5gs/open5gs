
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mo_exception_data_flag.h"

OpenAPI_mo_exception_data_flag_t *OpenAPI_mo_exception_data_flag_create(
    )
{
    OpenAPI_mo_exception_data_flag_t *mo_exception_data_flag_local_var = OpenAPI_malloc(sizeof(OpenAPI_mo_exception_data_flag_t));
    if (!mo_exception_data_flag_local_var) {
        return NULL;
    }

    return mo_exception_data_flag_local_var;
}

void OpenAPI_mo_exception_data_flag_free(OpenAPI_mo_exception_data_flag_t *mo_exception_data_flag)
{
    if (NULL == mo_exception_data_flag) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(mo_exception_data_flag);
}

cJSON *OpenAPI_mo_exception_data_flag_convertToJSON(OpenAPI_mo_exception_data_flag_t *mo_exception_data_flag)
{
    cJSON *item = NULL;

    if (mo_exception_data_flag == NULL) {
        ogs_error("OpenAPI_mo_exception_data_flag_convertToJSON() failed [MoExceptionDataFlag]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_mo_exception_data_flag_t *OpenAPI_mo_exception_data_flag_parseFromJSON(cJSON *mo_exception_data_flagJSON)
{
    OpenAPI_mo_exception_data_flag_t *mo_exception_data_flag_local_var = NULL;
    mo_exception_data_flag_local_var = OpenAPI_mo_exception_data_flag_create (
        );

    return mo_exception_data_flag_local_var;
end:
    return NULL;
}

OpenAPI_mo_exception_data_flag_t *OpenAPI_mo_exception_data_flag_copy(OpenAPI_mo_exception_data_flag_t *dst, OpenAPI_mo_exception_data_flag_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mo_exception_data_flag_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mo_exception_data_flag_convertToJSON() failed");
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

    OpenAPI_mo_exception_data_flag_free(dst);
    dst = OpenAPI_mo_exception_data_flag_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

