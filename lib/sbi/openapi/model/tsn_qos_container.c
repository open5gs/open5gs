
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsn_qos_container.h"

OpenAPI_tsn_qos_container_t *OpenAPI_tsn_qos_container_create(
    bool is_max_tsc_burst_size,
    int max_tsc_burst_size,
    bool is_tsc_pack_delay,
    int tsc_pack_delay,
    bool is_tsc_prio_level,
    int tsc_prio_level
)
{
    OpenAPI_tsn_qos_container_t *tsn_qos_container_local_var = ogs_malloc(sizeof(OpenAPI_tsn_qos_container_t));
    ogs_assert(tsn_qos_container_local_var);

    tsn_qos_container_local_var->is_max_tsc_burst_size = is_max_tsc_burst_size;
    tsn_qos_container_local_var->max_tsc_burst_size = max_tsc_burst_size;
    tsn_qos_container_local_var->is_tsc_pack_delay = is_tsc_pack_delay;
    tsn_qos_container_local_var->tsc_pack_delay = tsc_pack_delay;
    tsn_qos_container_local_var->is_tsc_prio_level = is_tsc_prio_level;
    tsn_qos_container_local_var->tsc_prio_level = tsc_prio_level;

    return tsn_qos_container_local_var;
}

void OpenAPI_tsn_qos_container_free(OpenAPI_tsn_qos_container_t *tsn_qos_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tsn_qos_container) {
        return;
    }
    ogs_free(tsn_qos_container);
}

cJSON *OpenAPI_tsn_qos_container_convertToJSON(OpenAPI_tsn_qos_container_t *tsn_qos_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tsn_qos_container == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_convertToJSON() failed [TsnQosContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsn_qos_container->is_max_tsc_burst_size) {
    if (cJSON_AddNumberToObject(item, "maxTscBurstSize", tsn_qos_container->max_tsc_burst_size) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_convertToJSON() failed [max_tsc_burst_size]");
        goto end;
    }
    }

    if (tsn_qos_container->is_tsc_pack_delay) {
    if (cJSON_AddNumberToObject(item, "tscPackDelay", tsn_qos_container->tsc_pack_delay) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_convertToJSON() failed [tsc_pack_delay]");
        goto end;
    }
    }

    if (tsn_qos_container->is_tsc_prio_level) {
    if (cJSON_AddNumberToObject(item, "tscPrioLevel", tsn_qos_container->tsc_prio_level) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_convertToJSON() failed [tsc_prio_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tsn_qos_container_t *OpenAPI_tsn_qos_container_parseFromJSON(cJSON *tsn_qos_containerJSON)
{
    OpenAPI_tsn_qos_container_t *tsn_qos_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_tsc_burst_size = NULL;
    cJSON *tsc_pack_delay = NULL;
    cJSON *tsc_prio_level = NULL;
    max_tsc_burst_size = cJSON_GetObjectItemCaseSensitive(tsn_qos_containerJSON, "maxTscBurstSize");
    if (max_tsc_burst_size) {
    if (!cJSON_IsNumber(max_tsc_burst_size)) {
        ogs_error("OpenAPI_tsn_qos_container_parseFromJSON() failed [max_tsc_burst_size]");
        goto end;
    }
    }

    tsc_pack_delay = cJSON_GetObjectItemCaseSensitive(tsn_qos_containerJSON, "tscPackDelay");
    if (tsc_pack_delay) {
    if (!cJSON_IsNumber(tsc_pack_delay)) {
        ogs_error("OpenAPI_tsn_qos_container_parseFromJSON() failed [tsc_pack_delay]");
        goto end;
    }
    }

    tsc_prio_level = cJSON_GetObjectItemCaseSensitive(tsn_qos_containerJSON, "tscPrioLevel");
    if (tsc_prio_level) {
    if (!cJSON_IsNumber(tsc_prio_level)) {
        ogs_error("OpenAPI_tsn_qos_container_parseFromJSON() failed [tsc_prio_level]");
        goto end;
    }
    }

    tsn_qos_container_local_var = OpenAPI_tsn_qos_container_create (
        max_tsc_burst_size ? true : false,
        max_tsc_burst_size ? max_tsc_burst_size->valuedouble : 0,
        tsc_pack_delay ? true : false,
        tsc_pack_delay ? tsc_pack_delay->valuedouble : 0,
        tsc_prio_level ? true : false,
        tsc_prio_level ? tsc_prio_level->valuedouble : 0
    );

    return tsn_qos_container_local_var;
end:
    return NULL;
}

OpenAPI_tsn_qos_container_t *OpenAPI_tsn_qos_container_copy(OpenAPI_tsn_qos_container_t *dst, OpenAPI_tsn_qos_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsn_qos_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsn_qos_container_convertToJSON() failed");
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

    OpenAPI_tsn_qos_container_free(dst);
    dst = OpenAPI_tsn_qos_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

