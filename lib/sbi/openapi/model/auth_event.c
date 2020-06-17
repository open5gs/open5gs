
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_event.h"

OpenAPI_auth_event_t *OpenAPI_auth_event_create(
    char *nf_instance_id,
    int success,
    char *time_stamp,
    OpenAPI_auth_type_e auth_type,
    char *serving_network_name
    )
{
    OpenAPI_auth_event_t *auth_event_local_var = OpenAPI_malloc(sizeof(OpenAPI_auth_event_t));
    if (!auth_event_local_var) {
        return NULL;
    }
    auth_event_local_var->nf_instance_id = nf_instance_id;
    auth_event_local_var->success = success;
    auth_event_local_var->time_stamp = time_stamp;
    auth_event_local_var->auth_type = auth_type;
    auth_event_local_var->serving_network_name = serving_network_name;

    return auth_event_local_var;
}

void OpenAPI_auth_event_free(OpenAPI_auth_event_t *auth_event)
{
    if (NULL == auth_event) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(auth_event->nf_instance_id);
    ogs_free(auth_event->time_stamp);
    ogs_free(auth_event->serving_network_name);
    ogs_free(auth_event);
}

cJSON *OpenAPI_auth_event_convertToJSON(OpenAPI_auth_event_t *auth_event)
{
    cJSON *item = NULL;

    if (auth_event == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [AuthEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!auth_event->nf_instance_id) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", auth_event->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (cJSON_AddBoolToObject(item, "success", auth_event->success) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [success]");
        goto end;
    }

    if (!auth_event->time_stamp) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [time_stamp]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "timeStamp", auth_event->time_stamp) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [time_stamp]");
        goto end;
    }

    if (!auth_event->auth_type) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [auth_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(auth_event->auth_type)) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (!auth_event->serving_network_name) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [serving_network_name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "servingNetworkName", auth_event->serving_network_name) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [serving_network_name]");
        goto end;
    }

end:
    return item;
}

OpenAPI_auth_event_t *OpenAPI_auth_event_parseFromJSON(cJSON *auth_eventJSON)
{
    OpenAPI_auth_event_t *auth_event_local_var = NULL;
    cJSON *nf_instance_id = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    cJSON *success = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "success");
    if (!success) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [success]");
        goto end;
    }


    if (!cJSON_IsBool(success)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [success]");
        goto end;
    }

    cJSON *time_stamp = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "timeStamp");
    if (!time_stamp) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [time_stamp]");
        goto end;
    }


    if (!cJSON_IsString(time_stamp)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [time_stamp]");
        goto end;
    }

    cJSON *auth_type = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [auth_type]");
        goto end;
    }

    OpenAPI_auth_type_e auth_typeVariable;

    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    cJSON *serving_network_name = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "servingNetworkName");
    if (!serving_network_name) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [serving_network_name]");
        goto end;
    }


    if (!cJSON_IsString(serving_network_name)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [serving_network_name]");
        goto end;
    }

    auth_event_local_var = OpenAPI_auth_event_create (
        ogs_strdup(nf_instance_id->valuestring),
        success->valueint,
        ogs_strdup(time_stamp->valuestring),
        auth_typeVariable,
        ogs_strdup(serving_network_name->valuestring)
        );

    return auth_event_local_var;
end:
    return NULL;
}

OpenAPI_auth_event_t *OpenAPI_auth_event_copy(OpenAPI_auth_event_t *dst, OpenAPI_auth_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_auth_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed");
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

    OpenAPI_auth_event_free(dst);
    dst = OpenAPI_auth_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

