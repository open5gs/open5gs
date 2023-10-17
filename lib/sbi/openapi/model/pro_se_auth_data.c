
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_auth_data.h"

OpenAPI_pro_se_auth_data_t *OpenAPI_pro_se_auth_data_create(
)
{
    OpenAPI_pro_se_auth_data_t *pro_se_auth_data_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_auth_data_t));
    ogs_assert(pro_se_auth_data_local_var);


    return pro_se_auth_data_local_var;
}

void OpenAPI_pro_se_auth_data_free(OpenAPI_pro_se_auth_data_t *pro_se_auth_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_auth_data) {
        return;
    }
    ogs_free(pro_se_auth_data);
}

cJSON *OpenAPI_pro_se_auth_data_convertToJSON(OpenAPI_pro_se_auth_data_t *pro_se_auth_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_auth_data == NULL) {
        ogs_error("OpenAPI_pro_se_auth_data_convertToJSON() failed [ProSeAuthData]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_pro_se_auth_data_t *OpenAPI_pro_se_auth_data_parseFromJSON(cJSON *pro_se_auth_dataJSON)
{
    OpenAPI_pro_se_auth_data_t *pro_se_auth_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    pro_se_auth_data_local_var = OpenAPI_pro_se_auth_data_create (
    );

    return pro_se_auth_data_local_var;
end:
    return NULL;
}

OpenAPI_pro_se_auth_data_t *OpenAPI_pro_se_auth_data_copy(OpenAPI_pro_se_auth_data_t *dst, OpenAPI_pro_se_auth_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_auth_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_auth_data_convertToJSON() failed");
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

    OpenAPI_pro_se_auth_data_free(dst);
    dst = OpenAPI_pro_se_auth_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

