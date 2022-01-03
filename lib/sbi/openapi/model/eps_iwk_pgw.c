
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_iwk_pgw.h"

OpenAPI_eps_iwk_pgw_t *OpenAPI_eps_iwk_pgw_create(
    char *pgw_fqdn,
    char *smf_instance_id
)
{
    OpenAPI_eps_iwk_pgw_t *eps_iwk_pgw_local_var = ogs_malloc(sizeof(OpenAPI_eps_iwk_pgw_t));
    ogs_assert(eps_iwk_pgw_local_var);

    eps_iwk_pgw_local_var->pgw_fqdn = pgw_fqdn;
    eps_iwk_pgw_local_var->smf_instance_id = smf_instance_id;

    return eps_iwk_pgw_local_var;
}

void OpenAPI_eps_iwk_pgw_free(OpenAPI_eps_iwk_pgw_t *eps_iwk_pgw)
{
    if (NULL == eps_iwk_pgw) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(eps_iwk_pgw->pgw_fqdn);
    ogs_free(eps_iwk_pgw->smf_instance_id);
    ogs_free(eps_iwk_pgw);
}

cJSON *OpenAPI_eps_iwk_pgw_convertToJSON(OpenAPI_eps_iwk_pgw_t *eps_iwk_pgw)
{
    cJSON *item = NULL;

    if (eps_iwk_pgw == NULL) {
        ogs_error("OpenAPI_eps_iwk_pgw_convertToJSON() failed [EpsIwkPgw]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "pgwFqdn", eps_iwk_pgw->pgw_fqdn) == NULL) {
        ogs_error("OpenAPI_eps_iwk_pgw_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "smfInstanceId", eps_iwk_pgw->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_eps_iwk_pgw_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_eps_iwk_pgw_t *OpenAPI_eps_iwk_pgw_parseFromJSON(cJSON *eps_iwk_pgwJSON)
{
    OpenAPI_eps_iwk_pgw_t *eps_iwk_pgw_local_var = NULL;
    cJSON *pgw_fqdn = cJSON_GetObjectItemCaseSensitive(eps_iwk_pgwJSON, "pgwFqdn");
    if (!pgw_fqdn) {
        ogs_error("OpenAPI_eps_iwk_pgw_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }

    if (!cJSON_IsString(pgw_fqdn)) {
        ogs_error("OpenAPI_eps_iwk_pgw_parseFromJSON() failed [pgw_fqdn]");
        goto end;
    }

    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(eps_iwk_pgwJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_eps_iwk_pgw_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_eps_iwk_pgw_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    eps_iwk_pgw_local_var = OpenAPI_eps_iwk_pgw_create (
        ogs_strdup(pgw_fqdn->valuestring),
        ogs_strdup(smf_instance_id->valuestring)
    );

    return eps_iwk_pgw_local_var;
end:
    return NULL;
}

OpenAPI_eps_iwk_pgw_t *OpenAPI_eps_iwk_pgw_copy(OpenAPI_eps_iwk_pgw_t *dst, OpenAPI_eps_iwk_pgw_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eps_iwk_pgw_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eps_iwk_pgw_convertToJSON() failed");
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

    OpenAPI_eps_iwk_pgw_free(dst);
    dst = OpenAPI_eps_iwk_pgw_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

