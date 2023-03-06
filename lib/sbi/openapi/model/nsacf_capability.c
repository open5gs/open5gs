
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsacf_capability.h"

OpenAPI_nsacf_capability_t *OpenAPI_nsacf_capability_create(
    bool is_support_ue_sac,
    int support_ue_sac,
    bool is_support_pdu_sac,
    int support_pdu_sac
)
{
    OpenAPI_nsacf_capability_t *nsacf_capability_local_var = ogs_malloc(sizeof(OpenAPI_nsacf_capability_t));
    ogs_assert(nsacf_capability_local_var);

    nsacf_capability_local_var->is_support_ue_sac = is_support_ue_sac;
    nsacf_capability_local_var->support_ue_sac = support_ue_sac;
    nsacf_capability_local_var->is_support_pdu_sac = is_support_pdu_sac;
    nsacf_capability_local_var->support_pdu_sac = support_pdu_sac;

    return nsacf_capability_local_var;
}

void OpenAPI_nsacf_capability_free(OpenAPI_nsacf_capability_t *nsacf_capability)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsacf_capability) {
        return;
    }
    ogs_free(nsacf_capability);
}

cJSON *OpenAPI_nsacf_capability_convertToJSON(OpenAPI_nsacf_capability_t *nsacf_capability)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsacf_capability == NULL) {
        ogs_error("OpenAPI_nsacf_capability_convertToJSON() failed [NsacfCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nsacf_capability->is_support_ue_sac) {
    if (cJSON_AddBoolToObject(item, "supportUeSAC", nsacf_capability->support_ue_sac) == NULL) {
        ogs_error("OpenAPI_nsacf_capability_convertToJSON() failed [support_ue_sac]");
        goto end;
    }
    }

    if (nsacf_capability->is_support_pdu_sac) {
    if (cJSON_AddBoolToObject(item, "supportPduSAC", nsacf_capability->support_pdu_sac) == NULL) {
        ogs_error("OpenAPI_nsacf_capability_convertToJSON() failed [support_pdu_sac]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nsacf_capability_t *OpenAPI_nsacf_capability_parseFromJSON(cJSON *nsacf_capabilityJSON)
{
    OpenAPI_nsacf_capability_t *nsacf_capability_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *support_ue_sac = NULL;
    cJSON *support_pdu_sac = NULL;
    support_ue_sac = cJSON_GetObjectItemCaseSensitive(nsacf_capabilityJSON, "supportUeSAC");
    if (support_ue_sac) {
    if (!cJSON_IsBool(support_ue_sac)) {
        ogs_error("OpenAPI_nsacf_capability_parseFromJSON() failed [support_ue_sac]");
        goto end;
    }
    }

    support_pdu_sac = cJSON_GetObjectItemCaseSensitive(nsacf_capabilityJSON, "supportPduSAC");
    if (support_pdu_sac) {
    if (!cJSON_IsBool(support_pdu_sac)) {
        ogs_error("OpenAPI_nsacf_capability_parseFromJSON() failed [support_pdu_sac]");
        goto end;
    }
    }

    nsacf_capability_local_var = OpenAPI_nsacf_capability_create (
        support_ue_sac ? true : false,
        support_ue_sac ? support_ue_sac->valueint : 0,
        support_pdu_sac ? true : false,
        support_pdu_sac ? support_pdu_sac->valueint : 0
    );

    return nsacf_capability_local_var;
end:
    return NULL;
}

OpenAPI_nsacf_capability_t *OpenAPI_nsacf_capability_copy(OpenAPI_nsacf_capability_t *dst, OpenAPI_nsacf_capability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsacf_capability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsacf_capability_convertToJSON() failed");
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

    OpenAPI_nsacf_capability_free(dst);
    dst = OpenAPI_nsacf_capability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

