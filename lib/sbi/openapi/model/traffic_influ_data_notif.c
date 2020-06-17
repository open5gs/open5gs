
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influ_data_notif.h"

OpenAPI_traffic_influ_data_notif_t *OpenAPI_traffic_influ_data_notif_create(
    char *res_uri,
    OpenAPI_traffic_influ_data_t *traffic_influ_data
    )
{
    OpenAPI_traffic_influ_data_notif_t *traffic_influ_data_notif_local_var = OpenAPI_malloc(sizeof(OpenAPI_traffic_influ_data_notif_t));
    if (!traffic_influ_data_notif_local_var) {
        return NULL;
    }
    traffic_influ_data_notif_local_var->res_uri = res_uri;
    traffic_influ_data_notif_local_var->traffic_influ_data = traffic_influ_data;

    return traffic_influ_data_notif_local_var;
}

void OpenAPI_traffic_influ_data_notif_free(OpenAPI_traffic_influ_data_notif_t *traffic_influ_data_notif)
{
    if (NULL == traffic_influ_data_notif) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(traffic_influ_data_notif->res_uri);
    OpenAPI_traffic_influ_data_free(traffic_influ_data_notif->traffic_influ_data);
    ogs_free(traffic_influ_data_notif);
}

cJSON *OpenAPI_traffic_influ_data_notif_convertToJSON(OpenAPI_traffic_influ_data_notif_t *traffic_influ_data_notif)
{
    cJSON *item = NULL;

    if (traffic_influ_data_notif == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_notif_convertToJSON() failed [TrafficInfluDataNotif]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influ_data_notif->res_uri) {
        if (cJSON_AddStringToObject(item, "resUri", traffic_influ_data_notif->res_uri) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_notif_convertToJSON() failed [res_uri]");
            goto end;
        }
    }

    if (traffic_influ_data_notif->traffic_influ_data) {
        cJSON *traffic_influ_data_local_JSON = OpenAPI_traffic_influ_data_convertToJSON(traffic_influ_data_notif->traffic_influ_data);
        if (traffic_influ_data_local_JSON == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_notif_convertToJSON() failed [traffic_influ_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "trafficInfluData", traffic_influ_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_notif_convertToJSON() failed [traffic_influ_data]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_traffic_influ_data_notif_t *OpenAPI_traffic_influ_data_notif_parseFromJSON(cJSON *traffic_influ_data_notifJSON)
{
    OpenAPI_traffic_influ_data_notif_t *traffic_influ_data_notif_local_var = NULL;
    cJSON *res_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_notifJSON, "resUri");

    if (res_uri) {
        if (!cJSON_IsString(res_uri)) {
            ogs_error("OpenAPI_traffic_influ_data_notif_parseFromJSON() failed [res_uri]");
            goto end;
        }
    }

    cJSON *traffic_influ_data = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_notifJSON, "trafficInfluData");

    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_nonprim = NULL;
    if (traffic_influ_data) {
        traffic_influ_data_local_nonprim = OpenAPI_traffic_influ_data_parseFromJSON(traffic_influ_data);
    }

    traffic_influ_data_notif_local_var = OpenAPI_traffic_influ_data_notif_create (
        res_uri ? ogs_strdup(res_uri->valuestring) : NULL,
        traffic_influ_data ? traffic_influ_data_local_nonprim : NULL
        );

    return traffic_influ_data_notif_local_var;
end:
    return NULL;
}

OpenAPI_traffic_influ_data_notif_t *OpenAPI_traffic_influ_data_notif_copy(OpenAPI_traffic_influ_data_notif_t *dst, OpenAPI_traffic_influ_data_notif_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influ_data_notif_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influ_data_notif_convertToJSON() failed");
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

    OpenAPI_traffic_influ_data_notif_free(dst);
    dst = OpenAPI_traffic_influ_data_notif_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

