
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "emergency_info.h"

OpenAPI_emergency_info_t *OpenAPI_emergency_info_create(
    char *pgw_fqdn,
    OpenAPI_ip_address_t *pgw_ip_address,
    char *smf_instance_id
    )
{
    OpenAPI_emergency_info_t *emergency_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_emergency_info_t));
    if (!emergency_info_local_var) {
        return NULL;
    }
    emergency_info_local_var->pgw_fqdn = pgw_fqdn;
    emergency_info_local_var->pgw_ip_address = pgw_ip_address;
    emergency_info_local_var->smf_instance_id = smf_instance_id;

    return emergency_info_local_var;
}

void OpenAPI_emergency_info_free(OpenAPI_emergency_info_t *emergency_info)
{
    if (NULL == emergency_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(emergency_info->pgw_fqdn);
    OpenAPI_ip_address_free(emergency_info->pgw_ip_address);
    ogs_free(emergency_info->smf_instance_id);
    ogs_free(emergency_info);
}

cJSON *OpenAPI_emergency_info_convertToJSON(OpenAPI_emergency_info_t *emergency_info)
{
    cJSON *item = NULL;

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

end:
    return item;
}

OpenAPI_emergency_info_t *OpenAPI_emergency_info_parseFromJSON(cJSON *emergency_infoJSON)
{
    OpenAPI_emergency_info_t *emergency_info_local_var = NULL;
    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "pgwFqdn");

    if (pgw_fqdn) {
        if (!cJSON_IsString(pgw_fqdn)) {
            ogs_error("OpenAPI_emergency_info_parseFromJSON() failed [pgw_fqdn]");
            goto end;
        }
    }

    cJSON *pgw_ip_address = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "pgwIpAddress");

    OpenAPI_ip_address_t *pgw_ip_address_local_nonprim = NULL;
    if (pgw_ip_address) {
        pgw_ip_address_local_nonprim = OpenAPI_ip_address_parseFromJSON(pgw_ip_address);
    }

    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(emergency_infoJSON, "smfInstanceId");

    if (smf_instance_id) {
        if (!cJSON_IsString(smf_instance_id)) {
            ogs_error("OpenAPI_emergency_info_parseFromJSON() failed [smf_instance_id]");
            goto end;
        }
    }

    emergency_info_local_var = OpenAPI_emergency_info_create (
        pgw_fqdn ? ogs_strdup(pgw_fqdn->valuestring) : NULL,
        pgw_ip_address ? pgw_ip_address_local_nonprim : NULL,
        smf_instance_id ? ogs_strdup(smf_instance_id->valuestring) : NULL
        );

    return emergency_info_local_var;
end:
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

