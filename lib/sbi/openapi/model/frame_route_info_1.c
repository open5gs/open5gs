
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "frame_route_info_1.h"

OpenAPI_frame_route_info_1_t *OpenAPI_frame_route_info_1_create(
    char *ipv4_mask,
    char *ipv6_prefix
)
{
    OpenAPI_frame_route_info_1_t *frame_route_info_1_local_var = ogs_malloc(sizeof(OpenAPI_frame_route_info_1_t));
    ogs_assert(frame_route_info_1_local_var);

    frame_route_info_1_local_var->ipv4_mask = ipv4_mask;
    frame_route_info_1_local_var->ipv6_prefix = ipv6_prefix;

    return frame_route_info_1_local_var;
}

void OpenAPI_frame_route_info_1_free(OpenAPI_frame_route_info_1_t *frame_route_info_1)
{
    if (NULL == frame_route_info_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(frame_route_info_1->ipv4_mask);
    ogs_free(frame_route_info_1->ipv6_prefix);
    ogs_free(frame_route_info_1);
}

cJSON *OpenAPI_frame_route_info_1_convertToJSON(OpenAPI_frame_route_info_1_t *frame_route_info_1)
{
    cJSON *item = NULL;

    if (frame_route_info_1 == NULL) {
        ogs_error("OpenAPI_frame_route_info_1_convertToJSON() failed [FrameRouteInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (frame_route_info_1->ipv4_mask) {
    if (cJSON_AddStringToObject(item, "ipv4Mask", frame_route_info_1->ipv4_mask) == NULL) {
        ogs_error("OpenAPI_frame_route_info_1_convertToJSON() failed [ipv4_mask]");
        goto end;
    }
    }

    if (frame_route_info_1->ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6Prefix", frame_route_info_1->ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_frame_route_info_1_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_frame_route_info_1_t *OpenAPI_frame_route_info_1_parseFromJSON(cJSON *frame_route_info_1JSON)
{
    OpenAPI_frame_route_info_1_t *frame_route_info_1_local_var = NULL;
    cJSON *ipv4_mask = cJSON_GetObjectItemCaseSensitive(frame_route_info_1JSON, "ipv4Mask");

    if (ipv4_mask) {
    if (!cJSON_IsString(ipv4_mask)) {
        ogs_error("OpenAPI_frame_route_info_1_parseFromJSON() failed [ipv4_mask]");
        goto end;
    }
    }

    cJSON *ipv6_prefix = cJSON_GetObjectItemCaseSensitive(frame_route_info_1JSON, "ipv6Prefix");

    if (ipv6_prefix) {
    if (!cJSON_IsString(ipv6_prefix)) {
        ogs_error("OpenAPI_frame_route_info_1_parseFromJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    frame_route_info_1_local_var = OpenAPI_frame_route_info_1_create (
        ipv4_mask ? ogs_strdup(ipv4_mask->valuestring) : NULL,
        ipv6_prefix ? ogs_strdup(ipv6_prefix->valuestring) : NULL
    );

    return frame_route_info_1_local_var;
end:
    return NULL;
}

OpenAPI_frame_route_info_1_t *OpenAPI_frame_route_info_1_copy(OpenAPI_frame_route_info_1_t *dst, OpenAPI_frame_route_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_frame_route_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_frame_route_info_1_convertToJSON() failed");
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

    OpenAPI_frame_route_info_1_free(dst);
    dst = OpenAPI_frame_route_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

