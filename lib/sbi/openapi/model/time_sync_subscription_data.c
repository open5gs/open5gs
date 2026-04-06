
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_sync_subscription_data.h"

OpenAPI_time_sync_subscription_data_t *OpenAPI_time_sync_subscription_data_create(
    OpenAPI_af_request_authorization_t *af_req_authorizations,
    OpenAPI_list_t *service_ids
)
{
    OpenAPI_time_sync_subscription_data_t *time_sync_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_time_sync_subscription_data_t));
    ogs_assert(time_sync_subscription_data_local_var);

    time_sync_subscription_data_local_var->af_req_authorizations = af_req_authorizations;
    time_sync_subscription_data_local_var->service_ids = service_ids;

    return time_sync_subscription_data_local_var;
}

void OpenAPI_time_sync_subscription_data_free(OpenAPI_time_sync_subscription_data_t *time_sync_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_sync_subscription_data) {
        return;
    }
    if (time_sync_subscription_data->af_req_authorizations) {
        OpenAPI_af_request_authorization_free(time_sync_subscription_data->af_req_authorizations);
        time_sync_subscription_data->af_req_authorizations = NULL;
    }
    if (time_sync_subscription_data->service_ids) {
        OpenAPI_list_for_each(time_sync_subscription_data->service_ids, node) {
            OpenAPI_time_sync_service_id_free(node->data);
        }
        OpenAPI_list_free(time_sync_subscription_data->service_ids);
        time_sync_subscription_data->service_ids = NULL;
    }
    ogs_free(time_sync_subscription_data);
}

cJSON *OpenAPI_time_sync_subscription_data_convertToJSON(OpenAPI_time_sync_subscription_data_t *time_sync_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_sync_subscription_data == NULL) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [TimeSyncSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!time_sync_subscription_data->af_req_authorizations) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [af_req_authorizations]");
        return NULL;
    }
    cJSON *af_req_authorizations_local_JSON = OpenAPI_af_request_authorization_convertToJSON(time_sync_subscription_data->af_req_authorizations);
    if (af_req_authorizations_local_JSON == NULL) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [af_req_authorizations]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afReqAuthorizations", af_req_authorizations_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [af_req_authorizations]");
        goto end;
    }

    if (!time_sync_subscription_data->service_ids) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [service_ids]");
        return NULL;
    }
    cJSON *service_idsList = cJSON_AddArrayToObject(item, "serviceIds");
    if (service_idsList == NULL) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [service_ids]");
        goto end;
    }
    OpenAPI_list_for_each(time_sync_subscription_data->service_ids, node) {
        cJSON *itemLocal = OpenAPI_time_sync_service_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed [service_ids]");
            goto end;
        }
        cJSON_AddItemToArray(service_idsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_time_sync_subscription_data_t *OpenAPI_time_sync_subscription_data_parseFromJSON(cJSON *time_sync_subscription_dataJSON)
{
    OpenAPI_time_sync_subscription_data_t *time_sync_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_req_authorizations = NULL;
    OpenAPI_af_request_authorization_t *af_req_authorizations_local_nonprim = NULL;
    cJSON *service_ids = NULL;
    OpenAPI_list_t *service_idsList = NULL;
    af_req_authorizations = cJSON_GetObjectItemCaseSensitive(time_sync_subscription_dataJSON, "afReqAuthorizations");
    if (!af_req_authorizations) {
        ogs_error("OpenAPI_time_sync_subscription_data_parseFromJSON() failed [af_req_authorizations]");
        goto end;
    }
    af_req_authorizations_local_nonprim = OpenAPI_af_request_authorization_parseFromJSON(af_req_authorizations);
    if (!af_req_authorizations_local_nonprim) {
        ogs_error("OpenAPI_af_request_authorization_parseFromJSON failed [af_req_authorizations]");
        goto end;
    }

    service_ids = cJSON_GetObjectItemCaseSensitive(time_sync_subscription_dataJSON, "serviceIds");
    if (!service_ids) {
        ogs_error("OpenAPI_time_sync_subscription_data_parseFromJSON() failed [service_ids]");
        goto end;
    }
        cJSON *service_ids_local = NULL;
        if (!cJSON_IsArray(service_ids)) {
            ogs_error("OpenAPI_time_sync_subscription_data_parseFromJSON() failed [service_ids]");
            goto end;
        }

        service_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(service_ids_local, service_ids) {
            if (!cJSON_IsObject(service_ids_local)) {
                ogs_error("OpenAPI_time_sync_subscription_data_parseFromJSON() failed [service_ids]");
                goto end;
            }
            OpenAPI_time_sync_service_id_t *service_idsItem = OpenAPI_time_sync_service_id_parseFromJSON(service_ids_local);
            if (!service_idsItem) {
                ogs_error("No service_idsItem");
                goto end;
            }
            OpenAPI_list_add(service_idsList, service_idsItem);
        }

    time_sync_subscription_data_local_var = OpenAPI_time_sync_subscription_data_create (
        af_req_authorizations_local_nonprim,
        service_idsList
    );

    return time_sync_subscription_data_local_var;
end:
    if (af_req_authorizations_local_nonprim) {
        OpenAPI_af_request_authorization_free(af_req_authorizations_local_nonprim);
        af_req_authorizations_local_nonprim = NULL;
    }
    if (service_idsList) {
        OpenAPI_list_for_each(service_idsList, node) {
            OpenAPI_time_sync_service_id_free(node->data);
        }
        OpenAPI_list_free(service_idsList);
        service_idsList = NULL;
    }
    return NULL;
}

OpenAPI_time_sync_subscription_data_t *OpenAPI_time_sync_subscription_data_copy(OpenAPI_time_sync_subscription_data_t *dst, OpenAPI_time_sync_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_sync_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_sync_subscription_data_convertToJSON() failed");
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

    OpenAPI_time_sync_subscription_data_free(dst);
    dst = OpenAPI_time_sync_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

