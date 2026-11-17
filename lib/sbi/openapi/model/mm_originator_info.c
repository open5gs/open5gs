
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mm_originator_info.h"

OpenAPI_mm_originator_info_t *OpenAPI_mm_originator_info_create(
    char *originator_supi,
    char *originator_gpsi,
    OpenAPI_list_t *originator_other_address
)
{
    OpenAPI_mm_originator_info_t *mm_originator_info_local_var = ogs_malloc(sizeof(OpenAPI_mm_originator_info_t));
    ogs_assert(mm_originator_info_local_var);

    mm_originator_info_local_var->originator_supi = originator_supi;
    mm_originator_info_local_var->originator_gpsi = originator_gpsi;
    mm_originator_info_local_var->originator_other_address = originator_other_address;

    return mm_originator_info_local_var;
}

void OpenAPI_mm_originator_info_free(OpenAPI_mm_originator_info_t *mm_originator_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mm_originator_info) {
        return;
    }
    if (mm_originator_info->originator_supi) {
        ogs_free(mm_originator_info->originator_supi);
        mm_originator_info->originator_supi = NULL;
    }
    if (mm_originator_info->originator_gpsi) {
        ogs_free(mm_originator_info->originator_gpsi);
        mm_originator_info->originator_gpsi = NULL;
    }
    if (mm_originator_info->originator_other_address) {
        OpenAPI_list_for_each(mm_originator_info->originator_other_address, node) {
            OpenAPI_sm_address_info_free(node->data);
        }
        OpenAPI_list_free(mm_originator_info->originator_other_address);
        mm_originator_info->originator_other_address = NULL;
    }
    ogs_free(mm_originator_info);
}

cJSON *OpenAPI_mm_originator_info_convertToJSON(OpenAPI_mm_originator_info_t *mm_originator_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mm_originator_info == NULL) {
        ogs_error("OpenAPI_mm_originator_info_convertToJSON() failed [MMOriginatorInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mm_originator_info->originator_supi) {
    if (cJSON_AddStringToObject(item, "originatorSUPI", mm_originator_info->originator_supi) == NULL) {
        ogs_error("OpenAPI_mm_originator_info_convertToJSON() failed [originator_supi]");
        goto end;
    }
    }

    if (mm_originator_info->originator_gpsi) {
    if (cJSON_AddStringToObject(item, "originatorGPSI", mm_originator_info->originator_gpsi) == NULL) {
        ogs_error("OpenAPI_mm_originator_info_convertToJSON() failed [originator_gpsi]");
        goto end;
    }
    }

    if (mm_originator_info->originator_other_address) {
    cJSON *originator_other_addressList = cJSON_AddArrayToObject(item, "originatorOtherAddress");
    if (originator_other_addressList == NULL) {
        ogs_error("OpenAPI_mm_originator_info_convertToJSON() failed [originator_other_address]");
        goto end;
    }
    OpenAPI_list_for_each(mm_originator_info->originator_other_address, node) {
        cJSON *itemLocal = OpenAPI_sm_address_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mm_originator_info_convertToJSON() failed [originator_other_address]");
            goto end;
        }
        cJSON_AddItemToArray(originator_other_addressList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mm_originator_info_t *OpenAPI_mm_originator_info_parseFromJSON(cJSON *mm_originator_infoJSON)
{
    OpenAPI_mm_originator_info_t *mm_originator_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *originator_supi = NULL;
    cJSON *originator_gpsi = NULL;
    cJSON *originator_other_address = NULL;
    OpenAPI_list_t *originator_other_addressList = NULL;
    originator_supi = cJSON_GetObjectItemCaseSensitive(mm_originator_infoJSON, "originatorSUPI");
    if (originator_supi) {
    if (!cJSON_IsString(originator_supi) && !cJSON_IsNull(originator_supi)) {
        ogs_error("OpenAPI_mm_originator_info_parseFromJSON() failed [originator_supi]");
        goto end;
    }
    }

    originator_gpsi = cJSON_GetObjectItemCaseSensitive(mm_originator_infoJSON, "originatorGPSI");
    if (originator_gpsi) {
    if (!cJSON_IsString(originator_gpsi) && !cJSON_IsNull(originator_gpsi)) {
        ogs_error("OpenAPI_mm_originator_info_parseFromJSON() failed [originator_gpsi]");
        goto end;
    }
    }

    originator_other_address = cJSON_GetObjectItemCaseSensitive(mm_originator_infoJSON, "originatorOtherAddress");
    if (originator_other_address) {
        cJSON *originator_other_address_local = NULL;
        if (!cJSON_IsArray(originator_other_address)) {
            ogs_error("OpenAPI_mm_originator_info_parseFromJSON() failed [originator_other_address]");
            goto end;
        }

        originator_other_addressList = OpenAPI_list_create();

        cJSON_ArrayForEach(originator_other_address_local, originator_other_address) {
            if (!cJSON_IsObject(originator_other_address_local)) {
                ogs_error("OpenAPI_mm_originator_info_parseFromJSON() failed [originator_other_address]");
                goto end;
            }
            OpenAPI_sm_address_info_t *originator_other_addressItem = OpenAPI_sm_address_info_parseFromJSON(originator_other_address_local);
            if (!originator_other_addressItem) {
                ogs_error("No originator_other_addressItem");
                goto end;
            }
            OpenAPI_list_add(originator_other_addressList, originator_other_addressItem);
        }
    }

    mm_originator_info_local_var = OpenAPI_mm_originator_info_create (
        originator_supi && !cJSON_IsNull(originator_supi) ? ogs_strdup(originator_supi->valuestring) : NULL,
        originator_gpsi && !cJSON_IsNull(originator_gpsi) ? ogs_strdup(originator_gpsi->valuestring) : NULL,
        originator_other_address ? originator_other_addressList : NULL
    );

    return mm_originator_info_local_var;
end:
    if (originator_other_addressList) {
        OpenAPI_list_for_each(originator_other_addressList, node) {
            OpenAPI_sm_address_info_free(node->data);
        }
        OpenAPI_list_free(originator_other_addressList);
        originator_other_addressList = NULL;
    }
    return NULL;
}

OpenAPI_mm_originator_info_t *OpenAPI_mm_originator_info_copy(OpenAPI_mm_originator_info_t *dst, OpenAPI_mm_originator_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mm_originator_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mm_originator_info_convertToJSON() failed");
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

    OpenAPI_mm_originator_info_free(dst);
    dst = OpenAPI_mm_originator_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

