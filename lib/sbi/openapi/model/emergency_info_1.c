
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "emergency_info_1.h"

OpenAPI_emergency_info_1_t *OpenAPI_emergency_info_1_create(
    char *pgw_fqdn,
    OpenAPI_ip_address_1_t *pgw_ip_address,
    char *smf_instance_id,
    bool is_epdg_ind,
    int epdg_ind
)
{
    OpenAPI_emergency_info_1_t *emergency_info_1_local_var = ogs_malloc(sizeof(OpenAPI_emergency_info_1_t));
    ogs_assert(emergency_info_1_local_var);

    emergency_info_1_local_var->pgw_fqdn = pgw_fqdn;
    emergency_info_1_local_var->pgw_ip_address = pgw_ip_address;
    emergency_info_1_local_var->smf_instance_id = smf_instance_id;
    emergency_info_1_local_var->is_epdg_ind = is_epdg_ind;
    emergency_info_1_local_var->epdg_ind = epdg_ind;

    return emergency_info_1_local_var;
}

void OpenAPI_emergency_info_1_free(OpenAPI_emergency_info_1_t *emergency_info_1)
{
    if (NULL == emergency_info_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(emergency_info_1->pgw_fqdn);
    OpenAPI_ip_address_1_free(emergency_info_1->pgw_ip_address);
    ogs_free(emergency_info_1->smf_instance_id);
    ogs_free(emergency_info_1);
}

cJSON *OpenAPI_emergency_info_1_convertToJSON(OpenAPI_emergency_info_1_t *emergency_info_1)
{
    cJSON *item = NULL;

    if (emergency_info_1 == NULL) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed [EmergencyInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (emergency_info_1->pgw_fqdn) {
    if (cJSON_AddStringToObject(item, "pgwFqdn", emergency_info_1->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    if (emergency_info_1->pgw_ip_address) {
    cJSON *pgw_ip_address_local_JSON = OpenAPI_ip_address_1_convertToJSON(emergency_info_1->pgw_ip_address);
    if (pgw_ip_address_local_JSON == NULL) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed [pgw_ip_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pgwIpAddress", pgw_ip_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed [pgw_ip_address]");
        goto end;
    }
    }

    if (emergency_info_1->smf_instance_id) {
    if (cJSON_AddStringToObject(item, "smfInstanceId", emergency_info_1->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed [smf_instance_id]");
        goto end;
    }
    }

    if (emergency_info_1->is_epdg_ind) {
    if (cJSON_AddBoolToObject(item, "epdgInd", emergency_info_1->epdg_ind) == NULL) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed [epdg_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_emergency_info_1_t *OpenAPI_emergency_info_1_parseFromJSON(cJSON *emergency_info_1JSON)
{
    OpenAPI_emergency_info_1_t *emergency_info_1_local_var = NULL;
    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(emergency_info_1JSON, "pgwFqdn");

    if (pgw_fqdn) {
    if (!cJSON_IsString(pgw_fqdn)) {
        ogs_error("OpenAPI_emergency_info_1_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

    cJSON *pgw_ip_address = cJSON_GetObjectItemCaseSensitive(emergency_info_1JSON, "pgwIpAddress");

    OpenAPI_ip_address_1_t *pgw_ip_address_local_nonprim = NULL;
    if (pgw_ip_address) {
    pgw_ip_address_local_nonprim = OpenAPI_ip_address_1_parseFromJSON(pgw_ip_address);
    }

    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(emergency_info_1JSON, "smfInstanceId");

    if (smf_instance_id) {
    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_emergency_info_1_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }
    }

    cJSON *epdg_ind = cJSON_GetObjectItemCaseSensitive(emergency_info_1JSON, "epdgInd");

    if (epdg_ind) {
    if (!cJSON_IsBool(epdg_ind)) {
        ogs_error("OpenAPI_emergency_info_1_parseFromJSON() failed [epdg_ind]");
        goto end;
    }
    }

    emergency_info_1_local_var = OpenAPI_emergency_info_1_create (
        pgw_fqdn ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        pgw_ip_address ? pgw_ip_address_local_nonprim : NULL,
        smf_instance_id ? ogs_strdup(smf_instance_id->valuestring) : NULL,
        epdg_ind ? true : false,
        epdg_ind ? epdg_ind->valueint : 0
    );

    return emergency_info_1_local_var;
end:
    return NULL;
}

OpenAPI_emergency_info_1_t *OpenAPI_emergency_info_1_copy(OpenAPI_emergency_info_1_t *dst, OpenAPI_emergency_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_emergency_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_emergency_info_1_convertToJSON() failed");
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

    OpenAPI_emergency_info_1_free(dst);
    dst = OpenAPI_emergency_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

