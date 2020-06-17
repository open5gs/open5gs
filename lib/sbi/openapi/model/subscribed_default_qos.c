
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_default_qos.h"

OpenAPI_subscribed_default_qos_t *OpenAPI_subscribed_default_qos_create(
    int _5qi,
    OpenAPI_arp_t *arp,
    int priority_level
    )
{
    OpenAPI_subscribed_default_qos_t *subscribed_default_qos_local_var = OpenAPI_malloc(sizeof(OpenAPI_subscribed_default_qos_t));
    if (!subscribed_default_qos_local_var) {
        return NULL;
    }
    subscribed_default_qos_local_var->_5qi = _5qi;
    subscribed_default_qos_local_var->arp = arp;
    subscribed_default_qos_local_var->priority_level = priority_level;

    return subscribed_default_qos_local_var;
}

void OpenAPI_subscribed_default_qos_free(OpenAPI_subscribed_default_qos_t *subscribed_default_qos)
{
    if (NULL == subscribed_default_qos) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_arp_free(subscribed_default_qos->arp);
    ogs_free(subscribed_default_qos);
}

cJSON *OpenAPI_subscribed_default_qos_convertToJSON(OpenAPI_subscribed_default_qos_t *subscribed_default_qos)
{
    cJSON *item = NULL;

    if (subscribed_default_qos == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [SubscribedDefaultQos]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!subscribed_default_qos->_5qi) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [_5qi]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "5qi", subscribed_default_qos->_5qi) == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [_5qi]");
        goto end;
    }

    if (!subscribed_default_qos->arp) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(subscribed_default_qos->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [arp]");
        goto end;
    }

    if (subscribed_default_qos->priority_level) {
        if (cJSON_AddNumberToObject(item, "priorityLevel", subscribed_default_qos->priority_level) == NULL) {
            ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed [priority_level]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_subscribed_default_qos_t *OpenAPI_subscribed_default_qos_parseFromJSON(cJSON *subscribed_default_qosJSON)
{
    OpenAPI_subscribed_default_qos_t *subscribed_default_qos_local_var = NULL;
    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(subscribed_default_qosJSON, "5qi");
    if (!_5qi) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }


    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }

    cJSON *arp = cJSON_GetObjectItemCaseSensitive(subscribed_default_qosJSON, "arp");
    if (!arp) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON() failed [arp]");
        goto end;
    }

    OpenAPI_arp_t *arp_local_nonprim = NULL;

    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);

    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(subscribed_default_qosJSON, "priorityLevel");

    if (priority_level) {
        if (!cJSON_IsNumber(priority_level)) {
            ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON() failed [priority_level]");
            goto end;
        }
    }

    subscribed_default_qos_local_var = OpenAPI_subscribed_default_qos_create (
        _5qi->valuedouble,
        arp_local_nonprim,
        priority_level ? priority_level->valuedouble : 0
        );

    return subscribed_default_qos_local_var;
end:
    return NULL;
}

OpenAPI_subscribed_default_qos_t *OpenAPI_subscribed_default_qos_copy(OpenAPI_subscribed_default_qos_t *dst, OpenAPI_subscribed_default_qos_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscribed_default_qos_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscribed_default_qos_convertToJSON() failed");
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

    OpenAPI_subscribed_default_qos_free(dst);
    dst = OpenAPI_subscribed_default_qos_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

