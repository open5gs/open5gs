
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "domain_name_protocol.h"

OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_create(
)
{
    OpenAPI_domain_name_protocol_t *domain_name_protocol_local_var = ogs_malloc(sizeof(OpenAPI_domain_name_protocol_t));
    ogs_assert(domain_name_protocol_local_var);


    return domain_name_protocol_local_var;
}

void OpenAPI_domain_name_protocol_free(OpenAPI_domain_name_protocol_t *domain_name_protocol)
{
    if (NULL == domain_name_protocol) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(domain_name_protocol);
}

cJSON *OpenAPI_domain_name_protocol_convertToJSON(OpenAPI_domain_name_protocol_t *domain_name_protocol)
{
    cJSON *item = NULL;

    if (domain_name_protocol == NULL) {
        ogs_error("OpenAPI_domain_name_protocol_convertToJSON() failed [DomainNameProtocol]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_parseFromJSON(cJSON *domain_name_protocolJSON)
{
    OpenAPI_domain_name_protocol_t *domain_name_protocol_local_var = NULL;
    domain_name_protocol_local_var = OpenAPI_domain_name_protocol_create (
    );

    return domain_name_protocol_local_var;
end:
    return NULL;
}

OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_copy(OpenAPI_domain_name_protocol_t *dst, OpenAPI_domain_name_protocol_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_domain_name_protocol_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_domain_name_protocol_convertToJSON() failed");
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

    OpenAPI_domain_name_protocol_free(dst);
    dst = OpenAPI_domain_name_protocol_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

