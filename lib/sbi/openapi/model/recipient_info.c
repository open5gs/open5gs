
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "recipient_info.h"

OpenAPI_recipient_info_t *OpenAPI_recipient_info_create(
    char *recipient_supi,
    char *recipient_gpsi,
    OpenAPI_sm_address_info_t *recipient_other_address,
    OpenAPI_list_t *recipient_other_addresses,
    OpenAPI_sm_address_info_t *recipient_received_address,
    char *recipient_sccp_address,
    OpenAPI_sm_interface_t *s_m_destination_interface,
    char *s_mrecipient_protocol_id
)
{
    OpenAPI_recipient_info_t *recipient_info_local_var = ogs_malloc(sizeof(OpenAPI_recipient_info_t));
    ogs_assert(recipient_info_local_var);

    recipient_info_local_var->recipient_supi = recipient_supi;
    recipient_info_local_var->recipient_gpsi = recipient_gpsi;
    recipient_info_local_var->recipient_other_address = recipient_other_address;
    recipient_info_local_var->recipient_other_addresses = recipient_other_addresses;
    recipient_info_local_var->recipient_received_address = recipient_received_address;
    recipient_info_local_var->recipient_sccp_address = recipient_sccp_address;
    recipient_info_local_var->s_m_destination_interface = s_m_destination_interface;
    recipient_info_local_var->s_mrecipient_protocol_id = s_mrecipient_protocol_id;

    return recipient_info_local_var;
}

void OpenAPI_recipient_info_free(OpenAPI_recipient_info_t *recipient_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == recipient_info) {
        return;
    }
    if (recipient_info->recipient_supi) {
        ogs_free(recipient_info->recipient_supi);
        recipient_info->recipient_supi = NULL;
    }
    if (recipient_info->recipient_gpsi) {
        ogs_free(recipient_info->recipient_gpsi);
        recipient_info->recipient_gpsi = NULL;
    }
    if (recipient_info->recipient_other_address) {
        OpenAPI_sm_address_info_free(recipient_info->recipient_other_address);
        recipient_info->recipient_other_address = NULL;
    }
    if (recipient_info->recipient_other_addresses) {
        OpenAPI_list_for_each(recipient_info->recipient_other_addresses, node) {
            OpenAPI_recipient_address_free(node->data);
        }
        OpenAPI_list_free(recipient_info->recipient_other_addresses);
        recipient_info->recipient_other_addresses = NULL;
    }
    if (recipient_info->recipient_received_address) {
        OpenAPI_sm_address_info_free(recipient_info->recipient_received_address);
        recipient_info->recipient_received_address = NULL;
    }
    if (recipient_info->recipient_sccp_address) {
        ogs_free(recipient_info->recipient_sccp_address);
        recipient_info->recipient_sccp_address = NULL;
    }
    if (recipient_info->s_m_destination_interface) {
        OpenAPI_sm_interface_free(recipient_info->s_m_destination_interface);
        recipient_info->s_m_destination_interface = NULL;
    }
    if (recipient_info->s_mrecipient_protocol_id) {
        ogs_free(recipient_info->s_mrecipient_protocol_id);
        recipient_info->s_mrecipient_protocol_id = NULL;
    }
    ogs_free(recipient_info);
}

cJSON *OpenAPI_recipient_info_convertToJSON(OpenAPI_recipient_info_t *recipient_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (recipient_info == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [RecipientInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (recipient_info->recipient_supi) {
    if (cJSON_AddStringToObject(item, "recipientSUPI", recipient_info->recipient_supi) == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_supi]");
        goto end;
    }
    }

    if (recipient_info->recipient_gpsi) {
    if (cJSON_AddStringToObject(item, "recipientGPSI", recipient_info->recipient_gpsi) == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_gpsi]");
        goto end;
    }
    }

    if (recipient_info->recipient_other_address) {
    cJSON *recipient_other_address_local_JSON = OpenAPI_sm_address_info_convertToJSON(recipient_info->recipient_other_address);
    if (recipient_other_address_local_JSON == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_other_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recipientOtherAddress", recipient_other_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_other_address]");
        goto end;
    }
    }

    if (recipient_info->recipient_other_addresses) {
    cJSON *recipient_other_addressesList = cJSON_AddArrayToObject(item, "recipientOtherAddresses");
    if (recipient_other_addressesList == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_other_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(recipient_info->recipient_other_addresses, node) {
        cJSON *itemLocal = OpenAPI_recipient_address_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_other_addresses]");
            goto end;
        }
        cJSON_AddItemToArray(recipient_other_addressesList, itemLocal);
    }
    }

    if (recipient_info->recipient_received_address) {
    cJSON *recipient_received_address_local_JSON = OpenAPI_sm_address_info_convertToJSON(recipient_info->recipient_received_address);
    if (recipient_received_address_local_JSON == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_received_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recipientReceivedAddress", recipient_received_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_received_address]");
        goto end;
    }
    }

    if (recipient_info->recipient_sccp_address) {
    if (cJSON_AddStringToObject(item, "recipientSCCPAddress", recipient_info->recipient_sccp_address) == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [recipient_sccp_address]");
        goto end;
    }
    }

    if (recipient_info->s_m_destination_interface) {
    cJSON *s_m_destination_interface_local_JSON = OpenAPI_sm_interface_convertToJSON(recipient_info->s_m_destination_interface);
    if (s_m_destination_interface_local_JSON == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [s_m_destination_interface]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sMDestinationInterface", s_m_destination_interface_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [s_m_destination_interface]");
        goto end;
    }
    }

    if (recipient_info->s_mrecipient_protocol_id) {
    if (cJSON_AddStringToObject(item, "sMrecipientProtocolId", recipient_info->s_mrecipient_protocol_id) == NULL) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed [s_mrecipient_protocol_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_recipient_info_t *OpenAPI_recipient_info_parseFromJSON(cJSON *recipient_infoJSON)
{
    OpenAPI_recipient_info_t *recipient_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *recipient_supi = NULL;
    cJSON *recipient_gpsi = NULL;
    cJSON *recipient_other_address = NULL;
    OpenAPI_sm_address_info_t *recipient_other_address_local_nonprim = NULL;
    cJSON *recipient_other_addresses = NULL;
    OpenAPI_list_t *recipient_other_addressesList = NULL;
    cJSON *recipient_received_address = NULL;
    OpenAPI_sm_address_info_t *recipient_received_address_local_nonprim = NULL;
    cJSON *recipient_sccp_address = NULL;
    cJSON *s_m_destination_interface = NULL;
    OpenAPI_sm_interface_t *s_m_destination_interface_local_nonprim = NULL;
    cJSON *s_mrecipient_protocol_id = NULL;
    recipient_supi = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "recipientSUPI");
    if (recipient_supi) {
    if (!cJSON_IsString(recipient_supi) && !cJSON_IsNull(recipient_supi)) {
        ogs_error("OpenAPI_recipient_info_parseFromJSON() failed [recipient_supi]");
        goto end;
    }
    }

    recipient_gpsi = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "recipientGPSI");
    if (recipient_gpsi) {
    if (!cJSON_IsString(recipient_gpsi) && !cJSON_IsNull(recipient_gpsi)) {
        ogs_error("OpenAPI_recipient_info_parseFromJSON() failed [recipient_gpsi]");
        goto end;
    }
    }

    recipient_other_address = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "recipientOtherAddress");
    if (recipient_other_address) {
    recipient_other_address_local_nonprim = OpenAPI_sm_address_info_parseFromJSON(recipient_other_address);
    if (!recipient_other_address_local_nonprim) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON failed [recipient_other_address]");
        goto end;
    }
    }

    recipient_other_addresses = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "recipientOtherAddresses");
    if (recipient_other_addresses) {
        cJSON *recipient_other_addresses_local = NULL;
        if (!cJSON_IsArray(recipient_other_addresses)) {
            ogs_error("OpenAPI_recipient_info_parseFromJSON() failed [recipient_other_addresses]");
            goto end;
        }

        recipient_other_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(recipient_other_addresses_local, recipient_other_addresses) {
            if (!cJSON_IsObject(recipient_other_addresses_local)) {
                ogs_error("OpenAPI_recipient_info_parseFromJSON() failed [recipient_other_addresses]");
                goto end;
            }
            OpenAPI_recipient_address_t *recipient_other_addressesItem = OpenAPI_recipient_address_parseFromJSON(recipient_other_addresses_local);
            if (!recipient_other_addressesItem) {
                ogs_error("No recipient_other_addressesItem");
                goto end;
            }
            OpenAPI_list_add(recipient_other_addressesList, recipient_other_addressesItem);
        }
    }

    recipient_received_address = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "recipientReceivedAddress");
    if (recipient_received_address) {
    recipient_received_address_local_nonprim = OpenAPI_sm_address_info_parseFromJSON(recipient_received_address);
    if (!recipient_received_address_local_nonprim) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON failed [recipient_received_address]");
        goto end;
    }
    }

    recipient_sccp_address = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "recipientSCCPAddress");
    if (recipient_sccp_address) {
    if (!cJSON_IsString(recipient_sccp_address) && !cJSON_IsNull(recipient_sccp_address)) {
        ogs_error("OpenAPI_recipient_info_parseFromJSON() failed [recipient_sccp_address]");
        goto end;
    }
    }

    s_m_destination_interface = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "sMDestinationInterface");
    if (s_m_destination_interface) {
    s_m_destination_interface_local_nonprim = OpenAPI_sm_interface_parseFromJSON(s_m_destination_interface);
    if (!s_m_destination_interface_local_nonprim) {
        ogs_error("OpenAPI_sm_interface_parseFromJSON failed [s_m_destination_interface]");
        goto end;
    }
    }

    s_mrecipient_protocol_id = cJSON_GetObjectItemCaseSensitive(recipient_infoJSON, "sMrecipientProtocolId");
    if (s_mrecipient_protocol_id) {
    if (!cJSON_IsString(s_mrecipient_protocol_id) && !cJSON_IsNull(s_mrecipient_protocol_id)) {
        ogs_error("OpenAPI_recipient_info_parseFromJSON() failed [s_mrecipient_protocol_id]");
        goto end;
    }
    }

    recipient_info_local_var = OpenAPI_recipient_info_create (
        recipient_supi && !cJSON_IsNull(recipient_supi) ? ogs_strdup(recipient_supi->valuestring) : NULL,
        recipient_gpsi && !cJSON_IsNull(recipient_gpsi) ? ogs_strdup(recipient_gpsi->valuestring) : NULL,
        recipient_other_address ? recipient_other_address_local_nonprim : NULL,
        recipient_other_addresses ? recipient_other_addressesList : NULL,
        recipient_received_address ? recipient_received_address_local_nonprim : NULL,
        recipient_sccp_address && !cJSON_IsNull(recipient_sccp_address) ? ogs_strdup(recipient_sccp_address->valuestring) : NULL,
        s_m_destination_interface ? s_m_destination_interface_local_nonprim : NULL,
        s_mrecipient_protocol_id && !cJSON_IsNull(s_mrecipient_protocol_id) ? ogs_strdup(s_mrecipient_protocol_id->valuestring) : NULL
    );

    return recipient_info_local_var;
end:
    if (recipient_other_address_local_nonprim) {
        OpenAPI_sm_address_info_free(recipient_other_address_local_nonprim);
        recipient_other_address_local_nonprim = NULL;
    }
    if (recipient_other_addressesList) {
        OpenAPI_list_for_each(recipient_other_addressesList, node) {
            OpenAPI_recipient_address_free(node->data);
        }
        OpenAPI_list_free(recipient_other_addressesList);
        recipient_other_addressesList = NULL;
    }
    if (recipient_received_address_local_nonprim) {
        OpenAPI_sm_address_info_free(recipient_received_address_local_nonprim);
        recipient_received_address_local_nonprim = NULL;
    }
    if (s_m_destination_interface_local_nonprim) {
        OpenAPI_sm_interface_free(s_m_destination_interface_local_nonprim);
        s_m_destination_interface_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_recipient_info_t *OpenAPI_recipient_info_copy(OpenAPI_recipient_info_t *dst, OpenAPI_recipient_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_recipient_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_recipient_info_convertToJSON() failed");
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

    OpenAPI_recipient_info_free(dst);
    dst = OpenAPI_recipient_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

