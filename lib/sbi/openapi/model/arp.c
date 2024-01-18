
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arp.h"

OpenAPI_arp_t *OpenAPI_arp_create(
    bool is_priority_level_null,
    int priority_level,
    OpenAPI_preemption_capability_e preempt_cap,
    OpenAPI_preemption_vulnerability_e preempt_vuln
)
{
    OpenAPI_arp_t *arp_local_var = ogs_malloc(sizeof(OpenAPI_arp_t));
    ogs_assert(arp_local_var);

    arp_local_var->is_priority_level_null = is_priority_level_null;
    arp_local_var->priority_level = priority_level;
    arp_local_var->preempt_cap = preempt_cap;
    arp_local_var->preempt_vuln = preempt_vuln;

    return arp_local_var;
}

void OpenAPI_arp_free(OpenAPI_arp_t *arp)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == arp) {
        return;
    }
    ogs_free(arp);
}

cJSON *OpenAPI_arp_convertToJSON(OpenAPI_arp_t *arp)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (arp == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [Arp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "priorityLevel", arp->priority_level) == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [priority_level]");
        goto end;
    }

    if (arp->preempt_cap == OpenAPI_preemption_capability_NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_cap]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "preemptCap", OpenAPI_preemption_capability_ToString(arp->preempt_cap)) == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_cap]");
        goto end;
    }

    if (arp->preempt_vuln == OpenAPI_preemption_vulnerability_NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_vuln]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "preemptVuln", OpenAPI_preemption_vulnerability_ToString(arp->preempt_vuln)) == NULL) {
        ogs_error("OpenAPI_arp_convertToJSON() failed [preempt_vuln]");
        goto end;
    }

end:
    return item;
}

OpenAPI_arp_t *OpenAPI_arp_parseFromJSON(cJSON *arpJSON)
{
    OpenAPI_arp_t *arp_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *priority_level = NULL;
    cJSON *preempt_cap = NULL;
    OpenAPI_preemption_capability_e preempt_capVariable = 0;
    cJSON *preempt_vuln = NULL;
    OpenAPI_preemption_vulnerability_e preempt_vulnVariable = 0;
    priority_level = cJSON_GetObjectItemCaseSensitive(arpJSON, "priorityLevel");
    if (!priority_level) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [priority_level]");
        goto end;
    }
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [priority_level]");
        goto end;
    }

    preempt_cap = cJSON_GetObjectItemCaseSensitive(arpJSON, "preemptCap");
    if (!preempt_cap) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [preempt_cap]");
        goto end;
    }
    if (!cJSON_IsString(preempt_cap)) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [preempt_cap]");
        goto end;
    }
    preempt_capVariable = OpenAPI_preemption_capability_FromString(preempt_cap->valuestring);

    preempt_vuln = cJSON_GetObjectItemCaseSensitive(arpJSON, "preemptVuln");
    if (!preempt_vuln) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [preempt_vuln]");
        goto end;
    }
    if (!cJSON_IsString(preempt_vuln)) {
        ogs_error("OpenAPI_arp_parseFromJSON() failed [preempt_vuln]");
        goto end;
    }
    preempt_vulnVariable = OpenAPI_preemption_vulnerability_FromString(preempt_vuln->valuestring);

    arp_local_var = OpenAPI_arp_create (
        priority_level && cJSON_IsNull(priority_level) ? true : false,
        
        priority_level->valuedouble,
        preempt_capVariable,
        preempt_vulnVariable
    );

    return arp_local_var;
end:
    return NULL;
}

OpenAPI_arp_t *OpenAPI_arp_copy(OpenAPI_arp_t *dst, OpenAPI_arp_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_arp_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_arp_convertToJSON() failed");
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

    OpenAPI_arp_free(dst);
    dst = OpenAPI_arp_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

