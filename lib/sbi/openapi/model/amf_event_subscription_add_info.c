
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_subscription_add_info.h"

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_create(
    OpenAPI_list_t *binding_info,
    OpenAPI_nf_type_e subscribing_nf_type
)
{
    OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_subscription_add_info_t));
    ogs_assert(amf_event_subscription_add_info_local_var);

    amf_event_subscription_add_info_local_var->binding_info = binding_info;
    amf_event_subscription_add_info_local_var->subscribing_nf_type = subscribing_nf_type;

    return amf_event_subscription_add_info_local_var;
}

void OpenAPI_amf_event_subscription_add_info_free(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info)
{
    if (NULL == amf_event_subscription_add_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(amf_event_subscription_add_info->binding_info, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(amf_event_subscription_add_info->binding_info);
    ogs_free(amf_event_subscription_add_info);
}

cJSON *OpenAPI_amf_event_subscription_add_info_convertToJSON(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info)
{
    cJSON *item = NULL;

    if (amf_event_subscription_add_info == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [AmfEventSubscriptionAddInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (amf_event_subscription_add_info->binding_info) {
    cJSON *binding_info = cJSON_AddArrayToObject(item, "bindingInfo");
    if (binding_info == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [binding_info]");
        goto end;
    }

    OpenAPI_lnode_t *binding_info_node;
    OpenAPI_list_for_each(amf_event_subscription_add_info->binding_info, binding_info_node)  {
    if (cJSON_AddStringToObject(binding_info, "", (char*)binding_info_node->data) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [binding_info]");
        goto end;
    }
                    }
    }

    if (amf_event_subscription_add_info->subscribing_nf_type) {
    if (cJSON_AddStringToObject(item, "subscribingNfType", OpenAPI_nf_type_ToString(amf_event_subscription_add_info->subscribing_nf_type)) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [subscribing_nf_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_parseFromJSON(cJSON *amf_event_subscription_add_infoJSON)
{
    OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info_local_var = NULL;
    cJSON *binding_info = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "bindingInfo");

    OpenAPI_list_t *binding_infoList;
    if (binding_info) {
    cJSON *binding_info_local;
    if (!cJSON_IsArray(binding_info)) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [binding_info]");
        goto end;
    }
    binding_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(binding_info_local, binding_info) {
    if (!cJSON_IsString(binding_info_local)) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [binding_info]");
        goto end;
    }
    OpenAPI_list_add(binding_infoList , ogs_strdup(binding_info_local->valuestring));
    }
    }

    cJSON *subscribing_nf_type = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "subscribingNfType");

    OpenAPI_nf_type_e subscribing_nf_typeVariable;
    if (subscribing_nf_type) {
    if (!cJSON_IsString(subscribing_nf_type)) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [subscribing_nf_type]");
        goto end;
    }
    subscribing_nf_typeVariable = OpenAPI_nf_type_FromString(subscribing_nf_type->valuestring);
    }

    amf_event_subscription_add_info_local_var = OpenAPI_amf_event_subscription_add_info_create (
        binding_info ? binding_infoList : NULL,
        subscribing_nf_type ? subscribing_nf_typeVariable : 0
    );

    return amf_event_subscription_add_info_local_var;
end:
    return NULL;
}

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_copy(OpenAPI_amf_event_subscription_add_info_t *dst, OpenAPI_amf_event_subscription_add_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_subscription_add_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed");
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

    OpenAPI_amf_event_subscription_add_info_free(dst);
    dst = OpenAPI_amf_event_subscription_add_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

