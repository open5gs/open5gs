
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arp.h"

OpenAPI_arp_t *OpenAPI_arp_create(
    int priority_level,
    OpenAPI_preemption_capability_t *preempt_cap,
    OpenAPI_preemption_vulnerability_t *preempt_vuln
    )
{
    OpenAPI_arp_t *arp_local_var = OpenAPI_malloc(sizeof(OpenAPI_arp_t));
    if (!arp_local_var) {
        return NULL;
    }
    arp_local_var->priority_level = priority_level;
    arp_local_var->preempt_cap = preempt_cap;
    arp_local_var->preempt_vuln = preempt_vuln;

    return arp_local_var;
}

void OpenAPI_arp_free(OpenAPI_arp_t *arp)
{
    if (NULL == arp) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_preemption_capability_free(arp->preempt_cap);
    OpenAPI_preemption_vulnerability_free(arp->preempt_vuln);
    ogs_free(arp);
}

cJSON *OpenAPI_arp_convertToJSON(OpenAPI_arp_t *arp)
{
    cJSON *item = NULL;

    if (arp == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [Arp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!arp->priority_level) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [priority_level]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "priorityLevel", arp->priority_level) == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [priority_level]");
        goto end;
    }

    if (!arp->preempt_cap) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_cap]");
        goto end;
    }
    cJSON *preempt_cap_local_JSON = OpenAPI_preemption_capability_convertToJSON(arp->preempt_cap);
    if (preempt_cap_local_JSON == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_cap]");
        goto end;
    }
    cJSON_AddItemToObject(item, "preemptCap", preempt_cap_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_cap]");
        goto end;
    }

    if (!arp->preempt_vuln) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_vuln]");
        goto end;
    }
    cJSON *preempt_vuln_local_JSON = OpenAPI_preemption_vulnerability_convertToJSON(arp->preempt_vuln);
    if (preempt_vuln_local_JSON == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_vuln]");
        goto end;
    }
    cJSON_AddItemToObject(item, "preemptVuln", preempt_vuln_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_vuln]");
        goto end;
    }

end:
    return item;
}

OpenAPI_arp_t *OpenAPI_arp_parseFromJSON(cJSON *arpJSON)
{
    OpenAPI_arp_t *arp_local_var = NULL;
    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(arpJSON, "priorityLevel");
    if (!priority_level) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [priority_level]");
        goto end;
    }


    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [priority_level]");
        goto end;
    }

    cJSON *preempt_cap = cJSON_GetObjectItemCaseSensitive(arpJSON, "preemptCap");
    if (!preempt_cap) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [preempt_cap]");
        goto end;
    }

    OpenAPI_preemption_capability_t *preempt_cap_local_nonprim = NULL;

    preempt_cap_local_nonprim = OpenAPI_preemption_capability_parseFromJSON(preempt_cap);

    cJSON *preempt_vuln = cJSON_GetObjectItemCaseSensitive(arpJSON, "preemptVuln");
    if (!preempt_vuln) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [preempt_vuln]");
        goto end;
    }

    OpenAPI_preemption_vulnerability_t *preempt_vuln_local_nonprim = NULL;

    preempt_vuln_local_nonprim = OpenAPI_preemption_vulnerability_parseFromJSON(preempt_vuln);

    arp_local_var = OpenAPI_arp_create (
        priority_level->valuedouble,
        preempt_cap_local_nonprim,
        preempt_vuln_local_nonprim
        );

    return arp_local_var;
end:
    return NULL;
}

