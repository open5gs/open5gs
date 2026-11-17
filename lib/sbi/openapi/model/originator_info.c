
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "originator_info.h"

OpenAPI_originator_info_t *OpenAPI_originator_info_create(
    char *originator_supi,
    char *originator_gpsi,
    OpenAPI_sm_address_info_t *originator_other_address,
    OpenAPI_sm_address_info_t *originator_received_address,
    char *originator_sccp_address,
    OpenAPI_sm_interface_t *s_m_originator_interface,
    char *s_m_originator_protocol_id
)
{
    OpenAPI_originator_info_t *originator_info_local_var = ogs_malloc(sizeof(OpenAPI_originator_info_t));
    ogs_assert(originator_info_local_var);

    originator_info_local_var->originator_supi = originator_supi;
    originator_info_local_var->originator_gpsi = originator_gpsi;
    originator_info_local_var->originator_other_address = originator_other_address;
    originator_info_local_var->originator_received_address = originator_received_address;
    originator_info_local_var->originator_sccp_address = originator_sccp_address;
    originator_info_local_var->s_m_originator_interface = s_m_originator_interface;
    originator_info_local_var->s_m_originator_protocol_id = s_m_originator_protocol_id;

    return originator_info_local_var;
}

void OpenAPI_originator_info_free(OpenAPI_originator_info_t *originator_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == originator_info) {
        return;
    }
    if (originator_info->originator_supi) {
        ogs_free(originator_info->originator_supi);
        originator_info->originator_supi = NULL;
    }
    if (originator_info->originator_gpsi) {
        ogs_free(originator_info->originator_gpsi);
        originator_info->originator_gpsi = NULL;
    }
    if (originator_info->originator_other_address) {
        OpenAPI_sm_address_info_free(originator_info->originator_other_address);
        originator_info->originator_other_address = NULL;
    }
    if (originator_info->originator_received_address) {
        OpenAPI_sm_address_info_free(originator_info->originator_received_address);
        originator_info->originator_received_address = NULL;
    }
    if (originator_info->originator_sccp_address) {
        ogs_free(originator_info->originator_sccp_address);
        originator_info->originator_sccp_address = NULL;
    }
    if (originator_info->s_m_originator_interface) {
        OpenAPI_sm_interface_free(originator_info->s_m_originator_interface);
        originator_info->s_m_originator_interface = NULL;
    }
    if (originator_info->s_m_originator_protocol_id) {
        ogs_free(originator_info->s_m_originator_protocol_id);
        originator_info->s_m_originator_protocol_id = NULL;
    }
    ogs_free(originator_info);
}

cJSON *OpenAPI_originator_info_convertToJSON(OpenAPI_originator_info_t *originator_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (originator_info == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [OriginatorInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (originator_info->originator_supi) {
    if (cJSON_AddStringToObject(item, "originatorSUPI", originator_info->originator_supi) == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_supi]");
        goto end;
    }
    }

    if (originator_info->originator_gpsi) {
    if (cJSON_AddStringToObject(item, "originatorGPSI", originator_info->originator_gpsi) == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_gpsi]");
        goto end;
    }
    }

    if (originator_info->originator_other_address) {
    cJSON *originator_other_address_local_JSON = OpenAPI_sm_address_info_convertToJSON(originator_info->originator_other_address);
    if (originator_other_address_local_JSON == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_other_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "originatorOtherAddress", originator_other_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_other_address]");
        goto end;
    }
    }

    if (originator_info->originator_received_address) {
    cJSON *originator_received_address_local_JSON = OpenAPI_sm_address_info_convertToJSON(originator_info->originator_received_address);
    if (originator_received_address_local_JSON == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_received_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "originatorReceivedAddress", originator_received_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_received_address]");
        goto end;
    }
    }

    if (originator_info->originator_sccp_address) {
    if (cJSON_AddStringToObject(item, "originatorSCCPAddress", originator_info->originator_sccp_address) == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [originator_sccp_address]");
        goto end;
    }
    }

    if (originator_info->s_m_originator_interface) {
    cJSON *s_m_originator_interface_local_JSON = OpenAPI_sm_interface_convertToJSON(originator_info->s_m_originator_interface);
    if (s_m_originator_interface_local_JSON == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [s_m_originator_interface]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sMOriginatorInterface", s_m_originator_interface_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [s_m_originator_interface]");
        goto end;
    }
    }

    if (originator_info->s_m_originator_protocol_id) {
    if (cJSON_AddStringToObject(item, "sMOriginatorProtocolId", originator_info->s_m_originator_protocol_id) == NULL) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed [s_m_originator_protocol_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_originator_info_t *OpenAPI_originator_info_parseFromJSON(cJSON *originator_infoJSON)
{
    OpenAPI_originator_info_t *originator_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *originator_supi = NULL;
    cJSON *originator_gpsi = NULL;
    cJSON *originator_other_address = NULL;
    OpenAPI_sm_address_info_t *originator_other_address_local_nonprim = NULL;
    cJSON *originator_received_address = NULL;
    OpenAPI_sm_address_info_t *originator_received_address_local_nonprim = NULL;
    cJSON *originator_sccp_address = NULL;
    cJSON *s_m_originator_interface = NULL;
    OpenAPI_sm_interface_t *s_m_originator_interface_local_nonprim = NULL;
    cJSON *s_m_originator_protocol_id = NULL;
    originator_supi = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "originatorSUPI");
    if (originator_supi) {
    if (!cJSON_IsString(originator_supi) && !cJSON_IsNull(originator_supi)) {
        ogs_error("OpenAPI_originator_info_parseFromJSON() failed [originator_supi]");
        goto end;
    }
    }

    originator_gpsi = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "originatorGPSI");
    if (originator_gpsi) {
    if (!cJSON_IsString(originator_gpsi) && !cJSON_IsNull(originator_gpsi)) {
        ogs_error("OpenAPI_originator_info_parseFromJSON() failed [originator_gpsi]");
        goto end;
    }
    }

    originator_other_address = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "originatorOtherAddress");
    if (originator_other_address) {
    originator_other_address_local_nonprim = OpenAPI_sm_address_info_parseFromJSON(originator_other_address);
    if (!originator_other_address_local_nonprim) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON failed [originator_other_address]");
        goto end;
    }
    }

    originator_received_address = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "originatorReceivedAddress");
    if (originator_received_address) {
    originator_received_address_local_nonprim = OpenAPI_sm_address_info_parseFromJSON(originator_received_address);
    if (!originator_received_address_local_nonprim) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON failed [originator_received_address]");
        goto end;
    }
    }

    originator_sccp_address = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "originatorSCCPAddress");
    if (originator_sccp_address) {
    if (!cJSON_IsString(originator_sccp_address) && !cJSON_IsNull(originator_sccp_address)) {
        ogs_error("OpenAPI_originator_info_parseFromJSON() failed [originator_sccp_address]");
        goto end;
    }
    }

    s_m_originator_interface = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "sMOriginatorInterface");
    if (s_m_originator_interface) {
    s_m_originator_interface_local_nonprim = OpenAPI_sm_interface_parseFromJSON(s_m_originator_interface);
    if (!s_m_originator_interface_local_nonprim) {
        ogs_error("OpenAPI_sm_interface_parseFromJSON failed [s_m_originator_interface]");
        goto end;
    }
    }

    s_m_originator_protocol_id = cJSON_GetObjectItemCaseSensitive(originator_infoJSON, "sMOriginatorProtocolId");
    if (s_m_originator_protocol_id) {
    if (!cJSON_IsString(s_m_originator_protocol_id) && !cJSON_IsNull(s_m_originator_protocol_id)) {
        ogs_error("OpenAPI_originator_info_parseFromJSON() failed [s_m_originator_protocol_id]");
        goto end;
    }
    }

    originator_info_local_var = OpenAPI_originator_info_create (
        originator_supi && !cJSON_IsNull(originator_supi) ? ogs_strdup(originator_supi->valuestring) : NULL,
        originator_gpsi && !cJSON_IsNull(originator_gpsi) ? ogs_strdup(originator_gpsi->valuestring) : NULL,
        originator_other_address ? originator_other_address_local_nonprim : NULL,
        originator_received_address ? originator_received_address_local_nonprim : NULL,
        originator_sccp_address && !cJSON_IsNull(originator_sccp_address) ? ogs_strdup(originator_sccp_address->valuestring) : NULL,
        s_m_originator_interface ? s_m_originator_interface_local_nonprim : NULL,
        s_m_originator_protocol_id && !cJSON_IsNull(s_m_originator_protocol_id) ? ogs_strdup(s_m_originator_protocol_id->valuestring) : NULL
    );

    return originator_info_local_var;
end:
    if (originator_other_address_local_nonprim) {
        OpenAPI_sm_address_info_free(originator_other_address_local_nonprim);
        originator_other_address_local_nonprim = NULL;
    }
    if (originator_received_address_local_nonprim) {
        OpenAPI_sm_address_info_free(originator_received_address_local_nonprim);
        originator_received_address_local_nonprim = NULL;
    }
    if (s_m_originator_interface_local_nonprim) {
        OpenAPI_sm_interface_free(s_m_originator_interface_local_nonprim);
        s_m_originator_interface_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_originator_info_t *OpenAPI_originator_info_copy(OpenAPI_originator_info_t *dst, OpenAPI_originator_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_originator_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_originator_info_convertToJSON() failed");
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

    OpenAPI_originator_info_free(dst);
    dst = OpenAPI_originator_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

