
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_area.h"

OpenAPI_service_area_t *OpenAPI_service_area_create(
    bool is_m_bs_service_area_null,
    OpenAPI_mbs_service_area_t *m_bs_service_area,
    OpenAPI_list_t *u_pfids,
    OpenAPI_list_t *ran_node_ids
)
{
    OpenAPI_service_area_t *service_area_local_var = ogs_malloc(sizeof(OpenAPI_service_area_t));
    ogs_assert(service_area_local_var);

    service_area_local_var->is_m_bs_service_area_null = is_m_bs_service_area_null;
    service_area_local_var->m_bs_service_area = m_bs_service_area;
    service_area_local_var->u_pfids = u_pfids;
    service_area_local_var->ran_node_ids = ran_node_ids;

    return service_area_local_var;
}

void OpenAPI_service_area_free(OpenAPI_service_area_t *service_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_area) {
        return;
    }
    if (service_area->m_bs_service_area) {
        OpenAPI_mbs_service_area_free(service_area->m_bs_service_area);
        service_area->m_bs_service_area = NULL;
    }
    if (service_area->u_pfids) {
        OpenAPI_list_for_each(service_area->u_pfids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_area->u_pfids);
        service_area->u_pfids = NULL;
    }
    if (service_area->ran_node_ids) {
        OpenAPI_list_for_each(service_area->ran_node_ids, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(service_area->ran_node_ids);
        service_area->ran_node_ids = NULL;
    }
    ogs_free(service_area);
}

cJSON *OpenAPI_service_area_convertToJSON(OpenAPI_service_area_t *service_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_area == NULL) {
        ogs_error("OpenAPI_service_area_convertToJSON() failed [ServiceArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_area->m_bs_service_area) {
    cJSON *m_bs_service_area_local_JSON = OpenAPI_mbs_service_area_convertToJSON(service_area->m_bs_service_area);
    if (m_bs_service_area_local_JSON == NULL) {
        ogs_error("OpenAPI_service_area_convertToJSON() failed [m_bs_service_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mBSServiceArea", m_bs_service_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_area_convertToJSON() failed [m_bs_service_area]");
        goto end;
    }
    } else if (service_area->is_m_bs_service_area_null) {
        if (cJSON_AddNullToObject(item, "mBSServiceArea") == NULL) {
            ogs_error("OpenAPI_service_area_convertToJSON() failed [m_bs_service_area]");
            goto end;
        }
    }

    if (service_area->u_pfids) {
    cJSON *u_pfidsList = cJSON_AddArrayToObject(item, "uPFIDs");
    if (u_pfidsList == NULL) {
        ogs_error("OpenAPI_service_area_convertToJSON() failed [u_pfids]");
        goto end;
    }
    OpenAPI_list_for_each(service_area->u_pfids, node) {
        if (cJSON_AddStringToObject(u_pfidsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_area_convertToJSON() failed [u_pfids]");
            goto end;
        }
    }
    }

    if (service_area->ran_node_ids) {
    cJSON *ran_node_idsList = cJSON_AddArrayToObject(item, "ranNodeIDs");
    if (ran_node_idsList == NULL) {
        ogs_error("OpenAPI_service_area_convertToJSON() failed [ran_node_ids]");
        goto end;
    }
    OpenAPI_list_for_each(service_area->ran_node_ids, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_area_convertToJSON() failed [ran_node_ids]");
            goto end;
        }
        cJSON_AddItemToArray(ran_node_idsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_service_area_t *OpenAPI_service_area_parseFromJSON(cJSON *service_areaJSON)
{
    OpenAPI_service_area_t *service_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *m_bs_service_area = NULL;
    OpenAPI_mbs_service_area_t *m_bs_service_area_local_nonprim = NULL;
    cJSON *u_pfids = NULL;
    OpenAPI_list_t *u_pfidsList = NULL;
    cJSON *ran_node_ids = NULL;
    OpenAPI_list_t *ran_node_idsList = NULL;
    m_bs_service_area = cJSON_GetObjectItemCaseSensitive(service_areaJSON, "mBSServiceArea");
    if (m_bs_service_area) {
    if (!cJSON_IsNull(m_bs_service_area)) {
    m_bs_service_area_local_nonprim = OpenAPI_mbs_service_area_parseFromJSON(m_bs_service_area);
    if (!m_bs_service_area_local_nonprim) {
        ogs_error("OpenAPI_mbs_service_area_parseFromJSON failed [m_bs_service_area]");
        goto end;
    }
    }
    }

    u_pfids = cJSON_GetObjectItemCaseSensitive(service_areaJSON, "uPFIDs");
    if (u_pfids) {
        cJSON *u_pfids_local = NULL;
        if (!cJSON_IsArray(u_pfids)) {
            ogs_error("OpenAPI_service_area_parseFromJSON() failed [u_pfids]");
            goto end;
        }

        u_pfidsList = OpenAPI_list_create();

        cJSON_ArrayForEach(u_pfids_local, u_pfids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(u_pfids_local)) {
                ogs_error("OpenAPI_service_area_parseFromJSON() failed [u_pfids]");
                goto end;
            }
            OpenAPI_list_add(u_pfidsList, ogs_strdup(u_pfids_local->valuestring));
        }
    }

    ran_node_ids = cJSON_GetObjectItemCaseSensitive(service_areaJSON, "ranNodeIDs");
    if (ran_node_ids) {
        cJSON *ran_node_ids_local = NULL;
        if (!cJSON_IsArray(ran_node_ids)) {
            ogs_error("OpenAPI_service_area_parseFromJSON() failed [ran_node_ids]");
            goto end;
        }

        ran_node_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_node_ids_local, ran_node_ids) {
            if (!cJSON_IsObject(ran_node_ids_local)) {
                ogs_error("OpenAPI_service_area_parseFromJSON() failed [ran_node_ids]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *ran_node_idsItem = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_ids_local);
            if (!ran_node_idsItem) {
                ogs_error("No ran_node_idsItem");
                goto end;
            }
            OpenAPI_list_add(ran_node_idsList, ran_node_idsItem);
        }
    }

    service_area_local_var = OpenAPI_service_area_create (
        m_bs_service_area && cJSON_IsNull(m_bs_service_area) ? true : false,
        m_bs_service_area ? m_bs_service_area_local_nonprim : NULL,
        u_pfids ? u_pfidsList : NULL,
        ran_node_ids ? ran_node_idsList : NULL
    );

    return service_area_local_var;
end:
    if (m_bs_service_area_local_nonprim) {
        OpenAPI_mbs_service_area_free(m_bs_service_area_local_nonprim);
        m_bs_service_area_local_nonprim = NULL;
    }
    if (u_pfidsList) {
        OpenAPI_list_for_each(u_pfidsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(u_pfidsList);
        u_pfidsList = NULL;
    }
    if (ran_node_idsList) {
        OpenAPI_list_for_each(ran_node_idsList, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(ran_node_idsList);
        ran_node_idsList = NULL;
    }
    return NULL;
}

OpenAPI_service_area_t *OpenAPI_service_area_copy(OpenAPI_service_area_t *dst, OpenAPI_service_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_area_convertToJSON() failed");
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

    OpenAPI_service_area_free(dst);
    dst = OpenAPI_service_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

