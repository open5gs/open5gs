
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arp_1.h"

OpenAPI_arp_1_t *OpenAPI_arp_1_create(
    int priority_level,
    OpenAPI_preemption_capability_e preempt_cap,
    OpenAPI_preemption_vulnerability_e preempt_vuln
)
{
    OpenAPI_arp_1_t *arp_1_local_var = ogs_malloc(sizeof(OpenAPI_arp_1_t));
    ogs_assert(arp_1_local_var);

    arp_1_local_var->priority_level = priority_level;
    arp_1_local_var->preempt_cap = preempt_cap;
    arp_1_local_var->preempt_vuln = preempt_vuln;

    return arp_1_local_var;
}

void OpenAPI_arp_1_free(OpenAPI_arp_1_t *arp_1)
{
    if (NULL == arp_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(arp_1);
}

cJSON *OpenAPI_arp_1_convertToJSON(OpenAPI_arp_1_t *arp_1)
{
    cJSON *item = NULL;

    if (arp_1 == NULL) {
        ogs_error("OpenAPI_arp_1_convertToJSON() failed [Arp_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "priorityLevel", arp_1->priority_level) == NULL) {
        ogs_error("OpenAPI_arp_1_convertToJSON() failed [priority_level]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "preemptCap", OpenAPI_preemption_capability_ToString(arp_1->preempt_cap)) == NULL) {
        ogs_error("OpenAPI_arp_1_convertToJSON() failed [preempt_cap]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "preemptVuln", OpenAPI_preemption_vulnerability_ToString(arp_1->preempt_vuln)) == NULL) {
        ogs_error("OpenAPI_arp_1_convertToJSON() failed [preempt_vuln]");
        goto end;
    }

end:
    return item;
}

OpenAPI_arp_1_t *OpenAPI_arp_1_parseFromJSON(cJSON *arp_1JSON)
{
    OpenAPI_arp_1_t *arp_1_local_var = NULL;
    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(arp_1JSON, "priorityLevel");
    if (!priority_level) {
        ogs_error("OpenAPI_arp_1_parseFromJSON() failed [priority_level]");
        goto end;
    }

    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_arp_1_parseFromJSON() failed [priority_level]");
        goto end;
    }

    cJSON *preempt_cap = cJSON_GetObjectItemCaseSensitive(arp_1JSON, "preemptCap");
    if (!preempt_cap) {
        ogs_error("OpenAPI_arp_1_parseFromJSON() failed [preempt_cap]");
        goto end;
    }

    OpenAPI_preemption_capability_e preempt_capVariable;
    if (!cJSON_IsString(preempt_cap)) {
        ogs_error("OpenAPI_arp_1_parseFromJSON() failed [preempt_cap]");
        goto end;
    }
    preempt_capVariable = OpenAPI_preemption_capability_FromString(preempt_cap->valuestring);

    cJSON *preempt_vuln = cJSON_GetObjectItemCaseSensitive(arp_1JSON, "preemptVuln");
    if (!preempt_vuln) {
        ogs_error("OpenAPI_arp_1_parseFromJSON() failed [preempt_vuln]");
        goto end;
    }

    OpenAPI_preemption_vulnerability_e preempt_vulnVariable;
    if (!cJSON_IsString(preempt_vuln)) {
        ogs_error("OpenAPI_arp_1_parseFromJSON() failed [preempt_vuln]");
        goto end;
    }
    preempt_vulnVariable = OpenAPI_preemption_vulnerability_FromString(preempt_vuln->valuestring);

    arp_1_local_var = OpenAPI_arp_1_create (
        
        priority_level->valuedouble,
        preempt_capVariable,
        preempt_vulnVariable
    );

    return arp_1_local_var;
end:
    return NULL;
}

OpenAPI_arp_1_t *OpenAPI_arp_1_copy(OpenAPI_arp_1_t *dst, OpenAPI_arp_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_arp_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_arp_1_convertToJSON() failed");
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

    OpenAPI_arp_1_free(dst);
    dst = OpenAPI_arp_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

