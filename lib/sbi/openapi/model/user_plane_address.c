
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "user_plane_address.h"

OpenAPI_user_plane_address_t *OpenAPI_user_plane_address_create(
char *value
)
{
    OpenAPI_user_plane_address_t *user_plane_address_local_var = ogs_malloc(sizeof(OpenAPI_user_plane_address_t));
    ogs_assert(user_plane_address_local_var);

    user_plane_address_local_var->value = value;

    return user_plane_address_local_var;
}

void OpenAPI_user_plane_address_free(OpenAPI_user_plane_address_t *user_plane_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == user_plane_address) {
        return;
    }
    if (user_plane_address->value) {
        ogs_free(user_plane_address->value);
        user_plane_address->value = NULL;
    }
    ogs_free(user_plane_address);
}

cJSON *OpenAPI_user_plane_address_convertToJSON(OpenAPI_user_plane_address_t *user_plane_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (user_plane_address == NULL) {
        ogs_error("OpenAPI_user_plane_address_convertToJSON() failed [UserPlaneAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_user_plane_address_t *OpenAPI_user_plane_address_parseFromJSON(cJSON *user_plane_addressJSON)
{
    OpenAPI_user_plane_address_t *user_plane_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    user_plane_address_local_var = OpenAPI_user_plane_address_create (
        NULL
    );

    return user_plane_address_local_var;
end:
    return NULL;
}

OpenAPI_user_plane_address_t *OpenAPI_user_plane_address_copy(OpenAPI_user_plane_address_t *dst, OpenAPI_user_plane_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_user_plane_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_user_plane_address_convertToJSON() failed");
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

    OpenAPI_user_plane_address_free(dst);
    dst = OpenAPI_user_plane_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

