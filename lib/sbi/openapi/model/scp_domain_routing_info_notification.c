
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_domain_routing_info_notification.h"

OpenAPI_scp_domain_routing_info_notification_t *OpenAPI_scp_domain_routing_info_notification_create(
    OpenAPI_scp_domain_routing_information_t *routing_info,
    bool is_local_ind,
    int local_ind
)
{
    OpenAPI_scp_domain_routing_info_notification_t *scp_domain_routing_info_notification_local_var = ogs_malloc(sizeof(OpenAPI_scp_domain_routing_info_notification_t));
    ogs_assert(scp_domain_routing_info_notification_local_var);

    scp_domain_routing_info_notification_local_var->routing_info = routing_info;
    scp_domain_routing_info_notification_local_var->is_local_ind = is_local_ind;
    scp_domain_routing_info_notification_local_var->local_ind = local_ind;

    return scp_domain_routing_info_notification_local_var;
}

void OpenAPI_scp_domain_routing_info_notification_free(OpenAPI_scp_domain_routing_info_notification_t *scp_domain_routing_info_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_domain_routing_info_notification) {
        return;
    }
    if (scp_domain_routing_info_notification->routing_info) {
        OpenAPI_scp_domain_routing_information_free(scp_domain_routing_info_notification->routing_info);
        scp_domain_routing_info_notification->routing_info = NULL;
    }
    ogs_free(scp_domain_routing_info_notification);
}

cJSON *OpenAPI_scp_domain_routing_info_notification_convertToJSON(OpenAPI_scp_domain_routing_info_notification_t *scp_domain_routing_info_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_domain_routing_info_notification == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_convertToJSON() failed [ScpDomainRoutingInfoNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!scp_domain_routing_info_notification->routing_info) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_convertToJSON() failed [routing_info]");
        return NULL;
    }
    cJSON *routing_info_local_JSON = OpenAPI_scp_domain_routing_information_convertToJSON(scp_domain_routing_info_notification->routing_info);
    if (routing_info_local_JSON == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_convertToJSON() failed [routing_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "routingInfo", routing_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_convertToJSON() failed [routing_info]");
        goto end;
    }

    if (scp_domain_routing_info_notification->is_local_ind) {
    if (cJSON_AddBoolToObject(item, "localInd", scp_domain_routing_info_notification->local_ind) == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_convertToJSON() failed [local_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_scp_domain_routing_info_notification_t *OpenAPI_scp_domain_routing_info_notification_parseFromJSON(cJSON *scp_domain_routing_info_notificationJSON)
{
    OpenAPI_scp_domain_routing_info_notification_t *scp_domain_routing_info_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *routing_info = NULL;
    OpenAPI_scp_domain_routing_information_t *routing_info_local_nonprim = NULL;
    cJSON *local_ind = NULL;
    routing_info = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_info_notificationJSON, "routingInfo");
    if (!routing_info) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_parseFromJSON() failed [routing_info]");
        goto end;
    }
    routing_info_local_nonprim = OpenAPI_scp_domain_routing_information_parseFromJSON(routing_info);
    if (!routing_info_local_nonprim) {
        ogs_error("OpenAPI_scp_domain_routing_information_parseFromJSON failed [routing_info]");
        goto end;
    }

    local_ind = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_info_notificationJSON, "localInd");
    if (local_ind) {
    if (!cJSON_IsBool(local_ind)) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_parseFromJSON() failed [local_ind]");
        goto end;
    }
    }

    scp_domain_routing_info_notification_local_var = OpenAPI_scp_domain_routing_info_notification_create (
        routing_info_local_nonprim,
        local_ind ? true : false,
        local_ind ? local_ind->valueint : 0
    );

    return scp_domain_routing_info_notification_local_var;
end:
    if (routing_info_local_nonprim) {
        OpenAPI_scp_domain_routing_information_free(routing_info_local_nonprim);
        routing_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_scp_domain_routing_info_notification_t *OpenAPI_scp_domain_routing_info_notification_copy(OpenAPI_scp_domain_routing_info_notification_t *dst, OpenAPI_scp_domain_routing_info_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_domain_routing_info_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_domain_routing_info_notification_convertToJSON() failed");
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

    OpenAPI_scp_domain_routing_info_notification_free(dst);
    dst = OpenAPI_scp_domain_routing_info_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

