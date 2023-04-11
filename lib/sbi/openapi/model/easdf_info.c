
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "easdf_info.h"

OpenAPI_easdf_info_t *OpenAPI_easdf_info_create(
    OpenAPI_list_t *s_nssai_easdf_info_list,
    OpenAPI_list_t *easdf_n6_ip_address_list,
    OpenAPI_list_t *upf_n6_ip_address_list
)
{
    OpenAPI_easdf_info_t *easdf_info_local_var = ogs_malloc(sizeof(OpenAPI_easdf_info_t));
    ogs_assert(easdf_info_local_var);

    easdf_info_local_var->s_nssai_easdf_info_list = s_nssai_easdf_info_list;
    easdf_info_local_var->easdf_n6_ip_address_list = easdf_n6_ip_address_list;
    easdf_info_local_var->upf_n6_ip_address_list = upf_n6_ip_address_list;

    return easdf_info_local_var;
}

void OpenAPI_easdf_info_free(OpenAPI_easdf_info_t *easdf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == easdf_info) {
        return;
    }
    if (easdf_info->s_nssai_easdf_info_list) {
        OpenAPI_list_for_each(easdf_info->s_nssai_easdf_info_list, node) {
            OpenAPI_snssai_easdf_info_item_free(node->data);
        }
        OpenAPI_list_free(easdf_info->s_nssai_easdf_info_list);
        easdf_info->s_nssai_easdf_info_list = NULL;
    }
    if (easdf_info->easdf_n6_ip_address_list) {
        OpenAPI_list_for_each(easdf_info->easdf_n6_ip_address_list, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(easdf_info->easdf_n6_ip_address_list);
        easdf_info->easdf_n6_ip_address_list = NULL;
    }
    if (easdf_info->upf_n6_ip_address_list) {
        OpenAPI_list_for_each(easdf_info->upf_n6_ip_address_list, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(easdf_info->upf_n6_ip_address_list);
        easdf_info->upf_n6_ip_address_list = NULL;
    }
    ogs_free(easdf_info);
}

cJSON *OpenAPI_easdf_info_convertToJSON(OpenAPI_easdf_info_t *easdf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (easdf_info == NULL) {
        ogs_error("OpenAPI_easdf_info_convertToJSON() failed [EasdfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (easdf_info->s_nssai_easdf_info_list) {
    cJSON *s_nssai_easdf_info_listList = cJSON_AddArrayToObject(item, "sNssaiEasdfInfoList");
    if (s_nssai_easdf_info_listList == NULL) {
        ogs_error("OpenAPI_easdf_info_convertToJSON() failed [s_nssai_easdf_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(easdf_info->s_nssai_easdf_info_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_easdf_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_easdf_info_convertToJSON() failed [s_nssai_easdf_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_easdf_info_listList, itemLocal);
    }
    }

    if (easdf_info->easdf_n6_ip_address_list) {
    cJSON *easdf_n6_ip_address_listList = cJSON_AddArrayToObject(item, "easdfN6IpAddressList");
    if (easdf_n6_ip_address_listList == NULL) {
        ogs_error("OpenAPI_easdf_info_convertToJSON() failed [easdf_n6_ip_address_list]");
        goto end;
    }
    OpenAPI_list_for_each(easdf_info->easdf_n6_ip_address_list, node) {
        cJSON *itemLocal = OpenAPI_ip_addr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_easdf_info_convertToJSON() failed [easdf_n6_ip_address_list]");
            goto end;
        }
        cJSON_AddItemToArray(easdf_n6_ip_address_listList, itemLocal);
    }
    }

    if (easdf_info->upf_n6_ip_address_list) {
    cJSON *upf_n6_ip_address_listList = cJSON_AddArrayToObject(item, "upfN6IpAddressList");
    if (upf_n6_ip_address_listList == NULL) {
        ogs_error("OpenAPI_easdf_info_convertToJSON() failed [upf_n6_ip_address_list]");
        goto end;
    }
    OpenAPI_list_for_each(easdf_info->upf_n6_ip_address_list, node) {
        cJSON *itemLocal = OpenAPI_ip_addr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_easdf_info_convertToJSON() failed [upf_n6_ip_address_list]");
            goto end;
        }
        cJSON_AddItemToArray(upf_n6_ip_address_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_easdf_info_t *OpenAPI_easdf_info_parseFromJSON(cJSON *easdf_infoJSON)
{
    OpenAPI_easdf_info_t *easdf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_easdf_info_list = NULL;
    OpenAPI_list_t *s_nssai_easdf_info_listList = NULL;
    cJSON *easdf_n6_ip_address_list = NULL;
    OpenAPI_list_t *easdf_n6_ip_address_listList = NULL;
    cJSON *upf_n6_ip_address_list = NULL;
    OpenAPI_list_t *upf_n6_ip_address_listList = NULL;
    s_nssai_easdf_info_list = cJSON_GetObjectItemCaseSensitive(easdf_infoJSON, "sNssaiEasdfInfoList");
    if (s_nssai_easdf_info_list) {
        cJSON *s_nssai_easdf_info_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_easdf_info_list)) {
            ogs_error("OpenAPI_easdf_info_parseFromJSON() failed [s_nssai_easdf_info_list]");
            goto end;
        }

        s_nssai_easdf_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_easdf_info_list_local, s_nssai_easdf_info_list) {
            if (!cJSON_IsObject(s_nssai_easdf_info_list_local)) {
                ogs_error("OpenAPI_easdf_info_parseFromJSON() failed [s_nssai_easdf_info_list]");
                goto end;
            }
            OpenAPI_snssai_easdf_info_item_t *s_nssai_easdf_info_listItem = OpenAPI_snssai_easdf_info_item_parseFromJSON(s_nssai_easdf_info_list_local);
            if (!s_nssai_easdf_info_listItem) {
                ogs_error("No s_nssai_easdf_info_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_easdf_info_listList, s_nssai_easdf_info_listItem);
        }
    }

    easdf_n6_ip_address_list = cJSON_GetObjectItemCaseSensitive(easdf_infoJSON, "easdfN6IpAddressList");
    if (easdf_n6_ip_address_list) {
        cJSON *easdf_n6_ip_address_list_local = NULL;
        if (!cJSON_IsArray(easdf_n6_ip_address_list)) {
            ogs_error("OpenAPI_easdf_info_parseFromJSON() failed [easdf_n6_ip_address_list]");
            goto end;
        }

        easdf_n6_ip_address_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(easdf_n6_ip_address_list_local, easdf_n6_ip_address_list) {
            if (!cJSON_IsObject(easdf_n6_ip_address_list_local)) {
                ogs_error("OpenAPI_easdf_info_parseFromJSON() failed [easdf_n6_ip_address_list]");
                goto end;
            }
            OpenAPI_ip_addr_t *easdf_n6_ip_address_listItem = OpenAPI_ip_addr_parseFromJSON(easdf_n6_ip_address_list_local);
            if (!easdf_n6_ip_address_listItem) {
                ogs_error("No easdf_n6_ip_address_listItem");
                goto end;
            }
            OpenAPI_list_add(easdf_n6_ip_address_listList, easdf_n6_ip_address_listItem);
        }
    }

    upf_n6_ip_address_list = cJSON_GetObjectItemCaseSensitive(easdf_infoJSON, "upfN6IpAddressList");
    if (upf_n6_ip_address_list) {
        cJSON *upf_n6_ip_address_list_local = NULL;
        if (!cJSON_IsArray(upf_n6_ip_address_list)) {
            ogs_error("OpenAPI_easdf_info_parseFromJSON() failed [upf_n6_ip_address_list]");
            goto end;
        }

        upf_n6_ip_address_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(upf_n6_ip_address_list_local, upf_n6_ip_address_list) {
            if (!cJSON_IsObject(upf_n6_ip_address_list_local)) {
                ogs_error("OpenAPI_easdf_info_parseFromJSON() failed [upf_n6_ip_address_list]");
                goto end;
            }
            OpenAPI_ip_addr_t *upf_n6_ip_address_listItem = OpenAPI_ip_addr_parseFromJSON(upf_n6_ip_address_list_local);
            if (!upf_n6_ip_address_listItem) {
                ogs_error("No upf_n6_ip_address_listItem");
                goto end;
            }
            OpenAPI_list_add(upf_n6_ip_address_listList, upf_n6_ip_address_listItem);
        }
    }

    easdf_info_local_var = OpenAPI_easdf_info_create (
        s_nssai_easdf_info_list ? s_nssai_easdf_info_listList : NULL,
        easdf_n6_ip_address_list ? easdf_n6_ip_address_listList : NULL,
        upf_n6_ip_address_list ? upf_n6_ip_address_listList : NULL
    );

    return easdf_info_local_var;
end:
    if (s_nssai_easdf_info_listList) {
        OpenAPI_list_for_each(s_nssai_easdf_info_listList, node) {
            OpenAPI_snssai_easdf_info_item_free(node->data);
        }
        OpenAPI_list_free(s_nssai_easdf_info_listList);
        s_nssai_easdf_info_listList = NULL;
    }
    if (easdf_n6_ip_address_listList) {
        OpenAPI_list_for_each(easdf_n6_ip_address_listList, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(easdf_n6_ip_address_listList);
        easdf_n6_ip_address_listList = NULL;
    }
    if (upf_n6_ip_address_listList) {
        OpenAPI_list_for_each(upf_n6_ip_address_listList, node) {
            OpenAPI_ip_addr_free(node->data);
        }
        OpenAPI_list_free(upf_n6_ip_address_listList);
        upf_n6_ip_address_listList = NULL;
    }
    return NULL;
}

OpenAPI_easdf_info_t *OpenAPI_easdf_info_copy(OpenAPI_easdf_info_t *dst, OpenAPI_easdf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_easdf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_easdf_info_convertToJSON() failed");
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

    OpenAPI_easdf_info_free(dst);
    dst = OpenAPI_easdf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

