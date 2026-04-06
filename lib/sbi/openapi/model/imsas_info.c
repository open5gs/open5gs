
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "imsas_info.h"

OpenAPI_imsas_info_t *OpenAPI_imsas_info_create(
    OpenAPI_list_t *ims_event_list
)
{
    OpenAPI_imsas_info_t *imsas_info_local_var = ogs_malloc(sizeof(OpenAPI_imsas_info_t));
    ogs_assert(imsas_info_local_var);

    imsas_info_local_var->ims_event_list = ims_event_list;

    return imsas_info_local_var;
}

void OpenAPI_imsas_info_free(OpenAPI_imsas_info_t *imsas_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == imsas_info) {
        return;
    }
    if (imsas_info->ims_event_list) {
        OpenAPI_list_free(imsas_info->ims_event_list);
        imsas_info->ims_event_list = NULL;
    }
    ogs_free(imsas_info);
}

cJSON *OpenAPI_imsas_info_convertToJSON(OpenAPI_imsas_info_t *imsas_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (imsas_info == NULL) {
        ogs_error("OpenAPI_imsas_info_convertToJSON() failed [ImsasInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (imsas_info->ims_event_list == OpenAPI_ims_event_NULL) {
        ogs_error("OpenAPI_imsas_info_convertToJSON() failed [ims_event_list]");
        return NULL;
    }
    cJSON *ims_event_listList = cJSON_AddArrayToObject(item, "imsEventList");
    if (ims_event_listList == NULL) {
        ogs_error("OpenAPI_imsas_info_convertToJSON() failed [ims_event_list]");
        goto end;
    }
    OpenAPI_list_for_each(imsas_info->ims_event_list, node) {
        if (cJSON_AddStringToObject(ims_event_listList, "", OpenAPI_ims_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_imsas_info_convertToJSON() failed [ims_event_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_imsas_info_t *OpenAPI_imsas_info_parseFromJSON(cJSON *imsas_infoJSON)
{
    OpenAPI_imsas_info_t *imsas_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ims_event_list = NULL;
    OpenAPI_list_t *ims_event_listList = NULL;
    ims_event_list = cJSON_GetObjectItemCaseSensitive(imsas_infoJSON, "imsEventList");
    if (!ims_event_list) {
        ogs_error("OpenAPI_imsas_info_parseFromJSON() failed [ims_event_list]");
        goto end;
    }
        cJSON *ims_event_list_local = NULL;
        if (!cJSON_IsArray(ims_event_list)) {
            ogs_error("OpenAPI_imsas_info_parseFromJSON() failed [ims_event_list]");
            goto end;
        }

        ims_event_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ims_event_list_local, ims_event_list) {
            OpenAPI_ims_event_e localEnum = OpenAPI_ims_event_NULL;
            if (!cJSON_IsString(ims_event_list_local)) {
                ogs_error("OpenAPI_imsas_info_parseFromJSON() failed [ims_event_list]");
                goto end;
            }
            localEnum = OpenAPI_ims_event_FromString(ims_event_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ims_event_list\" is not supported. Ignoring it ...",
                         ims_event_list_local->valuestring);
            } else {
                OpenAPI_list_add(ims_event_listList, (void *)localEnum);
            }
        }
        if (ims_event_listList->count == 0) {
            ogs_error("OpenAPI_imsas_info_parseFromJSON() failed: Expected ims_event_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    imsas_info_local_var = OpenAPI_imsas_info_create (
        ims_event_listList
    );

    return imsas_info_local_var;
end:
    if (ims_event_listList) {
        OpenAPI_list_free(ims_event_listList);
        ims_event_listList = NULL;
    }
    return NULL;
}

OpenAPI_imsas_info_t *OpenAPI_imsas_info_copy(OpenAPI_imsas_info_t *dst, OpenAPI_imsas_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_imsas_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_imsas_info_convertToJSON() failed");
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

    OpenAPI_imsas_info_free(dst);
    dst = OpenAPI_imsas_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

