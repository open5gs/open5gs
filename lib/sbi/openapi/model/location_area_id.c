
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_area_id.h"

OpenAPI_location_area_id_t *OpenAPI_location_area_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *lac
)
{
    OpenAPI_location_area_id_t *location_area_id_local_var = ogs_malloc(sizeof(OpenAPI_location_area_id_t));
    ogs_assert(location_area_id_local_var);

    location_area_id_local_var->plmn_id = plmn_id;
    location_area_id_local_var->lac = lac;

    return location_area_id_local_var;
}

void OpenAPI_location_area_id_free(OpenAPI_location_area_id_t *location_area_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_area_id) {
        return;
    }
    if (location_area_id->plmn_id) {
        OpenAPI_plmn_id_free(location_area_id->plmn_id);
        location_area_id->plmn_id = NULL;
    }
    if (location_area_id->lac) {
        ogs_free(location_area_id->lac);
        location_area_id->lac = NULL;
    }
    ogs_free(location_area_id);
}

cJSON *OpenAPI_location_area_id_convertToJSON(OpenAPI_location_area_id_t *location_area_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (location_area_id == NULL) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed [LocationAreaId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!location_area_id->plmn_id) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(location_area_id->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!location_area_id->lac) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed [lac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "lac", location_area_id->lac) == NULL) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed [lac]");
        goto end;
    }

end:
    return item;
}

OpenAPI_location_area_id_t *OpenAPI_location_area_id_parseFromJSON(cJSON *location_area_idJSON)
{
    OpenAPI_location_area_id_t *location_area_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *lac = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(location_area_idJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_location_area_id_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    lac = cJSON_GetObjectItemCaseSensitive(location_area_idJSON, "lac");
    if (!lac) {
        ogs_error("OpenAPI_location_area_id_parseFromJSON() failed [lac]");
        goto end;
    }
    if (!cJSON_IsString(lac)) {
        ogs_error("OpenAPI_location_area_id_parseFromJSON() failed [lac]");
        goto end;
    }

    location_area_id_local_var = OpenAPI_location_area_id_create (
        plmn_id_local_nonprim,
        ogs_strdup(lac->valuestring)
    );

    return location_area_id_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_location_area_id_t *OpenAPI_location_area_id_copy(OpenAPI_location_area_id_t *dst, OpenAPI_location_area_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_area_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_area_id_convertToJSON() failed");
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

    OpenAPI_location_area_id_free(dst);
    dst = OpenAPI_location_area_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

