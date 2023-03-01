
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_event.h"

OpenAPI_auth_event_t *OpenAPI_auth_event_create(
    char *nf_instance_id,
    int success,
    char *time_stamp,
    OpenAPI_auth_type_e auth_type,
    char *serving_network_name,
    bool is_auth_removal_ind,
    int auth_removal_ind,
    char *nf_set_id,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_auth_event_t *auth_event_local_var = ogs_malloc(sizeof(OpenAPI_auth_event_t));
    ogs_assert(auth_event_local_var);

    auth_event_local_var->nf_instance_id = nf_instance_id;
    auth_event_local_var->success = success;
    auth_event_local_var->time_stamp = time_stamp;
    auth_event_local_var->auth_type = auth_type;
    auth_event_local_var->serving_network_name = serving_network_name;
    auth_event_local_var->is_auth_removal_ind = is_auth_removal_ind;
    auth_event_local_var->auth_removal_ind = auth_removal_ind;
    auth_event_local_var->nf_set_id = nf_set_id;
    auth_event_local_var->reset_ids = reset_ids;

    return auth_event_local_var;
}

void OpenAPI_auth_event_free(OpenAPI_auth_event_t *auth_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == auth_event) {
        return;
    }
    if (auth_event->nf_instance_id) {
        ogs_free(auth_event->nf_instance_id);
        auth_event->nf_instance_id = NULL;
    }
    if (auth_event->time_stamp) {
        ogs_free(auth_event->time_stamp);
        auth_event->time_stamp = NULL;
    }
    if (auth_event->serving_network_name) {
        ogs_free(auth_event->serving_network_name);
        auth_event->serving_network_name = NULL;
    }
    if (auth_event->nf_set_id) {
        ogs_free(auth_event->nf_set_id);
        auth_event->nf_set_id = NULL;
    }
    if (auth_event->reset_ids) {
        OpenAPI_list_for_each(auth_event->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(auth_event->reset_ids);
        auth_event->reset_ids = NULL;
    }
    ogs_free(auth_event);
}

cJSON *OpenAPI_auth_event_convertToJSON(OpenAPI_auth_event_t *auth_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (auth_event == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [AuthEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!auth_event->nf_instance_id) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [nf_instance_id]");
        return NULL;
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
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "timeStamp", auth_event->time_stamp) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [time_stamp]");
        goto end;
    }

    if (auth_event->auth_type == OpenAPI_auth_type_NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [auth_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(auth_event->auth_type)) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (!auth_event->serving_network_name) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [serving_network_name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "servingNetworkName", auth_event->serving_network_name) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [serving_network_name]");
        goto end;
    }

    if (auth_event->is_auth_removal_ind) {
    if (cJSON_AddBoolToObject(item, "authRemovalInd", auth_event->auth_removal_ind) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [auth_removal_ind]");
        goto end;
    }
    }

    if (auth_event->nf_set_id) {
    if (cJSON_AddStringToObject(item, "nfSetId", auth_event->nf_set_id) == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [nf_set_id]");
        goto end;
    }
    }

    if (auth_event->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_auth_event_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(auth_event->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_auth_event_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_auth_event_t *OpenAPI_auth_event_parseFromJSON(cJSON *auth_eventJSON)
{
    OpenAPI_auth_event_t *auth_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *success = NULL;
    cJSON *time_stamp = NULL;
    cJSON *auth_type = NULL;
    OpenAPI_auth_type_e auth_typeVariable = 0;
    cJSON *serving_network_name = NULL;
    cJSON *auth_removal_ind = NULL;
    cJSON *nf_set_id = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    success = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "success");
    if (!success) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [success]");
        goto end;
    }
    if (!cJSON_IsBool(success)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [success]");
        goto end;
    }

    time_stamp = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "timeStamp");
    if (!time_stamp) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(time_stamp) && !cJSON_IsNull(time_stamp)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [time_stamp]");
        goto end;
    }

    auth_type = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [auth_type]");
        goto end;
    }
    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    serving_network_name = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "servingNetworkName");
    if (!serving_network_name) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [serving_network_name]");
        goto end;
    }
    if (!cJSON_IsString(serving_network_name)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [serving_network_name]");
        goto end;
    }

    auth_removal_ind = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "authRemovalInd");
    if (auth_removal_ind) {
    if (!cJSON_IsBool(auth_removal_ind)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [auth_removal_ind]");
        goto end;
    }
    }

    nf_set_id = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "nfSetId");
    if (nf_set_id) {
    if (!cJSON_IsString(nf_set_id) && !cJSON_IsNull(nf_set_id)) {
        ogs_error("OpenAPI_auth_event_parseFromJSON() failed [nf_set_id]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(auth_eventJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_auth_event_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_auth_event_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    auth_event_local_var = OpenAPI_auth_event_create (
        ogs_strdup(nf_instance_id->valuestring),
        
        success->valueint,
        ogs_strdup(time_stamp->valuestring),
        auth_typeVariable,
        ogs_strdup(serving_network_name->valuestring),
        auth_removal_ind ? true : false,
        auth_removal_ind ? auth_removal_ind->valueint : 0,
        nf_set_id && !cJSON_IsNull(nf_set_id) ? ogs_strdup(nf_set_id->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return auth_event_local_var;
end:
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

