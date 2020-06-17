
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_parameter_data.h"

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_create(
    char *app_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    char *ipv4_addr,
    char *ipv6_addr,
    char *mac_addr,
    OpenAPI_parameter_over_pc5_t *param_over_pc5,
    OpenAPI_parameter_over_uu_t *param_over_uu,
    char *supp_feat
    )
{
    OpenAPI_service_parameter_data_t *service_parameter_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_service_parameter_data_t));
    if (!service_parameter_data_local_var) {
        return NULL;
    }
    service_parameter_data_local_var->app_id = app_id;
    service_parameter_data_local_var->dnn = dnn;
    service_parameter_data_local_var->snssai = snssai;
    service_parameter_data_local_var->inter_group_id = inter_group_id;
    service_parameter_data_local_var->supi = supi;
    service_parameter_data_local_var->ipv4_addr = ipv4_addr;
    service_parameter_data_local_var->ipv6_addr = ipv6_addr;
    service_parameter_data_local_var->mac_addr = mac_addr;
    service_parameter_data_local_var->param_over_pc5 = param_over_pc5;
    service_parameter_data_local_var->param_over_uu = param_over_uu;
    service_parameter_data_local_var->supp_feat = supp_feat;

    return service_parameter_data_local_var;
}

void OpenAPI_service_parameter_data_free(OpenAPI_service_parameter_data_t *service_parameter_data)
{
    if (NULL == service_parameter_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(service_parameter_data->app_id);
    ogs_free(service_parameter_data->dnn);
    OpenAPI_snssai_free(service_parameter_data->snssai);
    ogs_free(service_parameter_data->inter_group_id);
    ogs_free(service_parameter_data->supi);
    ogs_free(service_parameter_data->ipv4_addr);
    ogs_free(service_parameter_data->ipv6_addr);
    ogs_free(service_parameter_data->mac_addr);
    OpenAPI_parameter_over_pc5_free(service_parameter_data->param_over_pc5);
    OpenAPI_parameter_over_uu_free(service_parameter_data->param_over_uu);
    ogs_free(service_parameter_data->supp_feat);
    ogs_free(service_parameter_data);
}

cJSON *OpenAPI_service_parameter_data_convertToJSON(OpenAPI_service_parameter_data_t *service_parameter_data)
{
    cJSON *item = NULL;

    if (service_parameter_data == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ServiceParameterData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_parameter_data->app_id) {
        if (cJSON_AddStringToObject(item, "appId", service_parameter_data->app_id) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [app_id]");
            goto end;
        }
    }

    if (service_parameter_data->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", service_parameter_data->dnn) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (service_parameter_data->snssai) {
        cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(service_parameter_data->snssai);
        if (snssai_local_JSON == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [snssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [snssai]");
            goto end;
        }
    }

    if (service_parameter_data->inter_group_id) {
        if (cJSON_AddStringToObject(item, "interGroupId", service_parameter_data->inter_group_id) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [inter_group_id]");
            goto end;
        }
    }

    if (service_parameter_data->supi) {
        if (cJSON_AddStringToObject(item, "supi", service_parameter_data->supi) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [supi]");
            goto end;
        }
    }

    if (service_parameter_data->ipv4_addr) {
        if (cJSON_AddStringToObject(item, "ipv4Addr", service_parameter_data->ipv4_addr) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    if (service_parameter_data->ipv6_addr) {
        if (cJSON_AddStringToObject(item, "ipv6Addr", service_parameter_data->ipv6_addr) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ipv6_addr]");
            goto end;
        }
    }

    if (service_parameter_data->mac_addr) {
        if (cJSON_AddStringToObject(item, "macAddr", service_parameter_data->mac_addr) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [mac_addr]");
            goto end;
        }
    }

    if (service_parameter_data->param_over_pc5) {
        cJSON *param_over_pc5_local_JSON = OpenAPI_parameter_over_pc5_convertToJSON(service_parameter_data->param_over_pc5);
        if (param_over_pc5_local_JSON == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_over_pc5]");
            goto end;
        }
        cJSON_AddItemToObject(item, "paramOverPc5", param_over_pc5_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_over_pc5]");
            goto end;
        }
    }

    if (service_parameter_data->param_over_uu) {
        cJSON *param_over_uu_local_JSON = OpenAPI_parameter_over_uu_convertToJSON(service_parameter_data->param_over_uu);
        if (param_over_uu_local_JSON == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_over_uu]");
            goto end;
        }
        cJSON_AddItemToObject(item, "paramOverUu", param_over_uu_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_over_uu]");
            goto end;
        }
    }

    if (service_parameter_data->supp_feat) {
        if (cJSON_AddStringToObject(item, "suppFeat", service_parameter_data->supp_feat) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [supp_feat]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_parseFromJSON(cJSON *service_parameter_dataJSON)
{
    OpenAPI_service_parameter_data_t *service_parameter_data_local_var = NULL;
    cJSON *app_id = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "appId");

    if (app_id) {
        if (!cJSON_IsString(app_id)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [app_id]");
            goto end;
        }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
        snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    cJSON *inter_group_id = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "interGroupId");

    if (inter_group_id) {
        if (!cJSON_IsString(inter_group_id)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [inter_group_id]");
            goto end;
        }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *ipv4_addr = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "ipv4Addr");

    if (ipv4_addr) {
        if (!cJSON_IsString(ipv4_addr)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    cJSON *ipv6_addr = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "ipv6Addr");

    if (ipv6_addr) {
        if (!cJSON_IsString(ipv6_addr)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ipv6_addr]");
            goto end;
        }
    }

    cJSON *mac_addr = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "macAddr");

    if (mac_addr) {
        if (!cJSON_IsString(mac_addr)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [mac_addr]");
            goto end;
        }
    }

    cJSON *param_over_pc5 = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramOverPc5");

    OpenAPI_parameter_over_pc5_t *param_over_pc5_local_nonprim = NULL;
    if (param_over_pc5) {
        param_over_pc5_local_nonprim = OpenAPI_parameter_over_pc5_parseFromJSON(param_over_pc5);
    }

    cJSON *param_over_uu = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramOverUu");

    OpenAPI_parameter_over_uu_t *param_over_uu_local_nonprim = NULL;
    if (param_over_uu) {
        param_over_uu_local_nonprim = OpenAPI_parameter_over_uu_parseFromJSON(param_over_uu);
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "suppFeat");

    if (supp_feat) {
        if (!cJSON_IsString(supp_feat)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [supp_feat]");
            goto end;
        }
    }

    service_parameter_data_local_var = OpenAPI_service_parameter_data_create (
        app_id ? ogs_strdup(app_id->valuestring) : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        inter_group_id ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        ipv4_addr ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        mac_addr ? ogs_strdup(mac_addr->valuestring) : NULL,
        param_over_pc5 ? param_over_pc5_local_nonprim : NULL,
        param_over_uu ? param_over_uu_local_nonprim : NULL,
        supp_feat ? ogs_strdup(supp_feat->valuestring) : NULL
        );

    return service_parameter_data_local_var;
end:
    return NULL;
}

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_copy(OpenAPI_service_parameter_data_t *dst, OpenAPI_service_parameter_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_parameter_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed");
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

    OpenAPI_service_parameter_data_free(dst);
    dst = OpenAPI_service_parameter_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

