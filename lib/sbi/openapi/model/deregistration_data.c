
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "deregistration_data.h"

OpenAPI_deregistration_data_t *OpenAPI_deregistration_data_create(
    OpenAPI_deregistration_reason_e dereg_reason,
    OpenAPI_access_type_e access_type,
    bool is_pdu_session_id,
    int pdu_session_id,
    char *new_smf_instance_id
)
{
    OpenAPI_deregistration_data_t *deregistration_data_local_var = ogs_malloc(sizeof(OpenAPI_deregistration_data_t));
    ogs_assert(deregistration_data_local_var);

    deregistration_data_local_var->dereg_reason = dereg_reason;
    deregistration_data_local_var->access_type = access_type;
    deregistration_data_local_var->is_pdu_session_id = is_pdu_session_id;
    deregistration_data_local_var->pdu_session_id = pdu_session_id;
    deregistration_data_local_var->new_smf_instance_id = new_smf_instance_id;

    return deregistration_data_local_var;
}

void OpenAPI_deregistration_data_free(OpenAPI_deregistration_data_t *deregistration_data)
{
    if (NULL == deregistration_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(deregistration_data->new_smf_instance_id);
    ogs_free(deregistration_data);
}

cJSON *OpenAPI_deregistration_data_convertToJSON(OpenAPI_deregistration_data_t *deregistration_data)
{
    cJSON *item = NULL;

    if (deregistration_data == NULL) {
        ogs_error("OpenAPI_deregistration_data_convertToJSON() failed [DeregistrationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "deregReason", OpenAPI_deregistration_reason_ToString(deregistration_data->dereg_reason)) == NULL) {
        ogs_error("OpenAPI_deregistration_data_convertToJSON() failed [dereg_reason]");
        goto end;
    }

    if (deregistration_data->access_type) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(deregistration_data->access_type)) == NULL) {
        ogs_error("OpenAPI_deregistration_data_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (deregistration_data->is_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "pduSessionId", deregistration_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_deregistration_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    if (deregistration_data->new_smf_instance_id) {
    if (cJSON_AddStringToObject(item, "newSmfInstanceId", deregistration_data->new_smf_instance_id) == NULL) {
        ogs_error("OpenAPI_deregistration_data_convertToJSON() failed [new_smf_instance_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_deregistration_data_t *OpenAPI_deregistration_data_parseFromJSON(cJSON *deregistration_dataJSON)
{
    OpenAPI_deregistration_data_t *deregistration_data_local_var = NULL;
    cJSON *dereg_reason = cJSON_GetObjectItemCaseSensitive(deregistration_dataJSON, "deregReason");
    if (!dereg_reason) {
        ogs_error("OpenAPI_deregistration_data_parseFromJSON() failed [dereg_reason]");
        goto end;
    }

    OpenAPI_deregistration_reason_e dereg_reasonVariable;
    if (!cJSON_IsString(dereg_reason)) {
        ogs_error("OpenAPI_deregistration_data_parseFromJSON() failed [dereg_reason]");
        goto end;
    }
    dereg_reasonVariable = OpenAPI_deregistration_reason_FromString(dereg_reason->valuestring);

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(deregistration_dataJSON, "accessType");

    OpenAPI_access_type_e access_typeVariable;
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_deregistration_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(deregistration_dataJSON, "pduSessionId");

    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_deregistration_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    cJSON *new_smf_instance_id = cJSON_GetObjectItemCaseSensitive(deregistration_dataJSON, "newSmfInstanceId");

    if (new_smf_instance_id) {
    if (!cJSON_IsString(new_smf_instance_id)) {
        ogs_error("OpenAPI_deregistration_data_parseFromJSON() failed [new_smf_instance_id]");
        goto end;
    }
    }

    deregistration_data_local_var = OpenAPI_deregistration_data_create (
        dereg_reasonVariable,
        access_type ? access_typeVariable : 0,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        new_smf_instance_id ? ogs_strdup(new_smf_instance_id->valuestring) : NULL
    );

    return deregistration_data_local_var;
end:
    return NULL;
}

OpenAPI_deregistration_data_t *OpenAPI_deregistration_data_copy(OpenAPI_deregistration_data_t *dst, OpenAPI_deregistration_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_deregistration_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_deregistration_data_convertToJSON() failed");
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

    OpenAPI_deregistration_data_free(dst);
    dst = OpenAPI_deregistration_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

