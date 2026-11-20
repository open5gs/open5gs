
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_gs_bridge_information.h"

OpenAPI_model_5_gs_bridge_information_t *OpenAPI_model_5_gs_bridge_information_create(
    long bridge_id,
    bool is_n_wtt_port_number,
    int n_wtt_port_number,
    bool is_d_stt_port_number,
    int d_stt_port_number
)
{
    OpenAPI_model_5_gs_bridge_information_t *model_5_gs_bridge_information_local_var = ogs_malloc(sizeof(OpenAPI_model_5_gs_bridge_information_t));
    ogs_assert(model_5_gs_bridge_information_local_var);

    model_5_gs_bridge_information_local_var->bridge_id = bridge_id;
    model_5_gs_bridge_information_local_var->is_n_wtt_port_number = is_n_wtt_port_number;
    model_5_gs_bridge_information_local_var->n_wtt_port_number = n_wtt_port_number;
    model_5_gs_bridge_information_local_var->is_d_stt_port_number = is_d_stt_port_number;
    model_5_gs_bridge_information_local_var->d_stt_port_number = d_stt_port_number;

    return model_5_gs_bridge_information_local_var;
}

void OpenAPI_model_5_gs_bridge_information_free(OpenAPI_model_5_gs_bridge_information_t *model_5_gs_bridge_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_gs_bridge_information) {
        return;
    }
    ogs_free(model_5_gs_bridge_information);
}

cJSON *OpenAPI_model_5_gs_bridge_information_convertToJSON(OpenAPI_model_5_gs_bridge_information_t *model_5_gs_bridge_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_gs_bridge_information == NULL) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_convertToJSON() failed [5GSBridgeInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "bridgeId", model_5_gs_bridge_information->bridge_id) == NULL) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_convertToJSON() failed [bridge_id]");
        goto end;
    }

    if (model_5_gs_bridge_information->is_n_wtt_port_number) {
    if (cJSON_AddNumberToObject(item, "nWTTPortNumber", model_5_gs_bridge_information->n_wtt_port_number) == NULL) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_convertToJSON() failed [n_wtt_port_number]");
        goto end;
    }
    }

    if (model_5_gs_bridge_information->is_d_stt_port_number) {
    if (cJSON_AddNumberToObject(item, "dSTTPortNumber", model_5_gs_bridge_information->d_stt_port_number) == NULL) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_convertToJSON() failed [d_stt_port_number]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_model_5_gs_bridge_information_t *OpenAPI_model_5_gs_bridge_information_parseFromJSON(cJSON *model_5_gs_bridge_informationJSON)
{
    OpenAPI_model_5_gs_bridge_information_t *model_5_gs_bridge_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *bridge_id = NULL;
    cJSON *n_wtt_port_number = NULL;
    cJSON *d_stt_port_number = NULL;
    bridge_id = cJSON_GetObjectItemCaseSensitive(model_5_gs_bridge_informationJSON, "bridgeId");
    if (!bridge_id) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_parseFromJSON() failed [bridge_id]");
        goto end;
    }
    if (!cJSON_IsNumber(bridge_id)) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_parseFromJSON() failed [bridge_id]");
        goto end;
    }

    n_wtt_port_number = cJSON_GetObjectItemCaseSensitive(model_5_gs_bridge_informationJSON, "nWTTPortNumber");
    if (n_wtt_port_number) {
    if (!cJSON_IsNumber(n_wtt_port_number)) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_parseFromJSON() failed [n_wtt_port_number]");
        goto end;
    }
    }

    d_stt_port_number = cJSON_GetObjectItemCaseSensitive(model_5_gs_bridge_informationJSON, "dSTTPortNumber");
    if (d_stt_port_number) {
    if (!cJSON_IsNumber(d_stt_port_number)) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_parseFromJSON() failed [d_stt_port_number]");
        goto end;
    }
    }

    model_5_gs_bridge_information_local_var = OpenAPI_model_5_gs_bridge_information_create (
        
        bridge_id->valuedouble,
        n_wtt_port_number ? true : false,
        n_wtt_port_number ? n_wtt_port_number->valuedouble : 0,
        d_stt_port_number ? true : false,
        d_stt_port_number ? d_stt_port_number->valuedouble : 0
    );

    return model_5_gs_bridge_information_local_var;
end:
    return NULL;
}

OpenAPI_model_5_gs_bridge_information_t *OpenAPI_model_5_gs_bridge_information_copy(OpenAPI_model_5_gs_bridge_information_t *dst, OpenAPI_model_5_gs_bridge_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_gs_bridge_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_convertToJSON() failed");
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

    OpenAPI_model_5_gs_bridge_information_free(dst);
    dst = OpenAPI_model_5_gs_bridge_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

