
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_snssai_specific_group.h"

OpenAPI_dnn_snssai_specific_group_t *OpenAPI_dnn_snssai_specific_group_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_af_req_default_qo_s_t *def_qos,
    OpenAPI_list_t *af_req_serv_area
)
{
    OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group_local_var = ogs_malloc(sizeof(OpenAPI_dnn_snssai_specific_group_t));
    ogs_assert(dnn_snssai_specific_group_local_var);

    dnn_snssai_specific_group_local_var->dnn = dnn;
    dnn_snssai_specific_group_local_var->snssai = snssai;
    dnn_snssai_specific_group_local_var->def_qos = def_qos;
    dnn_snssai_specific_group_local_var->af_req_serv_area = af_req_serv_area;

    return dnn_snssai_specific_group_local_var;
}

void OpenAPI_dnn_snssai_specific_group_free(OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_snssai_specific_group) {
        return;
    }
    if (dnn_snssai_specific_group->dnn) {
        ogs_free(dnn_snssai_specific_group->dnn);
        dnn_snssai_specific_group->dnn = NULL;
    }
    if (dnn_snssai_specific_group->snssai) {
        OpenAPI_snssai_free(dnn_snssai_specific_group->snssai);
        dnn_snssai_specific_group->snssai = NULL;
    }
    if (dnn_snssai_specific_group->def_qos) {
        OpenAPI_af_req_default_qo_s_free(dnn_snssai_specific_group->def_qos);
        dnn_snssai_specific_group->def_qos = NULL;
    }
    if (dnn_snssai_specific_group->af_req_serv_area) {
        OpenAPI_list_for_each(dnn_snssai_specific_group->af_req_serv_area, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(dnn_snssai_specific_group->af_req_serv_area);
        dnn_snssai_specific_group->af_req_serv_area = NULL;
    }
    ogs_free(dnn_snssai_specific_group);
}

cJSON *OpenAPI_dnn_snssai_specific_group_convertToJSON(OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_snssai_specific_group == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [DnnSnssaiSpecificGroup]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_snssai_specific_group->dnn) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_snssai_specific_group->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!dnn_snssai_specific_group->snssai) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(dnn_snssai_specific_group->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [snssai]");
        goto end;
    }

    if (dnn_snssai_specific_group->def_qos) {
    cJSON *def_qos_local_JSON = OpenAPI_af_req_default_qo_s_convertToJSON(dnn_snssai_specific_group->def_qos);
    if (def_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [def_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "defQos", def_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [def_qos]");
        goto end;
    }
    }

    if (dnn_snssai_specific_group->af_req_serv_area) {
    cJSON *af_req_serv_areaList = cJSON_AddArrayToObject(item, "afReqServArea");
    if (af_req_serv_areaList == NULL) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [af_req_serv_area]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_snssai_specific_group->af_req_serv_area, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed [af_req_serv_area]");
            goto end;
        }
        cJSON_AddItemToArray(af_req_serv_areaList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_dnn_snssai_specific_group_t *OpenAPI_dnn_snssai_specific_group_parseFromJSON(cJSON *dnn_snssai_specific_groupJSON)
{
    OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *def_qos = NULL;
    OpenAPI_af_req_default_qo_s_t *def_qos_local_nonprim = NULL;
    cJSON *af_req_serv_area = NULL;
    OpenAPI_list_t *af_req_serv_areaList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_snssai_specific_groupJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_parseFromJSON() failed [dnn]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(dnn_snssai_specific_groupJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    def_qos = cJSON_GetObjectItemCaseSensitive(dnn_snssai_specific_groupJSON, "defQos");
    if (def_qos) {
    def_qos_local_nonprim = OpenAPI_af_req_default_qo_s_parseFromJSON(def_qos);
    if (!def_qos_local_nonprim) {
        ogs_error("OpenAPI_af_req_default_qo_s_parseFromJSON failed [def_qos]");
        goto end;
    }
    }

    af_req_serv_area = cJSON_GetObjectItemCaseSensitive(dnn_snssai_specific_groupJSON, "afReqServArea");
    if (af_req_serv_area) {
        cJSON *af_req_serv_area_local = NULL;
        if (!cJSON_IsArray(af_req_serv_area)) {
            ogs_error("OpenAPI_dnn_snssai_specific_group_parseFromJSON() failed [af_req_serv_area]");
            goto end;
        }

        af_req_serv_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_req_serv_area_local, af_req_serv_area) {
            if (!cJSON_IsObject(af_req_serv_area_local)) {
                ogs_error("OpenAPI_dnn_snssai_specific_group_parseFromJSON() failed [af_req_serv_area]");
                goto end;
            }
            OpenAPI_tai_t *af_req_serv_areaItem = OpenAPI_tai_parseFromJSON(af_req_serv_area_local);
            if (!af_req_serv_areaItem) {
                ogs_error("No af_req_serv_areaItem");
                goto end;
            }
            OpenAPI_list_add(af_req_serv_areaList, af_req_serv_areaItem);
        }
    }

    dnn_snssai_specific_group_local_var = OpenAPI_dnn_snssai_specific_group_create (
        ogs_strdup(dnn->valuestring),
        snssai_local_nonprim,
        def_qos ? def_qos_local_nonprim : NULL,
        af_req_serv_area ? af_req_serv_areaList : NULL
    );

    return dnn_snssai_specific_group_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (def_qos_local_nonprim) {
        OpenAPI_af_req_default_qo_s_free(def_qos_local_nonprim);
        def_qos_local_nonprim = NULL;
    }
    if (af_req_serv_areaList) {
        OpenAPI_list_for_each(af_req_serv_areaList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(af_req_serv_areaList);
        af_req_serv_areaList = NULL;
    }
    return NULL;
}

OpenAPI_dnn_snssai_specific_group_t *OpenAPI_dnn_snssai_specific_group_copy(OpenAPI_dnn_snssai_specific_group_t *dst, OpenAPI_dnn_snssai_specific_group_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_snssai_specific_group_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_convertToJSON() failed");
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

    OpenAPI_dnn_snssai_specific_group_free(dst);
    dst = OpenAPI_dnn_snssai_specific_group_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

