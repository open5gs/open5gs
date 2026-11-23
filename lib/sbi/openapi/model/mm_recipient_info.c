
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_recipient_info.h"

OpenAPI_mm_recipient_info_t *OpenAPI_mm_recipient_info_create(
    char *recipient_supi,
    char *recipient_gpsi,
    OpenAPI_list_t *recipient_other_address
)
{
    OpenAPI_mm_recipient_info_t *mm_recipient_info_local_var = ogs_malloc(sizeof(OpenAPI_mm_recipient_info_t));
    ogs_assert(mm_recipient_info_local_var);

    mm_recipient_info_local_var->recipient_supi = recipient_supi;
    mm_recipient_info_local_var->recipient_gpsi = recipient_gpsi;
    mm_recipient_info_local_var->recipient_other_address = recipient_other_address;

    return mm_recipient_info_local_var;
}

void OpenAPI_mm_recipient_info_free(OpenAPI_mm_recipient_info_t *mm_recipient_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mm_recipient_info) {
        return;
    }
    if (mm_recipient_info->recipient_supi) {
        ogs_free(mm_recipient_info->recipient_supi);
        mm_recipient_info->recipient_supi = NULL;
    }
    if (mm_recipient_info->recipient_gpsi) {
        ogs_free(mm_recipient_info->recipient_gpsi);
        mm_recipient_info->recipient_gpsi = NULL;
    }
    if (mm_recipient_info->recipient_other_address) {
        OpenAPI_list_for_each(mm_recipient_info->recipient_other_address, node) {
            OpenAPI_sm_address_info_free(node->data);
        }
        OpenAPI_list_free(mm_recipient_info->recipient_other_address);
        mm_recipient_info->recipient_other_address = NULL;
    }
    ogs_free(mm_recipient_info);
}

cJSON *OpenAPI_mm_recipient_info_convertToJSON(OpenAPI_mm_recipient_info_t *mm_recipient_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mm_recipient_info == NULL) {
        ogs_error("OpenAPI_mm_recipient_info_convertToJSON() failed [MMRecipientInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mm_recipient_info->recipient_supi) {
    if (cJSON_AddStringToObject(item, "recipientSUPI", mm_recipient_info->recipient_supi) == NULL) {
        ogs_error("OpenAPI_mm_recipient_info_convertToJSON() failed [recipient_supi]");
        goto end;
    }
    }

    if (mm_recipient_info->recipient_gpsi) {
    if (cJSON_AddStringToObject(item, "recipientGPSI", mm_recipient_info->recipient_gpsi) == NULL) {
        ogs_error("OpenAPI_mm_recipient_info_convertToJSON() failed [recipient_gpsi]");
        goto end;
    }
    }

    if (mm_recipient_info->recipient_other_address) {
    cJSON *recipient_other_addressList = cJSON_AddArrayToObject(item, "recipientOtherAddress");
    if (recipient_other_addressList == NULL) {
        ogs_error("OpenAPI_mm_recipient_info_convertToJSON() failed [recipient_other_address]");
        goto end;
    }
    OpenAPI_list_for_each(mm_recipient_info->recipient_other_address, node) {
        cJSON *itemLocal = OpenAPI_sm_address_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_recipient_info_convertToJSON() failed [recipient_other_address]");
            goto end;
        }
        cJSON_AddItemToArray(recipient_other_addressList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mm_recipient_info_t *OpenAPI_mm_recipient_info_parseFromJSON(cJSON *mm_recipient_infoJSON)
{
    OpenAPI_mm_recipient_info_t *mm_recipient_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *recipient_supi = NULL;
    cJSON *recipient_gpsi = NULL;
    cJSON *recipient_other_address = NULL;
    OpenAPI_list_t *recipient_other_addressList = NULL;
    recipient_supi = cJSON_GetObjectItemCaseSensitive(mm_recipient_infoJSON, "recipientSUPI");
    if (recipient_supi) {
    if (!cJSON_IsString(recipient_supi) && !cJSON_IsNull(recipient_supi)) {
        ogs_error("OpenAPI_mm_recipient_info_parseFromJSON() failed [recipient_supi]");
        goto end;
    }
    }

    recipient_gpsi = cJSON_GetObjectItemCaseSensitive(mm_recipient_infoJSON, "recipientGPSI");
    if (recipient_gpsi) {
    if (!cJSON_IsString(recipient_gpsi) && !cJSON_IsNull(recipient_gpsi)) {
        ogs_error("OpenAPI_mm_recipient_info_parseFromJSON() failed [recipient_gpsi]");
        goto end;
    }
    }

    recipient_other_address = cJSON_GetObjectItemCaseSensitive(mm_recipient_infoJSON, "recipientOtherAddress");
    if (recipient_other_address) {
        cJSON *recipient_other_address_local = NULL;
        if (!cJSON_IsArray(recipient_other_address)) {
            ogs_error("OpenAPI_mm_recipient_info_parseFromJSON() failed [recipient_other_address]");
            goto end;
        }

        recipient_other_addressList = OpenAPI_list_create();

        cJSON_ArrayForEach(recipient_other_address_local, recipient_other_address) {
            if (!cJSON_IsObject(recipient_other_address_local)) {
                ogs_error("OpenAPI_mm_recipient_info_parseFromJSON() failed [recipient_other_address]");
                goto end;
            }
            OpenAPI_sm_address_info_t *recipient_other_addressItem = OpenAPI_sm_address_info_parseFromJSON(recipient_other_address_local);
            if (!recipient_other_addressItem) {
                ogs_error("No recipient_other_addressItem");
                goto end;
            }
            OpenAPI_list_add(recipient_other_addressList, recipient_other_addressItem);
        }
    }

    mm_recipient_info_local_var = OpenAPI_mm_recipient_info_create (
        recipient_supi && !cJSON_IsNull(recipient_supi) ? ogs_strdup(recipient_supi->valuestring) : NULL,
        recipient_gpsi && !cJSON_IsNull(recipient_gpsi) ? ogs_strdup(recipient_gpsi->valuestring) : NULL,
        recipient_other_address ? recipient_other_addressList : NULL
    );

    return mm_recipient_info_local_var;
end:
    if (recipient_other_addressList) {
        OpenAPI_list_for_each(recipient_other_addressList, node) {
            OpenAPI_sm_address_info_free(node->data);
        }
        OpenAPI_list_free(recipient_other_addressList);
        recipient_other_addressList = NULL;
    }
    return NULL;
}

OpenAPI_mm_recipient_info_t *OpenAPI_mm_recipient_info_copy(OpenAPI_mm_recipient_info_t *dst, OpenAPI_mm_recipient_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_recipient_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_recipient_info_convertToJSON() failed");
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

    OpenAPI_mm_recipient_info_free(dst);
    dst = OpenAPI_mm_recipient_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

