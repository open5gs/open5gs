
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "small_data_rate_status_info.h"

OpenAPI_small_data_rate_status_info_t *OpenAPI_small_data_rate_status_info_create(
    OpenAPI_snssai_t *snssai,
    char *dnn,
    OpenAPI_small_data_rate_status_t *small_data_rate_status
)
{
    OpenAPI_small_data_rate_status_info_t *small_data_rate_status_info_local_var = ogs_malloc(sizeof(OpenAPI_small_data_rate_status_info_t));
    ogs_assert(small_data_rate_status_info_local_var);

    small_data_rate_status_info_local_var->snssai = snssai;
    small_data_rate_status_info_local_var->dnn = dnn;
    small_data_rate_status_info_local_var->small_data_rate_status = small_data_rate_status;

    return small_data_rate_status_info_local_var;
}

void OpenAPI_small_data_rate_status_info_free(OpenAPI_small_data_rate_status_info_t *small_data_rate_status_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == small_data_rate_status_info) {
        return;
    }
    if (small_data_rate_status_info->snssai) {
        OpenAPI_snssai_free(small_data_rate_status_info->snssai);
        small_data_rate_status_info->snssai = NULL;
    }
    if (small_data_rate_status_info->dnn) {
        ogs_free(small_data_rate_status_info->dnn);
        small_data_rate_status_info->dnn = NULL;
    }
    if (small_data_rate_status_info->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_info->small_data_rate_status);
        small_data_rate_status_info->small_data_rate_status = NULL;
    }
    ogs_free(small_data_rate_status_info);
}

cJSON *OpenAPI_small_data_rate_status_info_convertToJSON(OpenAPI_small_data_rate_status_info_t *small_data_rate_status_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (small_data_rate_status_info == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [SmallDataRateStatusInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!small_data_rate_status_info->snssai) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(small_data_rate_status_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "Snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!small_data_rate_status_info->dnn) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "Dnn", small_data_rate_status_info->dnn) == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!small_data_rate_status_info->small_data_rate_status) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [small_data_rate_status]");
        return NULL;
    }
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(small_data_rate_status_info->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "SmallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }

end:
    return item;
}

OpenAPI_small_data_rate_status_info_t *OpenAPI_small_data_rate_status_info_parseFromJSON(cJSON *small_data_rate_status_infoJSON)
{
    OpenAPI_small_data_rate_status_info_t *small_data_rate_status_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(small_data_rate_status_infoJSON, "Snssai");
    if (!snssai) {
        ogs_error("OpenAPI_small_data_rate_status_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(small_data_rate_status_infoJSON, "Dnn");
    if (!dnn) {
        ogs_error("OpenAPI_small_data_rate_status_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_small_data_rate_status_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(small_data_rate_status_infoJSON, "SmallDataRateStatus");
    if (!small_data_rate_status) {
        ogs_error("OpenAPI_small_data_rate_status_info_parseFromJSON() failed [small_data_rate_status]");
        goto end;
    }
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }

    small_data_rate_status_info_local_var = OpenAPI_small_data_rate_status_info_create (
        snssai_local_nonprim,
        ogs_strdup(dnn->valuestring),
        small_data_rate_status_local_nonprim
    );

    return small_data_rate_status_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_small_data_rate_status_info_t *OpenAPI_small_data_rate_status_info_copy(OpenAPI_small_data_rate_status_info_t *dst, OpenAPI_small_data_rate_status_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_small_data_rate_status_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_small_data_rate_status_info_convertToJSON() failed");
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

    OpenAPI_small_data_rate_status_info_free(dst);
    dst = OpenAPI_small_data_rate_status_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

