
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcscf_restoration_request_data.h"

OpenAPI_pcscf_restoration_request_data_t *OpenAPI_pcscf_restoration_request_data_create(
    char *dnn,
    char *ip_domain,
    OpenAPI_snssai_t *slice_info,
    char *supi,
    char *ue_ipv4,
    char *ue_ipv6
)
{
    OpenAPI_pcscf_restoration_request_data_t *pcscf_restoration_request_data_local_var = ogs_malloc(sizeof(OpenAPI_pcscf_restoration_request_data_t));
    ogs_assert(pcscf_restoration_request_data_local_var);

    pcscf_restoration_request_data_local_var->dnn = dnn;
    pcscf_restoration_request_data_local_var->ip_domain = ip_domain;
    pcscf_restoration_request_data_local_var->slice_info = slice_info;
    pcscf_restoration_request_data_local_var->supi = supi;
    pcscf_restoration_request_data_local_var->ue_ipv4 = ue_ipv4;
    pcscf_restoration_request_data_local_var->ue_ipv6 = ue_ipv6;

    return pcscf_restoration_request_data_local_var;
}

void OpenAPI_pcscf_restoration_request_data_free(OpenAPI_pcscf_restoration_request_data_t *pcscf_restoration_request_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcscf_restoration_request_data) {
        return;
    }
    if (pcscf_restoration_request_data->dnn) {
        ogs_free(pcscf_restoration_request_data->dnn);
        pcscf_restoration_request_data->dnn = NULL;
    }
    if (pcscf_restoration_request_data->ip_domain) {
        ogs_free(pcscf_restoration_request_data->ip_domain);
        pcscf_restoration_request_data->ip_domain = NULL;
    }
    if (pcscf_restoration_request_data->slice_info) {
        OpenAPI_snssai_free(pcscf_restoration_request_data->slice_info);
        pcscf_restoration_request_data->slice_info = NULL;
    }
    if (pcscf_restoration_request_data->supi) {
        ogs_free(pcscf_restoration_request_data->supi);
        pcscf_restoration_request_data->supi = NULL;
    }
    if (pcscf_restoration_request_data->ue_ipv4) {
        ogs_free(pcscf_restoration_request_data->ue_ipv4);
        pcscf_restoration_request_data->ue_ipv4 = NULL;
    }
    if (pcscf_restoration_request_data->ue_ipv6) {
        ogs_free(pcscf_restoration_request_data->ue_ipv6);
        pcscf_restoration_request_data->ue_ipv6 = NULL;
    }
    ogs_free(pcscf_restoration_request_data);
}

cJSON *OpenAPI_pcscf_restoration_request_data_convertToJSON(OpenAPI_pcscf_restoration_request_data_t *pcscf_restoration_request_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcscf_restoration_request_data == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [PcscfRestorationRequestData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcscf_restoration_request_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pcscf_restoration_request_data->dnn) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (pcscf_restoration_request_data->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", pcscf_restoration_request_data->ip_domain) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (pcscf_restoration_request_data->slice_info) {
    cJSON *slice_info_local_JSON = OpenAPI_snssai_convertToJSON(pcscf_restoration_request_data->slice_info);
    if (slice_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceInfo", slice_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    }

    if (pcscf_restoration_request_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", pcscf_restoration_request_data->supi) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (pcscf_restoration_request_data->ue_ipv4) {
    if (cJSON_AddStringToObject(item, "ueIpv4", pcscf_restoration_request_data->ue_ipv4) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    if (pcscf_restoration_request_data->ue_ipv6) {
    if (cJSON_AddStringToObject(item, "ueIpv6", pcscf_restoration_request_data->ue_ipv6) == NULL) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed [ue_ipv6]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcscf_restoration_request_data_t *OpenAPI_pcscf_restoration_request_data_parseFromJSON(cJSON *pcscf_restoration_request_dataJSON)
{
    OpenAPI_pcscf_restoration_request_data_t *pcscf_restoration_request_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *ip_domain = NULL;
    cJSON *slice_info = NULL;
    OpenAPI_snssai_t *slice_info_local_nonprim = NULL;
    cJSON *supi = NULL;
    cJSON *ue_ipv4 = NULL;
    cJSON *ue_ipv6 = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_request_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_request_dataJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    slice_info = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_request_dataJSON, "sliceInfo");
    if (slice_info) {
    slice_info_local_nonprim = OpenAPI_snssai_parseFromJSON(slice_info);
    if (!slice_info_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [slice_info]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_request_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    ue_ipv4 = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_request_dataJSON, "ueIpv4");
    if (ue_ipv4) {
    if (!cJSON_IsString(ue_ipv4) && !cJSON_IsNull(ue_ipv4)) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_parseFromJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    ue_ipv6 = cJSON_GetObjectItemCaseSensitive(pcscf_restoration_request_dataJSON, "ueIpv6");
    if (ue_ipv6) {
    if (!cJSON_IsString(ue_ipv6) && !cJSON_IsNull(ue_ipv6)) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_parseFromJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    pcscf_restoration_request_data_local_var = OpenAPI_pcscf_restoration_request_data_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        slice_info ? slice_info_local_nonprim : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        ue_ipv4 && !cJSON_IsNull(ue_ipv4) ? ogs_strdup(ue_ipv4->valuestring) : NULL,
        ue_ipv6 && !cJSON_IsNull(ue_ipv6) ? ogs_strdup(ue_ipv6->valuestring) : NULL
    );

    return pcscf_restoration_request_data_local_var;
end:
    if (slice_info_local_nonprim) {
        OpenAPI_snssai_free(slice_info_local_nonprim);
        slice_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pcscf_restoration_request_data_t *OpenAPI_pcscf_restoration_request_data_copy(OpenAPI_pcscf_restoration_request_data_t *dst, OpenAPI_pcscf_restoration_request_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcscf_restoration_request_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcscf_restoration_request_data_convertToJSON() failed");
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

    OpenAPI_pcscf_restoration_request_data_free(dst);
    dst = OpenAPI_pcscf_restoration_request_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

