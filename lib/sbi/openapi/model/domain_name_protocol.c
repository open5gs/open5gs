
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "domain_name_protocol.h"

OpenAPI_domain_name_protocol_t *OpenAPI_domain_name_protocol_create(
    )
{
    OpenAPI_domain_name_protocol_t *domain_name_protocol_local_var = OpenAPI_malloc(sizeof(OpenAPI_domain_name_protocol_t));
    if (!domain_name_protocol_local_var) {
        return NULL;
    }

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

