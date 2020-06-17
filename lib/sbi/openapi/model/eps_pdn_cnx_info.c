
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_pdn_cnx_info.h"

OpenAPI_eps_pdn_cnx_info_t *OpenAPI_eps_pdn_cnx_info_create(
    char pgw_s8c_fteid,
    char pgw_node_name,
    int linked_bearer_id
    )
{
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_eps_pdn_cnx_info_t));
    if (!eps_pdn_cnx_info_local_var) {
        return NULL;
    }
    eps_pdn_cnx_info_local_var->pgw_s8c_fteid = pgw_s8c_fteid;
    eps_pdn_cnx_info_local_var->pgw_node_name = pgw_node_name;
    eps_pdn_cnx_info_local_var->linked_bearer_id = linked_bearer_id;

    return eps_pdn_cnx_info_local_var;
}

void OpenAPI_eps_pdn_cnx_info_free(OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info)
{
    if (NULL == eps_pdn_cnx_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(eps_pdn_cnx_info);
}

cJSON *OpenAPI_eps_pdn_cnx_info_convertToJSON(OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info)
{
    cJSON *item = NULL;

    if (eps_pdn_cnx_info == NULL) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [EpsPdnCnxInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eps_pdn_cnx_info->pgw_s8c_fteid) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [pgw_s8c_fteid]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pgwS8cFteid", eps_pdn_cnx_info->pgw_s8c_fteid) == NULL) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [pgw_s8c_fteid]");
        goto end;
    }

    if (eps_pdn_cnx_info->pgw_node_name) {
        if (cJSON_AddNumberToObject(item, "pgwNodeName", eps_pdn_cnx_info->pgw_node_name) == NULL) {
            ogs_error("OpenAPI_eps_pdn_cnx_info_convertToJSON() failed [pgw_node_name]");
            goto end;
        }
    }

    if (eps_pdn_cnx_info->linked_bearer_id) {
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
    cJSON *pgw_s8c_fteid = cJSON_GetObjectItemCaseSensitive(eps_pdn_cnx_infoJSON, "pgwS8cFteid");
    if (!pgw_s8c_fteid) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [pgw_s8c_fteid]");
        goto end;
    }


    if (!cJSON_IsNumber(pgw_s8c_fteid)) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [pgw_s8c_fteid]");
        goto end;
    }

    cJSON *pgw_node_name = cJSON_GetObjectItemCaseSensitive(eps_pdn_cnx_infoJSON, "pgwNodeName");

    if (pgw_node_name) {
        if (!cJSON_IsNumber(pgw_node_name)) {
            ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [pgw_node_name]");
            goto end;
        }
    }

    cJSON *linked_bearer_id = cJSON_GetObjectItemCaseSensitive(eps_pdn_cnx_infoJSON, "linkedBearerId");

    if (linked_bearer_id) {
        if (!cJSON_IsNumber(linked_bearer_id)) {
            ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON() failed [linked_bearer_id]");
            goto end;
        }
    }

    eps_pdn_cnx_info_local_var = OpenAPI_eps_pdn_cnx_info_create (
        pgw_s8c_fteid->valueint,
        pgw_node_name ? pgw_node_name->valueint : 0,
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

