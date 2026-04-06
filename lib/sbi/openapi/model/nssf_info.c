
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssf_info.h"

OpenAPI_nssf_info_t *OpenAPI_nssf_info_create(
    OpenAPI_list_t *event_list
)
{
    OpenAPI_nssf_info_t *nssf_info_local_var = ogs_malloc(sizeof(OpenAPI_nssf_info_t));
    ogs_assert(nssf_info_local_var);

    nssf_info_local_var->event_list = event_list;

    return nssf_info_local_var;
}

void OpenAPI_nssf_info_free(OpenAPI_nssf_info_t *nssf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssf_info) {
        return;
    }
    if (nssf_info->event_list) {
        OpenAPI_list_free(nssf_info->event_list);
        nssf_info->event_list = NULL;
    }
    ogs_free(nssf_info);
}

cJSON *OpenAPI_nssf_info_convertToJSON(OpenAPI_nssf_info_t *nssf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssf_info == NULL) {
        ogs_error("OpenAPI_nssf_info_convertToJSON() failed [NssfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nssf_info->event_list != OpenAPI_nssf_event_type_NULL) {
    cJSON *event_listList = cJSON_AddArrayToObject(item, "eventList");
    if (event_listList == NULL) {
        ogs_error("OpenAPI_nssf_info_convertToJSON() failed [event_list]");
        goto end;
    }
    OpenAPI_list_for_each(nssf_info->event_list, node) {
        if (cJSON_AddStringToObject(event_listList, "", OpenAPI_nssf_event_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nssf_info_convertToJSON() failed [event_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_nssf_info_t *OpenAPI_nssf_info_parseFromJSON(cJSON *nssf_infoJSON)
{
    OpenAPI_nssf_info_t *nssf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_list = NULL;
    OpenAPI_list_t *event_listList = NULL;
    event_list = cJSON_GetObjectItemCaseSensitive(nssf_infoJSON, "eventList");
    if (event_list) {
        cJSON *event_list_local = NULL;
        if (!cJSON_IsArray(event_list)) {
            ogs_error("OpenAPI_nssf_info_parseFromJSON() failed [event_list]");
            goto end;
        }

        event_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_list_local, event_list) {
            OpenAPI_nssf_event_type_e localEnum = OpenAPI_nssf_event_type_NULL;
            if (!cJSON_IsString(event_list_local)) {
                ogs_error("OpenAPI_nssf_info_parseFromJSON() failed [event_list]");
                goto end;
            }
            localEnum = OpenAPI_nssf_event_type_FromString(event_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"event_list\" is not supported. Ignoring it ...",
                         event_list_local->valuestring);
            } else {
                OpenAPI_list_add(event_listList, (void *)localEnum);
            }
        }
        if (event_listList->count == 0) {
            ogs_error("OpenAPI_nssf_info_parseFromJSON() failed: Expected event_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    nssf_info_local_var = OpenAPI_nssf_info_create (
        event_list ? event_listList : NULL
    );

    return nssf_info_local_var;
end:
    if (event_listList) {
        OpenAPI_list_free(event_listList);
        event_listList = NULL;
    }
    return NULL;
}

OpenAPI_nssf_info_t *OpenAPI_nssf_info_copy(OpenAPI_nssf_info_t *dst, OpenAPI_nssf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssf_info_convertToJSON() failed");
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

    OpenAPI_nssf_info_free(dst);
    dst = OpenAPI_nssf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

