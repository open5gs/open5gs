
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_functionality_data_1.h"

OpenAPI_upf_functionality_data_1_t *OpenAPI_upf_functionality_data_1_create(
    OpenAPI_upf_packet_inspection_functionality_e packet_insfunc,
    char *op_confupf_capa
)
{
    OpenAPI_upf_functionality_data_1_t *upf_functionality_data_1_local_var = ogs_malloc(sizeof(OpenAPI_upf_functionality_data_1_t));
    ogs_assert(upf_functionality_data_1_local_var);

    upf_functionality_data_1_local_var->packet_insfunc = packet_insfunc;
    upf_functionality_data_1_local_var->op_confupf_capa = op_confupf_capa;

    return upf_functionality_data_1_local_var;
}

void OpenAPI_upf_functionality_data_1_free(OpenAPI_upf_functionality_data_1_t *upf_functionality_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upf_functionality_data_1) {
        return;
    }
    if (upf_functionality_data_1->op_confupf_capa) {
        ogs_free(upf_functionality_data_1->op_confupf_capa);
        upf_functionality_data_1->op_confupf_capa = NULL;
    }
    ogs_free(upf_functionality_data_1);
}

cJSON *OpenAPI_upf_functionality_data_1_convertToJSON(OpenAPI_upf_functionality_data_1_t *upf_functionality_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upf_functionality_data_1 == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_1_convertToJSON() failed [UpfFunctionalityData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (upf_functionality_data_1->packet_insfunc != OpenAPI_upf_packet_inspection_functionality_NULL) {
    if (cJSON_AddStringToObject(item, "packetInsfunc", OpenAPI_upf_packet_inspection_functionality_ToString(upf_functionality_data_1->packet_insfunc)) == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_1_convertToJSON() failed [packet_insfunc]");
        goto end;
    }
    }

    if (upf_functionality_data_1->op_confupf_capa) {
    if (cJSON_AddStringToObject(item, "opConfupfCapa", upf_functionality_data_1->op_confupf_capa) == NULL) {
        ogs_error("OpenAPI_upf_functionality_data_1_convertToJSON() failed [op_confupf_capa]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_upf_functionality_data_1_t *OpenAPI_upf_functionality_data_1_parseFromJSON(cJSON *upf_functionality_data_1JSON)
{
    OpenAPI_upf_functionality_data_1_t *upf_functionality_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *packet_insfunc = NULL;
    OpenAPI_upf_packet_inspection_functionality_e packet_insfuncVariable = 0;
    cJSON *op_confupf_capa = NULL;
    packet_insfunc = cJSON_GetObjectItemCaseSensitive(upf_functionality_data_1JSON, "packetInsfunc");
    if (packet_insfunc) {
    if (!cJSON_IsString(packet_insfunc)) {
        ogs_error("OpenAPI_upf_functionality_data_1_parseFromJSON() failed [packet_insfunc]");
        goto end;
    }
    packet_insfuncVariable = OpenAPI_upf_packet_inspection_functionality_FromString(packet_insfunc->valuestring);
    }

    op_confupf_capa = cJSON_GetObjectItemCaseSensitive(upf_functionality_data_1JSON, "opConfupfCapa");
    if (op_confupf_capa) {
    if (!cJSON_IsString(op_confupf_capa) && !cJSON_IsNull(op_confupf_capa)) {
        ogs_error("OpenAPI_upf_functionality_data_1_parseFromJSON() failed [op_confupf_capa]");
        goto end;
    }
    }

    upf_functionality_data_1_local_var = OpenAPI_upf_functionality_data_1_create (
        packet_insfunc ? packet_insfuncVariable : 0,
        op_confupf_capa && !cJSON_IsNull(op_confupf_capa) ? ogs_strdup(op_confupf_capa->valuestring) : NULL
    );

    return upf_functionality_data_1_local_var;
end:
    return NULL;
}

OpenAPI_upf_functionality_data_1_t *OpenAPI_upf_functionality_data_1_copy(OpenAPI_upf_functionality_data_1_t *dst, OpenAPI_upf_functionality_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upf_functionality_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upf_functionality_data_1_convertToJSON() failed");
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

    OpenAPI_upf_functionality_data_1_free(dst);
    dst = OpenAPI_upf_functionality_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

