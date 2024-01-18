
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_management_subscription_data.h"

OpenAPI_sms_management_subscription_data_t *OpenAPI_sms_management_subscription_data_create(
    char *supported_features,
    bool is_mt_sms_subscribed,
    int mt_sms_subscribed,
    bool is_mt_sms_barring_all,
    int mt_sms_barring_all,
    bool is_mt_sms_barring_roaming,
    int mt_sms_barring_roaming,
    bool is_mo_sms_subscribed,
    int mo_sms_subscribed,
    bool is_mo_sms_barring_all,
    int mo_sms_barring_all,
    bool is_mo_sms_barring_roaming,
    int mo_sms_barring_roaming,
    OpenAPI_list_t *shared_sms_mng_data_ids,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data
)
{
    OpenAPI_sms_management_subscription_data_t *sms_management_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_sms_management_subscription_data_t));
    ogs_assert(sms_management_subscription_data_local_var);

    sms_management_subscription_data_local_var->supported_features = supported_features;
    sms_management_subscription_data_local_var->is_mt_sms_subscribed = is_mt_sms_subscribed;
    sms_management_subscription_data_local_var->mt_sms_subscribed = mt_sms_subscribed;
    sms_management_subscription_data_local_var->is_mt_sms_barring_all = is_mt_sms_barring_all;
    sms_management_subscription_data_local_var->mt_sms_barring_all = mt_sms_barring_all;
    sms_management_subscription_data_local_var->is_mt_sms_barring_roaming = is_mt_sms_barring_roaming;
    sms_management_subscription_data_local_var->mt_sms_barring_roaming = mt_sms_barring_roaming;
    sms_management_subscription_data_local_var->is_mo_sms_subscribed = is_mo_sms_subscribed;
    sms_management_subscription_data_local_var->mo_sms_subscribed = mo_sms_subscribed;
    sms_management_subscription_data_local_var->is_mo_sms_barring_all = is_mo_sms_barring_all;
    sms_management_subscription_data_local_var->mo_sms_barring_all = mo_sms_barring_all;
    sms_management_subscription_data_local_var->is_mo_sms_barring_roaming = is_mo_sms_barring_roaming;
    sms_management_subscription_data_local_var->mo_sms_barring_roaming = mo_sms_barring_roaming;
    sms_management_subscription_data_local_var->shared_sms_mng_data_ids = shared_sms_mng_data_ids;
    sms_management_subscription_data_local_var->is_trace_data_null = is_trace_data_null;
    sms_management_subscription_data_local_var->trace_data = trace_data;

    return sms_management_subscription_data_local_var;
}

void OpenAPI_sms_management_subscription_data_free(OpenAPI_sms_management_subscription_data_t *sms_management_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_management_subscription_data) {
        return;
    }
    if (sms_management_subscription_data->supported_features) {
        ogs_free(sms_management_subscription_data->supported_features);
        sms_management_subscription_data->supported_features = NULL;
    }
    if (sms_management_subscription_data->shared_sms_mng_data_ids) {
        OpenAPI_list_for_each(sms_management_subscription_data->shared_sms_mng_data_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sms_management_subscription_data->shared_sms_mng_data_ids);
        sms_management_subscription_data->shared_sms_mng_data_ids = NULL;
    }
    if (sms_management_subscription_data->trace_data) {
        OpenAPI_trace_data_free(sms_management_subscription_data->trace_data);
        sms_management_subscription_data->trace_data = NULL;
    }
    ogs_free(sms_management_subscription_data);
}

cJSON *OpenAPI_sms_management_subscription_data_convertToJSON(OpenAPI_sms_management_subscription_data_t *sms_management_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_management_subscription_data == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [SmsManagementSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sms_management_subscription_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sms_management_subscription_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sms_management_subscription_data->is_mt_sms_subscribed) {
    if (cJSON_AddBoolToObject(item, "mtSmsSubscribed", sms_management_subscription_data->mt_sms_subscribed) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [mt_sms_subscribed]");
        goto end;
    }
    }

    if (sms_management_subscription_data->is_mt_sms_barring_all) {
    if (cJSON_AddBoolToObject(item, "mtSmsBarringAll", sms_management_subscription_data->mt_sms_barring_all) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [mt_sms_barring_all]");
        goto end;
    }
    }

    if (sms_management_subscription_data->is_mt_sms_barring_roaming) {
    if (cJSON_AddBoolToObject(item, "mtSmsBarringRoaming", sms_management_subscription_data->mt_sms_barring_roaming) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [mt_sms_barring_roaming]");
        goto end;
    }
    }

    if (sms_management_subscription_data->is_mo_sms_subscribed) {
    if (cJSON_AddBoolToObject(item, "moSmsSubscribed", sms_management_subscription_data->mo_sms_subscribed) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [mo_sms_subscribed]");
        goto end;
    }
    }

    if (sms_management_subscription_data->is_mo_sms_barring_all) {
    if (cJSON_AddBoolToObject(item, "moSmsBarringAll", sms_management_subscription_data->mo_sms_barring_all) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [mo_sms_barring_all]");
        goto end;
    }
    }

    if (sms_management_subscription_data->is_mo_sms_barring_roaming) {
    if (cJSON_AddBoolToObject(item, "moSmsBarringRoaming", sms_management_subscription_data->mo_sms_barring_roaming) == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [mo_sms_barring_roaming]");
        goto end;
    }
    }

    if (sms_management_subscription_data->shared_sms_mng_data_ids) {
    cJSON *shared_sms_mng_data_idsList = cJSON_AddArrayToObject(item, "sharedSmsMngDataIds");
    if (shared_sms_mng_data_idsList == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [shared_sms_mng_data_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sms_management_subscription_data->shared_sms_mng_data_ids, node) {
        if (cJSON_AddStringToObject(shared_sms_mng_data_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [shared_sms_mng_data_ids]");
            goto end;
        }
    }
    }

    if (sms_management_subscription_data->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(sms_management_subscription_data->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (sms_management_subscription_data->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sms_management_subscription_data_t *OpenAPI_sms_management_subscription_data_parseFromJSON(cJSON *sms_management_subscription_dataJSON)
{
    OpenAPI_sms_management_subscription_data_t *sms_management_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supported_features = NULL;
    cJSON *mt_sms_subscribed = NULL;
    cJSON *mt_sms_barring_all = NULL;
    cJSON *mt_sms_barring_roaming = NULL;
    cJSON *mo_sms_subscribed = NULL;
    cJSON *mo_sms_barring_all = NULL;
    cJSON *mo_sms_barring_roaming = NULL;
    cJSON *shared_sms_mng_data_ids = NULL;
    OpenAPI_list_t *shared_sms_mng_data_idsList = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    supported_features = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    mt_sms_subscribed = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "mtSmsSubscribed");
    if (mt_sms_subscribed) {
    if (!cJSON_IsBool(mt_sms_subscribed)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [mt_sms_subscribed]");
        goto end;
    }
    }

    mt_sms_barring_all = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "mtSmsBarringAll");
    if (mt_sms_barring_all) {
    if (!cJSON_IsBool(mt_sms_barring_all)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [mt_sms_barring_all]");
        goto end;
    }
    }

    mt_sms_barring_roaming = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "mtSmsBarringRoaming");
    if (mt_sms_barring_roaming) {
    if (!cJSON_IsBool(mt_sms_barring_roaming)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [mt_sms_barring_roaming]");
        goto end;
    }
    }

    mo_sms_subscribed = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "moSmsSubscribed");
    if (mo_sms_subscribed) {
    if (!cJSON_IsBool(mo_sms_subscribed)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [mo_sms_subscribed]");
        goto end;
    }
    }

    mo_sms_barring_all = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "moSmsBarringAll");
    if (mo_sms_barring_all) {
    if (!cJSON_IsBool(mo_sms_barring_all)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [mo_sms_barring_all]");
        goto end;
    }
    }

    mo_sms_barring_roaming = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "moSmsBarringRoaming");
    if (mo_sms_barring_roaming) {
    if (!cJSON_IsBool(mo_sms_barring_roaming)) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [mo_sms_barring_roaming]");
        goto end;
    }
    }

    shared_sms_mng_data_ids = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "sharedSmsMngDataIds");
    if (shared_sms_mng_data_ids) {
        cJSON *shared_sms_mng_data_ids_local = NULL;
        if (!cJSON_IsArray(shared_sms_mng_data_ids)) {
            ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [shared_sms_mng_data_ids]");
            goto end;
        }

        shared_sms_mng_data_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_sms_mng_data_ids_local, shared_sms_mng_data_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(shared_sms_mng_data_ids_local)) {
                ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON() failed [shared_sms_mng_data_ids]");
                goto end;
            }
            OpenAPI_list_add(shared_sms_mng_data_idsList, ogs_strdup(shared_sms_mng_data_ids_local->valuestring));
        }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(sms_management_subscription_dataJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    sms_management_subscription_data_local_var = OpenAPI_sms_management_subscription_data_create (
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        mt_sms_subscribed ? true : false,
        mt_sms_subscribed ? mt_sms_subscribed->valueint : 0,
        mt_sms_barring_all ? true : false,
        mt_sms_barring_all ? mt_sms_barring_all->valueint : 0,
        mt_sms_barring_roaming ? true : false,
        mt_sms_barring_roaming ? mt_sms_barring_roaming->valueint : 0,
        mo_sms_subscribed ? true : false,
        mo_sms_subscribed ? mo_sms_subscribed->valueint : 0,
        mo_sms_barring_all ? true : false,
        mo_sms_barring_all ? mo_sms_barring_all->valueint : 0,
        mo_sms_barring_roaming ? true : false,
        mo_sms_barring_roaming ? mo_sms_barring_roaming->valueint : 0,
        shared_sms_mng_data_ids ? shared_sms_mng_data_idsList : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL
    );

    return sms_management_subscription_data_local_var;
end:
    if (shared_sms_mng_data_idsList) {
        OpenAPI_list_for_each(shared_sms_mng_data_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_sms_mng_data_idsList);
        shared_sms_mng_data_idsList = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sms_management_subscription_data_t *OpenAPI_sms_management_subscription_data_copy(OpenAPI_sms_management_subscription_data_t *dst, OpenAPI_sms_management_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_management_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_management_subscription_data_convertToJSON() failed");
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

    OpenAPI_sms_management_subscription_data_free(dst);
    dst = OpenAPI_sms_management_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

