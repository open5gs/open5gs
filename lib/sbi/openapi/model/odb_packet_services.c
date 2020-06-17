
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "odb_packet_services.h"

OpenAPI_odb_packet_services_t *OpenAPI_odb_packet_services_create(
    )
{
    OpenAPI_odb_packet_services_t *odb_packet_services_local_var = OpenAPI_malloc(sizeof(OpenAPI_odb_packet_services_t));
    if (!odb_packet_services_local_var) {
        return NULL;
    }

    return odb_packet_services_local_var;
}

void OpenAPI_odb_packet_services_free(OpenAPI_odb_packet_services_t *odb_packet_services)
{
    if (NULL == odb_packet_services) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(odb_packet_services);
}

cJSON *OpenAPI_odb_packet_services_convertToJSON(OpenAPI_odb_packet_services_t *odb_packet_services)
{
    cJSON *item = NULL;

    if (odb_packet_services == NULL) {
        ogs_error("OpenAPI_odb_packet_services_convertToJSON() failed [OdbPacketServices]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_odb_packet_services_t *OpenAPI_odb_packet_services_parseFromJSON(cJSON *odb_packet_servicesJSON)
{
    OpenAPI_odb_packet_services_t *odb_packet_services_local_var = NULL;
    odb_packet_services_local_var = OpenAPI_odb_packet_services_create (
        );

    return odb_packet_services_local_var;
end:
    return NULL;
}

OpenAPI_odb_packet_services_t *OpenAPI_odb_packet_services_copy(OpenAPI_odb_packet_services_t *dst, OpenAPI_odb_packet_services_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_odb_packet_services_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_odb_packet_services_convertToJSON() failed");
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

    OpenAPI_odb_packet_services_free(dst);
    dst = OpenAPI_odb_packet_services_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

