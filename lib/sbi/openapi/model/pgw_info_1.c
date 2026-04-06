
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pgw_info_1.h"

OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_create(
    char *dnn,
    char *pgw_fqdn,
    OpenAPI_ip_address_1_t *pgw_ip_addr,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_epdg_ind,
    int epdg_ind,
    char *pcf_id,
    char *registration_time,
    bool is_wildcard_ind,
    int wildcard_ind
)
{
    OpenAPI_pgw_info_1_t *pgw_info_1_local_var = ogs_malloc(sizeof(OpenAPI_pgw_info_1_t));
    ogs_assert(pgw_info_1_local_var);

    pgw_info_1_local_var->dnn = dnn;
    pgw_info_1_local_var->pgw_fqdn = pgw_fqdn;
    pgw_info_1_local_var->pgw_ip_addr = pgw_ip_addr;
    pgw_info_1_local_var->plmn_id = plmn_id;
    pgw_info_1_local_var->is_epdg_ind = is_epdg_ind;
    pgw_info_1_local_var->epdg_ind = epdg_ind;
    pgw_info_1_local_var->pcf_id = pcf_id;
    pgw_info_1_local_var->registration_time = registration_time;
    pgw_info_1_local_var->is_wildcard_ind = is_wildcard_ind;
    pgw_info_1_local_var->wildcard_ind = wildcard_ind;

    return pgw_info_1_local_var;
}

void OpenAPI_pgw_info_1_free(OpenAPI_pgw_info_1_t *pgw_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pgw_info_1) {
        return;
    }
    if (pgw_info_1->dnn) {
        ogs_free(pgw_info_1->dnn);
        pgw_info_1->dnn = NULL;
    }
    if (pgw_info_1->pgw_fqdn) {
        ogs_free(pgw_info_1->pgw_fqdn);
        pgw_info_1->pgw_fqdn = NULL;
    }
    if (pgw_info_1->pgw_ip_addr) {
        OpenAPI_ip_address_1_free(pgw_info_1->pgw_ip_addr);
        pgw_info_1->pgw_ip_addr = NULL;
    }
    if (pgw_info_1->plmn_id) {
        OpenAPI_plmn_id_free(pgw_info_1->plmn_id);
        pgw_info_1->plmn_id = NULL;
    }
    if (pgw_info_1->pcf_id) {
        ogs_free(pgw_info_1->pcf_id);
        pgw_info_1->pcf_id = NULL;
    }
    if (pgw_info_1->registration_time) {
        ogs_free(pgw_info_1->registration_time);
        pgw_info_1->registration_time = NULL;
    }
    ogs_free(pgw_info_1);
}

cJSON *OpenAPI_pgw_info_1_convertToJSON(OpenAPI_pgw_info_1_t *pgw_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pgw_info_1 == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [PgwInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pgw_info_1->dnn) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", pgw_info_1->dnn) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!pgw_info_1->pgw_fqdn) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [pgw_fqdn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pgwFqdn", pgw_info_1->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }

    if (pgw_info_1->pgw_ip_addr) {
    cJSON *pgw_ip_addr_local_JSON = OpenAPI_ip_address_1_convertToJSON(pgw_info_1->pgw_ip_addr);
    if (pgw_ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [pgw_ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pgwIpAddr", pgw_ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [pgw_ip_addr]");
        goto end;
    }
    }

    if (pgw_info_1->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(pgw_info_1->plmn_id);
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

    if (pgw_info_1->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pgw_info_1->pcf_id) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pgw_info_1->registration_time) {
    if (cJSON_AddStringToObject(item, "registrationTime", pgw_info_1->registration_time) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [registration_time]");
        goto end;
    }
    }

    if (pgw_info_1->is_wildcard_ind) {
    if (cJSON_AddBoolToObject(item, "wildcardInd", pgw_info_1->wildcard_ind) == NULL) {
        ogs_error("OpenAPI_pgw_info_1_convertToJSON() failed [wildcard_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pgw_info_1_t *OpenAPI_pgw_info_1_parseFromJSON(cJSON *pgw_info_1JSON)
{
    OpenAPI_pgw_info_1_t *pgw_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *pgw_fqdn = NULL;
    cJSON *pgw_ip_addr = NULL;
    OpenAPI_ip_address_1_t *pgw_ip_addr_local_nonprim = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *epdg_ind = NULL;
    cJSON *pcf_id = NULL;
    cJSON *registration_time = NULL;
    cJSON *wildcard_ind = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [dnn]");
        goto end;
    }

    pgw_fqdn = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "pgwFqdn");
    if (!pgw_fqdn) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    if (!cJSON_IsString(pgw_fqdn)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }

    pgw_ip_addr = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "pgwIpAddr");
    if (pgw_ip_addr) {
    pgw_ip_addr_local_nonprim = OpenAPI_ip_address_1_parseFromJSON(pgw_ip_addr);
    if (!pgw_ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_1_parseFromJSON failed [pgw_ip_addr]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    epdg_ind = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "epdgInd");
    if (epdg_ind) {
    if (!cJSON_IsBool(epdg_ind)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [epdg_ind]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    registration_time = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "registrationTime");
    if (registration_time) {
    if (!cJSON_IsString(registration_time) && !cJSON_IsNull(registration_time)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [registration_time]");
        goto end;
    }
    }

    wildcard_ind = cJSON_GetObjectItemCaseSensitive(pgw_info_1JSON, "wildcardInd");
    if (wildcard_ind) {
    if (!cJSON_IsBool(wildcard_ind)) {
        ogs_error("OpenAPI_pgw_info_1_parseFromJSON() failed [wildcard_ind]");
        goto end;
    }
    }

    pgw_info_1_local_var = OpenAPI_pgw_info_1_create (
        ogs_strdup(dnn->valuestring),
        ogs_strdup(pgw_fqdn->valuestring),
        pgw_ip_addr ? pgw_ip_addr_local_nonprim : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        epdg_ind ? true : false,
        epdg_ind ? epdg_ind->valueint : 0,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        registration_time && !cJSON_IsNull(registration_time) ? ogs_strdup(registration_time->valuestring) : NULL,
        wildcard_ind ? true : false,
        wildcard_ind ? wildcard_ind->valueint : 0
    );

    return pgw_info_1_local_var;
end:
    if (pgw_ip_addr_local_nonprim) {
        OpenAPI_ip_address_1_free(pgw_ip_addr_local_nonprim);
        pgw_ip_addr_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
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

