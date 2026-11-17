
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_g_multicast_service.h"

OpenAPI_model_5_g_multicast_service_t *OpenAPI_model_5_g_multicast_service_create(
    OpenAPI_list_t *m_bs_session_id_list
)
{
    OpenAPI_model_5_g_multicast_service_t *model_5_g_multicast_service_local_var = ogs_malloc(sizeof(OpenAPI_model_5_g_multicast_service_t));
    ogs_assert(model_5_g_multicast_service_local_var);

    model_5_g_multicast_service_local_var->m_bs_session_id_list = m_bs_session_id_list;

    return model_5_g_multicast_service_local_var;
}

void OpenAPI_model_5_g_multicast_service_free(OpenAPI_model_5_g_multicast_service_t *model_5_g_multicast_service)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_g_multicast_service) {
        return;
    }
    if (model_5_g_multicast_service->m_bs_session_id_list) {
        OpenAPI_list_for_each(model_5_g_multicast_service->m_bs_session_id_list, node) {
            OpenAPI_mbs_session_id_free(node->data);
        }
        OpenAPI_list_free(model_5_g_multicast_service->m_bs_session_id_list);
        model_5_g_multicast_service->m_bs_session_id_list = NULL;
    }
    ogs_free(model_5_g_multicast_service);
}

cJSON *OpenAPI_model_5_g_multicast_service_convertToJSON(OpenAPI_model_5_g_multicast_service_t *model_5_g_multicast_service)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_g_multicast_service == NULL) {
        ogs_error("OpenAPI_model_5_g_multicast_service_convertToJSON() failed [5GMulticastService]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (model_5_g_multicast_service->m_bs_session_id_list) {
    cJSON *m_bs_session_id_listList = cJSON_AddArrayToObject(item, "mBSSessionIdList");
    if (m_bs_session_id_listList == NULL) {
        ogs_error("OpenAPI_model_5_g_multicast_service_convertToJSON() failed [m_bs_session_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(model_5_g_multicast_service->m_bs_session_id_list, node) {
        cJSON *itemLocal = OpenAPI_mbs_session_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_model_5_g_multicast_service_convertToJSON() failed [m_bs_session_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(m_bs_session_id_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_model_5_g_multicast_service_t *OpenAPI_model_5_g_multicast_service_parseFromJSON(cJSON *model_5_g_multicast_serviceJSON)
{
    OpenAPI_model_5_g_multicast_service_t *model_5_g_multicast_service_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *m_bs_session_id_list = NULL;
    OpenAPI_list_t *m_bs_session_id_listList = NULL;
    m_bs_session_id_list = cJSON_GetObjectItemCaseSensitive(model_5_g_multicast_serviceJSON, "mBSSessionIdList");
    if (m_bs_session_id_list) {
        cJSON *m_bs_session_id_list_local = NULL;
        if (!cJSON_IsArray(m_bs_session_id_list)) {
            ogs_error("OpenAPI_model_5_g_multicast_service_parseFromJSON() failed [m_bs_session_id_list]");
            goto end;
        }

        m_bs_session_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(m_bs_session_id_list_local, m_bs_session_id_list) {
            if (!cJSON_IsObject(m_bs_session_id_list_local)) {
                ogs_error("OpenAPI_model_5_g_multicast_service_parseFromJSON() failed [m_bs_session_id_list]");
                goto end;
            }
            OpenAPI_mbs_session_id_t *m_bs_session_id_listItem = OpenAPI_mbs_session_id_parseFromJSON(m_bs_session_id_list_local);
            if (!m_bs_session_id_listItem) {
                ogs_error("No m_bs_session_id_listItem");
                goto end;
            }
            OpenAPI_list_add(m_bs_session_id_listList, m_bs_session_id_listItem);
        }
    }

    model_5_g_multicast_service_local_var = OpenAPI_model_5_g_multicast_service_create (
        m_bs_session_id_list ? m_bs_session_id_listList : NULL
    );

    return model_5_g_multicast_service_local_var;
end:
    if (m_bs_session_id_listList) {
        OpenAPI_list_for_each(m_bs_session_id_listList, node) {
            OpenAPI_mbs_session_id_free(node->data);
        }
        OpenAPI_list_free(m_bs_session_id_listList);
        m_bs_session_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_model_5_g_multicast_service_t *OpenAPI_model_5_g_multicast_service_copy(OpenAPI_model_5_g_multicast_service_t *dst, OpenAPI_model_5_g_multicast_service_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_g_multicast_service_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_g_multicast_service_convertToJSON() failed");
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

    OpenAPI_model_5_g_multicast_service_free(dst);
    dst = OpenAPI_model_5_g_multicast_service_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

