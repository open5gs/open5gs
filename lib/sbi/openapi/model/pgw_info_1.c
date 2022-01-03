
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pgw_info_1.h"

OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_create(
    char *dnn,
    char *pgw_fqdn,
    OpenAPI_plmn_id_1_t *plmn_id,
    bool is_epdg_ind,
    int epdg_ind
)
{
    OpenAPI_pgw_info_1_t *pgw_info_1_local_var = ogs_malloc(sizeof(OpenAPI_pgw_info_1_t));
    ogs_assert(pgw_info_1_local_var);

    pgw_info_1_local_var->dnn = dnn;
    pgw_info_1_local_var->pgw_fqdn = pgw_fqdn;
    pgw_info_1_local_var->plmn_id = plmn_id;
    pgw_info_1_local_var->is_epdg_ind = is_epdg_ind;
    pgw_info_1_local_var->epdg_ind = epdg_ind;

    return pgw_info_1_local_var;
}

void OpenAPI_pgw_info_1_free(OpenAPI_pgw_info_1_t *pgw_info_1)
{
    if (NULL == pgw_info_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pgw_info_1->dnn);
    ogs_free(pgw_info_1->pgw_fqdn);
    OpenAPI_plmn_id_1_free(pgw_info_1->plmn_id);
    ogs_free(pgw_info_1);
}

cJSON *OpenAPI_pgw_info_1_convertToJSON(OpenAPI_pgw_info_1_t *pgw_info_1)
{
    cJSON *item = NULL;

    if (pgw_info_1 == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [PgwInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "dnn", pgw_info_1->dnn) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [dnn]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "pgwFqdn", pgw_info_1->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }

    if (pgw_info_1->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(pgw_info_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (pgw_info_1->is_epdg_ind) {
    if (cJSON_AddBoolToObject(item, "epdgInd", pgw_info_1->epdg_ind) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [epdg_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_parseFromJSON(cJSON *pgw_info_1JSON)
{
    OpenAPI_pgw_info_1_t *pgw_info_1_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [dnn]");
        goto end;
    }

    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "pgwFqdn");
    if (!pgw_fqdn) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }

    if (!cJSON_IsString(pgw_fqdn)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "plmnId");

    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    }

    cJSON *epdg_ind = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "epdgInd");

    if (epdg_ind) {
    if (!cJSON_IsBool(epdg_ind)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [epdg_ind]");
        goto end;
    }
    }

    pgw_info_1_local_var = OpenAPI_pgw_info_1_create (
        ogs_strdup(dnn->valuestring),
        ogs_strdup(pgw_fqdn->valuestring),
        plmn_id ? plmn_id_local_nonprim : NULL,
        epdg_ind ? true : false,
        epdg_ind ? epdg_ind->valueint : 0
    );

    return pgw_info_1_local_var;
end:
    return NULL;
}

OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_copy(OpenAPI_pgw_info_1_t *dst, OpenAPI_pgw_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pgw_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed");
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

    OpenAPI_pgw_info_1_free(dst);
    dst = OpenAPI_pgw_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

