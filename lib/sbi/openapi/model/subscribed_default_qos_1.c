
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_default_qos_1.h"

OpenAPI_subscribed_default_qos_1_t *OpenAPI_subscribed_default_qos_1_create(
    int _5qi,
    OpenAPI_arp_1_t *arp,
    bool is_priority_level,
    int priority_level
)
{
    OpenAPI_subscribed_default_qos_1_t *subscribed_default_qos_1_local_var = ogs_malloc(sizeof(OpenAPI_subscribed_default_qos_1_t));
    ogs_assert(subscribed_default_qos_1_local_var);

    subscribed_default_qos_1_local_var->_5qi = _5qi;
    subscribed_default_qos_1_local_var->arp = arp;
    subscribed_default_qos_1_local_var->is_priority_level = is_priority_level;
    subscribed_default_qos_1_local_var->priority_level = priority_level;

    return subscribed_default_qos_1_local_var;
}

void OpenAPI_subscribed_default_qos_1_free(OpenAPI_subscribed_default_qos_1_t *subscribed_default_qos_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscribed_default_qos_1) {
        return;
    }
    if (subscribed_default_qos_1->arp) {
        OpenAPI_arp_1_free(subscribed_default_qos_1->arp);
        subscribed_default_qos_1->arp = NULL;
    }
    ogs_free(subscribed_default_qos_1);
}

cJSON *OpenAPI_subscribed_default_qos_1_convertToJSON(OpenAPI_subscribed_default_qos_1_t *subscribed_default_qos_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscribed_default_qos_1 == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed [SubscribedDefaultQos_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "5qi", subscribed_default_qos_1->_5qi) == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed [_5qi]");
        goto end;
    }

    if (!subscribed_default_qos_1->arp) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed [arp]");
        return NULL;
    }
    cJSON *arp_local_JSON = OpenAPI_arp_1_convertToJSON(subscribed_default_qos_1->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed [arp]");
        goto end;
    }

    if (subscribed_default_qos_1->is_priority_level) {
    if (cJSON_AddNumberToObject(item, "priorityLevel", subscribed_default_qos_1->priority_level) == NULL) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed [priority_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_subscribed_default_qos_1_t *OpenAPI_subscribed_default_qos_1_parseFromJSON(cJSON *subscribed_default_qos_1JSON)
{
    OpenAPI_subscribed_default_qos_1_t *subscribed_default_qos_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_5qi = NULL;
    cJSON *arp = NULL;
    OpenAPI_arp_1_t *arp_local_nonprim = NULL;
    cJSON *priority_level = NULL;
    _5qi = cJSON_GetObjectItemCaseSensitive(subscribed_default_qos_1JSON, "5qi");
    if (!_5qi) {
        ogs_error("OpenAPI_subscribed_default_qos_1_parseFromJSON() failed [_5qi]");
        goto end;
    }
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_subscribed_default_qos_1_parseFromJSON() failed [_5qi]");
        goto end;
    }

    arp = cJSON_GetObjectItemCaseSensitive(subscribed_default_qos_1JSON, "arp");
    if (!arp) {
        ogs_error("OpenAPI_subscribed_default_qos_1_parseFromJSON() failed [arp]");
        goto end;
    }
    arp_local_nonprim = OpenAPI_arp_1_parseFromJSON(arp);
    if (!arp_local_nonprim) {
        ogs_error("OpenAPI_arp_1_parseFromJSON failed [arp]");
        goto end;
    }

    priority_level = cJSON_GetObjectItemCaseSensitive(subscribed_default_qos_1JSON, "priorityLevel");
    if (priority_level) {
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_subscribed_default_qos_1_parseFromJSON() failed [priority_level]");
        goto end;
    }
    }

    subscribed_default_qos_1_local_var = OpenAPI_subscribed_default_qos_1_create (
        
        _5qi->valuedouble,
        arp_local_nonprim,
        priority_level ? true : false,
        priority_level ? priority_level->valuedouble : 0
    );

    return subscribed_default_qos_1_local_var;
end:
    if (arp_local_nonprim) {
        OpenAPI_arp_1_free(arp_local_nonprim);
        arp_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_subscribed_default_qos_1_t *OpenAPI_subscribed_default_qos_1_copy(OpenAPI_subscribed_default_qos_1_t *dst, OpenAPI_subscribed_default_qos_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscribed_default_qos_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscribed_default_qos_1_convertToJSON() failed");
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

    OpenAPI_subscribed_default_qos_1_free(dst);
    dst = OpenAPI_subscribed_default_qos_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

