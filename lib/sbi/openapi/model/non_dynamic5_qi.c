
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non_dynamic5_qi.h"

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_create(
    int priority_level,
    int aver_window,
    int max_data_burst_vol,
    int ext_max_data_burst_vol
    )
{
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi_local_var = OpenAPI_malloc(sizeof(OpenAPI_non_dynamic5_qi_t));
    if (!non_dynamic5_qi_local_var) {
        return NULL;
    }
    non_dynamic5_qi_local_var->priority_level = priority_level;
    non_dynamic5_qi_local_var->aver_window = aver_window;
    non_dynamic5_qi_local_var->max_data_burst_vol = max_data_burst_vol;
    non_dynamic5_qi_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;

    return non_dynamic5_qi_local_var;
}

void OpenAPI_non_dynamic5_qi_free(OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi)
{
    if (NULL == non_dynamic5_qi) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(non_dynamic5_qi);
}

cJSON *OpenAPI_non_dynamic5_qi_convertToJSON(OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi)
{
    cJSON *item = NULL;

    if (non_dynamic5_qi == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [NonDynamic5Qi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (non_dynamic5_qi->priority_level) {
        if (cJSON_AddNumberToObject(item, "priorityLevel", non_dynamic5_qi->priority_level) == NULL) {
            ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [priority_level]");
            goto end;
        }
    }

    if (non_dynamic5_qi->aver_window) {
        if (cJSON_AddNumberToObject(item, "averWindow", non_dynamic5_qi->aver_window) == NULL) {
            ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [aver_window]");
            goto end;
        }
    }

    if (non_dynamic5_qi->max_data_burst_vol) {
        if (cJSON_AddNumberToObject(item, "maxDataBurstVol", non_dynamic5_qi->max_data_burst_vol) == NULL) {
            ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    if (non_dynamic5_qi->ext_max_data_burst_vol) {
        if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", non_dynamic5_qi->ext_max_data_burst_vol) == NULL) {
            ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_parseFromJSON(cJSON *non_dynamic5_qiJSON)
{
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi_local_var = NULL;
    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "priorityLevel");

    if (priority_level) {
        if (!cJSON_IsNumber(priority_level)) {
            ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [priority_level]");
            goto end;
        }
    }

    cJSON *aver_window = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "averWindow");

    if (aver_window) {
        if (!cJSON_IsNumber(aver_window)) {
            ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [aver_window]");
            goto end;
        }
    }

    cJSON *max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "maxDataBurstVol");

    if (max_data_burst_vol) {
        if (!cJSON_IsNumber(max_data_burst_vol)) {
            ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    cJSON *ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "extMaxDataBurstVol");

    if (ext_max_data_burst_vol) {
        if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
            ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

    non_dynamic5_qi_local_var = OpenAPI_non_dynamic5_qi_create (
        priority_level ? priority_level->valuedouble : 0,
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0
        );

    return non_dynamic5_qi_local_var;
end:
    return NULL;
}

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_copy(OpenAPI_non_dynamic5_qi_t *dst, OpenAPI_non_dynamic5_qi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non_dynamic5_qi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed");
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

    OpenAPI_non_dynamic5_qi_free(dst);
    dst = OpenAPI_non_dynamic5_qi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

