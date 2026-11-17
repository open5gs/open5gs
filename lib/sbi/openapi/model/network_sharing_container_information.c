
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_sharing_container_information.h"

OpenAPI_network_sharing_container_information_t *OpenAPI_network_sharing_container_information_create(
    bool is_uplink_volume,
    int uplink_volume,
    bool is_downlink_volume,
    int downlink_volume,
    bool is_number_of_pdu_sessions_req,
    int number_of_pdu_sessions_req,
    bool is_number_of_pdu_sessions_success,
    int number_of_pdu_sessions_success
)
{
    OpenAPI_network_sharing_container_information_t *network_sharing_container_information_local_var = ogs_malloc(sizeof(OpenAPI_network_sharing_container_information_t));
    ogs_assert(network_sharing_container_information_local_var);

    network_sharing_container_information_local_var->is_uplink_volume = is_uplink_volume;
    network_sharing_container_information_local_var->uplink_volume = uplink_volume;
    network_sharing_container_information_local_var->is_downlink_volume = is_downlink_volume;
    network_sharing_container_information_local_var->downlink_volume = downlink_volume;
    network_sharing_container_information_local_var->is_number_of_pdu_sessions_req = is_number_of_pdu_sessions_req;
    network_sharing_container_information_local_var->number_of_pdu_sessions_req = number_of_pdu_sessions_req;
    network_sharing_container_information_local_var->is_number_of_pdu_sessions_success = is_number_of_pdu_sessions_success;
    network_sharing_container_information_local_var->number_of_pdu_sessions_success = number_of_pdu_sessions_success;

    return network_sharing_container_information_local_var;
}

void OpenAPI_network_sharing_container_information_free(OpenAPI_network_sharing_container_information_t *network_sharing_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_sharing_container_information) {
        return;
    }
    ogs_free(network_sharing_container_information);
}

cJSON *OpenAPI_network_sharing_container_information_convertToJSON(OpenAPI_network_sharing_container_information_t *network_sharing_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_sharing_container_information == NULL) {
        ogs_error("OpenAPI_network_sharing_container_information_convertToJSON() failed [NetworkSharingContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (network_sharing_container_information->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", network_sharing_container_information->uplink_volume) == NULL) {
        ogs_error("OpenAPI_network_sharing_container_information_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (network_sharing_container_information->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", network_sharing_container_information->downlink_volume) == NULL) {
        ogs_error("OpenAPI_network_sharing_container_information_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (network_sharing_container_information->is_number_of_pdu_sessions_req) {
    if (cJSON_AddNumberToObject(item, "numberOfPDUSessionsReq", network_sharing_container_information->number_of_pdu_sessions_req) == NULL) {
        ogs_error("OpenAPI_network_sharing_container_information_convertToJSON() failed [number_of_pdu_sessions_req]");
        goto end;
    }
    }

    if (network_sharing_container_information->is_number_of_pdu_sessions_success) {
    if (cJSON_AddNumberToObject(item, "numberOfPDUSessionsSuccess", network_sharing_container_information->number_of_pdu_sessions_success) == NULL) {
        ogs_error("OpenAPI_network_sharing_container_information_convertToJSON() failed [number_of_pdu_sessions_success]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_network_sharing_container_information_t *OpenAPI_network_sharing_container_information_parseFromJSON(cJSON *network_sharing_container_informationJSON)
{
    OpenAPI_network_sharing_container_information_t *network_sharing_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *number_of_pdu_sessions_req = NULL;
    cJSON *number_of_pdu_sessions_success = NULL;
    uplink_volume = cJSON_GetObjectItemCaseSensitive(network_sharing_container_informationJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_network_sharing_container_information_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(network_sharing_container_informationJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_network_sharing_container_information_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    number_of_pdu_sessions_req = cJSON_GetObjectItemCaseSensitive(network_sharing_container_informationJSON, "numberOfPDUSessionsReq");
    if (number_of_pdu_sessions_req) {
    if (!cJSON_IsNumber(number_of_pdu_sessions_req)) {
        ogs_error("OpenAPI_network_sharing_container_information_parseFromJSON() failed [number_of_pdu_sessions_req]");
        goto end;
    }
    }

    number_of_pdu_sessions_success = cJSON_GetObjectItemCaseSensitive(network_sharing_container_informationJSON, "numberOfPDUSessionsSuccess");
    if (number_of_pdu_sessions_success) {
    if (!cJSON_IsNumber(number_of_pdu_sessions_success)) {
        ogs_error("OpenAPI_network_sharing_container_information_parseFromJSON() failed [number_of_pdu_sessions_success]");
        goto end;
    }
    }

    network_sharing_container_information_local_var = OpenAPI_network_sharing_container_information_create (
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        number_of_pdu_sessions_req ? true : false,
        number_of_pdu_sessions_req ? number_of_pdu_sessions_req->valuedouble : 0,
        number_of_pdu_sessions_success ? true : false,
        number_of_pdu_sessions_success ? number_of_pdu_sessions_success->valuedouble : 0
    );

    return network_sharing_container_information_local_var;
end:
    return NULL;
}

OpenAPI_network_sharing_container_information_t *OpenAPI_network_sharing_container_information_copy(OpenAPI_network_sharing_container_information_t *dst, OpenAPI_network_sharing_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_sharing_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_sharing_container_information_convertToJSON() failed");
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

    OpenAPI_network_sharing_container_information_free(dst);
    dst = OpenAPI_network_sharing_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

