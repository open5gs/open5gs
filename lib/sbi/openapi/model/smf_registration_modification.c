
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_registration_modification.h"

OpenAPI_smf_registration_modification_t *OpenAPI_smf_registration_modification_create(
    char *smf_instance_id,
    char *smf_set_id,
    OpenAPI_fqdn_rm_t *pgw_fqdn
)
{
    OpenAPI_smf_registration_modification_t *smf_registration_modification_local_var = ogs_malloc(sizeof(OpenAPI_smf_registration_modification_t));
    ogs_assert(smf_registration_modification_local_var);

    smf_registration_modification_local_var->smf_instance_id = smf_instance_id;
    smf_registration_modification_local_var->smf_set_id = smf_set_id;
    smf_registration_modification_local_var->pgw_fqdn = pgw_fqdn;

    return smf_registration_modification_local_var;
}

void OpenAPI_smf_registration_modification_free(OpenAPI_smf_registration_modification_t *smf_registration_modification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_registration_modification) {
        return;
    }
    if (smf_registration_modification->smf_instance_id) {
        ogs_free(smf_registration_modification->smf_instance_id);
        smf_registration_modification->smf_instance_id = NULL;
    }
    if (smf_registration_modification->smf_set_id) {
        ogs_free(smf_registration_modification->smf_set_id);
        smf_registration_modification->smf_set_id = NULL;
    }
    if (smf_registration_modification->pgw_fqdn) {
        OpenAPI_fqdn_rm_free(smf_registration_modification->pgw_fqdn);
        smf_registration_modification->pgw_fqdn = NULL;
    }
    ogs_free(smf_registration_modification);
}

cJSON *OpenAPI_smf_registration_modification_convertToJSON(OpenAPI_smf_registration_modification_t *smf_registration_modification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_registration_modification == NULL) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed [SmfRegistrationModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_registration_modification->smf_instance_id) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed [smf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smfInstanceId", smf_registration_modification->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

    if (smf_registration_modification->smf_set_id) {
    if (cJSON_AddStringToObject(item, "smfSetId", smf_registration_modification->smf_set_id) == NULL) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed [smf_set_id]");
        goto end;
    }
    }

    if (smf_registration_modification->pgw_fqdn) {
    cJSON *pgw_fqdn_local_JSON = OpenAPI_fqdn_rm_convertToJSON(smf_registration_modification->pgw_fqdn);
    if (pgw_fqdn_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pgwFqdn", pgw_fqdn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed [pgw_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_registration_modification_t *OpenAPI_smf_registration_modification_parseFromJSON(cJSON *smf_registration_modificationJSON)
{
    OpenAPI_smf_registration_modification_t *smf_registration_modification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smf_instance_id = NULL;
    cJSON *smf_set_id = NULL;
    cJSON *pgw_fqdn = NULL;
    OpenAPI_fqdn_rm_t *pgw_fqdn_local_nonprim = NULL;
    smf_instance_id = cJSON_GetObjectItemCaseSensitive(smf_registration_modificationJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_smf_registration_modification_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_smf_registration_modification_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    smf_set_id = cJSON_GetObjectItemCaseSensitive(smf_registration_modificationJSON, "smfSetId");
    if (smf_set_id) {
    if (!cJSON_IsString(smf_set_id) && !cJSON_IsNull(smf_set_id)) {
        ogs_error("OpenAPI_smf_registration_modification_parseFromJSON() failed [smf_set_id]");
        goto end;
    }
    }

    pgw_fqdn = cJSON_GetObjectItemCaseSensitive(smf_registration_modificationJSON, "pgwFqdn");
    if (pgw_fqdn) {
    pgw_fqdn_local_nonprim = OpenAPI_fqdn_rm_parseFromJSON(pgw_fqdn);
    if (!pgw_fqdn_local_nonprim) {
        ogs_error("OpenAPI_fqdn_rm_parseFromJSON failed [pgw_fqdn]");
        goto end;
    }
    }

    smf_registration_modification_local_var = OpenAPI_smf_registration_modification_create (
        ogs_strdup(smf_instance_id->valuestring),
        smf_set_id && !cJSON_IsNull(smf_set_id) ? ogs_strdup(smf_set_id->valuestring) : NULL,
        pgw_fqdn ? pgw_fqdn_local_nonprim : NULL
    );

    return smf_registration_modification_local_var;
end:
    if (pgw_fqdn_local_nonprim) {
        OpenAPI_fqdn_rm_free(pgw_fqdn_local_nonprim);
        pgw_fqdn_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_smf_registration_modification_t *OpenAPI_smf_registration_modification_copy(OpenAPI_smf_registration_modification_t *dst, OpenAPI_smf_registration_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_registration_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_registration_modification_convertToJSON() failed");
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

    OpenAPI_smf_registration_modification_free(dst);
    dst = OpenAPI_smf_registration_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

