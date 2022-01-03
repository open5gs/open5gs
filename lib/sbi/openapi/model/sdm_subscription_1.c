
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdm_subscription_1.h"

OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_create(
    char *nf_instance_id,
    bool is_implicit_unsubscribe,
    int implicit_unsubscribe,
    char *expires,
    char *callback_reference,
    char *amf_service_name,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    char *subscription_id,
    OpenAPI_plmn_id_1_t *plmn_id,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_subscription_data_sets_1_t *report,
    char *supported_features,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_sdm_subscription_1_t *sdm_subscription_1_local_var = ogs_malloc(sizeof(OpenAPI_sdm_subscription_1_t));
    ogs_assert(sdm_subscription_1_local_var);

    sdm_subscription_1_local_var->nf_instance_id = nf_instance_id;
    sdm_subscription_1_local_var->is_implicit_unsubscribe = is_implicit_unsubscribe;
    sdm_subscription_1_local_var->implicit_unsubscribe = implicit_unsubscribe;
    sdm_subscription_1_local_var->expires = expires;
    sdm_subscription_1_local_var->callback_reference = callback_reference;
    sdm_subscription_1_local_var->amf_service_name = amf_service_name;
    sdm_subscription_1_local_var->monitored_resource_uris = monitored_resource_uris;
    sdm_subscription_1_local_var->single_nssai = single_nssai;
    sdm_subscription_1_local_var->dnn = dnn;
    sdm_subscription_1_local_var->subscription_id = subscription_id;
    sdm_subscription_1_local_var->plmn_id = plmn_id;
    sdm_subscription_1_local_var->is_immediate_report = is_immediate_report;
    sdm_subscription_1_local_var->immediate_report = immediate_report;
    sdm_subscription_1_local_var->report = report;
    sdm_subscription_1_local_var->supported_features = supported_features;
    sdm_subscription_1_local_var->context_info = context_info;

    return sdm_subscription_1_local_var;
}

void OpenAPI_sdm_subscription_1_free(OpenAPI_sdm_subscription_1_t *sdm_subscription_1)
{
    if (NULL == sdm_subscription_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sdm_subscription_1->nf_instance_id);
    ogs_free(sdm_subscription_1->expires);
    ogs_free(sdm_subscription_1->callback_reference);
    ogs_free(sdm_subscription_1->amf_service_name);
    OpenAPI_list_for_each(sdm_subscription_1->monitored_resource_uris, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sdm_subscription_1->monitored_resource_uris);
    OpenAPI_snssai_free(sdm_subscription_1->single_nssai);
    ogs_free(sdm_subscription_1->dnn);
    ogs_free(sdm_subscription_1->subscription_id);
    OpenAPI_plmn_id_1_free(sdm_subscription_1->plmn_id);
    OpenAPI_subscription_data_sets_1_free(sdm_subscription_1->report);
    ogs_free(sdm_subscription_1->supported_features);
    OpenAPI_context_info_free(sdm_subscription_1->context_info);
    ogs_free(sdm_subscription_1);
}

cJSON *OpenAPI_sdm_subscription_1_convertToJSON(OpenAPI_sdm_subscription_1_t *sdm_subscription_1)
{
    cJSON *item = NULL;

    if (sdm_subscription_1 == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [SdmSubscription_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "nfInstanceId", sdm_subscription_1->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (sdm_subscription_1->is_implicit_unsubscribe) {
    if (cJSON_AddBoolToObject(item, "implicitUnsubscribe", sdm_subscription_1->implicit_unsubscribe) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [implicit_unsubscribe]");
        goto end;
    }
    }

    if (sdm_subscription_1->expires) {
    if (cJSON_AddStringToObject(item, "expires", sdm_subscription_1->expires) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [expires]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "callbackReference", sdm_subscription_1->callback_reference) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (sdm_subscription_1->amf_service_name) {
    if (cJSON_AddStringToObject(item, "amfServiceName", sdm_subscription_1->amf_service_name) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [amf_service_name]");
        goto end;
    }
    }

    cJSON *monitored_resource_uris = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_uris == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_lnode_t *monitored_resource_uris_node;
    OpenAPI_list_for_each(sdm_subscription_1->monitored_resource_uris, monitored_resource_uris_node)  {
    if (cJSON_AddStringToObject(monitored_resource_uris, "", (char*)monitored_resource_uris_node->data) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
                    }

    if (sdm_subscription_1->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sdm_subscription_1->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (sdm_subscription_1->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", sdm_subscription_1->dnn) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (sdm_subscription_1->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", sdm_subscription_1->subscription_id) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (sdm_subscription_1->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(sdm_subscription_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (sdm_subscription_1->is_immediate_report) {
    if (cJSON_AddBoolToObject(item, "immediateReport", sdm_subscription_1->immediate_report) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [immediate_report]");
        goto end;
    }
    }

    if (sdm_subscription_1->report) {
    cJSON *report_local_JSON = OpenAPI_subscription_data_sets_1_convertToJSON(sdm_subscription_1->report);
    if (report_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [report]");
        goto end;
    }
    cJSON_AddItemToObject(item, "report", report_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [report]");
        goto end;
    }
    }

    if (sdm_subscription_1->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sdm_subscription_1->supported_features) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sdm_subscription_1->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(sdm_subscription_1->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_parseFromJSON(cJSON *sdm_subscription_1JSON)
{
    OpenAPI_sdm_subscription_1_t *sdm_subscription_1_local_var = NULL;
    cJSON *nf_instance_id = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    cJSON *implicit_unsubscribe = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "implicitUnsubscribe");

    if (implicit_unsubscribe) {
    if (!cJSON_IsBool(implicit_unsubscribe)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [implicit_unsubscribe]");
        goto end;
    }
    }

    cJSON *expires = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "expires");

    if (expires) {
    if (!cJSON_IsString(expires)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [expires]");
        goto end;
    }
    }

    cJSON *callback_reference = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    cJSON *amf_service_name = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "amfServiceName");

    if (amf_service_name) {
    if (!cJSON_IsString(amf_service_name)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [amf_service_name]");
        goto end;
    }
    }

    cJSON *monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_list_t *monitored_resource_urisList;
    cJSON *monitored_resource_uris_local;
    if (!cJSON_IsArray(monitored_resource_uris)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    monitored_resource_urisList = OpenAPI_list_create();

    cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
    if (!cJSON_IsString(monitored_resource_uris_local)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_add(monitored_resource_urisList , ogs_strdup(monitored_resource_uris_local->valuestring));
    }

    cJSON *single_nssai = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "singleNssai");

    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *subscription_id = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "subscriptionId");

    if (subscription_id) {
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "plmnId");

    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    }

    cJSON *immediate_report = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "immediateReport");

    if (immediate_report) {
    if (!cJSON_IsBool(immediate_report)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [immediate_report]");
        goto end;
    }
    }

    cJSON *report = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "report");

    OpenAPI_subscription_data_sets_1_t *report_local_nonprim = NULL;
    if (report) {
    report_local_nonprim = OpenAPI_subscription_data_sets_1_parseFromJSON(report);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *context_info = cJSON_GetObjectItemCaseSensitive(sdm_subscription_1JSON, "contextInfo");

    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    }

    sdm_subscription_1_local_var = OpenAPI_sdm_subscription_1_create (
        ogs_strdup(nf_instance_id->valuestring),
        implicit_unsubscribe ? true : false,
        implicit_unsubscribe ? implicit_unsubscribe->valueint : 0,
        expires ? ogs_strdup(expires->valuestring) : NULL,
        ogs_strdup(callback_reference->valuestring),
        amf_service_name ? ogs_strdup(amf_service_name->valuestring) : NULL,
        monitored_resource_urisList,
        single_nssai ? single_nssai_local_nonprim : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        subscription_id ? ogs_strdup(subscription_id->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        immediate_report ? true : false,
        immediate_report ? immediate_report->valueint : 0,
        report ? report_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL
    );

    return sdm_subscription_1_local_var;
end:
    return NULL;
}

OpenAPI_sdm_subscription_1_t *OpenAPI_sdm_subscription_1_copy(OpenAPI_sdm_subscription_1_t *dst, OpenAPI_sdm_subscription_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdm_subscription_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdm_subscription_1_convertToJSON() failed");
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

    OpenAPI_sdm_subscription_1_free(dst);
    dst = OpenAPI_sdm_subscription_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

