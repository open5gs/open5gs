
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_data_subscr_cond.h"

OpenAPI_subscription_data_subscr_cond_t *OpenAPI_subscription_data_subscr_cond_create(
    char *nf_instance_id,
    OpenAPI_nf_type_e nf_type,
    char *service_name,
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *nsi_list,
    char *nf_group_id,
    char *nf_set_id,
    char *nf_service_set_id,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *tai_list
)
{
    OpenAPI_subscription_data_subscr_cond_t *subscription_data_subscr_cond_local_var = ogs_malloc(sizeof(OpenAPI_subscription_data_subscr_cond_t));
    ogs_assert(subscription_data_subscr_cond_local_var);

    subscription_data_subscr_cond_local_var->nf_instance_id = nf_instance_id;
    subscription_data_subscr_cond_local_var->nf_type = nf_type;
    subscription_data_subscr_cond_local_var->service_name = service_name;
    subscription_data_subscr_cond_local_var->amf_set_id = amf_set_id;
    subscription_data_subscr_cond_local_var->amf_region_id = amf_region_id;
    subscription_data_subscr_cond_local_var->guami_list = guami_list;
    subscription_data_subscr_cond_local_var->snssai_list = snssai_list;
    subscription_data_subscr_cond_local_var->nsi_list = nsi_list;
    subscription_data_subscr_cond_local_var->nf_group_id = nf_group_id;
    subscription_data_subscr_cond_local_var->nf_set_id = nf_set_id;
    subscription_data_subscr_cond_local_var->nf_service_set_id = nf_service_set_id;
    subscription_data_subscr_cond_local_var->smf_serving_area = smf_serving_area;
    subscription_data_subscr_cond_local_var->tai_list = tai_list;

    return subscription_data_subscr_cond_local_var;
}

void OpenAPI_subscription_data_subscr_cond_free(OpenAPI_subscription_data_subscr_cond_t *subscription_data_subscr_cond)
{
    if (NULL == subscription_data_subscr_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(subscription_data_subscr_cond->nf_instance_id);
    ogs_free(subscription_data_subscr_cond->service_name);
    ogs_free(subscription_data_subscr_cond->amf_set_id);
    ogs_free(subscription_data_subscr_cond->amf_region_id);
    OpenAPI_list_for_each(subscription_data_subscr_cond->guami_list, node) {
        OpenAPI_guami_free(node->data);
    }
    OpenAPI_list_free(subscription_data_subscr_cond->guami_list);
    OpenAPI_list_for_each(subscription_data_subscr_cond->snssai_list, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(subscription_data_subscr_cond->snssai_list);
    OpenAPI_list_for_each(subscription_data_subscr_cond->nsi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(subscription_data_subscr_cond->nsi_list);
    ogs_free(subscription_data_subscr_cond->nf_group_id);
    ogs_free(subscription_data_subscr_cond->nf_set_id);
    ogs_free(subscription_data_subscr_cond->nf_service_set_id);
    OpenAPI_list_for_each(subscription_data_subscr_cond->smf_serving_area, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(subscription_data_subscr_cond->smf_serving_area);
    OpenAPI_list_for_each(subscription_data_subscr_cond->tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(subscription_data_subscr_cond->tai_list);
    ogs_free(subscription_data_subscr_cond);
}

cJSON *OpenAPI_subscription_data_subscr_cond_convertToJSON(OpenAPI_subscription_data_subscr_cond_t *subscription_data_subscr_cond)
{
    cJSON *item = NULL;

    if (subscription_data_subscr_cond == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [SubscriptionData_subscrCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (subscription_data_subscr_cond->nf_instance_id) {
    if (cJSON_AddStringToObject(item, "nfInstanceId", subscription_data_subscr_cond->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->nf_type) {
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(subscription_data_subscr_cond->nf_type)) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nf_type]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->service_name) {
    if (cJSON_AddStringToObject(item, "serviceName", subscription_data_subscr_cond->service_name) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [service_name]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->amf_set_id) {
    if (cJSON_AddStringToObject(item, "amfSetId", subscription_data_subscr_cond->amf_set_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [amf_set_id]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->amf_region_id) {
    if (cJSON_AddStringToObject(item, "amfRegionId", subscription_data_subscr_cond->amf_region_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [amf_region_id]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->guami_list) {
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [guami_list]");
        goto end;
    }

    OpenAPI_lnode_t *guami_list_node;
    if (subscription_data_subscr_cond->guami_list) {
        OpenAPI_list_for_each(subscription_data_subscr_cond->guami_list, guami_list_node) {
            cJSON *itemLocal = OpenAPI_guami_convertToJSON(guami_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [guami_list]");
                goto end;
            }
            cJSON_AddItemToArray(guami_listList, itemLocal);
        }
    }
    }

    if (subscription_data_subscr_cond->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [snssai_list]");
        goto end;
    }

    OpenAPI_lnode_t *snssai_list_node;
    if (subscription_data_subscr_cond->snssai_list) {
        OpenAPI_list_for_each(subscription_data_subscr_cond->snssai_list, snssai_list_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(snssai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [snssai_list]");
                goto end;
            }
            cJSON_AddItemToArray(snssai_listList, itemLocal);
        }
    }
    }

    if (subscription_data_subscr_cond->nsi_list) {
    cJSON *nsi_list = cJSON_AddArrayToObject(item, "nsiList");
    if (nsi_list == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nsi_list]");
        goto end;
    }

    OpenAPI_lnode_t *nsi_list_node;
    OpenAPI_list_for_each(subscription_data_subscr_cond->nsi_list, nsi_list_node)  {
    if (cJSON_AddStringToObject(nsi_list, "", (char*)nsi_list_node->data) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nsi_list]");
        goto end;
    }
                    }
    }

    if (subscription_data_subscr_cond->nf_group_id) {
    if (cJSON_AddStringToObject(item, "nfGroupId", subscription_data_subscr_cond->nf_group_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nf_group_id]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->nf_set_id) {
    if (cJSON_AddStringToObject(item, "nfSetId", subscription_data_subscr_cond->nf_set_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nf_set_id]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->nf_service_set_id) {
    if (cJSON_AddStringToObject(item, "nfServiceSetId", subscription_data_subscr_cond->nf_service_set_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [nf_service_set_id]");
        goto end;
    }
    }

    if (subscription_data_subscr_cond->smf_serving_area) {
    cJSON *smf_serving_area = cJSON_AddArrayToObject(item, "smfServingArea");
    if (smf_serving_area == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [smf_serving_area]");
        goto end;
    }

    OpenAPI_lnode_t *smf_serving_area_node;
    OpenAPI_list_for_each(subscription_data_subscr_cond->smf_serving_area, smf_serving_area_node)  {
    if (cJSON_AddStringToObject(smf_serving_area, "", (char*)smf_serving_area_node->data) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [smf_serving_area]");
        goto end;
    }
                    }
    }

    if (subscription_data_subscr_cond->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [tai_list]");
        goto end;
    }

    OpenAPI_lnode_t *tai_list_node;
    if (subscription_data_subscr_cond->tai_list) {
        OpenAPI_list_for_each(subscription_data_subscr_cond->tai_list, tai_list_node) {
            cJSON *itemLocal = OpenAPI_tai_convertToJSON(tai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed [tai_list]");
                goto end;
            }
            cJSON_AddItemToArray(tai_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_subscription_data_subscr_cond_t *OpenAPI_subscription_data_subscr_cond_parseFromJSON(cJSON *subscription_data_subscr_condJSON)
{
    OpenAPI_subscription_data_subscr_cond_t *subscription_data_subscr_cond_local_var = NULL;
    cJSON *nf_instance_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "nfInstanceId");

    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    cJSON *nf_type = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "nfType");

    OpenAPI_nf_type_e nf_typeVariable;
    if (nf_type) {
    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);
    }

    cJSON *service_name = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "serviceName");

    if (service_name) {
    if (!cJSON_IsString(service_name)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [service_name]");
        goto end;
    }
    }

    cJSON *amf_set_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "amfSetId");

    if (amf_set_id) {
    if (!cJSON_IsString(amf_set_id)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [amf_set_id]");
        goto end;
    }
    }

    cJSON *amf_region_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "amfRegionId");

    if (amf_region_id) {
    if (!cJSON_IsString(amf_region_id)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [amf_region_id]");
        goto end;
    }
    }

    cJSON *guami_list = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "guamiList");

    OpenAPI_list_t *guami_listList;
    if (guami_list) {
    cJSON *guami_list_local_nonprimitive;
    if (!cJSON_IsArray(guami_list)){
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [guami_list]");
        goto end;
    }

    guami_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(guami_list_local_nonprimitive, guami_list ) {
        if (!cJSON_IsObject(guami_list_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [guami_list]");
            goto end;
        }
        OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local_nonprimitive);

        if (!guami_listItem) {
            ogs_error("No guami_listItem");
            OpenAPI_list_free(guami_listList);
            goto end;
        }

        OpenAPI_list_add(guami_listList, guami_listItem);
    }
    }

    cJSON *snssai_list = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "snssaiList");

    OpenAPI_list_t *snssai_listList;
    if (snssai_list) {
    cJSON *snssai_list_local_nonprimitive;
    if (!cJSON_IsArray(snssai_list)){
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [snssai_list]");
        goto end;
    }

    snssai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssai_list_local_nonprimitive, snssai_list ) {
        if (!cJSON_IsObject(snssai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [snssai_list]");
            goto end;
        }
        OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local_nonprimitive);

        if (!snssai_listItem) {
            ogs_error("No snssai_listItem");
            OpenAPI_list_free(snssai_listList);
            goto end;
        }

        OpenAPI_list_add(snssai_listList, snssai_listItem);
    }
    }

    cJSON *nsi_list = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "nsiList");

    OpenAPI_list_t *nsi_listList;
    if (nsi_list) {
    cJSON *nsi_list_local;
    if (!cJSON_IsArray(nsi_list)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nsi_list]");
        goto end;
    }
    nsi_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(nsi_list_local, nsi_list) {
    if (!cJSON_IsString(nsi_list_local)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nsi_list]");
        goto end;
    }
    OpenAPI_list_add(nsi_listList , ogs_strdup(nsi_list_local->valuestring));
    }
    }

    cJSON *nf_group_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "nfGroupId");

    if (nf_group_id) {
    if (!cJSON_IsString(nf_group_id)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nf_group_id]");
        goto end;
    }
    }

    cJSON *nf_set_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "nfSetId");

    if (nf_set_id) {
    if (!cJSON_IsString(nf_set_id)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nf_set_id]");
        goto end;
    }
    }

    cJSON *nf_service_set_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "nfServiceSetId");

    if (nf_service_set_id) {
    if (!cJSON_IsString(nf_service_set_id)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [nf_service_set_id]");
        goto end;
    }
    }

    cJSON *smf_serving_area = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "smfServingArea");

    OpenAPI_list_t *smf_serving_areaList;
    if (smf_serving_area) {
    cJSON *smf_serving_area_local;
    if (!cJSON_IsArray(smf_serving_area)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [smf_serving_area]");
        goto end;
    }
    smf_serving_areaList = OpenAPI_list_create();

    cJSON_ArrayForEach(smf_serving_area_local, smf_serving_area) {
    if (!cJSON_IsString(smf_serving_area_local)) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [smf_serving_area]");
        goto end;
    }
    OpenAPI_list_add(smf_serving_areaList , ogs_strdup(smf_serving_area_local->valuestring));
    }
    }

    cJSON *tai_list = cJSON_GetObjectItemCaseSensitive(subscription_data_subscr_condJSON, "taiList");

    OpenAPI_list_t *tai_listList;
    if (tai_list) {
    cJSON *tai_list_local_nonprimitive;
    if (!cJSON_IsArray(tai_list)){
        ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [tai_list]");
        goto end;
    }

    tai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(tai_list_local_nonprimitive, tai_list ) {
        if (!cJSON_IsObject(tai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_subscription_data_subscr_cond_parseFromJSON() failed [tai_list]");
            goto end;
        }
        OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local_nonprimitive);

        if (!tai_listItem) {
            ogs_error("No tai_listItem");
            OpenAPI_list_free(tai_listList);
            goto end;
        }

        OpenAPI_list_add(tai_listList, tai_listItem);
    }
    }

    subscription_data_subscr_cond_local_var = OpenAPI_subscription_data_subscr_cond_create (
        nf_instance_id ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        nf_type ? nf_typeVariable : 0,
        service_name ? ogs_strdup(service_name->valuestring) : NULL,
        amf_set_id ? ogs_strdup(amf_set_id->valuestring) : NULL,
        amf_region_id ? ogs_strdup(amf_region_id->valuestring) : NULL,
        guami_list ? guami_listList : NULL,
        snssai_list ? snssai_listList : NULL,
        nsi_list ? nsi_listList : NULL,
        nf_group_id ? ogs_strdup(nf_group_id->valuestring) : NULL,
        nf_set_id ? ogs_strdup(nf_set_id->valuestring) : NULL,
        nf_service_set_id ? ogs_strdup(nf_service_set_id->valuestring) : NULL,
        smf_serving_area ? smf_serving_areaList : NULL,
        tai_list ? tai_listList : NULL
    );

    return subscription_data_subscr_cond_local_var;
end:
    return NULL;
}

OpenAPI_subscription_data_subscr_cond_t *OpenAPI_subscription_data_subscr_cond_copy(OpenAPI_subscription_data_subscr_cond_t *dst, OpenAPI_subscription_data_subscr_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscription_data_subscr_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscription_data_subscr_cond_convertToJSON() failed");
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

    OpenAPI_subscription_data_subscr_cond_free(dst);
    dst = OpenAPI_subscription_data_subscr_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

