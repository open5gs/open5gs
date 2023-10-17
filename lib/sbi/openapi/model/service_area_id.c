
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_area_id.h"

OpenAPI_service_area_id_t *OpenAPI_service_area_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *lac,
    char *sac
)
{
    OpenAPI_service_area_id_t *service_area_id_local_var = ogs_malloc(sizeof(OpenAPI_service_area_id_t));
    ogs_assert(service_area_id_local_var);

    service_area_id_local_var->plmn_id = plmn_id;
    service_area_id_local_var->lac = lac;
    service_area_id_local_var->sac = sac;

    return service_area_id_local_var;
}

void OpenAPI_service_area_id_free(OpenAPI_service_area_id_t *service_area_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_area_id) {
        return;
    }
    if (service_area_id->plmn_id) {
        OpenAPI_plmn_id_free(service_area_id->plmn_id);
        service_area_id->plmn_id = NULL;
    }
    if (service_area_id->lac) {
        ogs_free(service_area_id->lac);
        service_area_id->lac = NULL;
    }
    if (service_area_id->sac) {
        ogs_free(service_area_id->sac);
        service_area_id->sac = NULL;
    }
    ogs_free(service_area_id);
}

cJSON *OpenAPI_service_area_id_convertToJSON(OpenAPI_service_area_id_t *service_area_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_area_id == NULL) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [ServiceAreaId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_area_id->plmn_id) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(service_area_id->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!service_area_id->lac) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [lac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "lac", service_area_id->lac) == NULL) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [lac]");
        goto end;
    }

    if (!service_area_id->sac) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [sac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sac", service_area_id->sac) == NULL) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed [sac]");
        goto end;
    }

end:
    return item;
}

OpenAPI_service_area_id_t *OpenAPI_service_area_id_parseFromJSON(cJSON *service_area_idJSON)
{
    OpenAPI_service_area_id_t *service_area_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *lac = NULL;
    cJSON *sac = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(service_area_idJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    lac = cJSON_GetObjectItemCaseSensitive(service_area_idJSON, "lac");
    if (!lac) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON() failed [lac]");
        goto end;
    }
    if (!cJSON_IsString(lac)) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON() failed [lac]");
        goto end;
    }

    sac = cJSON_GetObjectItemCaseSensitive(service_area_idJSON, "sac");
    if (!sac) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON() failed [sac]");
        goto end;
    }
    if (!cJSON_IsString(sac)) {
        ogs_error("OpenAPI_service_area_id_parseFromJSON() failed [sac]");
        goto end;
    }

    service_area_id_local_var = OpenAPI_service_area_id_create (
        plmn_id_local_nonprim,
        ogs_strdup(lac->valuestring),
        ogs_strdup(sac->valuestring)
    );

    return service_area_id_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_service_area_id_t *OpenAPI_service_area_id_copy(OpenAPI_service_area_id_t *dst, OpenAPI_service_area_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_area_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_area_id_convertToJSON() failed");
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

    OpenAPI_service_area_id_free(dst);
    dst = OpenAPI_service_area_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

