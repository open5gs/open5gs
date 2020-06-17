
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc5_flow_bit_rates.h"

OpenAPI_pc5_flow_bit_rates_t *OpenAPI_pc5_flow_bit_rates_create(
    char *gua_fbr,
    char *max_fbr
    )
{
    OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates_local_var = OpenAPI_malloc(sizeof(OpenAPI_pc5_flow_bit_rates_t));
    if (!pc5_flow_bit_rates_local_var) {
        return NULL;
    }
    pc5_flow_bit_rates_local_var->gua_fbr = gua_fbr;
    pc5_flow_bit_rates_local_var->max_fbr = max_fbr;

    return pc5_flow_bit_rates_local_var;
}

void OpenAPI_pc5_flow_bit_rates_free(OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates)
{
    if (NULL == pc5_flow_bit_rates) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pc5_flow_bit_rates->gua_fbr);
    ogs_free(pc5_flow_bit_rates->max_fbr);
    ogs_free(pc5_flow_bit_rates);
}

cJSON *OpenAPI_pc5_flow_bit_rates_convertToJSON(OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates)
{
    cJSON *item = NULL;

    if (pc5_flow_bit_rates == NULL) {
        ogs_error("OpenAPI_pc5_flow_bit_rates_convertToJSON() failed [Pc5FlowBitRates]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pc5_flow_bit_rates->gua_fbr) {
        if (cJSON_AddStringToObject(item, "guaFbr", pc5_flow_bit_rates->gua_fbr) == NULL) {
            ogs_error("OpenAPI_pc5_flow_bit_rates_convertToJSON() failed [gua_fbr]");
            goto end;
        }
    }

    if (pc5_flow_bit_rates->max_fbr) {
        if (cJSON_AddStringToObject(item, "maxFbr", pc5_flow_bit_rates->max_fbr) == NULL) {
            ogs_error("OpenAPI_pc5_flow_bit_rates_convertToJSON() failed [max_fbr]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pc5_flow_bit_rates_t *OpenAPI_pc5_flow_bit_rates_parseFromJSON(cJSON *pc5_flow_bit_ratesJSON)
{
    OpenAPI_pc5_flow_bit_rates_t *pc5_flow_bit_rates_local_var = NULL;
    cJSON *gua_fbr = cJSON_GetObjectItemCaseSensitive(pc5_flow_bit_ratesJSON, "guaFbr");

    if (gua_fbr) {
        if (!cJSON_IsString(gua_fbr)) {
            ogs_error("OpenAPI_pc5_flow_bit_rates_parseFromJSON() failed [gua_fbr]");
            goto end;
        }
    }

    cJSON *max_fbr = cJSON_GetObjectItemCaseSensitive(pc5_flow_bit_ratesJSON, "maxFbr");

    if (max_fbr) {
        if (!cJSON_IsString(max_fbr)) {
            ogs_error("OpenAPI_pc5_flow_bit_rates_parseFromJSON() failed [max_fbr]");
            goto end;
        }
    }

    pc5_flow_bit_rates_local_var = OpenAPI_pc5_flow_bit_rates_create (
        gua_fbr ? ogs_strdup(gua_fbr->valuestring) : NULL,
        max_fbr ? ogs_strdup(max_fbr->valuestring) : NULL
        );

    return pc5_flow_bit_rates_local_var;
end:
    return NULL;
}

OpenAPI_pc5_flow_bit_rates_t *OpenAPI_pc5_flow_bit_rates_copy(OpenAPI_pc5_flow_bit_rates_t *dst, OpenAPI_pc5_flow_bit_rates_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pc5_flow_bit_rates_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pc5_flow_bit_rates_convertToJSON() failed");
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

    OpenAPI_pc5_flow_bit_rates_free(dst);
    dst = OpenAPI_pc5_flow_bit_rates_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

