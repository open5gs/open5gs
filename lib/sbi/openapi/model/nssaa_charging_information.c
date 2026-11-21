
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssaa_charging_information.h"

OpenAPI_nssaa_charging_information_t *OpenAPI_nssaa_charging_information_create(
    OpenAPI_nssaa_message_type_e n_ssaa_message_type,
    OpenAPI_user_information_t *user_identification,
    bool is_a_aap_address_null,
    OpenAPI_server_addressing_info_t *a_aap_address,
    bool is_a_aas_address_null,
    OpenAPI_server_addressing_info_t *a_aas_address,
    char *e_apid_response,
    OpenAPI_auth_status_e e_a_pauthstatus,
    char *a_mfid
)
{
    OpenAPI_nssaa_charging_information_t *nssaa_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_nssaa_charging_information_t));
    ogs_assert(nssaa_charging_information_local_var);

    nssaa_charging_information_local_var->n_ssaa_message_type = n_ssaa_message_type;
    nssaa_charging_information_local_var->user_identification = user_identification;
    nssaa_charging_information_local_var->is_a_aap_address_null = is_a_aap_address_null;
    nssaa_charging_information_local_var->a_aap_address = a_aap_address;
    nssaa_charging_information_local_var->is_a_aas_address_null = is_a_aas_address_null;
    nssaa_charging_information_local_var->a_aas_address = a_aas_address;
    nssaa_charging_information_local_var->e_apid_response = e_apid_response;
    nssaa_charging_information_local_var->e_a_pauthstatus = e_a_pauthstatus;
    nssaa_charging_information_local_var->a_mfid = a_mfid;

    return nssaa_charging_information_local_var;
}

void OpenAPI_nssaa_charging_information_free(OpenAPI_nssaa_charging_information_t *nssaa_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssaa_charging_information) {
        return;
    }
    if (nssaa_charging_information->user_identification) {
        OpenAPI_user_information_free(nssaa_charging_information->user_identification);
        nssaa_charging_information->user_identification = NULL;
    }
    if (nssaa_charging_information->a_aap_address) {
        OpenAPI_server_addressing_info_free(nssaa_charging_information->a_aap_address);
        nssaa_charging_information->a_aap_address = NULL;
    }
    if (nssaa_charging_information->a_aas_address) {
        OpenAPI_server_addressing_info_free(nssaa_charging_information->a_aas_address);
        nssaa_charging_information->a_aas_address = NULL;
    }
    if (nssaa_charging_information->e_apid_response) {
        ogs_free(nssaa_charging_information->e_apid_response);
        nssaa_charging_information->e_apid_response = NULL;
    }
    if (nssaa_charging_information->a_mfid) {
        ogs_free(nssaa_charging_information->a_mfid);
        nssaa_charging_information->a_mfid = NULL;
    }
    ogs_free(nssaa_charging_information);
}

cJSON *OpenAPI_nssaa_charging_information_convertToJSON(OpenAPI_nssaa_charging_information_t *nssaa_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssaa_charging_information == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [NSSAAChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nssaa_charging_information->n_ssaa_message_type == OpenAPI_nssaa_message_type_NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [n_ssaa_message_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nSSAAMessageType", OpenAPI_nssaa_message_type_ToString(nssaa_charging_information->n_ssaa_message_type)) == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [n_ssaa_message_type]");
        goto end;
    }

    if (!nssaa_charging_information->user_identification) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [user_identification]");
        return NULL;
    }
    cJSON *user_identification_local_JSON = OpenAPI_user_information_convertToJSON(nssaa_charging_information->user_identification);
    if (user_identification_local_JSON == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [user_identification]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userIdentification", user_identification_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [user_identification]");
        goto end;
    }

    if (nssaa_charging_information->a_aap_address) {
    cJSON *a_aap_address_local_JSON = OpenAPI_server_addressing_info_convertToJSON(nssaa_charging_information->a_aap_address);
    if (a_aap_address_local_JSON == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_aap_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "aAAPAddress", a_aap_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_aap_address]");
        goto end;
    }
    } else if (nssaa_charging_information->is_a_aap_address_null) {
        if (cJSON_AddNullToObject(item, "aAAPAddress") == NULL) {
            ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_aap_address]");
            goto end;
        }
    }

    if (nssaa_charging_information->a_aas_address) {
    cJSON *a_aas_address_local_JSON = OpenAPI_server_addressing_info_convertToJSON(nssaa_charging_information->a_aas_address);
    if (a_aas_address_local_JSON == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_aas_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "aAASAddress", a_aas_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_aas_address]");
        goto end;
    }
    } else if (nssaa_charging_information->is_a_aas_address_null) {
        if (cJSON_AddNullToObject(item, "aAASAddress") == NULL) {
            ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_aas_address]");
            goto end;
        }
    }

    if (nssaa_charging_information->e_apid_response) {
    if (cJSON_AddStringToObject(item, "eAPIDResponse", nssaa_charging_information->e_apid_response) == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [e_apid_response]");
        goto end;
    }
    }

    if (nssaa_charging_information->e_a_pauthstatus != OpenAPI_auth_status_NULL) {
    if (cJSON_AddStringToObject(item, "eAPauthstatus", OpenAPI_auth_status_ToString(nssaa_charging_information->e_a_pauthstatus)) == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [e_a_pauthstatus]");
        goto end;
    }
    }

    if (nssaa_charging_information->a_mfid) {
    if (cJSON_AddStringToObject(item, "aMFId", nssaa_charging_information->a_mfid) == NULL) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed [a_mfid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nssaa_charging_information_t *OpenAPI_nssaa_charging_information_parseFromJSON(cJSON *nssaa_charging_informationJSON)
{
    OpenAPI_nssaa_charging_information_t *nssaa_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n_ssaa_message_type = NULL;
    OpenAPI_nssaa_message_type_e n_ssaa_message_typeVariable = 0;
    cJSON *user_identification = NULL;
    OpenAPI_user_information_t *user_identification_local_nonprim = NULL;
    cJSON *a_aap_address = NULL;
    OpenAPI_server_addressing_info_t *a_aap_address_local_nonprim = NULL;
    cJSON *a_aas_address = NULL;
    OpenAPI_server_addressing_info_t *a_aas_address_local_nonprim = NULL;
    cJSON *e_apid_response = NULL;
    cJSON *e_a_pauthstatus = NULL;
    OpenAPI_auth_status_e e_a_pauthstatusVariable = 0;
    cJSON *a_mfid = NULL;
    n_ssaa_message_type = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "nSSAAMessageType");
    if (!n_ssaa_message_type) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON() failed [n_ssaa_message_type]");
        goto end;
    }
    if (!cJSON_IsString(n_ssaa_message_type)) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON() failed [n_ssaa_message_type]");
        goto end;
    }
    n_ssaa_message_typeVariable = OpenAPI_nssaa_message_type_FromString(n_ssaa_message_type->valuestring);

    user_identification = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "userIdentification");
    if (!user_identification) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON() failed [user_identification]");
        goto end;
    }
    user_identification_local_nonprim = OpenAPI_user_information_parseFromJSON(user_identification);
    if (!user_identification_local_nonprim) {
        ogs_error("OpenAPI_user_information_parseFromJSON failed [user_identification]");
        goto end;
    }

    a_aap_address = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "aAAPAddress");
    if (a_aap_address) {
    if (!cJSON_IsNull(a_aap_address)) {
    a_aap_address_local_nonprim = OpenAPI_server_addressing_info_parseFromJSON(a_aap_address);
    if (!a_aap_address_local_nonprim) {
        ogs_error("OpenAPI_server_addressing_info_parseFromJSON failed [a_aap_address]");
        goto end;
    }
    }
    }

    a_aas_address = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "aAASAddress");
    if (a_aas_address) {
    if (!cJSON_IsNull(a_aas_address)) {
    a_aas_address_local_nonprim = OpenAPI_server_addressing_info_parseFromJSON(a_aas_address);
    if (!a_aas_address_local_nonprim) {
        ogs_error("OpenAPI_server_addressing_info_parseFromJSON failed [a_aas_address]");
        goto end;
    }
    }
    }

    e_apid_response = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "eAPIDResponse");
    if (e_apid_response) {
    if (!cJSON_IsString(e_apid_response) && !cJSON_IsNull(e_apid_response)) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON() failed [e_apid_response]");
        goto end;
    }
    }

    e_a_pauthstatus = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "eAPauthstatus");
    if (e_a_pauthstatus) {
    if (!cJSON_IsString(e_a_pauthstatus)) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON() failed [e_a_pauthstatus]");
        goto end;
    }
    e_a_pauthstatusVariable = OpenAPI_auth_status_FromString(e_a_pauthstatus->valuestring);
    }

    a_mfid = cJSON_GetObjectItemCaseSensitive(nssaa_charging_informationJSON, "aMFId");
    if (a_mfid) {
    if (!cJSON_IsString(a_mfid) && !cJSON_IsNull(a_mfid)) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON() failed [a_mfid]");
        goto end;
    }
    }

    nssaa_charging_information_local_var = OpenAPI_nssaa_charging_information_create (
        n_ssaa_message_typeVariable,
        user_identification_local_nonprim,
        a_aap_address && cJSON_IsNull(a_aap_address) ? true : false,
        a_aap_address ? a_aap_address_local_nonprim : NULL,
        a_aas_address && cJSON_IsNull(a_aas_address) ? true : false,
        a_aas_address ? a_aas_address_local_nonprim : NULL,
        e_apid_response && !cJSON_IsNull(e_apid_response) ? ogs_strdup(e_apid_response->valuestring) : NULL,
        e_a_pauthstatus ? e_a_pauthstatusVariable : 0,
        a_mfid && !cJSON_IsNull(a_mfid) ? ogs_strdup(a_mfid->valuestring) : NULL
    );

    return nssaa_charging_information_local_var;
end:
    if (user_identification_local_nonprim) {
        OpenAPI_user_information_free(user_identification_local_nonprim);
        user_identification_local_nonprim = NULL;
    }
    if (a_aap_address_local_nonprim) {
        OpenAPI_server_addressing_info_free(a_aap_address_local_nonprim);
        a_aap_address_local_nonprim = NULL;
    }
    if (a_aas_address_local_nonprim) {
        OpenAPI_server_addressing_info_free(a_aas_address_local_nonprim);
        a_aas_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nssaa_charging_information_t *OpenAPI_nssaa_charging_information_copy(OpenAPI_nssaa_charging_information_t *dst, OpenAPI_nssaa_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssaa_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssaa_charging_information_convertToJSON() failed");
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

    OpenAPI_nssaa_charging_information_free(dst);
    dst = OpenAPI_nssaa_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

