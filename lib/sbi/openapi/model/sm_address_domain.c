
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_address_domain.h"

OpenAPI_sm_address_domain_t *OpenAPI_sm_address_domain_create(
    char *domain_name,
    char *_3_gppimsimccmnc
)
{
    OpenAPI_sm_address_domain_t *sm_address_domain_local_var = ogs_malloc(sizeof(OpenAPI_sm_address_domain_t));
    ogs_assert(sm_address_domain_local_var);

    sm_address_domain_local_var->domain_name = domain_name;
    sm_address_domain_local_var->_3_gppimsimccmnc = _3_gppimsimccmnc;

    return sm_address_domain_local_var;
}

void OpenAPI_sm_address_domain_free(OpenAPI_sm_address_domain_t *sm_address_domain)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_address_domain) {
        return;
    }
    if (sm_address_domain->domain_name) {
        ogs_free(sm_address_domain->domain_name);
        sm_address_domain->domain_name = NULL;
    }
    if (sm_address_domain->_3_gppimsimccmnc) {
        ogs_free(sm_address_domain->_3_gppimsimccmnc);
        sm_address_domain->_3_gppimsimccmnc = NULL;
    }
    ogs_free(sm_address_domain);
}

cJSON *OpenAPI_sm_address_domain_convertToJSON(OpenAPI_sm_address_domain_t *sm_address_domain)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_address_domain == NULL) {
        ogs_error("OpenAPI_sm_address_domain_convertToJSON() failed [SMAddressDomain]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_address_domain->domain_name) {
    if (cJSON_AddStringToObject(item, "domainName", sm_address_domain->domain_name) == NULL) {
        ogs_error("OpenAPI_sm_address_domain_convertToJSON() failed [domain_name]");
        goto end;
    }
    }

    if (sm_address_domain->_3_gppimsimccmnc) {
    if (cJSON_AddStringToObject(item, "3GPPIMSIMCCMNC", sm_address_domain->_3_gppimsimccmnc) == NULL) {
        ogs_error("OpenAPI_sm_address_domain_convertToJSON() failed [_3_gppimsimccmnc]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_address_domain_t *OpenAPI_sm_address_domain_parseFromJSON(cJSON *sm_address_domainJSON)
{
    OpenAPI_sm_address_domain_t *sm_address_domain_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *domain_name = NULL;
    cJSON *_3_gppimsimccmnc = NULL;
    domain_name = cJSON_GetObjectItemCaseSensitive(sm_address_domainJSON, "domainName");
    if (domain_name) {
    if (!cJSON_IsString(domain_name) && !cJSON_IsNull(domain_name)) {
        ogs_error("OpenAPI_sm_address_domain_parseFromJSON() failed [domain_name]");
        goto end;
    }
    }

    _3_gppimsimccmnc = cJSON_GetObjectItemCaseSensitive(sm_address_domainJSON, "3GPPIMSIMCCMNC");
    if (_3_gppimsimccmnc) {
    if (!cJSON_IsString(_3_gppimsimccmnc) && !cJSON_IsNull(_3_gppimsimccmnc)) {
        ogs_error("OpenAPI_sm_address_domain_parseFromJSON() failed [_3_gppimsimccmnc]");
        goto end;
    }
    }

    sm_address_domain_local_var = OpenAPI_sm_address_domain_create (
        domain_name && !cJSON_IsNull(domain_name) ? ogs_strdup(domain_name->valuestring) : NULL,
        _3_gppimsimccmnc && !cJSON_IsNull(_3_gppimsimccmnc) ? ogs_strdup(_3_gppimsimccmnc->valuestring) : NULL
    );

    return sm_address_domain_local_var;
end:
    return NULL;
}

OpenAPI_sm_address_domain_t *OpenAPI_sm_address_domain_copy(OpenAPI_sm_address_domain_t *dst, OpenAPI_sm_address_domain_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_address_domain_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_address_domain_convertToJSON() failed");
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

    OpenAPI_sm_address_domain_free(dst);
    dst = OpenAPI_sm_address_domain_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

