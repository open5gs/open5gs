
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "routing_area_id_range.h"

OpenAPI_routing_area_id_range_t *OpenAPI_routing_area_id_range_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *start_lac,
    char *end_lac,
    char *start_rac,
    char *end_rac
)
{
    OpenAPI_routing_area_id_range_t *routing_area_id_range_local_var = ogs_malloc(sizeof(OpenAPI_routing_area_id_range_t));
    ogs_assert(routing_area_id_range_local_var);

    routing_area_id_range_local_var->plmn_id = plmn_id;
    routing_area_id_range_local_var->start_lac = start_lac;
    routing_area_id_range_local_var->end_lac = end_lac;
    routing_area_id_range_local_var->start_rac = start_rac;
    routing_area_id_range_local_var->end_rac = end_rac;

    return routing_area_id_range_local_var;
}

void OpenAPI_routing_area_id_range_free(OpenAPI_routing_area_id_range_t *routing_area_id_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == routing_area_id_range) {
        return;
    }
    if (routing_area_id_range->plmn_id) {
        OpenAPI_plmn_id_free(routing_area_id_range->plmn_id);
        routing_area_id_range->plmn_id = NULL;
    }
    if (routing_area_id_range->start_lac) {
        ogs_free(routing_area_id_range->start_lac);
        routing_area_id_range->start_lac = NULL;
    }
    if (routing_area_id_range->end_lac) {
        ogs_free(routing_area_id_range->end_lac);
        routing_area_id_range->end_lac = NULL;
    }
    if (routing_area_id_range->start_rac) {
        ogs_free(routing_area_id_range->start_rac);
        routing_area_id_range->start_rac = NULL;
    }
    if (routing_area_id_range->end_rac) {
        ogs_free(routing_area_id_range->end_rac);
        routing_area_id_range->end_rac = NULL;
    }
    ogs_free(routing_area_id_range);
}

cJSON *OpenAPI_routing_area_id_range_convertToJSON(OpenAPI_routing_area_id_range_t *routing_area_id_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (routing_area_id_range == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [RoutingAreaIdRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!routing_area_id_range->plmn_id) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(routing_area_id_range->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!routing_area_id_range->start_lac) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [start_lac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "startLac", routing_area_id_range->start_lac) == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [start_lac]");
        goto end;
    }

    if (!routing_area_id_range->end_lac) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [end_lac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "endLac", routing_area_id_range->end_lac) == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [end_lac]");
        goto end;
    }

    if (!routing_area_id_range->start_rac) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [start_rac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "startRac", routing_area_id_range->start_rac) == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [start_rac]");
        goto end;
    }

    if (!routing_area_id_range->end_rac) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [end_rac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "endRac", routing_area_id_range->end_rac) == NULL) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed [end_rac]");
        goto end;
    }

end:
    return item;
}

OpenAPI_routing_area_id_range_t *OpenAPI_routing_area_id_range_parseFromJSON(cJSON *routing_area_id_rangeJSON)
{
    OpenAPI_routing_area_id_range_t *routing_area_id_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *start_lac = NULL;
    cJSON *end_lac = NULL;
    cJSON *start_rac = NULL;
    cJSON *end_rac = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(routing_area_id_rangeJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    start_lac = cJSON_GetObjectItemCaseSensitive(routing_area_id_rangeJSON, "startLac");
    if (!start_lac) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [start_lac]");
        goto end;
    }
    if (!cJSON_IsString(start_lac)) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [start_lac]");
        goto end;
    }

    end_lac = cJSON_GetObjectItemCaseSensitive(routing_area_id_rangeJSON, "endLac");
    if (!end_lac) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [end_lac]");
        goto end;
    }
    if (!cJSON_IsString(end_lac)) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [end_lac]");
        goto end;
    }

    start_rac = cJSON_GetObjectItemCaseSensitive(routing_area_id_rangeJSON, "startRac");
    if (!start_rac) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [start_rac]");
        goto end;
    }
    if (!cJSON_IsString(start_rac)) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [start_rac]");
        goto end;
    }

    end_rac = cJSON_GetObjectItemCaseSensitive(routing_area_id_rangeJSON, "endRac");
    if (!end_rac) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [end_rac]");
        goto end;
    }
    if (!cJSON_IsString(end_rac)) {
        ogs_error("OpenAPI_routing_area_id_range_parseFromJSON() failed [end_rac]");
        goto end;
    }

    routing_area_id_range_local_var = OpenAPI_routing_area_id_range_create (
        plmn_id_local_nonprim,
        ogs_strdup(start_lac->valuestring),
        ogs_strdup(end_lac->valuestring),
        ogs_strdup(start_rac->valuestring),
        ogs_strdup(end_rac->valuestring)
    );

    return routing_area_id_range_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_routing_area_id_range_t *OpenAPI_routing_area_id_range_copy(OpenAPI_routing_area_id_range_t *dst, OpenAPI_routing_area_id_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_routing_area_id_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_routing_area_id_range_convertToJSON() failed");
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

    OpenAPI_routing_area_id_range_free(dst);
    dst = OpenAPI_routing_area_id_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

