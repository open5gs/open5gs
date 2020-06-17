
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "user_location.h"

OpenAPI_user_location_t *OpenAPI_user_location_create(
    OpenAPI_eutra_location_t *eutra_location,
    OpenAPI_nr_location_t *nr_location,
    OpenAPI_n3ga_location_t *n3ga_location
    )
{
    OpenAPI_user_location_t *user_location_local_var = OpenAPI_malloc(sizeof(OpenAPI_user_location_t));
    if (!user_location_local_var) {
        return NULL;
    }
    user_location_local_var->eutra_location = eutra_location;
    user_location_local_var->nr_location = nr_location;
    user_location_local_var->n3ga_location = n3ga_location;

    return user_location_local_var;
}

void OpenAPI_user_location_free(OpenAPI_user_location_t *user_location)
{
    if (NULL == user_location) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_eutra_location_free(user_location->eutra_location);
    OpenAPI_nr_location_free(user_location->nr_location);
    OpenAPI_n3ga_location_free(user_location->n3ga_location);
    ogs_free(user_location);
}

cJSON *OpenAPI_user_location_convertToJSON(OpenAPI_user_location_t *user_location)
{
    cJSON *item = NULL;

    if (user_location == NULL) {
        ogs_error("OpenAPI_user_location_convertToJSON() failed [UserLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (user_location->eutra_location) {
        cJSON *eutra_location_local_JSON = OpenAPI_eutra_location_convertToJSON(user_location->eutra_location);
        if (eutra_location_local_JSON == NULL) {
            ogs_error("OpenAPI_user_location_convertToJSON() failed [eutra_location]");
            goto end;
        }
        cJSON_AddItemToObject(item, "eutraLocation", eutra_location_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_user_location_convertToJSON() failed [eutra_location]");
            goto end;
        }
    }

    if (user_location->nr_location) {
        cJSON *nr_location_local_JSON = OpenAPI_nr_location_convertToJSON(user_location->nr_location);
        if (nr_location_local_JSON == NULL) {
            ogs_error("OpenAPI_user_location_convertToJSON() failed [nr_location]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nrLocation", nr_location_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_user_location_convertToJSON() failed [nr_location]");
            goto end;
        }
    }

    if (user_location->n3ga_location) {
        cJSON *n3ga_location_local_JSON = OpenAPI_n3ga_location_convertToJSON(user_location->n3ga_location);
        if (n3ga_location_local_JSON == NULL) {
            ogs_error("OpenAPI_user_location_convertToJSON() failed [n3ga_location]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n3gaLocation", n3ga_location_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_user_location_convertToJSON() failed [n3ga_location]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_user_location_t *OpenAPI_user_location_parseFromJSON(cJSON *user_locationJSON)
{
    OpenAPI_user_location_t *user_location_local_var = NULL;
    cJSON *eutra_location = cJSON_GetObjectItemCaseSensitive(user_locationJSON, "eutraLocation");

    OpenAPI_eutra_location_t *eutra_location_local_nonprim = NULL;
    if (eutra_location) {
        eutra_location_local_nonprim = OpenAPI_eutra_location_parseFromJSON(eutra_location);
    }

    cJSON *nr_location = cJSON_GetObjectItemCaseSensitive(user_locationJSON, "nrLocation");

    OpenAPI_nr_location_t *nr_location_local_nonprim = NULL;
    if (nr_location) {
        nr_location_local_nonprim = OpenAPI_nr_location_parseFromJSON(nr_location);
    }

    cJSON *n3ga_location = cJSON_GetObjectItemCaseSensitive(user_locationJSON, "n3gaLocation");

    OpenAPI_n3ga_location_t *n3ga_location_local_nonprim = NULL;
    if (n3ga_location) {
        n3ga_location_local_nonprim = OpenAPI_n3ga_location_parseFromJSON(n3ga_location);
    }

    user_location_local_var = OpenAPI_user_location_create (
        eutra_location ? eutra_location_local_nonprim : NULL,
        nr_location ? nr_location_local_nonprim : NULL,
        n3ga_location ? n3ga_location_local_nonprim : NULL
        );

    return user_location_local_var;
end:
    return NULL;
}

OpenAPI_user_location_t *OpenAPI_user_location_copy(OpenAPI_user_location_t *dst, OpenAPI_user_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_user_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_user_location_convertToJSON() failed");
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

    OpenAPI_user_location_free(dst);
    dst = OpenAPI_user_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

