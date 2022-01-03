
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsn_bridge_info.h"

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_create(
    bool is_bridge_id,
    int bridge_id,
    char *dstt_addr,
    bool is_dstt_port_num,
    int dstt_port_num,
    bool is_dstt_resid_time,
    int dstt_resid_time
)
{
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_var = ogs_malloc(sizeof(OpenAPI_tsn_bridge_info_t));
    ogs_assert(tsn_bridge_info_local_var);

    tsn_bridge_info_local_var->is_bridge_id = is_bridge_id;
    tsn_bridge_info_local_var->bridge_id = bridge_id;
    tsn_bridge_info_local_var->dstt_addr = dstt_addr;
    tsn_bridge_info_local_var->is_dstt_port_num = is_dstt_port_num;
    tsn_bridge_info_local_var->dstt_port_num = dstt_port_num;
    tsn_bridge_info_local_var->is_dstt_resid_time = is_dstt_resid_time;
    tsn_bridge_info_local_var->dstt_resid_time = dstt_resid_time;

    return tsn_bridge_info_local_var;
}

void OpenAPI_tsn_bridge_info_free(OpenAPI_tsn_bridge_info_t *tsn_bridge_info)
{
    if (NULL == tsn_bridge_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(tsn_bridge_info->dstt_addr);
    ogs_free(tsn_bridge_info);
}

cJSON *OpenAPI_tsn_bridge_info_convertToJSON(OpenAPI_tsn_bridge_info_t *tsn_bridge_info)
{
    cJSON *item = NULL;

    if (tsn_bridge_info == NULL) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [TsnBridgeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsn_bridge_info->is_bridge_id) {
    if (cJSON_AddNumberToObject(item, "bridgeId", tsn_bridge_info->bridge_id) == NULL) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [bridge_id]");
        goto end;
    }
    }

    if (tsn_bridge_info->dstt_addr) {
    if (cJSON_AddStringToObject(item, "dsttAddr", tsn_bridge_info->dstt_addr) == NULL) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [dstt_addr]");
        goto end;
    }
    }

    if (tsn_bridge_info->is_dstt_port_num) {
    if (cJSON_AddNumberToObject(item, "dsttPortNum", tsn_bridge_info->dstt_port_num) == NULL) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [dstt_port_num]");
        goto end;
    }
    }

    if (tsn_bridge_info->is_dstt_resid_time) {
    if (cJSON_AddNumberToObject(item, "dsttResidTime", tsn_bridge_info->dstt_resid_time) == NULL) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed [dstt_resid_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_parseFromJSON(cJSON *tsn_bridge_infoJSON)
{
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_var = NULL;
    cJSON *bridge_id = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "bridgeId");

    if (bridge_id) {
    if (!cJSON_IsNumber(bridge_id)) {
        ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [bridge_id]");
        goto end;
    }
    }

    cJSON *dstt_addr = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "dsttAddr");

    if (dstt_addr) {
    if (!cJSON_IsString(dstt_addr)) {
        ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [dstt_addr]");
        goto end;
    }
    }

    cJSON *dstt_port_num = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "dsttPortNum");

    if (dstt_port_num) {
    if (!cJSON_IsNumber(dstt_port_num)) {
        ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [dstt_port_num]");
        goto end;
    }
    }

    cJSON *dstt_resid_time = cJSON_GetObjectItemCaseSensitive(tsn_bridge_infoJSON, "dsttResidTime");

    if (dstt_resid_time) {
    if (!cJSON_IsNumber(dstt_resid_time)) {
        ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON() failed [dstt_resid_time]");
        goto end;
    }
    }

    tsn_bridge_info_local_var = OpenAPI_tsn_bridge_info_create (
        bridge_id ? true : false,
        bridge_id ? bridge_id->valuedouble : 0,
        dstt_addr ? ogs_strdup(dstt_addr->valuestring) : NULL,
        dstt_port_num ? true : false,
        dstt_port_num ? dstt_port_num->valuedouble : 0,
        dstt_resid_time ? true : false,
        dstt_resid_time ? dstt_resid_time->valuedouble : 0
    );

    return tsn_bridge_info_local_var;
end:
    return NULL;
}

OpenAPI_tsn_bridge_info_t *OpenAPI_tsn_bridge_info_copy(OpenAPI_tsn_bridge_info_t *dst, OpenAPI_tsn_bridge_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsn_bridge_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsn_bridge_info_convertToJSON() failed");
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

    OpenAPI_tsn_bridge_info_free(dst);
    dst = OpenAPI_tsn_bridge_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

