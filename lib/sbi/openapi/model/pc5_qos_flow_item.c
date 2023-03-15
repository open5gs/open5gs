
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc5_qos_flow_item.h"

OpenAPI_pc5_qos_flow_item_t *OpenAPI_pc5_qos_flow_item_create(
    int pqi,
    OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates,
    bool is_range,
    int range
)
{
    OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_item_local_var = ogs_malloc(sizeof(OpenAPI_pc5_qos_flow_item_t));
    ogs_assert(pc5_qos_flow_item_local_var);

    pc5_qos_flow_item_local_var->pqi = pqi;
    pc5_qos_flow_item_local_var->pc5_flow_bit_rates = pc5_flow_bit_rates;
    pc5_qos_flow_item_local_var->is_range = is_range;
    pc5_qos_flow_item_local_var->range = range;

    return pc5_qos_flow_item_local_var;
}

void OpenAPI_pc5_qos_flow_item_free(OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pc5_qos_flow_item) {
        return;
    }
    if (pc5_qos_flow_item->pc5_flow_bit_rates) {
        OpenAPI_pc5_flow_bit_rates_free(pc5_qos_flow_item->pc5_flow_bit_rates);
        pc5_qos_flow_item->pc5_flow_bit_rates = NULL;
    }
    ogs_free(pc5_qos_flow_item);
}

cJSON *OpenAPI_pc5_qos_flow_item_convertToJSON(OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pc5_qos_flow_item == NULL) {
        ogs_error("OpenAPI_pc5_qos_flow_item_convertToJSON() failed [Pc5QosFlowItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pqi", pc5_qos_flow_item->pqi) == NULL) {
        ogs_error("OpenAPI_pc5_qos_flow_item_convertToJSON() failed [pqi]");
        goto end;
    }

    if (pc5_qos_flow_item->pc5_flow_bit_rates) {
    cJSON *pc5_flow_bit_rates_local_JSON = OpenAPI_pc5_flow_bit_rates_convertToJSON(pc5_qos_flow_item->pc5_flow_bit_rates);
    if (pc5_flow_bit_rates_local_JSON == NULL) {
        ogs_error("OpenAPI_pc5_qos_flow_item_convertToJSON() failed [pc5_flow_bit_rates]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pc5FlowBitRates", pc5_flow_bit_rates_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pc5_qos_flow_item_convertToJSON() failed [pc5_flow_bit_rates]");
        goto end;
    }
    }

    if (pc5_qos_flow_item->is_range) {
    if (cJSON_AddNumberToObject(item, "range", pc5_qos_flow_item->range) == NULL) {
        ogs_error("OpenAPI_pc5_qos_flow_item_convertToJSON() failed [range]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pc5_qos_flow_item_t *OpenAPI_pc5_qos_flow_item_parseFromJSON(cJSON *pc5_qos_flow_itemJSON)
{
    OpenAPI_pc5_qos_flow_item_t *pc5_qos_flow_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pqi = NULL;
    cJSON *pc5_flow_bit_rates = NULL;
    OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates_local_nonprim = NULL;
    cJSON *range = NULL;
    pqi = cJSON_GetObjectItemCaseSensitive(pc5_qos_flow_itemJSON, "pqi");
    if (!pqi) {
        ogs_error("OpenAPI_pc5_qos_flow_item_parseFromJSON() failed [pqi]");
        goto end;
    }
    if (!cJSON_IsNumber(pqi)) {
        ogs_error("OpenAPI_pc5_qos_flow_item_parseFromJSON() failed [pqi]");
        goto end;
    }

    pc5_flow_bit_rates = cJSON_GetObjectItemCaseSensitive(pc5_qos_flow_itemJSON, "pc5FlowBitRates");
    if (pc5_flow_bit_rates) {
    pc5_flow_bit_rates_local_nonprim = OpenAPI_pc5_flow_bit_rates_parseFromJSON(pc5_flow_bit_rates);
    if (!pc5_flow_bit_rates_local_nonprim) {
        ogs_error("OpenAPI_pc5_flow_bit_rates_parseFromJSON failed [pc5_flow_bit_rates]");
        goto end;
    }
    }

    range = cJSON_GetObjectItemCaseSensitive(pc5_qos_flow_itemJSON, "range");
    if (range) {
    if (!cJSON_IsNumber(range)) {
        ogs_error("OpenAPI_pc5_qos_flow_item_parseFromJSON() failed [range]");
        goto end;
    }
    }

    pc5_qos_flow_item_local_var = OpenAPI_pc5_qos_flow_item_create (
        
        pqi->valuedouble,
        pc5_flow_bit_rates ? pc5_flow_bit_rates_local_nonprim : NULL,
        range ? true : false,
        range ? range->valuedouble : 0
    );

    return pc5_qos_flow_item_local_var;
end:
    if (pc5_flow_bit_rates_local_nonprim) {
        OpenAPI_pc5_flow_bit_rates_free(pc5_flow_bit_rates_local_nonprim);
        pc5_flow_bit_rates_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pc5_qos_flow_item_t *OpenAPI_pc5_qos_flow_item_copy(OpenAPI_pc5_qos_flow_item_t *dst, OpenAPI_pc5_qos_flow_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pc5_qos_flow_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pc5_qos_flow_item_convertToJSON() failed");
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

    OpenAPI_pc5_qos_flow_item_free(dst);
    dst = OpenAPI_pc5_qos_flow_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

