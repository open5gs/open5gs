
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_sm_gw_guidance.h"

OpenAPI_ip_sm_gw_guidance_t *OpenAPI_ip_sm_gw_guidance_create(
    int min_delivery_time,
    int recomm_delivery_time
)
{
    OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance_local_var = ogs_malloc(sizeof(OpenAPI_ip_sm_gw_guidance_t));
    ogs_assert(ip_sm_gw_guidance_local_var);

    ip_sm_gw_guidance_local_var->min_delivery_time = min_delivery_time;
    ip_sm_gw_guidance_local_var->recomm_delivery_time = recomm_delivery_time;

    return ip_sm_gw_guidance_local_var;
}

void OpenAPI_ip_sm_gw_guidance_free(OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_sm_gw_guidance) {
        return;
    }
    ogs_free(ip_sm_gw_guidance);
}

cJSON *OpenAPI_ip_sm_gw_guidance_convertToJSON(OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ip_sm_gw_guidance == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_convertToJSON() failed [IpSmGwGuidance]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "minDeliveryTime", ip_sm_gw_guidance->min_delivery_time) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_convertToJSON() failed [min_delivery_time]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "recommDeliveryTime", ip_sm_gw_guidance->recomm_delivery_time) == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_convertToJSON() failed [recomm_delivery_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ip_sm_gw_guidance_t *OpenAPI_ip_sm_gw_guidance_parseFromJSON(cJSON *ip_sm_gw_guidanceJSON)
{
    OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *min_delivery_time = NULL;
    cJSON *recomm_delivery_time = NULL;
    min_delivery_time = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_guidanceJSON, "minDeliveryTime");
    if (!min_delivery_time) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_parseFromJSON() failed [min_delivery_time]");
        goto end;
    }
    if (!cJSON_IsNumber(min_delivery_time)) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_parseFromJSON() failed [min_delivery_time]");
        goto end;
    }

    recomm_delivery_time = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_guidanceJSON, "recommDeliveryTime");
    if (!recomm_delivery_time) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_parseFromJSON() failed [recomm_delivery_time]");
        goto end;
    }
    if (!cJSON_IsNumber(recomm_delivery_time)) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_parseFromJSON() failed [recomm_delivery_time]");
        goto end;
    }

    ip_sm_gw_guidance_local_var = OpenAPI_ip_sm_gw_guidance_create (
        
        min_delivery_time->valuedouble,
        
        recomm_delivery_time->valuedouble
    );

    return ip_sm_gw_guidance_local_var;
end:
    return NULL;
}

OpenAPI_ip_sm_gw_guidance_t *OpenAPI_ip_sm_gw_guidance_copy(OpenAPI_ip_sm_gw_guidance_t *dst, OpenAPI_ip_sm_gw_guidance_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_sm_gw_guidance_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_convertToJSON() failed");
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

    OpenAPI_ip_sm_gw_guidance_free(dst);
    dst = OpenAPI_ip_sm_gw_guidance_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

