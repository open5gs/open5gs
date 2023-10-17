
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "user_data_congestion_info.h"

OpenAPI_user_data_congestion_info_t *OpenAPI_user_data_congestion_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_congestion_info_t *congestion_info,
    OpenAPI_snssai_t *snssai
)
{
    OpenAPI_user_data_congestion_info_t *user_data_congestion_info_local_var = ogs_malloc(sizeof(OpenAPI_user_data_congestion_info_t));
    ogs_assert(user_data_congestion_info_local_var);

    user_data_congestion_info_local_var->network_area = network_area;
    user_data_congestion_info_local_var->congestion_info = congestion_info;
    user_data_congestion_info_local_var->snssai = snssai;

    return user_data_congestion_info_local_var;
}

void OpenAPI_user_data_congestion_info_free(OpenAPI_user_data_congestion_info_t *user_data_congestion_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == user_data_congestion_info) {
        return;
    }
    if (user_data_congestion_info->network_area) {
        OpenAPI_network_area_info_free(user_data_congestion_info->network_area);
        user_data_congestion_info->network_area = NULL;
    }
    if (user_data_congestion_info->congestion_info) {
        OpenAPI_congestion_info_free(user_data_congestion_info->congestion_info);
        user_data_congestion_info->congestion_info = NULL;
    }
    if (user_data_congestion_info->snssai) {
        OpenAPI_snssai_free(user_data_congestion_info->snssai);
        user_data_congestion_info->snssai = NULL;
    }
    ogs_free(user_data_congestion_info);
}

cJSON *OpenAPI_user_data_congestion_info_convertToJSON(OpenAPI_user_data_congestion_info_t *user_data_congestion_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (user_data_congestion_info == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [UserDataCongestionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!user_data_congestion_info->network_area) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [network_area]");
        return NULL;
    }
    cJSON *network_area_local_JSON = OpenAPI_network_area_info_convertToJSON(user_data_congestion_info->network_area);
    if (network_area_local_JSON == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [network_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkArea", network_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [network_area]");
        goto end;
    }

    if (!user_data_congestion_info->congestion_info) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [congestion_info]");
        return NULL;
    }
    cJSON *congestion_info_local_JSON = OpenAPI_congestion_info_convertToJSON(user_data_congestion_info->congestion_info);
    if (congestion_info_local_JSON == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [congestion_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "congestionInfo", congestion_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [congestion_info]");
        goto end;
    }

    if (user_data_congestion_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(user_data_congestion_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_user_data_congestion_info_t *OpenAPI_user_data_congestion_info_parseFromJSON(cJSON *user_data_congestion_infoJSON)
{
    OpenAPI_user_data_congestion_info_t *user_data_congestion_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *network_area = NULL;
    OpenAPI_network_area_info_t *network_area_local_nonprim = NULL;
    cJSON *congestion_info = NULL;
    OpenAPI_congestion_info_t *congestion_info_local_nonprim = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    network_area = cJSON_GetObjectItemCaseSensitive(user_data_congestion_infoJSON, "networkArea");
    if (!network_area) {
        ogs_error("OpenAPI_user_data_congestion_info_parseFromJSON() failed [network_area]");
        goto end;
    }
    network_area_local_nonprim = OpenAPI_network_area_info_parseFromJSON(network_area);
    if (!network_area_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [network_area]");
        goto end;
    }

    congestion_info = cJSON_GetObjectItemCaseSensitive(user_data_congestion_infoJSON, "congestionInfo");
    if (!congestion_info) {
        ogs_error("OpenAPI_user_data_congestion_info_parseFromJSON() failed [congestion_info]");
        goto end;
    }
    congestion_info_local_nonprim = OpenAPI_congestion_info_parseFromJSON(congestion_info);
    if (!congestion_info_local_nonprim) {
        ogs_error("OpenAPI_congestion_info_parseFromJSON failed [congestion_info]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(user_data_congestion_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    user_data_congestion_info_local_var = OpenAPI_user_data_congestion_info_create (
        network_area_local_nonprim,
        congestion_info_local_nonprim,
        snssai ? snssai_local_nonprim : NULL
    );

    return user_data_congestion_info_local_var;
end:
    if (network_area_local_nonprim) {
        OpenAPI_network_area_info_free(network_area_local_nonprim);
        network_area_local_nonprim = NULL;
    }
    if (congestion_info_local_nonprim) {
        OpenAPI_congestion_info_free(congestion_info_local_nonprim);
        congestion_info_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_user_data_congestion_info_t *OpenAPI_user_data_congestion_info_copy(OpenAPI_user_data_congestion_info_t *dst, OpenAPI_user_data_congestion_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_user_data_congestion_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_user_data_congestion_info_convertToJSON() failed");
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

    OpenAPI_user_data_congestion_info_free(dst);
    dst = OpenAPI_user_data_congestion_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

