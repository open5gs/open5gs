
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_info_for_ue_configuration_update.h"

OpenAPI_slice_info_for_ue_configuration_update_t *OpenAPI_slice_info_for_ue_configuration_update_create(
    OpenAPI_list_t *subscribed_nssai,
    OpenAPI_allowed_nssai_t *allowed_nssai_current_access,
    OpenAPI_allowed_nssai_t *allowed_nssai_other_access,
    bool is_default_configured_snssai_ind,
    int default_configured_snssai_ind,
    OpenAPI_list_t *requested_nssai,
    OpenAPI_list_t *mapping_of_nssai,
    bool is_ue_sup_nssrg_ind,
    int ue_sup_nssrg_ind,
    bool is_suppress_nssrg_ind,
    int suppress_nssrg_ind,
    OpenAPI_list_t *rejected_nssai_ra,
    bool is_nsag_supported,
    int nsag_supported
)
{
    OpenAPI_slice_info_for_ue_configuration_update_t *slice_info_for_ue_configuration_update_local_var = ogs_malloc(sizeof(OpenAPI_slice_info_for_ue_configuration_update_t));
    ogs_assert(slice_info_for_ue_configuration_update_local_var);

    slice_info_for_ue_configuration_update_local_var->subscribed_nssai = subscribed_nssai;
    slice_info_for_ue_configuration_update_local_var->allowed_nssai_current_access = allowed_nssai_current_access;
    slice_info_for_ue_configuration_update_local_var->allowed_nssai_other_access = allowed_nssai_other_access;
    slice_info_for_ue_configuration_update_local_var->is_default_configured_snssai_ind = is_default_configured_snssai_ind;
    slice_info_for_ue_configuration_update_local_var->default_configured_snssai_ind = default_configured_snssai_ind;
    slice_info_for_ue_configuration_update_local_var->requested_nssai = requested_nssai;
    slice_info_for_ue_configuration_update_local_var->mapping_of_nssai = mapping_of_nssai;
    slice_info_for_ue_configuration_update_local_var->is_ue_sup_nssrg_ind = is_ue_sup_nssrg_ind;
    slice_info_for_ue_configuration_update_local_var->ue_sup_nssrg_ind = ue_sup_nssrg_ind;
    slice_info_for_ue_configuration_update_local_var->is_suppress_nssrg_ind = is_suppress_nssrg_ind;
    slice_info_for_ue_configuration_update_local_var->suppress_nssrg_ind = suppress_nssrg_ind;
    slice_info_for_ue_configuration_update_local_var->rejected_nssai_ra = rejected_nssai_ra;
    slice_info_for_ue_configuration_update_local_var->is_nsag_supported = is_nsag_supported;
    slice_info_for_ue_configuration_update_local_var->nsag_supported = nsag_supported;

    return slice_info_for_ue_configuration_update_local_var;
}

void OpenAPI_slice_info_for_ue_configuration_update_free(OpenAPI_slice_info_for_ue_configuration_update_t *slice_info_for_ue_configuration_update)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_info_for_ue_configuration_update) {
        return;
    }
    if (slice_info_for_ue_configuration_update->subscribed_nssai) {
        OpenAPI_list_for_each(slice_info_for_ue_configuration_update->subscribed_nssai, node) {
            OpenAPI_subscribed_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_info_for_ue_configuration_update->subscribed_nssai);
        slice_info_for_ue_configuration_update->subscribed_nssai = NULL;
    }
    if (slice_info_for_ue_configuration_update->allowed_nssai_current_access) {
        OpenAPI_allowed_nssai_free(slice_info_for_ue_configuration_update->allowed_nssai_current_access);
        slice_info_for_ue_configuration_update->allowed_nssai_current_access = NULL;
    }
    if (slice_info_for_ue_configuration_update->allowed_nssai_other_access) {
        OpenAPI_allowed_nssai_free(slice_info_for_ue_configuration_update->allowed_nssai_other_access);
        slice_info_for_ue_configuration_update->allowed_nssai_other_access = NULL;
    }
    if (slice_info_for_ue_configuration_update->requested_nssai) {
        OpenAPI_list_for_each(slice_info_for_ue_configuration_update->requested_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_info_for_ue_configuration_update->requested_nssai);
        slice_info_for_ue_configuration_update->requested_nssai = NULL;
    }
    if (slice_info_for_ue_configuration_update->mapping_of_nssai) {
        OpenAPI_list_for_each(slice_info_for_ue_configuration_update->mapping_of_nssai, node) {
            OpenAPI_mapping_of_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_info_for_ue_configuration_update->mapping_of_nssai);
        slice_info_for_ue_configuration_update->mapping_of_nssai = NULL;
    }
    if (slice_info_for_ue_configuration_update->rejected_nssai_ra) {
        OpenAPI_list_for_each(slice_info_for_ue_configuration_update->rejected_nssai_ra, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(slice_info_for_ue_configuration_update->rejected_nssai_ra);
        slice_info_for_ue_configuration_update->rejected_nssai_ra = NULL;
    }
    ogs_free(slice_info_for_ue_configuration_update);
}

cJSON *OpenAPI_slice_info_for_ue_configuration_update_convertToJSON(OpenAPI_slice_info_for_ue_configuration_update_t *slice_info_for_ue_configuration_update)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_info_for_ue_configuration_update == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [SliceInfoForUEConfigurationUpdate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (slice_info_for_ue_configuration_update->subscribed_nssai) {
    cJSON *subscribed_nssaiList = cJSON_AddArrayToObject(item, "subscribedNssai");
    if (subscribed_nssaiList == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [subscribed_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(slice_info_for_ue_configuration_update->subscribed_nssai, node) {
        cJSON *itemLocal = OpenAPI_subscribed_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [subscribed_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(subscribed_nssaiList, itemLocal);
    }
    }

    if (slice_info_for_ue_configuration_update->allowed_nssai_current_access) {
    cJSON *allowed_nssai_current_access_local_JSON = OpenAPI_allowed_nssai_convertToJSON(slice_info_for_ue_configuration_update->allowed_nssai_current_access);
    if (allowed_nssai_current_access_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [allowed_nssai_current_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedNssaiCurrentAccess", allowed_nssai_current_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [allowed_nssai_current_access]");
        goto end;
    }
    }

    if (slice_info_for_ue_configuration_update->allowed_nssai_other_access) {
    cJSON *allowed_nssai_other_access_local_JSON = OpenAPI_allowed_nssai_convertToJSON(slice_info_for_ue_configuration_update->allowed_nssai_other_access);
    if (allowed_nssai_other_access_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [allowed_nssai_other_access]");
        goto end;
    }
    cJSON_AddItemToObject(item, "allowedNssaiOtherAccess", allowed_nssai_other_access_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [allowed_nssai_other_access]");
        goto end;
    }
    }

    if (slice_info_for_ue_configuration_update->is_default_configured_snssai_ind) {
    if (cJSON_AddBoolToObject(item, "defaultConfiguredSnssaiInd", slice_info_for_ue_configuration_update->default_configured_snssai_ind) == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [default_configured_snssai_ind]");
        goto end;
    }
    }

    if (slice_info_for_ue_configuration_update->requested_nssai) {
    cJSON *requested_nssaiList = cJSON_AddArrayToObject(item, "requestedNssai");
    if (requested_nssaiList == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [requested_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(slice_info_for_ue_configuration_update->requested_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [requested_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(requested_nssaiList, itemLocal);
    }
    }

    if (slice_info_for_ue_configuration_update->mapping_of_nssai) {
    cJSON *mapping_of_nssaiList = cJSON_AddArrayToObject(item, "mappingOfNssai");
    if (mapping_of_nssaiList == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [mapping_of_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(slice_info_for_ue_configuration_update->mapping_of_nssai, node) {
        cJSON *itemLocal = OpenAPI_mapping_of_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [mapping_of_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(mapping_of_nssaiList, itemLocal);
    }
    }

    if (slice_info_for_ue_configuration_update->is_ue_sup_nssrg_ind) {
    if (cJSON_AddBoolToObject(item, "ueSupNssrgInd", slice_info_for_ue_configuration_update->ue_sup_nssrg_ind) == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [ue_sup_nssrg_ind]");
        goto end;
    }
    }

    if (slice_info_for_ue_configuration_update->is_suppress_nssrg_ind) {
    if (cJSON_AddBoolToObject(item, "suppressNssrgInd", slice_info_for_ue_configuration_update->suppress_nssrg_ind) == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [suppress_nssrg_ind]");
        goto end;
    }
    }

    if (slice_info_for_ue_configuration_update->rejected_nssai_ra) {
    cJSON *rejected_nssai_raList = cJSON_AddArrayToObject(item, "rejectedNssaiRa");
    if (rejected_nssai_raList == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [rejected_nssai_ra]");
        goto end;
    }
    OpenAPI_list_for_each(slice_info_for_ue_configuration_update->rejected_nssai_ra, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [rejected_nssai_ra]");
            goto end;
        }
        cJSON_AddItemToArray(rejected_nssai_raList, itemLocal);
    }
    }

    if (slice_info_for_ue_configuration_update->is_nsag_supported) {
    if (cJSON_AddBoolToObject(item, "nsagSupported", slice_info_for_ue_configuration_update->nsag_supported) == NULL) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed [nsag_supported]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_slice_info_for_ue_configuration_update_t *OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON(cJSON *slice_info_for_ue_configuration_updateJSON)
{
    OpenAPI_slice_info_for_ue_configuration_update_t *slice_info_for_ue_configuration_update_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *subscribed_nssai = NULL;
    OpenAPI_list_t *subscribed_nssaiList = NULL;
    cJSON *allowed_nssai_current_access = NULL;
    OpenAPI_allowed_nssai_t *allowed_nssai_current_access_local_nonprim = NULL;
    cJSON *allowed_nssai_other_access = NULL;
    OpenAPI_allowed_nssai_t *allowed_nssai_other_access_local_nonprim = NULL;
    cJSON *default_configured_snssai_ind = NULL;
    cJSON *requested_nssai = NULL;
    OpenAPI_list_t *requested_nssaiList = NULL;
    cJSON *mapping_of_nssai = NULL;
    OpenAPI_list_t *mapping_of_nssaiList = NULL;
    cJSON *ue_sup_nssrg_ind = NULL;
    cJSON *suppress_nssrg_ind = NULL;
    cJSON *rejected_nssai_ra = NULL;
    OpenAPI_list_t *rejected_nssai_raList = NULL;
    cJSON *nsag_supported = NULL;
    subscribed_nssai = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "subscribedNssai");
    if (subscribed_nssai) {
        cJSON *subscribed_nssai_local = NULL;
        if (!cJSON_IsArray(subscribed_nssai)) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [subscribed_nssai]");
            goto end;
        }

        subscribed_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_nssai_local, subscribed_nssai) {
            if (!cJSON_IsObject(subscribed_nssai_local)) {
                ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [subscribed_nssai]");
                goto end;
            }
            OpenAPI_subscribed_snssai_t *subscribed_nssaiItem = OpenAPI_subscribed_snssai_parseFromJSON(subscribed_nssai_local);
            if (!subscribed_nssaiItem) {
                ogs_error("No subscribed_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(subscribed_nssaiList, subscribed_nssaiItem);
        }
    }

    allowed_nssai_current_access = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "allowedNssaiCurrentAccess");
    if (allowed_nssai_current_access) {
    allowed_nssai_current_access_local_nonprim = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai_current_access);
    if (!allowed_nssai_current_access_local_nonprim) {
        ogs_error("OpenAPI_allowed_nssai_parseFromJSON failed [allowed_nssai_current_access]");
        goto end;
    }
    }

    allowed_nssai_other_access = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "allowedNssaiOtherAccess");
    if (allowed_nssai_other_access) {
    allowed_nssai_other_access_local_nonprim = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai_other_access);
    if (!allowed_nssai_other_access_local_nonprim) {
        ogs_error("OpenAPI_allowed_nssai_parseFromJSON failed [allowed_nssai_other_access]");
        goto end;
    }
    }

    default_configured_snssai_ind = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "defaultConfiguredSnssaiInd");
    if (default_configured_snssai_ind) {
    if (!cJSON_IsBool(default_configured_snssai_ind)) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [default_configured_snssai_ind]");
        goto end;
    }
    }

    requested_nssai = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "requestedNssai");
    if (requested_nssai) {
        cJSON *requested_nssai_local = NULL;
        if (!cJSON_IsArray(requested_nssai)) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [requested_nssai]");
            goto end;
        }

        requested_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(requested_nssai_local, requested_nssai) {
            if (!cJSON_IsObject(requested_nssai_local)) {
                ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [requested_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *requested_nssaiItem = OpenAPI_snssai_parseFromJSON(requested_nssai_local);
            if (!requested_nssaiItem) {
                ogs_error("No requested_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(requested_nssaiList, requested_nssaiItem);
        }
    }

    mapping_of_nssai = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "mappingOfNssai");
    if (mapping_of_nssai) {
        cJSON *mapping_of_nssai_local = NULL;
        if (!cJSON_IsArray(mapping_of_nssai)) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [mapping_of_nssai]");
            goto end;
        }

        mapping_of_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(mapping_of_nssai_local, mapping_of_nssai) {
            if (!cJSON_IsObject(mapping_of_nssai_local)) {
                ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [mapping_of_nssai]");
                goto end;
            }
            OpenAPI_mapping_of_snssai_t *mapping_of_nssaiItem = OpenAPI_mapping_of_snssai_parseFromJSON(mapping_of_nssai_local);
            if (!mapping_of_nssaiItem) {
                ogs_error("No mapping_of_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(mapping_of_nssaiList, mapping_of_nssaiItem);
        }
    }

    ue_sup_nssrg_ind = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "ueSupNssrgInd");
    if (ue_sup_nssrg_ind) {
    if (!cJSON_IsBool(ue_sup_nssrg_ind)) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [ue_sup_nssrg_ind]");
        goto end;
    }
    }

    suppress_nssrg_ind = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "suppressNssrgInd");
    if (suppress_nssrg_ind) {
    if (!cJSON_IsBool(suppress_nssrg_ind)) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [suppress_nssrg_ind]");
        goto end;
    }
    }

    rejected_nssai_ra = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "rejectedNssaiRa");
    if (rejected_nssai_ra) {
        cJSON *rejected_nssai_ra_local = NULL;
        if (!cJSON_IsArray(rejected_nssai_ra)) {
            ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [rejected_nssai_ra]");
            goto end;
        }

        rejected_nssai_raList = OpenAPI_list_create();

        cJSON_ArrayForEach(rejected_nssai_ra_local, rejected_nssai_ra) {
            if (!cJSON_IsObject(rejected_nssai_ra_local)) {
                ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [rejected_nssai_ra]");
                goto end;
            }
            OpenAPI_snssai_t *rejected_nssai_raItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_ra_local);
            if (!rejected_nssai_raItem) {
                ogs_error("No rejected_nssai_raItem");
                goto end;
            }
            OpenAPI_list_add(rejected_nssai_raList, rejected_nssai_raItem);
        }
    }

    nsag_supported = cJSON_GetObjectItemCaseSensitive(slice_info_for_ue_configuration_updateJSON, "nsagSupported");
    if (nsag_supported) {
    if (!cJSON_IsBool(nsag_supported)) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON() failed [nsag_supported]");
        goto end;
    }
    }

    slice_info_for_ue_configuration_update_local_var = OpenAPI_slice_info_for_ue_configuration_update_create (
        subscribed_nssai ? subscribed_nssaiList : NULL,
        allowed_nssai_current_access ? allowed_nssai_current_access_local_nonprim : NULL,
        allowed_nssai_other_access ? allowed_nssai_other_access_local_nonprim : NULL,
        default_configured_snssai_ind ? true : false,
        default_configured_snssai_ind ? default_configured_snssai_ind->valueint : 0,
        requested_nssai ? requested_nssaiList : NULL,
        mapping_of_nssai ? mapping_of_nssaiList : NULL,
        ue_sup_nssrg_ind ? true : false,
        ue_sup_nssrg_ind ? ue_sup_nssrg_ind->valueint : 0,
        suppress_nssrg_ind ? true : false,
        suppress_nssrg_ind ? suppress_nssrg_ind->valueint : 0,
        rejected_nssai_ra ? rejected_nssai_raList : NULL,
        nsag_supported ? true : false,
        nsag_supported ? nsag_supported->valueint : 0
    );

    return slice_info_for_ue_configuration_update_local_var;
end:
    if (subscribed_nssaiList) {
        OpenAPI_list_for_each(subscribed_nssaiList, node) {
            OpenAPI_subscribed_snssai_free(node->data);
        }
        OpenAPI_list_free(subscribed_nssaiList);
        subscribed_nssaiList = NULL;
    }
    if (allowed_nssai_current_access_local_nonprim) {
        OpenAPI_allowed_nssai_free(allowed_nssai_current_access_local_nonprim);
        allowed_nssai_current_access_local_nonprim = NULL;
    }
    if (allowed_nssai_other_access_local_nonprim) {
        OpenAPI_allowed_nssai_free(allowed_nssai_other_access_local_nonprim);
        allowed_nssai_other_access_local_nonprim = NULL;
    }
    if (requested_nssaiList) {
        OpenAPI_list_for_each(requested_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(requested_nssaiList);
        requested_nssaiList = NULL;
    }
    if (mapping_of_nssaiList) {
        OpenAPI_list_for_each(mapping_of_nssaiList, node) {
            OpenAPI_mapping_of_snssai_free(node->data);
        }
        OpenAPI_list_free(mapping_of_nssaiList);
        mapping_of_nssaiList = NULL;
    }
    if (rejected_nssai_raList) {
        OpenAPI_list_for_each(rejected_nssai_raList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(rejected_nssai_raList);
        rejected_nssai_raList = NULL;
    }
    return NULL;
}

OpenAPI_slice_info_for_ue_configuration_update_t *OpenAPI_slice_info_for_ue_configuration_update_copy(OpenAPI_slice_info_for_ue_configuration_update_t *dst, OpenAPI_slice_info_for_ue_configuration_update_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_info_for_ue_configuration_update_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_info_for_ue_configuration_update_convertToJSON() failed");
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

    OpenAPI_slice_info_for_ue_configuration_update_free(dst);
    dst = OpenAPI_slice_info_for_ue_configuration_update_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

