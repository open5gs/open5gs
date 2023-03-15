
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_release_data.h"

OpenAPI_sm_context_release_data_t *OpenAPI_sm_context_release_data_create(
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    bool is_vsmf_release_only,
    int vsmf_release_only,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    bool is_ismf_release_only,
    int ismf_release_only
)
{
    OpenAPI_sm_context_release_data_t *sm_context_release_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_release_data_t));
    ogs_assert(sm_context_release_data_local_var);

    sm_context_release_data_local_var->cause = cause;
    sm_context_release_data_local_var->ng_ap_cause = ng_ap_cause;
    sm_context_release_data_local_var->is__5g_mm_cause_value = is__5g_mm_cause_value;
    sm_context_release_data_local_var->_5g_mm_cause_value = _5g_mm_cause_value;
    sm_context_release_data_local_var->ue_location = ue_location;
    sm_context_release_data_local_var->ue_time_zone = ue_time_zone;
    sm_context_release_data_local_var->add_ue_location = add_ue_location;
    sm_context_release_data_local_var->is_vsmf_release_only = is_vsmf_release_only;
    sm_context_release_data_local_var->vsmf_release_only = vsmf_release_only;
    sm_context_release_data_local_var->n2_sm_info = n2_sm_info;
    sm_context_release_data_local_var->n2_sm_info_type = n2_sm_info_type;
    sm_context_release_data_local_var->is_ismf_release_only = is_ismf_release_only;
    sm_context_release_data_local_var->ismf_release_only = ismf_release_only;

    return sm_context_release_data_local_var;
}

void OpenAPI_sm_context_release_data_free(OpenAPI_sm_context_release_data_t *sm_context_release_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_release_data) {
        return;
    }
    if (sm_context_release_data->ng_ap_cause) {
        OpenAPI_ng_ap_cause_free(sm_context_release_data->ng_ap_cause);
        sm_context_release_data->ng_ap_cause = NULL;
    }
    if (sm_context_release_data->ue_location) {
        OpenAPI_user_location_free(sm_context_release_data->ue_location);
        sm_context_release_data->ue_location = NULL;
    }
    if (sm_context_release_data->ue_time_zone) {
        ogs_free(sm_context_release_data->ue_time_zone);
        sm_context_release_data->ue_time_zone = NULL;
    }
    if (sm_context_release_data->add_ue_location) {
        OpenAPI_user_location_free(sm_context_release_data->add_ue_location);
        sm_context_release_data->add_ue_location = NULL;
    }
    if (sm_context_release_data->n2_sm_info) {
        OpenAPI_ref_to_binary_data_free(sm_context_release_data->n2_sm_info);
        sm_context_release_data->n2_sm_info = NULL;
    }
    ogs_free(sm_context_release_data);
}

cJSON *OpenAPI_sm_context_release_data_convertToJSON(OpenAPI_sm_context_release_data_t *sm_context_release_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_release_data == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [SmContextReleaseData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_release_data->cause != OpenAPI_cause_NULL) {
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_cause_ToString(sm_context_release_data->cause)) == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (sm_context_release_data->ng_ap_cause) {
    cJSON *ng_ap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(sm_context_release_data->ng_ap_cause);
    if (ng_ap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngApCause", ng_ap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    }

    if (sm_context_release_data->is__5g_mm_cause_value) {
    if (cJSON_AddNumberToObject(item, "5gMmCauseValue", sm_context_release_data->_5g_mm_cause_value) == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    if (sm_context_release_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sm_context_release_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (sm_context_release_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sm_context_release_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sm_context_release_data->add_ue_location) {
    cJSON *add_ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sm_context_release_data->add_ue_location);
    if (add_ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addUeLocation", add_ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    }

    if (sm_context_release_data->is_vsmf_release_only) {
    if (cJSON_AddBoolToObject(item, "vsmfReleaseOnly", sm_context_release_data->vsmf_release_only) == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [vsmf_release_only]");
        goto end;
    }
    }

    if (sm_context_release_data->n2_sm_info) {
    cJSON *n2_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_release_data->n2_sm_info);
    if (n2_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfo", n2_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    }

    if (sm_context_release_data->n2_sm_info_type != OpenAPI_n2_sm_info_type_NULL) {
    if (cJSON_AddStringToObject(item, "n2SmInfoType", OpenAPI_n2_sm_info_type_ToString(sm_context_release_data->n2_sm_info_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [n2_sm_info_type]");
        goto end;
    }
    }

    if (sm_context_release_data->is_ismf_release_only) {
    if (cJSON_AddBoolToObject(item, "ismfReleaseOnly", sm_context_release_data->ismf_release_only) == NULL) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed [ismf_release_only]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_release_data_t *OpenAPI_sm_context_release_data_parseFromJSON(cJSON *sm_context_release_dataJSON)
{
    OpenAPI_sm_context_release_data_t *sm_context_release_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cause = NULL;
    OpenAPI_cause_e causeVariable = 0;
    cJSON *ng_ap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    cJSON *_5g_mm_cause_value = NULL;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *add_ue_location = NULL;
    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    cJSON *vsmf_release_only = NULL;
    cJSON *n2_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    cJSON *n2_sm_info_type = NULL;
    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable = 0;
    cJSON *ismf_release_only = NULL;
    cause = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    ng_ap_cause = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "ngApCause");
    if (ng_ap_cause) {
    ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    if (!ng_ap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ng_ap_cause]");
        goto end;
    }
    }

    _5g_mm_cause_value = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "5gMmCauseValue");
    if (_5g_mm_cause_value) {
    if (!cJSON_IsNumber(_5g_mm_cause_value)) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    add_ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "addUeLocation");
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    if (!add_ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [add_ue_location]");
        goto end;
    }
    }

    vsmf_release_only = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "vsmfReleaseOnly");
    if (vsmf_release_only) {
    if (!cJSON_IsBool(vsmf_release_only)) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON() failed [vsmf_release_only]");
        goto end;
    }
    }

    n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "n2SmInfo");
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    if (!n2_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n2_sm_info]");
        goto end;
    }
    }

    n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "n2SmInfoType");
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    ismf_release_only = cJSON_GetObjectItemCaseSensitive(sm_context_release_dataJSON, "ismfReleaseOnly");
    if (ismf_release_only) {
    if (!cJSON_IsBool(ismf_release_only)) {
        ogs_error("OpenAPI_sm_context_release_data_parseFromJSON() failed [ismf_release_only]");
        goto end;
    }
    }

    sm_context_release_data_local_var = OpenAPI_sm_context_release_data_create (
        cause ? causeVariable : 0,
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL,
        _5g_mm_cause_value ? true : false,
        _5g_mm_cause_value ? _5g_mm_cause_value->valuedouble : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        vsmf_release_only ? true : false,
        vsmf_release_only ? vsmf_release_only->valueint : 0,
        n2_sm_info ? n2_sm_info_local_nonprim : NULL,
        n2_sm_info_type ? n2_sm_info_typeVariable : 0,
        ismf_release_only ? true : false,
        ismf_release_only ? ismf_release_only->valueint : 0
    );

    return sm_context_release_data_local_var;
end:
    if (ng_ap_cause_local_nonprim) {
        OpenAPI_ng_ap_cause_free(ng_ap_cause_local_nonprim);
        ng_ap_cause_local_nonprim = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    if (add_ue_location_local_nonprim) {
        OpenAPI_user_location_free(add_ue_location_local_nonprim);
        add_ue_location_local_nonprim = NULL;
    }
    if (n2_sm_info_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n2_sm_info_local_nonprim);
        n2_sm_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_release_data_t *OpenAPI_sm_context_release_data_copy(OpenAPI_sm_context_release_data_t *dst, OpenAPI_sm_context_release_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_release_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_release_data_convertToJSON() failed");
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

    OpenAPI_sm_context_release_data_free(dst);
    dst = OpenAPI_sm_context_release_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

