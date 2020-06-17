
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authentication_info.h"

OpenAPI_authentication_info_t *OpenAPI_authentication_info_create(
    char *supi_or_suci,
    char *serving_network_name,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *pei,
    OpenAPI_trace_data_t *trace_data,
    char *udm_group_id,
    char *routing_indicator,
    char *cag_id
    )
{
    OpenAPI_authentication_info_t *authentication_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_authentication_info_t));
    if (!authentication_info_local_var) {
        return NULL;
    }
    authentication_info_local_var->supi_or_suci = supi_or_suci;
    authentication_info_local_var->serving_network_name = serving_network_name;
    authentication_info_local_var->resynchronization_info = resynchronization_info;
    authentication_info_local_var->pei = pei;
    authentication_info_local_var->trace_data = trace_data;
    authentication_info_local_var->udm_group_id = udm_group_id;
    authentication_info_local_var->routing_indicator = routing_indicator;
    authentication_info_local_var->cag_id = cag_id;

    return authentication_info_local_var;
}

void OpenAPI_authentication_info_free(OpenAPI_authentication_info_t *authentication_info)
{
    if (NULL == authentication_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(authentication_info->supi_or_suci);
    ogs_free(authentication_info->serving_network_name);
    OpenAPI_resynchronization_info_free(authentication_info->resynchronization_info);
    ogs_free(authentication_info->pei);
    OpenAPI_trace_data_free(authentication_info->trace_data);
    ogs_free(authentication_info->udm_group_id);
    ogs_free(authentication_info->routing_indicator);
    ogs_free(authentication_info->cag_id);
    ogs_free(authentication_info);
}

cJSON *OpenAPI_authentication_info_convertToJSON(OpenAPI_authentication_info_t *authentication_info)
{
    cJSON *item = NULL;

    if (authentication_info == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [AuthenticationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!authentication_info->supi_or_suci) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [supi_or_suci]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "supiOrSuci", authentication_info->supi_or_suci) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [supi_or_suci]");
        goto end;
    }

    if (!authentication_info->serving_network_name) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [serving_network_name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "servingNetworkName", authentication_info->serving_network_name) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [serving_network_name]");
        goto end;
    }

    if (authentication_info->resynchronization_info) {
        cJSON *resynchronization_info_local_JSON = OpenAPI_resynchronization_info_convertToJSON(authentication_info->resynchronization_info);
        if (resynchronization_info_local_JSON == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [resynchronization_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "resynchronizationInfo", resynchronization_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [resynchronization_info]");
            goto end;
        }
    }

    if (authentication_info->pei) {
        if (cJSON_AddStringToObject(item, "pei", authentication_info->pei) == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [pei]");
            goto end;
        }
    }

    if (authentication_info->trace_data) {
        cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(authentication_info->trace_data);
        if (trace_data_local_JSON == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [trace_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (authentication_info->udm_group_id) {
        if (cJSON_AddStringToObject(item, "udmGroupId", authentication_info->udm_group_id) == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [udm_group_id]");
            goto end;
        }
    }

    if (authentication_info->routing_indicator) {
        if (cJSON_AddStringToObject(item, "routingIndicator", authentication_info->routing_indicator) == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [routing_indicator]");
            goto end;
        }
    }

    if (authentication_info->cag_id) {
        if (cJSON_AddStringToObject(item, "cagId", authentication_info->cag_id) == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [cag_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_authentication_info_t *OpenAPI_authentication_info_parseFromJSON(cJSON *authentication_infoJSON)
{
    OpenAPI_authentication_info_t *authentication_info_local_var = NULL;
    cJSON *supi_or_suci = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "supiOrSuci");
    if (!supi_or_suci) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [supi_or_suci]");
        goto end;
    }


    if (!cJSON_IsString(supi_or_suci)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [supi_or_suci]");
        goto end;
    }

    cJSON *serving_network_name = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "servingNetworkName");
    if (!serving_network_name) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [serving_network_name]");
        goto end;
    }


    if (!cJSON_IsString(serving_network_name)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [serving_network_name]");
        goto end;
    }

    cJSON *resynchronization_info = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "resynchronizationInfo");

    OpenAPI_resynchronization_info_t *resynchronization_info_local_nonprim = NULL;
    if (resynchronization_info) {
        resynchronization_info_local_nonprim = OpenAPI_resynchronization_info_parseFromJSON(resynchronization_info);
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *trace_data = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "traceData");

    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    if (trace_data) {
        trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    cJSON *udm_group_id = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "udmGroupId");

    if (udm_group_id) {
        if (!cJSON_IsString(udm_group_id)) {
            ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [udm_group_id]");
            goto end;
        }
    }

    cJSON *routing_indicator = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "routingIndicator");

    if (routing_indicator) {
        if (!cJSON_IsString(routing_indicator)) {
            ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [routing_indicator]");
            goto end;
        }
    }

    cJSON *cag_id = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "cagId");

    if (cag_id) {
        if (!cJSON_IsString(cag_id)) {
            ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [cag_id]");
            goto end;
        }
    }

    authentication_info_local_var = OpenAPI_authentication_info_create (
        ogs_strdup(supi_or_suci->valuestring),
        ogs_strdup(serving_network_name->valuestring),
        resynchronization_info ? resynchronization_info_local_nonprim : NULL,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        trace_data ? trace_data_local_nonprim : NULL,
        udm_group_id ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator ? ogs_strdup(routing_indicator->valuestring) : NULL,
        cag_id ? ogs_strdup(cag_id->valuestring) : NULL
        );

    return authentication_info_local_var;
end:
    return NULL;
}

OpenAPI_authentication_info_t *OpenAPI_authentication_info_copy(OpenAPI_authentication_info_t *dst, OpenAPI_authentication_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authentication_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed");
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

    OpenAPI_authentication_info_free(dst);
    dst = OpenAPI_authentication_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

