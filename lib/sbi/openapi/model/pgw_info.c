
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pgw_info.h"

OpenAPI_pgw_info_t *OpenAPI_pgw_info_create(
    char *dnn,
    char *pgw_fqdn,
    OpenAPI_plmn_id_t *plmn_id
    )
{
    OpenAPI_pgw_info_t *pgw_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_pgw_info_t));
    if (!pgw_info_local_var) {
        return NULL;
    }
    pgw_info_local_var->dnn = dnn;
    pgw_info_local_var->pgw_fqdn = pgw_fqdn;
    pgw_info_local_var->plmn_id = plmn_id;

    return pgw_info_local_var;
}

void OpenAPI_pgw_info_free(OpenAPI_pgw_info_t *pgw_info)
{
    if (NULL == pgw_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pgw_info->dnn);
    ogs_free(pgw_info->pgw_fqdn);
    OpenAPI_plmn_id_free(pgw_info->plmn_id);
    ogs_free(pgw_info);
}

cJSON *OpenAPI_pgw_info_convertToJSON(OpenAPI_pgw_info_t *pgw_info)
{
    cJSON *item = NULL;

    if (pgw_info == NULL) {
        ogs_error("OpenAPI_pgw_info_convertToJSON() failed [PgwInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pgw_info->dnn) {
        ogs_error("OpenAPI_pgw_info_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", pgw_info->dnn) == NULL) {
        ogs_error("OpenAPI_pgw_info_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!pgw_info->pgw_fqdn) {
        ogs_error("OpenAPI_pgw_info_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "pgwFqdn", pgw_info->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_pgw_info_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }

    if (pgw_info->plmn_id) {
        cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(pgw_info->plmn_id);
        if (plmn_id_local_JSON == NULL) {
            ogs_error("OpenAPI_pgw_info_convertToJSON() failed [plmn_id]");
            goto end;
        }
        cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_pgw_info_convertToJSON() failed [plmn_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pgw_info_t *OpenAPI_pgw_info_parseFromJSON(cJSON *pgw_infoJSON)
{
    OpenAPI_pgw_info_t *pgw_info_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pgw_infoJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pgw_info_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pgw_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(pgw_infoJSON, "pgwFqdn");
    if (!pgw_fqdn) {
        ogs_error("OpenAPI_pgw_info_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }


    if (!cJSON_IsString(pgw_fqdn)) {
        ogs_error("OpenAPI_pgw_info_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(pgw_infoJSON, "plmnId");

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
        plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    }

    pgw_info_local_var = OpenAPI_pgw_info_create (
        ogs_strdup(dnn->valuestring),
        ogs_strdup(pgw_fqdn->valuestring),
        plmn_id ? plmn_id_local_nonprim : NULL
        );

    return pgw_info_local_var;
end:
    return NULL;
}

OpenAPI_pgw_info_t *OpenAPI_pgw_info_copy(OpenAPI_pgw_info_t *dst, OpenAPI_pgw_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pgw_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pgw_info_convertToJSON() failed");
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

    OpenAPI_pgw_info_free(dst);
    dst = OpenAPI_pgw_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

