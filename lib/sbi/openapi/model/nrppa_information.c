
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrppa_information.h"

OpenAPI_nrppa_information_t *OpenAPI_nrppa_information_create(
    char *nf_id,
    OpenAPI_n2_info_content_t *nrppa_pdu,
    char *service_instance_id
)
{
    OpenAPI_nrppa_information_t *nrppa_information_local_var = ogs_malloc(sizeof(OpenAPI_nrppa_information_t));
    ogs_assert(nrppa_information_local_var);

    nrppa_information_local_var->nf_id = nf_id;
    nrppa_information_local_var->nrppa_pdu = nrppa_pdu;
    nrppa_information_local_var->service_instance_id = service_instance_id;

    return nrppa_information_local_var;
}

void OpenAPI_nrppa_information_free(OpenAPI_nrppa_information_t *nrppa_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrppa_information) {
        return;
    }
    if (nrppa_information->nf_id) {
        ogs_free(nrppa_information->nf_id);
        nrppa_information->nf_id = NULL;
    }
    if (nrppa_information->nrppa_pdu) {
        OpenAPI_n2_info_content_free(nrppa_information->nrppa_pdu);
        nrppa_information->nrppa_pdu = NULL;
    }
    if (nrppa_information->service_instance_id) {
        ogs_free(nrppa_information->service_instance_id);
        nrppa_information->service_instance_id = NULL;
    }
    ogs_free(nrppa_information);
}

cJSON *OpenAPI_nrppa_information_convertToJSON(OpenAPI_nrppa_information_t *nrppa_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrppa_information == NULL) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [NrppaInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nrppa_information->nf_id) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [nf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfId", nrppa_information->nf_id) == NULL) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [nf_id]");
        goto end;
    }

    if (!nrppa_information->nrppa_pdu) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [nrppa_pdu]");
        return NULL;
    }
    cJSON *nrppa_pdu_local_JSON = OpenAPI_n2_info_content_convertToJSON(nrppa_information->nrppa_pdu);
    if (nrppa_pdu_local_JSON == NULL) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [nrppa_pdu]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nrppaPdu", nrppa_pdu_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [nrppa_pdu]");
        goto end;
    }

    if (nrppa_information->service_instance_id) {
    if (cJSON_AddStringToObject(item, "serviceInstanceId", nrppa_information->service_instance_id) == NULL) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed [service_instance_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nrppa_information_t *OpenAPI_nrppa_information_parseFromJSON(cJSON *nrppa_informationJSON)
{
    OpenAPI_nrppa_information_t *nrppa_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_id = NULL;
    cJSON *nrppa_pdu = NULL;
    OpenAPI_n2_info_content_t *nrppa_pdu_local_nonprim = NULL;
    cJSON *service_instance_id = NULL;
    nf_id = cJSON_GetObjectItemCaseSensitive(nrppa_informationJSON, "nfId");
    if (!nf_id) {
        ogs_error("OpenAPI_nrppa_information_parseFromJSON() failed [nf_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_id)) {
        ogs_error("OpenAPI_nrppa_information_parseFromJSON() failed [nf_id]");
        goto end;
    }

    nrppa_pdu = cJSON_GetObjectItemCaseSensitive(nrppa_informationJSON, "nrppaPdu");
    if (!nrppa_pdu) {
        ogs_error("OpenAPI_nrppa_information_parseFromJSON() failed [nrppa_pdu]");
        goto end;
    }
    nrppa_pdu_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(nrppa_pdu);
    if (!nrppa_pdu_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [nrppa_pdu]");
        goto end;
    }

    service_instance_id = cJSON_GetObjectItemCaseSensitive(nrppa_informationJSON, "serviceInstanceId");
    if (service_instance_id) {
    if (!cJSON_IsString(service_instance_id) && !cJSON_IsNull(service_instance_id)) {
        ogs_error("OpenAPI_nrppa_information_parseFromJSON() failed [service_instance_id]");
        goto end;
    }
    }

    nrppa_information_local_var = OpenAPI_nrppa_information_create (
        ogs_strdup(nf_id->valuestring),
        nrppa_pdu_local_nonprim,
        service_instance_id && !cJSON_IsNull(service_instance_id) ? ogs_strdup(service_instance_id->valuestring) : NULL
    );

    return nrppa_information_local_var;
end:
    if (nrppa_pdu_local_nonprim) {
        OpenAPI_n2_info_content_free(nrppa_pdu_local_nonprim);
        nrppa_pdu_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nrppa_information_t *OpenAPI_nrppa_information_copy(OpenAPI_nrppa_information_t *dst, OpenAPI_nrppa_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrppa_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrppa_information_convertToJSON() failed");
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

    OpenAPI_nrppa_information_free(dst);
    dst = OpenAPI_nrppa_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

