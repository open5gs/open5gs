
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdm_subscription.h"

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_create(
    char *nf_instance_id,
    int implicit_unsubscribe,
    char *expires,
    char *callback_reference,
    char *amf_service_name,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    char *subscription_id,
    OpenAPI_plmn_id_t *plmn_id,
    int immediate_report,
    OpenAPI_subscription_data_sets_t *report
    )
{
    OpenAPI_sdm_subscription_t *sdm_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_sdm_subscription_t));
    if (!sdm_subscription_local_var) {
        return NULL;
    }
    sdm_subscription_local_var->nf_instance_id = nf_instance_id;
    sdm_subscription_local_var->implicit_unsubscribe = implicit_unsubscribe;
    sdm_subscription_local_var->expires = expires;
    sdm_subscription_local_var->callback_reference = callback_reference;
    sdm_subscription_local_var->amf_service_name = amf_service_name;
    sdm_subscription_local_var->monitored_resource_uris = monitored_resource_uris;
    sdm_subscription_local_var->single_nssai = single_nssai;
    sdm_subscription_local_var->dnn = dnn;
    sdm_subscription_local_var->subscription_id = subscription_id;
    sdm_subscription_local_var->plmn_id = plmn_id;
    sdm_subscription_local_var->immediate_report = immediate_report;
    sdm_subscription_local_var->report = report;

    return sdm_subscription_local_var;
}

void OpenAPI_sdm_subscription_free(OpenAPI_sdm_subscription_t *sdm_subscription)
{
    if (NULL == sdm_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sdm_subscription->nf_instance_id);
    ogs_free(sdm_subscription->expires);
    ogs_free(sdm_subscription->callback_reference);
    ogs_free(sdm_subscription->amf_service_name);
    OpenAPI_list_for_each(sdm_subscription->monitored_resource_uris, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sdm_subscription->monitored_resource_uris);
    OpenAPI_snssai_free(sdm_subscription->single_nssai);
    ogs_free(sdm_subscription->dnn);
    ogs_free(sdm_subscription->subscription_id);
    OpenAPI_plmn_id_free(sdm_subscription->plmn_id);
    OpenAPI_subscription_data_sets_free(sdm_subscription->report);
    ogs_free(sdm_subscription);
}

cJSON *OpenAPI_sdm_subscription_convertToJSON(OpenAPI_sdm_subscription_t *sdm_subscription)
{
    cJSON *item = NULL;

    if (sdm_subscription == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [SdmSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sdm_subscription->nf_instance_id) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", sdm_subscription->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (sdm_subscription->implicit_unsubscribe) {
        if (cJSON_AddBoolToObject(item, "implicitUnsubscribe", sdm_subscription->implicit_unsubscribe) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [implicit_unsubscribe]");
            goto end;
        }
    }

    if (sdm_subscription->expires) {
        if (cJSON_AddStringToObject(item, "expires", sdm_subscription->expires) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [expires]");
            goto end;
        }
    }

    if (!sdm_subscription->callback_reference) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [callback_reference]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", sdm_subscription->callback_reference) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (sdm_subscription->amf_service_name) {
        if (cJSON_AddStringToObject(item, "amfServiceName", sdm_subscription->amf_service_name) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [amf_service_name]");
            goto end;
        }
    }

    if (!sdm_subscription->monitored_resource_uris) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    cJSON *monitored_resource_uris = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_uris == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_lnode_t *monitored_resource_uris_node;
    OpenAPI_list_for_each(sdm_subscription->monitored_resource_uris, monitored_resource_uris_node)  {
        if (cJSON_AddStringToObject(monitored_resource_uris, "", (char*)monitored_resource_uris_node->data) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (sdm_subscription->single_nssai) {
        cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sdm_subscription->single_nssai);
        if (single_nssai_local_JSON == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [single_nssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [single_nssai]");
            goto end;
        }
    }

    if (sdm_subscription->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", sdm_subscription->dnn) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (sdm_subscription->subscription_id) {
        if (cJSON_AddStringToObject(item, "subscriptionId", sdm_subscription->subscription_id) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [subscription_id]");
            goto end;
        }
    }

    if (sdm_subscription->plmn_id) {
        cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(sdm_subscription->plmn_id);
        if (plmn_id_local_JSON == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [plmn_id]");
            goto end;
        }
        cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [plmn_id]");
            goto end;
        }
    }

    if (sdm_subscription->immediate_report) {
        if (cJSON_AddBoolToObject(item, "immediateReport", sdm_subscription->immediate_report) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [immediate_report]");
            goto end;
        }
    }

    if (sdm_subscription->report) {
        cJSON *report_local_JSON = OpenAPI_subscription_data_sets_convertToJSON(sdm_subscription->report);
        if (report_local_JSON == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [report]");
            goto end;
        }
        cJSON_AddItemToObject(item, "report", report_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [report]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_parseFromJSON(cJSON *sdm_subscriptionJSON)
{
    OpenAPI_sdm_subscription_t *sdm_subscription_local_var = NULL;
    cJSON *nf_instance_id = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    cJSON *implicit_unsubscribe = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "implicitUnsubscribe");

    if (implicit_unsubscribe) {
        if (!cJSON_IsBool(implicit_unsubscribe)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [implicit_unsubscribe]");
            goto end;
        }
    }

    cJSON *expires = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "expires");

    if (expires) {
        if (!cJSON_IsString(expires)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [expires]");
            goto end;
        }
    }

    cJSON *callback_reference = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }


    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    cJSON *amf_service_name = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "amfServiceName");

    if (amf_service_name) {
        if (!cJSON_IsString(amf_service_name)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [amf_service_name]");
            goto end;
        }
    }

    cJSON *monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_list_t *monitored_resource_urisList;

    cJSON *monitored_resource_uris_local;
    if (!cJSON_IsArray(monitored_resource_uris)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    monitored_resource_urisList = OpenAPI_list_create();

    cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
        if (!cJSON_IsString(monitored_resource_uris_local)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }
        OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
    }

    cJSON *single_nssai = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "singleNssai");

    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    if (single_nssai) {
        single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *subscription_id = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "subscriptionId");

    if (subscription_id) {
        if (!cJSON_IsString(subscription_id)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [subscription_id]");
            goto end;
        }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "plmnId");

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
        plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    }

    cJSON *immediate_report = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "immediateReport");

    if (immediate_report) {
        if (!cJSON_IsBool(immediate_report)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [immediate_report]");
            goto end;
        }
    }

    cJSON *report = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "report");

    OpenAPI_subscription_data_sets_t *report_local_nonprim = NULL;
    if (report) {
        report_local_nonprim = OpenAPI_subscription_data_sets_parseFromJSON(report);
    }

    sdm_subscription_local_var = OpenAPI_sdm_subscription_create (
        ogs_strdup(nf_instance_id->valuestring),
        implicit_unsubscribe ? implicit_unsubscribe->valueint : 0,
        expires ? ogs_strdup(expires->valuestring) : NULL,
        ogs_strdup(callback_reference->valuestring),
        amf_service_name ? ogs_strdup(amf_service_name->valuestring) : NULL,
        monitored_resource_urisList,
        single_nssai ? single_nssai_local_nonprim : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        subscription_id ? ogs_strdup(subscription_id->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        immediate_report ? immediate_report->valueint : 0,
        report ? report_local_nonprim : NULL
        );

    return sdm_subscription_local_var;
end:
    return NULL;
}

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_copy(OpenAPI_sdm_subscription_t *dst, OpenAPI_sdm_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdm_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed");
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

    OpenAPI_sdm_subscription_free(dst);
    dst = OpenAPI_sdm_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

