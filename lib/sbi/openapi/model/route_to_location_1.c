
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "route_to_location_1.h"

OpenAPI_route_to_location_1_t *OpenAPI_route_to_location_1_create(
    char *dnai,
    bool is_route_info_null,
    OpenAPI_route_information_1_t *route_info,
    bool is_route_prof_id_null,
    char *route_prof_id
)
{
    OpenAPI_route_to_location_1_t *route_to_location_1_local_var = ogs_malloc(sizeof(OpenAPI_route_to_location_1_t));
    ogs_assert(route_to_location_1_local_var);

    route_to_location_1_local_var->dnai = dnai;
    route_to_location_1_local_var->is_route_info_null = is_route_info_null;
    route_to_location_1_local_var->route_info = route_info;
    route_to_location_1_local_var->is_route_prof_id_null = is_route_prof_id_null;
    route_to_location_1_local_var->route_prof_id = route_prof_id;

    return route_to_location_1_local_var;
}

void OpenAPI_route_to_location_1_free(OpenAPI_route_to_location_1_t *route_to_location_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == route_to_location_1) {
        return;
    }
    if (route_to_location_1->dnai) {
        ogs_free(route_to_location_1->dnai);
        route_to_location_1->dnai = NULL;
    }
    if (route_to_location_1->route_info) {
        OpenAPI_route_information_1_free(route_to_location_1->route_info);
        route_to_location_1->route_info = NULL;
    }
    if (route_to_location_1->route_prof_id) {
        ogs_free(route_to_location_1->route_prof_id);
        route_to_location_1->route_prof_id = NULL;
    }
    ogs_free(route_to_location_1);
}

cJSON *OpenAPI_route_to_location_1_convertToJSON(OpenAPI_route_to_location_1_t *route_to_location_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (route_to_location_1 == NULL) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [RouteToLocation_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!route_to_location_1->dnai) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [dnai]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnai", route_to_location_1->dnai) == NULL) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [dnai]");
        goto end;
    }

    if (route_to_location_1->route_info) {
    cJSON *route_info_local_JSON = OpenAPI_route_information_1_convertToJSON(route_to_location_1->route_info);
    if (route_info_local_JSON == NULL) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [route_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "routeInfo", route_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [route_info]");
        goto end;
    }
    } else if (route_to_location_1->is_route_info_null) {
        if (cJSON_AddNullToObject(item, "routeInfo") == NULL) {
            ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [route_info]");
            goto end;
        }
    }

    if (route_to_location_1->route_prof_id) {
    if (cJSON_AddStringToObject(item, "routeProfId", route_to_location_1->route_prof_id) == NULL) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [route_prof_id]");
        goto end;
    }
    } else if (route_to_location_1->is_route_prof_id_null) {
        if (cJSON_AddNullToObject(item, "routeProfId") == NULL) {
            ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed [route_prof_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_route_to_location_1_t *OpenAPI_route_to_location_1_parseFromJSON(cJSON *route_to_location_1JSON)
{
    OpenAPI_route_to_location_1_t *route_to_location_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnai = NULL;
    cJSON *route_info = NULL;
    OpenAPI_route_information_1_t *route_info_local_nonprim = NULL;
    cJSON *route_prof_id = NULL;
    dnai = cJSON_GetObjectItemCaseSensitive(route_to_location_1JSON, "dnai");
    if (!dnai) {
        ogs_error("OpenAPI_route_to_location_1_parseFromJSON() failed [dnai]");
        goto end;
    }
    if (!cJSON_IsString(dnai)) {
        ogs_error("OpenAPI_route_to_location_1_parseFromJSON() failed [dnai]");
        goto end;
    }

    route_info = cJSON_GetObjectItemCaseSensitive(route_to_location_1JSON, "routeInfo");
    if (route_info) {
    if (!cJSON_IsNull(route_info)) {
    route_info_local_nonprim = OpenAPI_route_information_1_parseFromJSON(route_info);
    if (!route_info_local_nonprim) {
        ogs_error("OpenAPI_route_information_1_parseFromJSON failed [route_info]");
        goto end;
    }
    }
    }

    route_prof_id = cJSON_GetObjectItemCaseSensitive(route_to_location_1JSON, "routeProfId");
    if (route_prof_id) {
    if (!cJSON_IsNull(route_prof_id)) {
    if (!cJSON_IsString(route_prof_id) && !cJSON_IsNull(route_prof_id)) {
        ogs_error("OpenAPI_route_to_location_1_parseFromJSON() failed [route_prof_id]");
        goto end;
    }
    }
    }

    route_to_location_1_local_var = OpenAPI_route_to_location_1_create (
        ogs_strdup(dnai->valuestring),
        route_info && cJSON_IsNull(route_info) ? true : false,
        route_info ? route_info_local_nonprim : NULL,
        route_prof_id && cJSON_IsNull(route_prof_id) ? true : false,
        route_prof_id && !cJSON_IsNull(route_prof_id) ? ogs_strdup(route_prof_id->valuestring) : NULL
    );

    return route_to_location_1_local_var;
end:
    if (route_info_local_nonprim) {
        OpenAPI_route_information_1_free(route_info_local_nonprim);
        route_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_route_to_location_1_t *OpenAPI_route_to_location_1_copy(OpenAPI_route_to_location_1_t *dst, OpenAPI_route_to_location_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_route_to_location_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_route_to_location_1_convertToJSON() failed");
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

    OpenAPI_route_to_location_1_free(dst);
    dst = OpenAPI_route_to_location_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

