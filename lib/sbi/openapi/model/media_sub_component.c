
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_sub_component.h"

OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_create(
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    OpenAPI_list_t *ethf_descs,
    int f_num,
    OpenAPI_list_t *f_descs,
    OpenAPI_flow_status_e f_status,
    char *mar_bw_dl,
    char *mar_bw_ul,
    char *tos_tr_cl,
    OpenAPI_flow_usage_e flow_usage
)
{
    OpenAPI_media_sub_component_t *media_sub_component_local_var = ogs_malloc(sizeof(OpenAPI_media_sub_component_t));
    ogs_assert(media_sub_component_local_var);

    media_sub_component_local_var->af_sig_protocol = af_sig_protocol;
    media_sub_component_local_var->ethf_descs = ethf_descs;
    media_sub_component_local_var->f_num = f_num;
    media_sub_component_local_var->f_descs = f_descs;
    media_sub_component_local_var->f_status = f_status;
    media_sub_component_local_var->mar_bw_dl = mar_bw_dl;
    media_sub_component_local_var->mar_bw_ul = mar_bw_ul;
    media_sub_component_local_var->tos_tr_cl = tos_tr_cl;
    media_sub_component_local_var->flow_usage = flow_usage;

    return media_sub_component_local_var;
}

void OpenAPI_media_sub_component_free(OpenAPI_media_sub_component_t *media_sub_component)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == media_sub_component) {
        return;
    }
    if (media_sub_component->ethf_descs) {
        OpenAPI_list_for_each(media_sub_component->ethf_descs, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(media_sub_component->ethf_descs);
        media_sub_component->ethf_descs = NULL;
    }
    if (media_sub_component->f_descs) {
        OpenAPI_list_for_each(media_sub_component->f_descs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(media_sub_component->f_descs);
        media_sub_component->f_descs = NULL;
    }
    if (media_sub_component->mar_bw_dl) {
        ogs_free(media_sub_component->mar_bw_dl);
        media_sub_component->mar_bw_dl = NULL;
    }
    if (media_sub_component->mar_bw_ul) {
        ogs_free(media_sub_component->mar_bw_ul);
        media_sub_component->mar_bw_ul = NULL;
    }
    if (media_sub_component->tos_tr_cl) {
        ogs_free(media_sub_component->tos_tr_cl);
        media_sub_component->tos_tr_cl = NULL;
    }
    ogs_free(media_sub_component);
}

cJSON *OpenAPI_media_sub_component_convertToJSON(OpenAPI_media_sub_component_t *media_sub_component)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (media_sub_component == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [MediaSubComponent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (media_sub_component->af_sig_protocol != OpenAPI_af_sig_protocol_NULL) {
    if (cJSON_AddStringToObject(item, "afSigProtocol", OpenAPI_af_sig_protocol_ToString(media_sub_component->af_sig_protocol)) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [af_sig_protocol]");
        goto end;
    }
    }

    if (media_sub_component->ethf_descs) {
    cJSON *ethf_descsList = cJSON_AddArrayToObject(item, "ethfDescs");
    if (ethf_descsList == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [ethf_descs]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component->ethf_descs, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [ethf_descs]");
            goto end;
        }
        cJSON_AddItemToArray(ethf_descsList, itemLocal);
    }
    }

    if (cJSON_AddNumberToObject(item, "fNum", media_sub_component->f_num) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [f_num]");
        goto end;
    }

    if (media_sub_component->f_descs) {
    cJSON *f_descsList = cJSON_AddArrayToObject(item, "fDescs");
    if (f_descsList == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [f_descs]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component->f_descs, node) {
        if (cJSON_AddStringToObject(f_descsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [f_descs]");
            goto end;
        }
    }
    }

    if (media_sub_component->f_status != OpenAPI_flow_status_NULL) {
    if (cJSON_AddStringToObject(item, "fStatus", OpenAPI_flow_status_ToString(media_sub_component->f_status)) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [f_status]");
        goto end;
    }
    }

    if (media_sub_component->mar_bw_dl) {
    if (cJSON_AddStringToObject(item, "marBwDl", media_sub_component->mar_bw_dl) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    if (media_sub_component->mar_bw_ul) {
    if (cJSON_AddStringToObject(item, "marBwUl", media_sub_component->mar_bw_ul) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    if (media_sub_component->tos_tr_cl) {
    if (cJSON_AddStringToObject(item, "tosTrCl", media_sub_component->tos_tr_cl) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [tos_tr_cl]");
        goto end;
    }
    }

    if (media_sub_component->flow_usage != OpenAPI_flow_usage_NULL) {
    if (cJSON_AddStringToObject(item, "flowUsage", OpenAPI_flow_usage_ToString(media_sub_component->flow_usage)) == NULL) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed [flow_usage]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_parseFromJSON(cJSON *media_sub_componentJSON)
{
    OpenAPI_media_sub_component_t *media_sub_component_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_sig_protocol = NULL;
    OpenAPI_af_sig_protocol_e af_sig_protocolVariable = 0;
    cJSON *ethf_descs = NULL;
    OpenAPI_list_t *ethf_descsList = NULL;
    cJSON *f_num = NULL;
    cJSON *f_descs = NULL;
    OpenAPI_list_t *f_descsList = NULL;
    cJSON *f_status = NULL;
    OpenAPI_flow_status_e f_statusVariable = 0;
    cJSON *mar_bw_dl = NULL;
    cJSON *mar_bw_ul = NULL;
    cJSON *tos_tr_cl = NULL;
    cJSON *flow_usage = NULL;
    OpenAPI_flow_usage_e flow_usageVariable = 0;
    af_sig_protocol = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "afSigProtocol");
    if (af_sig_protocol) {
    if (!cJSON_IsString(af_sig_protocol)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [af_sig_protocol]");
        goto end;
    }
    af_sig_protocolVariable = OpenAPI_af_sig_protocol_FromString(af_sig_protocol->valuestring);
    }

    ethf_descs = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "ethfDescs");
    if (ethf_descs) {
        cJSON *ethf_descs_local = NULL;
        if (!cJSON_IsArray(ethf_descs)) {
            ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [ethf_descs]");
            goto end;
        }

        ethf_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ethf_descs_local, ethf_descs) {
            if (!cJSON_IsObject(ethf_descs_local)) {
                ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [ethf_descs]");
                goto end;
            }
            OpenAPI_eth_flow_description_t *ethf_descsItem = OpenAPI_eth_flow_description_parseFromJSON(ethf_descs_local);
            if (!ethf_descsItem) {
                ogs_error("No ethf_descsItem");
                goto end;
            }
            OpenAPI_list_add(ethf_descsList, ethf_descsItem);
        }
    }

    f_num = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "fNum");
    if (!f_num) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [f_num]");
        goto end;
    }
    if (!cJSON_IsNumber(f_num)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [f_num]");
        goto end;
    }

    f_descs = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "fDescs");
    if (f_descs) {
        cJSON *f_descs_local = NULL;
        if (!cJSON_IsArray(f_descs)) {
            ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [f_descs]");
            goto end;
        }

        f_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(f_descs_local, f_descs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(f_descs_local)) {
                ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [f_descs]");
                goto end;
            }
            OpenAPI_list_add(f_descsList, ogs_strdup(f_descs_local->valuestring));
        }
    }

    f_status = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "fStatus");
    if (f_status) {
    if (!cJSON_IsString(f_status)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [f_status]");
        goto end;
    }
    f_statusVariable = OpenAPI_flow_status_FromString(f_status->valuestring);
    }

    mar_bw_dl = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "marBwDl");
    if (mar_bw_dl) {
    if (!cJSON_IsString(mar_bw_dl) && !cJSON_IsNull(mar_bw_dl)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    mar_bw_ul = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "marBwUl");
    if (mar_bw_ul) {
    if (!cJSON_IsString(mar_bw_ul) && !cJSON_IsNull(mar_bw_ul)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    tos_tr_cl = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "tosTrCl");
    if (tos_tr_cl) {
    if (!cJSON_IsString(tos_tr_cl) && !cJSON_IsNull(tos_tr_cl)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [tos_tr_cl]");
        goto end;
    }
    }

    flow_usage = cJSON_GetObjectItemCaseSensitive(media_sub_componentJSON, "flowUsage");
    if (flow_usage) {
    if (!cJSON_IsString(flow_usage)) {
        ogs_error("OpenAPI_media_sub_component_parseFromJSON() failed [flow_usage]");
        goto end;
    }
    flow_usageVariable = OpenAPI_flow_usage_FromString(flow_usage->valuestring);
    }

    media_sub_component_local_var = OpenAPI_media_sub_component_create (
        af_sig_protocol ? af_sig_protocolVariable : 0,
        ethf_descs ? ethf_descsList : NULL,
        
        f_num->valuedouble,
        f_descs ? f_descsList : NULL,
        f_status ? f_statusVariable : 0,
        mar_bw_dl && !cJSON_IsNull(mar_bw_dl) ? ogs_strdup(mar_bw_dl->valuestring) : NULL,
        mar_bw_ul && !cJSON_IsNull(mar_bw_ul) ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        tos_tr_cl && !cJSON_IsNull(tos_tr_cl) ? ogs_strdup(tos_tr_cl->valuestring) : NULL,
        flow_usage ? flow_usageVariable : 0
    );

    return media_sub_component_local_var;
end:
    if (ethf_descsList) {
        OpenAPI_list_for_each(ethf_descsList, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(ethf_descsList);
        ethf_descsList = NULL;
    }
    if (f_descsList) {
        OpenAPI_list_for_each(f_descsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(f_descsList);
        f_descsList = NULL;
    }
    return NULL;
}

OpenAPI_media_sub_component_t *OpenAPI_media_sub_component_copy(OpenAPI_media_sub_component_t *dst, OpenAPI_media_sub_component_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_media_sub_component_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_media_sub_component_convertToJSON() failed");
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

    OpenAPI_media_sub_component_free(dst);
    dst = OpenAPI_media_sub_component_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

