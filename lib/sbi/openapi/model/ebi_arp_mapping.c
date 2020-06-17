
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ebi_arp_mapping.h"

OpenAPI_ebi_arp_mapping_t *OpenAPI_ebi_arp_mapping_create(
    int eps_bearer_id,
    OpenAPI_arp_t *arp
    )
{
    OpenAPI_ebi_arp_mapping_t *ebi_arp_mapping_local_var = OpenAPI_malloc(sizeof(OpenAPI_ebi_arp_mapping_t));
    if (!ebi_arp_mapping_local_var) {
        return NULL;
    }
    ebi_arp_mapping_local_var->eps_bearer_id = eps_bearer_id;
    ebi_arp_mapping_local_var->arp = arp;

    return ebi_arp_mapping_local_var;
}

void OpenAPI_ebi_arp_mapping_free(OpenAPI_ebi_arp_mapping_t *ebi_arp_mapping)
{
    if (NULL == ebi_arp_mapping) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_arp_free(ebi_arp_mapping->arp);
    ogs_free(ebi_arp_mapping);
}

cJSON *OpenAPI_ebi_arp_mapping_convertToJSON(OpenAPI_ebi_arp_mapping_t *ebi_arp_mapping)
{
    cJSON *item = NULL;

    if (ebi_arp_mapping == NULL) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed [EbiArpMapping]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ebi_arp_mapping->eps_bearer_id) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed [eps_bearer_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "epsBearerId", ebi_arp_mapping->eps_bearer_id) == NULL) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed [eps_bearer_id]");
        goto end;
    }

    if (!ebi_arp_mapping->arp) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(ebi_arp_mapping->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed [arp]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ebi_arp_mapping_t *OpenAPI_ebi_arp_mapping_parseFromJSON(cJSON *ebi_arp_mappingJSON)
{
    OpenAPI_ebi_arp_mapping_t *ebi_arp_mapping_local_var = NULL;
    cJSON *eps_bearer_id = cJSON_GetObjectItemCaseSensitive(ebi_arp_mappingJSON, "epsBearerId");
    if (!eps_bearer_id) {
        ogs_error("OpenAPI_ebi_arp_mapping_parseFromJSON() failed [eps_bearer_id]");
        goto end;
    }


    if (!cJSON_IsNumber(eps_bearer_id)) {
        ogs_error("OpenAPI_ebi_arp_mapping_parseFromJSON() failed [eps_bearer_id]");
        goto end;
    }

    cJSON *arp = cJSON_GetObjectItemCaseSensitive(ebi_arp_mappingJSON, "arp");
    if (!arp) {
        ogs_error("OpenAPI_ebi_arp_mapping_parseFromJSON() failed [arp]");
        goto end;
    }

    OpenAPI_arp_t *arp_local_nonprim = NULL;

    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);

    ebi_arp_mapping_local_var = OpenAPI_ebi_arp_mapping_create (
        eps_bearer_id->valuedouble,
        arp_local_nonprim
        );

    return ebi_arp_mapping_local_var;
end:
    return NULL;
}

OpenAPI_ebi_arp_mapping_t *OpenAPI_ebi_arp_mapping_copy(OpenAPI_ebi_arp_mapping_t *dst, OpenAPI_ebi_arp_mapping_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ebi_arp_mapping_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ebi_arp_mapping_convertToJSON() failed");
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

    OpenAPI_ebi_arp_mapping_free(dst);
    dst = OpenAPI_ebi_arp_mapping_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

