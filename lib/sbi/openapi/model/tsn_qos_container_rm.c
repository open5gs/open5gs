
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsn_qos_container_rm.h"

OpenAPI_tsn_qos_container_rm_t *OpenAPI_tsn_qos_container_rm_create(
    bool is_max_tsc_burst_size_null,
    bool is_max_tsc_burst_size,
    int max_tsc_burst_size,
    bool is_max_tsc_burst_size_small_null,
    bool is_max_tsc_burst_size_small,
    int max_tsc_burst_size_small,
    bool is_tsc_pack_delay_null,
    bool is_tsc_pack_delay,
    int tsc_pack_delay,
    bool is_max_per_null,
    char *max_per,
    bool is_tsc_prio_level_null,
    bool is_tsc_prio_level,
    int tsc_prio_level
)
{
    OpenAPI_tsn_qos_container_rm_t *tsn_qos_container_rm_local_var = ogs_malloc(sizeof(OpenAPI_tsn_qos_container_rm_t));
    ogs_assert(tsn_qos_container_rm_local_var);

    tsn_qos_container_rm_local_var->is_max_tsc_burst_size_null = is_max_tsc_burst_size_null;
    tsn_qos_container_rm_local_var->is_max_tsc_burst_size = is_max_tsc_burst_size;
    tsn_qos_container_rm_local_var->max_tsc_burst_size = max_tsc_burst_size;
    tsn_qos_container_rm_local_var->is_max_tsc_burst_size_small_null = is_max_tsc_burst_size_small_null;
    tsn_qos_container_rm_local_var->is_max_tsc_burst_size_small = is_max_tsc_burst_size_small;
    tsn_qos_container_rm_local_var->max_tsc_burst_size_small = max_tsc_burst_size_small;
    tsn_qos_container_rm_local_var->is_tsc_pack_delay_null = is_tsc_pack_delay_null;
    tsn_qos_container_rm_local_var->is_tsc_pack_delay = is_tsc_pack_delay;
    tsn_qos_container_rm_local_var->tsc_pack_delay = tsc_pack_delay;
    tsn_qos_container_rm_local_var->is_max_per_null = is_max_per_null;
    tsn_qos_container_rm_local_var->max_per = max_per;
    tsn_qos_container_rm_local_var->is_tsc_prio_level_null = is_tsc_prio_level_null;
    tsn_qos_container_rm_local_var->is_tsc_prio_level = is_tsc_prio_level;
    tsn_qos_container_rm_local_var->tsc_prio_level = tsc_prio_level;

    return tsn_qos_container_rm_local_var;
}

void OpenAPI_tsn_qos_container_rm_free(OpenAPI_tsn_qos_container_rm_t *tsn_qos_container_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tsn_qos_container_rm) {
        return;
    }
    if (tsn_qos_container_rm->max_per) {
        ogs_free(tsn_qos_container_rm->max_per);
        tsn_qos_container_rm->max_per = NULL;
    }
    ogs_free(tsn_qos_container_rm);
}

cJSON *OpenAPI_tsn_qos_container_rm_convertToJSON(OpenAPI_tsn_qos_container_rm_t *tsn_qos_container_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tsn_qos_container_rm == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [TsnQosContainerRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsn_qos_container_rm->is_max_tsc_burst_size) {
    if (cJSON_AddNumberToObject(item, "maxTscBurstSize", tsn_qos_container_rm->max_tsc_burst_size) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [max_tsc_burst_size]");
        goto end;
    }
    } else if (tsn_qos_container_rm->is_max_tsc_burst_size_null) {
        if (cJSON_AddNullToObject(item, "maxTscBurstSize") == NULL) {
            ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [max_tsc_burst_size]");
            goto end;
        }
    }

    if (tsn_qos_container_rm->is_max_tsc_burst_size_small) {
    if (cJSON_AddNumberToObject(item, "maxTscBurstSizeSmall", tsn_qos_container_rm->max_tsc_burst_size_small) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [max_tsc_burst_size_small]");
        goto end;
    }
    } else if (tsn_qos_container_rm->is_max_tsc_burst_size_small_null) {
        if (cJSON_AddNullToObject(item, "maxTscBurstSizeSmall") == NULL) {
            ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [max_tsc_burst_size_small]");
            goto end;
        }
    }

    if (tsn_qos_container_rm->is_tsc_pack_delay) {
    if (cJSON_AddNumberToObject(item, "tscPackDelay", tsn_qos_container_rm->tsc_pack_delay) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [tsc_pack_delay]");
        goto end;
    }
    } else if (tsn_qos_container_rm->is_tsc_pack_delay_null) {
        if (cJSON_AddNullToObject(item, "tscPackDelay") == NULL) {
            ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [tsc_pack_delay]");
            goto end;
        }
    }

    if (tsn_qos_container_rm->max_per) {
    if (cJSON_AddStringToObject(item, "maxPer", tsn_qos_container_rm->max_per) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [max_per]");
        goto end;
    }
    } else if (tsn_qos_container_rm->is_max_per_null) {
        if (cJSON_AddNullToObject(item, "maxPer") == NULL) {
            ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [max_per]");
            goto end;
        }
    }

    if (tsn_qos_container_rm->is_tsc_prio_level) {
    if (cJSON_AddNumberToObject(item, "tscPrioLevel", tsn_qos_container_rm->tsc_prio_level) == NULL) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [tsc_prio_level]");
        goto end;
    }
    } else if (tsn_qos_container_rm->is_tsc_prio_level_null) {
        if (cJSON_AddNullToObject(item, "tscPrioLevel") == NULL) {
            ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed [tsc_prio_level]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_tsn_qos_container_rm_t *OpenAPI_tsn_qos_container_rm_parseFromJSON(cJSON *tsn_qos_container_rmJSON)
{
    OpenAPI_tsn_qos_container_rm_t *tsn_qos_container_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *max_tsc_burst_size = NULL;
    cJSON *max_tsc_burst_size_small = NULL;
    cJSON *tsc_pack_delay = NULL;
    cJSON *max_per = NULL;
    cJSON *tsc_prio_level = NULL;
    max_tsc_burst_size = cJSON_GetObjectItemCaseSensitive(tsn_qos_container_rmJSON, "maxTscBurstSize");
    if (max_tsc_burst_size) {
    if (!cJSON_IsNull(max_tsc_burst_size)) {
    if (!cJSON_IsNumber(max_tsc_burst_size)) {
        ogs_error("OpenAPI_tsn_qos_container_rm_parseFromJSON() failed [max_tsc_burst_size]");
        goto end;
    }
    }
    }

    max_tsc_burst_size_small = cJSON_GetObjectItemCaseSensitive(tsn_qos_container_rmJSON, "maxTscBurstSizeSmall");
    if (max_tsc_burst_size_small) {
    if (!cJSON_IsNull(max_tsc_burst_size_small)) {
    if (!cJSON_IsNumber(max_tsc_burst_size_small)) {
        ogs_error("OpenAPI_tsn_qos_container_rm_parseFromJSON() failed [max_tsc_burst_size_small]");
        goto end;
    }
    }
    }

    tsc_pack_delay = cJSON_GetObjectItemCaseSensitive(tsn_qos_container_rmJSON, "tscPackDelay");
    if (tsc_pack_delay) {
    if (!cJSON_IsNull(tsc_pack_delay)) {
    if (!cJSON_IsNumber(tsc_pack_delay)) {
        ogs_error("OpenAPI_tsn_qos_container_rm_parseFromJSON() failed [tsc_pack_delay]");
        goto end;
    }
    }
    }

    max_per = cJSON_GetObjectItemCaseSensitive(tsn_qos_container_rmJSON, "maxPer");
    if (max_per) {
    if (!cJSON_IsNull(max_per)) {
    if (!cJSON_IsString(max_per) && !cJSON_IsNull(max_per)) {
        ogs_error("OpenAPI_tsn_qos_container_rm_parseFromJSON() failed [max_per]");
        goto end;
    }
    }
    }

    tsc_prio_level = cJSON_GetObjectItemCaseSensitive(tsn_qos_container_rmJSON, "tscPrioLevel");
    if (tsc_prio_level) {
    if (!cJSON_IsNull(tsc_prio_level)) {
    if (!cJSON_IsNumber(tsc_prio_level)) {
        ogs_error("OpenAPI_tsn_qos_container_rm_parseFromJSON() failed [tsc_prio_level]");
        goto end;
    }
    }
    }

    tsn_qos_container_rm_local_var = OpenAPI_tsn_qos_container_rm_create (
        max_tsc_burst_size && cJSON_IsNull(max_tsc_burst_size) ? true : false,
        max_tsc_burst_size ? true : false,
        max_tsc_burst_size ? max_tsc_burst_size->valuedouble : 0,
        max_tsc_burst_size_small && cJSON_IsNull(max_tsc_burst_size_small) ? true : false,
        max_tsc_burst_size_small ? true : false,
        max_tsc_burst_size_small ? max_tsc_burst_size_small->valuedouble : 0,
        tsc_pack_delay && cJSON_IsNull(tsc_pack_delay) ? true : false,
        tsc_pack_delay ? true : false,
        tsc_pack_delay ? tsc_pack_delay->valuedouble : 0,
        max_per && cJSON_IsNull(max_per) ? true : false,
        max_per && !cJSON_IsNull(max_per) ? ogs_strdup(max_per->valuestring) : NULL,
        tsc_prio_level && cJSON_IsNull(tsc_prio_level) ? true : false,
        tsc_prio_level ? true : false,
        tsc_prio_level ? tsc_prio_level->valuedouble : 0
    );

    return tsn_qos_container_rm_local_var;
end:
    return NULL;
}

OpenAPI_tsn_qos_container_rm_t *OpenAPI_tsn_qos_container_rm_copy(OpenAPI_tsn_qos_container_rm_t *dst, OpenAPI_tsn_qos_container_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsn_qos_container_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsn_qos_container_rm_convertToJSON() failed");
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

    OpenAPI_tsn_qos_container_rm_free(dst);
    dst = OpenAPI_tsn_qos_container_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

