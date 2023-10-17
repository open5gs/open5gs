
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_pdn_cnx_info.h"

OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_create(
    char *pgw_s8c_fteid,
    char *pgw_node_name,
    bool is_linked_bearer_id,
    int linked_bearer_id
)
{
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_var = ogs_malloc(sizeof(OpenAPI_eps_pdn_cnx_info_t));
    ogs_assert(eps_pdn_cnx_info_local_var);

    eps_pdn_cnx_info_local_var->pgw_s8c_fteid = pgw_s8c_fteid;
    eps_pdn_cnx_info_local_var->pgw_node_name = pgw_node_name;
    eps_pdn_cnx_info_local_var->is_linked_bearer_id = is_linked_bearer_id;
    eps_pdn_cnx_info_local_var->linked_bearer_id = linked_bearer_id;

    return eps_pdn_cnx_info_local_var;
}

void OpenAPI_eps_pdn_cnx_info_free(OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eps_pdn_cnx_info) {
        return;
    }
    if (eps_pdn_cnx_info->pgw_s8c_fteid) {
        ogs_free(eps_pdn_cnx_info->pgw_s8c_fteid);
        eps_pdn_cnx_info->pgw_s8c_fteid = NULL;
    }
    if (eps_pdn_cnx_info->pgw_node_name) {
        ogs_free(eps_pdn_cnx_info->pgw_node_name);
        eps_pdn_cnx_info->pgw_node_name = NULL;
    }
    ogs_free(eps_pdn_cnx_info);
}

cJSON *OpenAPI_eps_pdn_cnx_info_convertToJSON(OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eps_pdn_cnx_info == NULL) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [EpsPdnCnxInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eps_pdn_cnx_info->pgw_s8c_fteid) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [pgw_s8c_fteid]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pgwS8cFteid", eps_pdn_cnx_info->pgw_s8c_fteid) == NULL) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [pgw_s8c_fteid]");
        goto end;
    }

    if (eps_pdn_cnx_info->pgw_node_name) {
    if (cJSON_AddStringToObject(item, "pgwNodeName", eps_pdn_cnx_info->pgw_node_name) == NULL) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [pgw_node_name]");
        goto end;
    }
    }

    if (eps_pdn_cnx_info->is_linked_bearer_id) {
    if (cJSON_AddNumberToObject(item, "linkedBearerId", eps_pdn_cnx_info->linked_bearer_id) == NULL) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [linked_bearer_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_parseFromJSON(cJSON *eps_pdn_cnx_infoJSON)
{
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pgw_s8c_fteid = NULL;
    cJSON *pgw_node_name = NULL;
    cJSON *linked_bearer_id = NULL;
    pgw_s8c_fteid = cJSON_GetObjectItemCaseSensitive(eps_pdn_cnx_infoJSON, "pgwS8cFteid");
    if (!pgw_s8c_fteid) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [pgw_s8c_fteid]");
        goto end;
    }
    if (!cJSON_IsString(pgw_s8c_fteid)) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [pgw_s8c_fteid]");
        goto end;
    }

    pgw_node_name = cJSON_GetObjectItemCaseSensitive(eps_pdn_cnx_infoJSON, "pgwNodeName");
    if (pgw_node_name) {
    if (!cJSON_IsString(pgw_node_name) && !cJSON_IsNull(pgw_node_name)) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [pgw_node_name]");
        goto end;
    }
    }

    linked_bearer_id = cJSON_GetObjectItemCaseSensitive(eps_pdn_cnx_infoJSON, "linkedBearerId");
    if (linked_bearer_id) {
    if (!cJSON_IsNumber(linked_bearer_id)) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [linked_bearer_id]");
        goto end;
    }
    }

    eps_pdn_cnx_info_local_var = OpenAPI_eps_pdn_cnx_info_create (
        ogs_strdup(pgw_s8c_fteid->valuestring),
        pgw_node_name && !cJSON_IsNull(pgw_node_name) ? ogs_strdup(pgw_node_name->valuestring) : NULL,
        linked_bearer_id ? true : false,
        linked_bearer_id ? linked_bearer_id->valuedouble : 0
    );

    return eps_pdn_cnx_info_local_var;
end:
    return NULL;
}

OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_copy(OpenAPI_eps_pdn_cnx_info_t *dst, OpenAPI_eps_pdn_cnx_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eps_pdn_cnx_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed");
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

    OpenAPI_eps_pdn_cnx_info_free(dst);
    dst = OpenAPI_eps_pdn_cnx_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

