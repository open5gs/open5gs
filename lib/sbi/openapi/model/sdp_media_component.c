
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdp_media_component.h"

OpenAPI_sdp_media_component_t *OpenAPI_sdp_media_component_create(
    char *s_dp_media_name,
    OpenAPI_list_t *sdp_media_description,
    bool is_local_gw_inserted_indication,
    int local_gw_inserted_indication,
    bool is_ip_realm_default_indication,
    int ip_realm_default_indication,
    bool is_transcoder_inserted_indication,
    int transcoder_inserted_indication,
    OpenAPI_media_initiator_flag_e media_initiator_flag,
    char *media_initiator_party,
    char *three_gpp_charging_id,
    char *access_network_charging_identifier_value,
    OpenAPI_sdp_type_e s_dp_type
)
{
    OpenAPI_sdp_media_component_t *sdp_media_component_local_var = ogs_malloc(sizeof(OpenAPI_sdp_media_component_t));
    ogs_assert(sdp_media_component_local_var);

    sdp_media_component_local_var->s_dp_media_name = s_dp_media_name;
    sdp_media_component_local_var->sdp_media_description = sdp_media_description;
    sdp_media_component_local_var->is_local_gw_inserted_indication = is_local_gw_inserted_indication;
    sdp_media_component_local_var->local_gw_inserted_indication = local_gw_inserted_indication;
    sdp_media_component_local_var->is_ip_realm_default_indication = is_ip_realm_default_indication;
    sdp_media_component_local_var->ip_realm_default_indication = ip_realm_default_indication;
    sdp_media_component_local_var->is_transcoder_inserted_indication = is_transcoder_inserted_indication;
    sdp_media_component_local_var->transcoder_inserted_indication = transcoder_inserted_indication;
    sdp_media_component_local_var->media_initiator_flag = media_initiator_flag;
    sdp_media_component_local_var->media_initiator_party = media_initiator_party;
    sdp_media_component_local_var->three_gpp_charging_id = three_gpp_charging_id;
    sdp_media_component_local_var->access_network_charging_identifier_value = access_network_charging_identifier_value;
    sdp_media_component_local_var->s_dp_type = s_dp_type;

    return sdp_media_component_local_var;
}

void OpenAPI_sdp_media_component_free(OpenAPI_sdp_media_component_t *sdp_media_component)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sdp_media_component) {
        return;
    }
    if (sdp_media_component->s_dp_media_name) {
        ogs_free(sdp_media_component->s_dp_media_name);
        sdp_media_component->s_dp_media_name = NULL;
    }
    if (sdp_media_component->sdp_media_description) {
        OpenAPI_list_for_each(sdp_media_component->sdp_media_description, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sdp_media_component->sdp_media_description);
        sdp_media_component->sdp_media_description = NULL;
    }
    if (sdp_media_component->media_initiator_party) {
        ogs_free(sdp_media_component->media_initiator_party);
        sdp_media_component->media_initiator_party = NULL;
    }
    if (sdp_media_component->three_gpp_charging_id) {
        ogs_free(sdp_media_component->three_gpp_charging_id);
        sdp_media_component->three_gpp_charging_id = NULL;
    }
    if (sdp_media_component->access_network_charging_identifier_value) {
        ogs_free(sdp_media_component->access_network_charging_identifier_value);
        sdp_media_component->access_network_charging_identifier_value = NULL;
    }
    ogs_free(sdp_media_component);
}

cJSON *OpenAPI_sdp_media_component_convertToJSON(OpenAPI_sdp_media_component_t *sdp_media_component)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sdp_media_component == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [SDPMediaComponent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sdp_media_component->s_dp_media_name) {
    if (cJSON_AddStringToObject(item, "sDPMediaName", sdp_media_component->s_dp_media_name) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [s_dp_media_name]");
        goto end;
    }
    }

    if (sdp_media_component->sdp_media_description) {
    cJSON *sdp_media_descriptionList = cJSON_AddArrayToObject(item, "SDPMediaDescription");
    if (sdp_media_descriptionList == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [sdp_media_description]");
        goto end;
    }
    OpenAPI_list_for_each(sdp_media_component->sdp_media_description, node) {
        if (cJSON_AddStringToObject(sdp_media_descriptionList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [sdp_media_description]");
            goto end;
        }
    }
    }

    if (sdp_media_component->is_local_gw_inserted_indication) {
    if (cJSON_AddBoolToObject(item, "localGWInsertedIndication", sdp_media_component->local_gw_inserted_indication) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [local_gw_inserted_indication]");
        goto end;
    }
    }

    if (sdp_media_component->is_ip_realm_default_indication) {
    if (cJSON_AddBoolToObject(item, "ipRealmDefaultIndication", sdp_media_component->ip_realm_default_indication) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [ip_realm_default_indication]");
        goto end;
    }
    }

    if (sdp_media_component->is_transcoder_inserted_indication) {
    if (cJSON_AddBoolToObject(item, "transcoderInsertedIndication", sdp_media_component->transcoder_inserted_indication) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [transcoder_inserted_indication]");
        goto end;
    }
    }

    if (sdp_media_component->media_initiator_flag != OpenAPI_media_initiator_flag_NULL) {
    if (cJSON_AddStringToObject(item, "mediaInitiatorFlag", OpenAPI_media_initiator_flag_ToString(sdp_media_component->media_initiator_flag)) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [media_initiator_flag]");
        goto end;
    }
    }

    if (sdp_media_component->media_initiator_party) {
    if (cJSON_AddStringToObject(item, "mediaInitiatorParty", sdp_media_component->media_initiator_party) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [media_initiator_party]");
        goto end;
    }
    }

    if (sdp_media_component->three_gpp_charging_id) {
    if (cJSON_AddStringToObject(item, "threeGPPChargingId", sdp_media_component->three_gpp_charging_id) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [three_gpp_charging_id]");
        goto end;
    }
    }

    if (sdp_media_component->access_network_charging_identifier_value) {
    if (cJSON_AddStringToObject(item, "accessNetworkChargingIdentifierValue", sdp_media_component->access_network_charging_identifier_value) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [access_network_charging_identifier_value]");
        goto end;
    }
    }

    if (sdp_media_component->s_dp_type != OpenAPI_sdp_type_NULL) {
    if (cJSON_AddStringToObject(item, "sDPType", OpenAPI_sdp_type_ToString(sdp_media_component->s_dp_type)) == NULL) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed [s_dp_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sdp_media_component_t *OpenAPI_sdp_media_component_parseFromJSON(cJSON *sdp_media_componentJSON)
{
    OpenAPI_sdp_media_component_t *sdp_media_component_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_dp_media_name = NULL;
    cJSON *sdp_media_description = NULL;
    OpenAPI_list_t *sdp_media_descriptionList = NULL;
    cJSON *local_gw_inserted_indication = NULL;
    cJSON *ip_realm_default_indication = NULL;
    cJSON *transcoder_inserted_indication = NULL;
    cJSON *media_initiator_flag = NULL;
    OpenAPI_media_initiator_flag_e media_initiator_flagVariable = 0;
    cJSON *media_initiator_party = NULL;
    cJSON *three_gpp_charging_id = NULL;
    cJSON *access_network_charging_identifier_value = NULL;
    cJSON *s_dp_type = NULL;
    OpenAPI_sdp_type_e s_dp_typeVariable = 0;
    s_dp_media_name = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "sDPMediaName");
    if (s_dp_media_name) {
    if (!cJSON_IsString(s_dp_media_name) && !cJSON_IsNull(s_dp_media_name)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [s_dp_media_name]");
        goto end;
    }
    }

    sdp_media_description = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "SDPMediaDescription");
    if (sdp_media_description) {
        cJSON *sdp_media_description_local = NULL;
        if (!cJSON_IsArray(sdp_media_description)) {
            ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [sdp_media_description]");
            goto end;
        }

        sdp_media_descriptionList = OpenAPI_list_create();

        cJSON_ArrayForEach(sdp_media_description_local, sdp_media_description) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(sdp_media_description_local)) {
                ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [sdp_media_description]");
                goto end;
            }
            OpenAPI_list_add(sdp_media_descriptionList, ogs_strdup(sdp_media_description_local->valuestring));
        }
    }

    local_gw_inserted_indication = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "localGWInsertedIndication");
    if (local_gw_inserted_indication) {
    if (!cJSON_IsBool(local_gw_inserted_indication)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [local_gw_inserted_indication]");
        goto end;
    }
    }

    ip_realm_default_indication = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "ipRealmDefaultIndication");
    if (ip_realm_default_indication) {
    if (!cJSON_IsBool(ip_realm_default_indication)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [ip_realm_default_indication]");
        goto end;
    }
    }

    transcoder_inserted_indication = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "transcoderInsertedIndication");
    if (transcoder_inserted_indication) {
    if (!cJSON_IsBool(transcoder_inserted_indication)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [transcoder_inserted_indication]");
        goto end;
    }
    }

    media_initiator_flag = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "mediaInitiatorFlag");
    if (media_initiator_flag) {
    if (!cJSON_IsString(media_initiator_flag)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [media_initiator_flag]");
        goto end;
    }
    media_initiator_flagVariable = OpenAPI_media_initiator_flag_FromString(media_initiator_flag->valuestring);
    }

    media_initiator_party = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "mediaInitiatorParty");
    if (media_initiator_party) {
    if (!cJSON_IsString(media_initiator_party) && !cJSON_IsNull(media_initiator_party)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [media_initiator_party]");
        goto end;
    }
    }

    three_gpp_charging_id = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "threeGPPChargingId");
    if (three_gpp_charging_id) {
    if (!cJSON_IsString(three_gpp_charging_id) && !cJSON_IsNull(three_gpp_charging_id)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [three_gpp_charging_id]");
        goto end;
    }
    }

    access_network_charging_identifier_value = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "accessNetworkChargingIdentifierValue");
    if (access_network_charging_identifier_value) {
    if (!cJSON_IsString(access_network_charging_identifier_value) && !cJSON_IsNull(access_network_charging_identifier_value)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [access_network_charging_identifier_value]");
        goto end;
    }
    }

    s_dp_type = cJSON_GetObjectItemCaseSensitive(sdp_media_componentJSON, "sDPType");
    if (s_dp_type) {
    if (!cJSON_IsString(s_dp_type)) {
        ogs_error("OpenAPI_sdp_media_component_parseFromJSON() failed [s_dp_type]");
        goto end;
    }
    s_dp_typeVariable = OpenAPI_sdp_type_FromString(s_dp_type->valuestring);
    }

    sdp_media_component_local_var = OpenAPI_sdp_media_component_create (
        s_dp_media_name && !cJSON_IsNull(s_dp_media_name) ? ogs_strdup(s_dp_media_name->valuestring) : NULL,
        sdp_media_description ? sdp_media_descriptionList : NULL,
        local_gw_inserted_indication ? true : false,
        local_gw_inserted_indication ? local_gw_inserted_indication->valueint : 0,
        ip_realm_default_indication ? true : false,
        ip_realm_default_indication ? ip_realm_default_indication->valueint : 0,
        transcoder_inserted_indication ? true : false,
        transcoder_inserted_indication ? transcoder_inserted_indication->valueint : 0,
        media_initiator_flag ? media_initiator_flagVariable : 0,
        media_initiator_party && !cJSON_IsNull(media_initiator_party) ? ogs_strdup(media_initiator_party->valuestring) : NULL,
        three_gpp_charging_id && !cJSON_IsNull(three_gpp_charging_id) ? ogs_strdup(three_gpp_charging_id->valuestring) : NULL,
        access_network_charging_identifier_value && !cJSON_IsNull(access_network_charging_identifier_value) ? ogs_strdup(access_network_charging_identifier_value->valuestring) : NULL,
        s_dp_type ? s_dp_typeVariable : 0
    );

    return sdp_media_component_local_var;
end:
    if (sdp_media_descriptionList) {
        OpenAPI_list_for_each(sdp_media_descriptionList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sdp_media_descriptionList);
        sdp_media_descriptionList = NULL;
    }
    return NULL;
}

OpenAPI_sdp_media_component_t *OpenAPI_sdp_media_component_copy(OpenAPI_sdp_media_component_t *dst, OpenAPI_sdp_media_component_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdp_media_component_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdp_media_component_convertToJSON() failed");
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

    OpenAPI_sdp_media_component_free(dst);
    dst = OpenAPI_sdp_media_component_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

