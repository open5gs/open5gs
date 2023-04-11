
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "emergency_info.h"

OpenAPI_emergency_info_t *OpenAPI_emergency_info_create(
    char *pgw_fqdn,
    OpenAPI_ip_address_t *pgw_ip_address,
    char *smf_instance_id,
    bool is_epdg_ind,
    int epdg_ind,
    OpenAPI_plmn_id_t *plmn_id
)
{
    OpenAPI_emergency_info_t *emergency_info_local_var = ogs_malloc(sizeof(OpenAPI_emergency_info_t));
    ogs_assert(emergency_info_local_var);

    emergency_info_local_var->pgw_fqdn = pgw_fqdn;
    emergency_info_local_var->pgw_ip_address = pgw_ip_address;
    emergency_info_local_var->smf_instance_id = smf_instance_id;
    emergency_info_local_var->is_epdg_ind = is_epdg_ind;
    emergency_info_local_var->epdg_ind = epdg_ind;
    emergency_info_local_var->plmn_id = plmn_id;

    return emergency_info_local_var;
}

void OpenAPI_emergency_info_free(OpenAPI_emergency_info_t *emergency_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == emergency_info) {
        return;
    }
    if (emergency_info->pgw_fqdn) {
        ogs_free(emergency_info->pgw_fqdn);
        emergency_info->pgw_fqdn = NULL;
    }
    if (emergency_info->pgw_ip_address) {
        OpenAPI_ip_address_free(emergency_info->pgw_ip_address);
        emergency_info->pgw_ip_address = NULL;
    }
    if (emergency_info->smf_instance_id) {
        ogs_free(emergency_info->smf_instance_id);
        emergency_info->smf_instance_id = NULL;
    }
    if (emergency_info->plmn_id) {
        OpenAPI_plmn_id_free(emergency_info->plmn_id);
        emergency_info->plmn_id = NULL;
    }
    ogs_free(emergency_info);
}

cJSON *OpenAPI_emergency_info_convertToJSON(OpenAPI_emergency_info_t *emergency_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (emergency_info == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [EmergencyInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (emergency_info->pgw_fqdn) {
    if (cJSON_AddStringToObject(item, "pgwFqdn", emergency_info->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    if (emergency_info->pgw_ip_address) {
    cJSON *pgw_ip_address_local_JSON = OpenAPI_ip_address_convertToJSON(emergency_info->pgw_ip_address);
    if (pgw_ip_address_local_JSON == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [pgw_ip_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pgwIpAddress", pgw_ip_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [pgw_ip_address]");
        goto end;
    }
    }

    if (emergency_info->smf_instance_id) {
    if (cJSON_AddStringToObject(item, "smfInstanceId", emergency_info->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [smf_instance_id]");
        goto end;
    }
    }

    if (emergency_info->is_epdg_ind) {
    if (cJSON_AddBoolToObject(item, "epdgInd", emergency_info->epdg_ind) == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [epdg_ind]");
        goto end;
    }
    }

    if (emergency_info->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(emergency_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_emergency_info_t *OpenAPI_emergency_info_parseFromJSON(cJSON *emergency_infoJSON)
{
    OpenAPI_emergency_info_t *emergency_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pgw_fqdn = NULL;
    cJSON *pgw_ip_address = NULL;
    OpenAPI_ip_address_t *pgw_ip_address_local_nonprim = NULL;
    cJSON *smf_instance_id = NULL;
    cJSON *epdg_ind = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    pgw_fqdn = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "pgwFqdn");
    if (pgw_fqdn) {
    if (!cJSON_IsString(pgw_fqdn) && !cJSON_IsNull(pgw_fqdn)) {
        ogs_error("OpenAPI_emergency_info_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    pgw_ip_address = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "pgwIpAddress");
    if (pgw_ip_address) {
    pgw_ip_address_local_nonprim = OpenAPI_ip_address_parseFromJSON(pgw_ip_address);
    if (!pgw_ip_address_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [pgw_ip_address]");
        goto end;
    }
    }

    smf_instance_id = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "smfInstanceId");
    if (smf_instance_id) {
    if (!cJSON_IsString(smf_instance_id) && !cJSON_IsNull(smf_instance_id)) {
        ogs_error("OpenAPI_emergency_info_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }
    }

    epdg_ind = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "epdgInd");
    if (epdg_ind) {
    if (!cJSON_IsBool(epdg_ind)) {
        ogs_error("OpenAPI_emergency_info_parseFromJSON() failed [epdg_ind]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    emergency_info_local_var = OpenAPI_emergency_info_create (
        pgw_fqdn && !cJSON_IsNull(pgw_fqdn) ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        pgw_ip_address ? pgw_ip_address_local_nonprim : NULL,
        smf_instance_id && !cJSON_IsNull(smf_instance_id) ? ogs_strdup(smf_instance_id->valuestring) : NULL,
        epdg_ind ? true : false,
        epdg_ind ? epdg_ind->valueint : 0,
        plmn_id ? plmn_id_local_nonprim : NULL
    );

    return emergency_info_local_var;
end:
    if (pgw_ip_address_local_nonprim) {
        OpenAPI_ip_address_free(pgw_ip_address_local_nonprim);
        pgw_ip_address_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_emergency_info_t *OpenAPI_emergency_info_copy(OpenAPI_emergency_info_t *dst, OpenAPI_emergency_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_emergency_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_emergency_info_convertToJSON() failed");
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

    OpenAPI_emergency_info_free(dst);
    dst = OpenAPI_emergency_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

