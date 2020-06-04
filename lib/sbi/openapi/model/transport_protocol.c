
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transport_protocol.h"

OpenAPI_transport_protocol_t *OpenAPI_transport_protocol_create(
    )
{
    OpenAPI_transport_protocol_t *transport_protocol_local_var = OpenAPI_malloc(sizeof(OpenAPI_transport_protocol_t));
    if (!transport_protocol_local_var) {
        return NULL;
    }

    return transport_protocol_local_var;
}

void OpenAPI_transport_protocol_free(OpenAPI_transport_protocol_t *transport_protocol)
{
    if (NULL == transport_protocol) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(transport_protocol);
}

cJSON *OpenAPI_transport_protocol_convertToJSON(OpenAPI_transport_protocol_t *transport_protocol)
{
    cJSON *item = NULL;

    if (transport_protocol == NULL) {
        ogs_error("OpenAPI_transport_protocol_convertToJSON() failed [TransportProtocol]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_transport_protocol_t *OpenAPI_transport_protocol_parseFromJSON(cJSON *transport_protocolJSON)
{
    OpenAPI_transport_protocol_t *transport_protocol_local_var = NULL;
    transport_protocol_local_var = OpenAPI_transport_protocol_create (
        );

    return transport_protocol_local_var;
end:
    return NULL;
}

