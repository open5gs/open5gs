
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_info_for_registration.h"

OpenAPI_slice_info_for_registration_t *OpenAPI_slice_info_for_registration_create(
    OpenAPI_list_t *subscribed_nssai,
    OpenAPI_allowed_nssai_t *allowed_nssai_current_access,
    OpenAPI_allowed_nssai_t *allowed_nssai_other_access,
    OpenAPI_list_t *s_nssai_for_mapping,
    OpenAPI_list_t *requested_nssai,
    bool is_default_configured_snssai_ind,
    int default_configured_snssai_ind,
    OpenAPI_list_t *mapping_of_nssai,
    bool is_request_mapping,
    int request_mapping
)
{
    OpenAPI_slice_info_for_registration_t *slice_info_for_registration_local_var = ogs_malloc(sizeof(OpenAPI_slice_info_for_registration_t));
    ogs_assert(slice_info_for_registration_local_var);

    slice_info_for_registration_local_var->subscribed_nssai = subscribed_nssai;
    slice_info_for_registration_local_var->allowed_nssai_current_access = allowed_nssai_current_access;
    slice_info_for_registration_local_var->allowed_nssai_other_access = allowed_nssai_other_access;
    slice_info_for_registration_local_var->s_nssai_for_mapping = s_nssai_for_mapping;
    slice_info_for_registration_local_var->requested_nssai = requested_nssai;
    slice_info_for_registration_local_var->is_default_configured_snssai_ind = is_default_configured_snssai_ind;
    slice_info_for_registration_local_var->default_configured_snssai_ind = default_configured_snssai_ind;
    slice_info_for_registration_local_var->mapping_of_nssai = mapping_of_nssai;
    slice_info_for_registration_local_var->is_request_mapping = is_request_mapping;
    slice_info_for_registration_local_var->request_mapping = request_mapping;

    return slice_info_for_registration_local_var;
}

void OpenAPI_slice_info_for_registration_free(OpenAPI_slice_info_for_registration_t *slice_info_for_registration)
{
    if (NULL == slice_info_for_registration) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(slice_info_for_registration->subscribed_nssai, node) {
        OpenAPI_subscribed_snssai_free(node->data);
    }
    OpenAPI_list_free(slice_info_for_registration->subscribed_nssai);
    OpenAPI_allowed_nssai_free(slice_info_for_registration->allowed_nssai_current_access);
    OpenAPI_allowed_nssai_free(slice_info_for_registration->allowed_nssai_other_access);
    OpenAPI_list_for_each(slice_info_for_registration->s_nssai_for_mapping, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(slice_info_for_registration->s_nssai_for_mapping);
    OpenAPI_list_for_each(slice_info_for_registration->requested_nssai, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(slice_info_for_registration->requested_nssai);
    OpenAPI_list_for_each(slice_info_for_registration->mapping_of_nssai, node) {
        OpenAPI_mapping_of_snssai_free(node->data);
    }
    OpenAPI_list_free(slice_info_for_registration->mapping_of_nssai);
    ogs_free(slice_info_for_registration);
}

cJSON *OpenAPI_slice_info_for_registration_convertToJSON(OpenAPI_slice_info_for_registration_t *slice_info_for_registration)
{
    cJSON *item = NULL;

    if (slice_info_for_registration == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [SliceInfoForRegistration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (slice_info_for_registration->subscribed_nssai) {
    cJSON *subscribed_nssaiList = cJSON_AddArrayToObject(item, "subscribedNssai");
    if (subscribed_nssaiList == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [subscribed_nssai]");
        goto end;
    }

    OpenAPI_lnode_t *subscribed_nssai_node;
    if (slice_info_for_registration->subscribed_nssai) {
        OpenAPI_list_for_each(slice_info_for_registration->subscribed_nssai, subscribed_nssai_node) {
            cJSON *itemLocal = OpenAPI_subscribed_snssai_convertToJSON(subscribed_nssai_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [subscribed_nssai]");
                goto end;
            }
            cJSON_AddItemToArray(subscribed_nssaiList, itemLocal);
        }
    }
    }

    if (slice_info_for_registration->allowed_nssai_current_access) {
    cJSON *allowed_nssai_current_access_local_JSON = OpenAPI_allowed_nssai_convertToJSON(slice_info_for_registration->allowed_nssai_current_access);
    if (allowed_nssai_current_access_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [allowed_nssai_current_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedNssaiCurrentAccess", allowed_nssai_current_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [allowed_nssai_current_access]");
        goto end;
    }
    }

    if (slice_info_for_registration->allowed_nssai_other_access) {
    cJSON *allowed_nssai_other_access_local_JSON = OpenAPI_allowed_nssai_convertToJSON(slice_info_for_registration->allowed_nssai_other_access);
    if (allowed_nssai_other_access_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [allowed_nssai_other_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedNssaiOtherAccess", allowed_nssai_other_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [allowed_nssai_other_access]");
        goto end;
    }
    }

    if (slice_info_for_registration->s_nssai_for_mapping) {
    cJSON *s_nssai_for_mappingList = cJSON_AddArrayToObject(item, "sNssaiForMapping");
    if (s_nssai_for_mappingList == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [s_nssai_for_mapping]");
        goto end;
    }

    OpenAPI_lnode_t *s_nssai_for_mapping_node;
    if (slice_info_for_registration->s_nssai_for_mapping) {
        OpenAPI_list_for_each(slice_info_for_registration->s_nssai_for_mapping, s_nssai_for_mapping_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(s_nssai_for_mapping_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [s_nssai_for_mapping]");
                goto end;
            }
            cJSON_AddItemToArray(s_nssai_for_mappingList, itemLocal);
        }
    }
    }

    if (slice_info_for_registration->requested_nssai) {
    cJSON *requested_nssaiList = cJSON_AddArrayToObject(item, "requestedNssai");
    if (requested_nssaiList == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [requested_nssai]");
        goto end;
    }

    OpenAPI_lnode_t *requested_nssai_node;
    if (slice_info_for_registration->requested_nssai) {
        OpenAPI_list_for_each(slice_info_for_registration->requested_nssai, requested_nssai_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(requested_nssai_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [requested_nssai]");
                goto end;
            }
            cJSON_AddItemToArray(requested_nssaiList, itemLocal);
        }
    }
    }

    if (slice_info_for_registration->is_default_configured_snssai_ind) {
    if (cJSON_AddBoolToObject(item, "defaultConfiguredSnssaiInd", slice_info_for_registration->default_configured_snssai_ind) == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [default_configured_snssai_ind]");
        goto end;
    }
    }

    if (slice_info_for_registration->mapping_of_nssai) {
    cJSON *mapping_of_nssaiList = cJSON_AddArrayToObject(item, "mappingOfNssai");
    if (mapping_of_nssaiList == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [mapping_of_nssai]");
        goto end;
    }

    OpenAPI_lnode_t *mapping_of_nssai_node;
    if (slice_info_for_registration->mapping_of_nssai) {
        OpenAPI_list_for_each(slice_info_for_registration->mapping_of_nssai, mapping_of_nssai_node) {
            cJSON *itemLocal = OpenAPI_mapping_of_snssai_convertToJSON(mapping_of_nssai_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [mapping_of_nssai]");
                goto end;
            }
            cJSON_AddItemToArray(mapping_of_nssaiList, itemLocal);
        }
    }
    }

    if (slice_info_for_registration->is_request_mapping) {
    if (cJSON_AddBoolToObject(item, "requestMapping", slice_info_for_registration->request_mapping) == NULL) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed [request_mapping]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_slice_info_for_registration_t *OpenAPI_slice_info_for_registration_parseFromJSON(cJSON *slice_info_for_registrationJSON)
{
    OpenAPI_slice_info_for_registration_t *slice_info_for_registration_local_var = NULL;
    cJSON *subscribed_nssai = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "subscribedNssai");

    OpenAPI_list_t *subscribed_nssaiList;
    if (subscribed_nssai) {
    cJSON *subscribed_nssai_local_nonprimitive;
    if (!cJSON_IsArray(subscribed_nssai)){
        ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [subscribed_nssai]");
        goto end;
    }

    subscribed_nssaiList = OpenAPI_list_create();

    cJSON_ArrayForEach(subscribed_nssai_local_nonprimitive, subscribed_nssai ) {
        if (!cJSON_IsObject(subscribed_nssai_local_nonprimitive)) {
            ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [subscribed_nssai]");
            goto end;
        }
        OpenAPI_subscribed_snssai_t *subscribed_nssaiItem = OpenAPI_subscribed_snssai_parseFromJSON(subscribed_nssai_local_nonprimitive);

        if (!subscribed_nssaiItem) {
            ogs_error("No subscribed_nssaiItem");
            OpenAPI_list_free(subscribed_nssaiList);
            goto end;
        }

        OpenAPI_list_add(subscribed_nssaiList, subscribed_nssaiItem);
    }
    }

    cJSON *allowed_nssai_current_access = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "allowedNssaiCurrentAccess");

    OpenAPI_allowed_nssai_t *allowed_nssai_current_access_local_nonprim = NULL;
    if (allowed_nssai_current_access) {
    allowed_nssai_current_access_local_nonprim = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai_current_access);
    }

    cJSON *allowed_nssai_other_access = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "allowedNssaiOtherAccess");

    OpenAPI_allowed_nssai_t *allowed_nssai_other_access_local_nonprim = NULL;
    if (allowed_nssai_other_access) {
    allowed_nssai_other_access_local_nonprim = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai_other_access);
    }

    cJSON *s_nssai_for_mapping = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "sNssaiForMapping");

    OpenAPI_list_t *s_nssai_for_mappingList;
    if (s_nssai_for_mapping) {
    cJSON *s_nssai_for_mapping_local_nonprimitive;
    if (!cJSON_IsArray(s_nssai_for_mapping)){
        ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [s_nssai_for_mapping]");
        goto end;
    }

    s_nssai_for_mappingList = OpenAPI_list_create();

    cJSON_ArrayForEach(s_nssai_for_mapping_local_nonprimitive, s_nssai_for_mapping ) {
        if (!cJSON_IsObject(s_nssai_for_mapping_local_nonprimitive)) {
            ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [s_nssai_for_mapping]");
            goto end;
        }
        OpenAPI_snssai_t *s_nssai_for_mappingItem = OpenAPI_snssai_parseFromJSON(s_nssai_for_mapping_local_nonprimitive);

        if (!s_nssai_for_mappingItem) {
            ogs_error("No s_nssai_for_mappingItem");
            OpenAPI_list_free(s_nssai_for_mappingList);
            goto end;
        }

        OpenAPI_list_add(s_nssai_for_mappingList, s_nssai_for_mappingItem);
    }
    }

    cJSON *requested_nssai = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "requestedNssai");

    OpenAPI_list_t *requested_nssaiList;
    if (requested_nssai) {
    cJSON *requested_nssai_local_nonprimitive;
    if (!cJSON_IsArray(requested_nssai)){
        ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [requested_nssai]");
        goto end;
    }

    requested_nssaiList = OpenAPI_list_create();

    cJSON_ArrayForEach(requested_nssai_local_nonprimitive, requested_nssai ) {
        if (!cJSON_IsObject(requested_nssai_local_nonprimitive)) {
            ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [requested_nssai]");
            goto end;
        }
        OpenAPI_snssai_t *requested_nssaiItem = OpenAPI_snssai_parseFromJSON(requested_nssai_local_nonprimitive);

        if (!requested_nssaiItem) {
            ogs_error("No requested_nssaiItem");
            OpenAPI_list_free(requested_nssaiList);
            goto end;
        }

        OpenAPI_list_add(requested_nssaiList, requested_nssaiItem);
    }
    }

    cJSON *default_configured_snssai_ind = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "defaultConfiguredSnssaiInd");

    if (default_configured_snssai_ind) {
    if (!cJSON_IsBool(default_configured_snssai_ind)) {
        ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [default_configured_snssai_ind]");
        goto end;
    }
    }

    cJSON *mapping_of_nssai = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "mappingOfNssai");

    OpenAPI_list_t *mapping_of_nssaiList;
    if (mapping_of_nssai) {
    cJSON *mapping_of_nssai_local_nonprimitive;
    if (!cJSON_IsArray(mapping_of_nssai)){
        ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [mapping_of_nssai]");
        goto end;
    }

    mapping_of_nssaiList = OpenAPI_list_create();

    cJSON_ArrayForEach(mapping_of_nssai_local_nonprimitive, mapping_of_nssai ) {
        if (!cJSON_IsObject(mapping_of_nssai_local_nonprimitive)) {
            ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [mapping_of_nssai]");
            goto end;
        }
        OpenAPI_mapping_of_snssai_t *mapping_of_nssaiItem = OpenAPI_mapping_of_snssai_parseFromJSON(mapping_of_nssai_local_nonprimitive);

        if (!mapping_of_nssaiItem) {
            ogs_error("No mapping_of_nssaiItem");
            OpenAPI_list_free(mapping_of_nssaiList);
            goto end;
        }

        OpenAPI_list_add(mapping_of_nssaiList, mapping_of_nssaiItem);
    }
    }

    cJSON *request_mapping = cJSON_GetObjectItemCaseSensitive(slice_info_for_registrationJSON, "requestMapping");

    if (request_mapping) {
    if (!cJSON_IsBool(request_mapping)) {
        ogs_error("OpenAPI_slice_info_for_registration_parseFromJSON() failed [request_mapping]");
        goto end;
    }
    }

    slice_info_for_registration_local_var = OpenAPI_slice_info_for_registration_create (
        subscribed_nssai ? subscribed_nssaiList : NULL,
        allowed_nssai_current_access ? allowed_nssai_current_access_local_nonprim : NULL,
        allowed_nssai_other_access ? allowed_nssai_other_access_local_nonprim : NULL,
        s_nssai_for_mapping ? s_nssai_for_mappingList : NULL,
        requested_nssai ? requested_nssaiList : NULL,
        default_configured_snssai_ind ? true : false,
        default_configured_snssai_ind ? default_configured_snssai_ind->valueint : 0,
        mapping_of_nssai ? mapping_of_nssaiList : NULL,
        request_mapping ? true : false,
        request_mapping ? request_mapping->valueint : 0
    );

    return slice_info_for_registration_local_var;
end:
    return NULL;
}

OpenAPI_slice_info_for_registration_t *OpenAPI_slice_info_for_registration_copy(OpenAPI_slice_info_for_registration_t *dst, OpenAPI_slice_info_for_registration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_info_for_registration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_info_for_registration_convertToJSON() failed");
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

    OpenAPI_slice_info_for_registration_free(dst);
    dst = OpenAPI_slice_info_for_registration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

