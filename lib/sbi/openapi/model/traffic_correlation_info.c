
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_correlation_info.h"

OpenAPI_traffic_correlation_info_t *OpenAPI_traffic_correlation_info_create(
    OpenAPI_correlation_type_e corr_type,
    char *tfc_corr_id,
    bool is_com_eas_ipv4_addr_null,
    char *com_eas_ipv4_addr,
    bool is_com_eas_ipv6_addr_null,
    char *com_eas_ipv6_addr,
    bool is_fqdn_range_null,
    OpenAPI_list_t *fqdn_range,
    bool is_notif_uri_null,
    char *notif_uri,
    bool is_notif_corr_id_null,
    char *notif_corr_id
)
{
    OpenAPI_traffic_correlation_info_t *traffic_correlation_info_local_var = ogs_malloc(sizeof(OpenAPI_traffic_correlation_info_t));
    ogs_assert(traffic_correlation_info_local_var);

    traffic_correlation_info_local_var->corr_type = corr_type;
    traffic_correlation_info_local_var->tfc_corr_id = tfc_corr_id;
    traffic_correlation_info_local_var->is_com_eas_ipv4_addr_null = is_com_eas_ipv4_addr_null;
    traffic_correlation_info_local_var->com_eas_ipv4_addr = com_eas_ipv4_addr;
    traffic_correlation_info_local_var->is_com_eas_ipv6_addr_null = is_com_eas_ipv6_addr_null;
    traffic_correlation_info_local_var->com_eas_ipv6_addr = com_eas_ipv6_addr;
    traffic_correlation_info_local_var->is_fqdn_range_null = is_fqdn_range_null;
    traffic_correlation_info_local_var->fqdn_range = fqdn_range;
    traffic_correlation_info_local_var->is_notif_uri_null = is_notif_uri_null;
    traffic_correlation_info_local_var->notif_uri = notif_uri;
    traffic_correlation_info_local_var->is_notif_corr_id_null = is_notif_corr_id_null;
    traffic_correlation_info_local_var->notif_corr_id = notif_corr_id;

    return traffic_correlation_info_local_var;
}

void OpenAPI_traffic_correlation_info_free(OpenAPI_traffic_correlation_info_t *traffic_correlation_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_correlation_info) {
        return;
    }
    if (traffic_correlation_info->tfc_corr_id) {
        ogs_free(traffic_correlation_info->tfc_corr_id);
        traffic_correlation_info->tfc_corr_id = NULL;
    }
    if (traffic_correlation_info->com_eas_ipv4_addr) {
        ogs_free(traffic_correlation_info->com_eas_ipv4_addr);
        traffic_correlation_info->com_eas_ipv4_addr = NULL;
    }
    if (traffic_correlation_info->com_eas_ipv6_addr) {
        ogs_free(traffic_correlation_info->com_eas_ipv6_addr);
        traffic_correlation_info->com_eas_ipv6_addr = NULL;
    }
    if (traffic_correlation_info->fqdn_range) {
        OpenAPI_list_for_each(traffic_correlation_info->fqdn_range, node) {
            OpenAPI_fqdn_pattern_matching_rule_free(node->data);
        }
        OpenAPI_list_free(traffic_correlation_info->fqdn_range);
        traffic_correlation_info->fqdn_range = NULL;
    }
    if (traffic_correlation_info->notif_uri) {
        ogs_free(traffic_correlation_info->notif_uri);
        traffic_correlation_info->notif_uri = NULL;
    }
    if (traffic_correlation_info->notif_corr_id) {
        ogs_free(traffic_correlation_info->notif_corr_id);
        traffic_correlation_info->notif_corr_id = NULL;
    }
    ogs_free(traffic_correlation_info);
}

cJSON *OpenAPI_traffic_correlation_info_convertToJSON(OpenAPI_traffic_correlation_info_t *traffic_correlation_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_correlation_info == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [TrafficCorrelationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_correlation_info->corr_type != OpenAPI_correlation_type_NULL) {
    if (cJSON_AddStringToObject(item, "corrType", OpenAPI_correlation_type_ToString(traffic_correlation_info->corr_type)) == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [corr_type]");
        goto end;
    }
    }

    if (traffic_correlation_info->tfc_corr_id) {
    if (cJSON_AddStringToObject(item, "tfcCorrId", traffic_correlation_info->tfc_corr_id) == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [tfc_corr_id]");
        goto end;
    }
    }

    if (traffic_correlation_info->com_eas_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "comEasIpv4Addr", traffic_correlation_info->com_eas_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [com_eas_ipv4_addr]");
        goto end;
    }
    } else if (traffic_correlation_info->is_com_eas_ipv4_addr_null) {
        if (cJSON_AddNullToObject(item, "comEasIpv4Addr") == NULL) {
            ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [com_eas_ipv4_addr]");
            goto end;
        }
    }

    if (traffic_correlation_info->com_eas_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "comEasIpv6Addr", traffic_correlation_info->com_eas_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [com_eas_ipv6_addr]");
        goto end;
    }
    } else if (traffic_correlation_info->is_com_eas_ipv6_addr_null) {
        if (cJSON_AddNullToObject(item, "comEasIpv6Addr") == NULL) {
            ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [com_eas_ipv6_addr]");
            goto end;
        }
    }

    if (traffic_correlation_info->fqdn_range) {
    cJSON *fqdn_rangeList = cJSON_AddArrayToObject(item, "fqdnRange");
    if (fqdn_rangeList == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [fqdn_range]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_correlation_info->fqdn_range, node) {
        cJSON *itemLocal = OpenAPI_fqdn_pattern_matching_rule_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [fqdn_range]");
            goto end;
        }
        cJSON_AddItemToArray(fqdn_rangeList, itemLocal);
    }
    } else if (traffic_correlation_info->is_fqdn_range_null) {
        if (cJSON_AddNullToObject(item, "fqdnRange") == NULL) {
            ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [fqdn_range]");
            goto end;
        }
    }

    if (traffic_correlation_info->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", traffic_correlation_info->notif_uri) == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [notif_uri]");
        goto end;
    }
    } else if (traffic_correlation_info->is_notif_uri_null) {
        if (cJSON_AddNullToObject(item, "notifUri") == NULL) {
            ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [notif_uri]");
            goto end;
        }
    }

    if (traffic_correlation_info->notif_corr_id) {
    if (cJSON_AddStringToObject(item, "notifCorrId", traffic_correlation_info->notif_corr_id) == NULL) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [notif_corr_id]");
        goto end;
    }
    } else if (traffic_correlation_info->is_notif_corr_id_null) {
        if (cJSON_AddNullToObject(item, "notifCorrId") == NULL) {
            ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed [notif_corr_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_traffic_correlation_info_t *OpenAPI_traffic_correlation_info_parseFromJSON(cJSON *traffic_correlation_infoJSON)
{
    OpenAPI_traffic_correlation_info_t *traffic_correlation_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *corr_type = NULL;
    OpenAPI_correlation_type_e corr_typeVariable = 0;
    cJSON *tfc_corr_id = NULL;
    cJSON *com_eas_ipv4_addr = NULL;
    cJSON *com_eas_ipv6_addr = NULL;
    cJSON *fqdn_range = NULL;
    OpenAPI_list_t *fqdn_rangeList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *notif_corr_id = NULL;
    corr_type = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "corrType");
    if (corr_type) {
    if (!cJSON_IsString(corr_type)) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [corr_type]");
        goto end;
    }
    corr_typeVariable = OpenAPI_correlation_type_FromString(corr_type->valuestring);
    }

    tfc_corr_id = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "tfcCorrId");
    if (tfc_corr_id) {
    if (!cJSON_IsString(tfc_corr_id) && !cJSON_IsNull(tfc_corr_id)) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [tfc_corr_id]");
        goto end;
    }
    }

    com_eas_ipv4_addr = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "comEasIpv4Addr");
    if (com_eas_ipv4_addr) {
    if (!cJSON_IsNull(com_eas_ipv4_addr)) {
    if (!cJSON_IsString(com_eas_ipv4_addr) && !cJSON_IsNull(com_eas_ipv4_addr)) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [com_eas_ipv4_addr]");
        goto end;
    }
    }
    }

    com_eas_ipv6_addr = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "comEasIpv6Addr");
    if (com_eas_ipv6_addr) {
    if (!cJSON_IsNull(com_eas_ipv6_addr)) {
    if (!cJSON_IsString(com_eas_ipv6_addr) && !cJSON_IsNull(com_eas_ipv6_addr)) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [com_eas_ipv6_addr]");
        goto end;
    }
    }
    }

    fqdn_range = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "fqdnRange");
    if (fqdn_range) {
    if (!cJSON_IsNull(fqdn_range)) {
        cJSON *fqdn_range_local = NULL;
        if (!cJSON_IsArray(fqdn_range)) {
            ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [fqdn_range]");
            goto end;
        }

        fqdn_rangeList = OpenAPI_list_create();

        cJSON_ArrayForEach(fqdn_range_local, fqdn_range) {
            if (!cJSON_IsObject(fqdn_range_local)) {
                ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [fqdn_range]");
                goto end;
            }
            OpenAPI_fqdn_pattern_matching_rule_t *fqdn_rangeItem = OpenAPI_fqdn_pattern_matching_rule_parseFromJSON(fqdn_range_local);
            if (!fqdn_rangeItem) {
                ogs_error("No fqdn_rangeItem");
                goto end;
            }
            OpenAPI_list_add(fqdn_rangeList, fqdn_rangeItem);
        }
    }
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsNull(notif_uri)) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }
    }

    notif_corr_id = cJSON_GetObjectItemCaseSensitive(traffic_correlation_infoJSON, "notifCorrId");
    if (notif_corr_id) {
    if (!cJSON_IsNull(notif_corr_id)) {
    if (!cJSON_IsString(notif_corr_id) && !cJSON_IsNull(notif_corr_id)) {
        ogs_error("OpenAPI_traffic_correlation_info_parseFromJSON() failed [notif_corr_id]");
        goto end;
    }
    }
    }

    traffic_correlation_info_local_var = OpenAPI_traffic_correlation_info_create (
        corr_type ? corr_typeVariable : 0,
        tfc_corr_id && !cJSON_IsNull(tfc_corr_id) ? ogs_strdup(tfc_corr_id->valuestring) : NULL,
        com_eas_ipv4_addr && cJSON_IsNull(com_eas_ipv4_addr) ? true : false,
        com_eas_ipv4_addr && !cJSON_IsNull(com_eas_ipv4_addr) ? ogs_strdup(com_eas_ipv4_addr->valuestring) : NULL,
        com_eas_ipv6_addr && cJSON_IsNull(com_eas_ipv6_addr) ? true : false,
        com_eas_ipv6_addr && !cJSON_IsNull(com_eas_ipv6_addr) ? ogs_strdup(com_eas_ipv6_addr->valuestring) : NULL,
        fqdn_range && cJSON_IsNull(fqdn_range) ? true : false,
        fqdn_range ? fqdn_rangeList : NULL,
        notif_uri && cJSON_IsNull(notif_uri) ? true : false,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        notif_corr_id && cJSON_IsNull(notif_corr_id) ? true : false,
        notif_corr_id && !cJSON_IsNull(notif_corr_id) ? ogs_strdup(notif_corr_id->valuestring) : NULL
    );

    return traffic_correlation_info_local_var;
end:
    if (fqdn_rangeList) {
        OpenAPI_list_for_each(fqdn_rangeList, node) {
            OpenAPI_fqdn_pattern_matching_rule_free(node->data);
        }
        OpenAPI_list_free(fqdn_rangeList);
        fqdn_rangeList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_correlation_info_t *OpenAPI_traffic_correlation_info_copy(OpenAPI_traffic_correlation_info_t *dst, OpenAPI_traffic_correlation_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_correlation_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_correlation_info_convertToJSON() failed");
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

    OpenAPI_traffic_correlation_info_free(dst);
    dst = OpenAPI_traffic_correlation_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

