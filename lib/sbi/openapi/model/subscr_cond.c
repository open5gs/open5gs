
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscr_cond.h"

OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_create(
    char *nf_instance_id,
    OpenAPI_list_t *nf_instance_list,
    OpenAPI_nf_type_e nf_type,
    char *service_name,
    OpenAPI_list_t *service_name_list,
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
    OpenAPI_subscr_cond_t *subscr_cond_local_var = ogs_malloc(sizeof(OpenAPI_subscr_cond_t));
    ogs_assert(subscr_cond_local_var);

    subscr_cond_local_var->nf_instance_id = nf_instance_id;
    subscr_cond_local_var->nf_instance_list = nf_instance_list;
    subscr_cond_local_var->nf_type = nf_type;
    subscr_cond_local_var->service_name = service_name;
    subscr_cond_local_var->service_name_list = service_name_list;
    subscr_cond_local_var->amf_set_id = amf_set_id;
    subscr_cond_local_var->amf_region_id = amf_region_id;
    subscr_cond_local_var->guami_list = guami_list;
    subscr_cond_local_var->snssai_list = snssai_list;
    subscr_cond_local_var->nsi_list = nsi_list;
    subscr_cond_local_var->nf_group_id = nf_group_id;
    subscr_cond_local_var->nf_set_id = nf_set_id;
    subscr_cond_local_var->nf_service_set_id = nf_service_set_id;
    subscr_cond_local_var->smf_serving_area = smf_serving_area;
    subscr_cond_local_var->tai_list = tai_list;

    return subscr_cond_local_var;
}

void OpenAPI_subscr_cond_free(OpenAPI_subscr_cond_t *subscr_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscr_cond) {
        return;
    }
    if (subscr_cond->nf_instance_id) {
        ogs_free(subscr_cond->nf_instance_id);
        subscr_cond->nf_instance_id = NULL;
    }
    if (subscr_cond->nf_instance_list) {
        OpenAPI_list_for_each(subscr_cond->nf_instance_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->nf_instance_list);
        subscr_cond->nf_instance_list = NULL;
    }
    if (subscr_cond->service_name) {
        ogs_free(subscr_cond->service_name);
        subscr_cond->service_name = NULL;
    }
    if (subscr_cond->service_name_list) {
        OpenAPI_list_for_each(subscr_cond->service_name_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->service_name_list);
        subscr_cond->service_name_list = NULL;
    }
    if (subscr_cond->amf_set_id) {
        ogs_free(subscr_cond->amf_set_id);
        subscr_cond->amf_set_id = NULL;
    }
    if (subscr_cond->amf_region_id) {
        ogs_free(subscr_cond->amf_region_id);
        subscr_cond->amf_region_id = NULL;
    }
    if (subscr_cond->guami_list) {
        OpenAPI_list_for_each(subscr_cond->guami_list, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->guami_list);
        subscr_cond->guami_list = NULL;
    }
    if (subscr_cond->snssai_list) {
        OpenAPI_list_for_each(subscr_cond->snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->snssai_list);
        subscr_cond->snssai_list = NULL;
    }
    if (subscr_cond->nsi_list) {
        OpenAPI_list_for_each(subscr_cond->nsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->nsi_list);
        subscr_cond->nsi_list = NULL;
    }
    if (subscr_cond->nf_group_id) {
        ogs_free(subscr_cond->nf_group_id);
        subscr_cond->nf_group_id = NULL;
    }
    if (subscr_cond->nf_set_id) {
        ogs_free(subscr_cond->nf_set_id);
        subscr_cond->nf_set_id = NULL;
    }
    if (subscr_cond->nf_service_set_id) {
        ogs_free(subscr_cond->nf_service_set_id);
        subscr_cond->nf_service_set_id = NULL;
    }
    if (subscr_cond->smf_serving_area) {
        OpenAPI_list_for_each(subscr_cond->smf_serving_area, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->smf_serving_area);
        subscr_cond->smf_serving_area = NULL;
    }
    if (subscr_cond->tai_list) {
        OpenAPI_list_for_each(subscr_cond->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->tai_list);
        subscr_cond->tai_list = NULL;
    }
    ogs_free(subscr_cond);
}

cJSON *OpenAPI_subscr_cond_convertToJSON(OpenAPI_subscr_cond_t *subscr_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscr_cond == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [SubscrCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (subscr_cond->nf_instance_id) {
    if (cJSON_AddStringToObject(item, "nfInstanceId", subscr_cond->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    if (subscr_cond->nf_instance_list) {
    cJSON *nf_instance_listList = cJSON_AddArrayToObject(item, "nfInstanceList");
    if (nf_instance_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_instance_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->nf_instance_list, node) {
        if (cJSON_AddStringToObject(nf_instance_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_instance_list]");
            goto end;
        }
    }
    }

    if (subscr_cond->nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(subscr_cond->nf_type)) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_type]");
        goto end;
    }
    }

    if (subscr_cond->service_name) {
    if (cJSON_AddStringToObject(item, "serviceName", subscr_cond->service_name) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [service_name]");
        goto end;
    }
    }

    if (subscr_cond->service_name_list) {
    cJSON *service_name_listList = cJSON_AddArrayToObject(item, "serviceNameList");
    if (service_name_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [service_name_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->service_name_list, node) {
        if (cJSON_AddStringToObject(service_name_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [service_name_list]");
            goto end;
        }
    }
    }

    if (subscr_cond->amf_set_id) {
    if (cJSON_AddStringToObject(item, "amfSetId", subscr_cond->amf_set_id) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [amf_set_id]");
        goto end;
    }
    }

    if (subscr_cond->amf_region_id) {
    if (cJSON_AddStringToObject(item, "amfRegionId", subscr_cond->amf_region_id) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [amf_region_id]");
        goto end;
    }
    }

    if (subscr_cond->guami_list) {
    cJSON *guami_listList = cJSON_AddArrayToObject(item, "guamiList");
    if (guami_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [guami_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->guami_list, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [guami_list]");
            goto end;
        }
        cJSON_AddItemToArray(guami_listList, itemLocal);
    }
    }

    if (subscr_cond->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }
    }

    if (subscr_cond->nsi_list) {
    cJSON *nsi_listList = cJSON_AddArrayToObject(item, "nsiList");
    if (nsi_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->nsi_list, node) {
        if (cJSON_AddStringToObject(nsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nsi_list]");
            goto end;
        }
    }
    }

    if (subscr_cond->nf_group_id) {
    if (cJSON_AddStringToObject(item, "nfGroupId", subscr_cond->nf_group_id) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_group_id]");
        goto end;
    }
    }

    if (subscr_cond->nf_set_id) {
    if (cJSON_AddStringToObject(item, "nfSetId", subscr_cond->nf_set_id) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_set_id]");
        goto end;
    }
    }

    if (subscr_cond->nf_service_set_id) {
    if (cJSON_AddStringToObject(item, "nfServiceSetId", subscr_cond->nf_service_set_id) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_service_set_id]");
        goto end;
    }
    }

    if (subscr_cond->smf_serving_area) {
    cJSON *smf_serving_areaList = cJSON_AddArrayToObject(item, "smfServingArea");
    if (smf_serving_areaList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [smf_serving_area]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->smf_serving_area, node) {
        if (cJSON_AddStringToObject(smf_serving_areaList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [smf_serving_area]");
            goto end;
        }
    }
    }

    if (subscr_cond->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_parseFromJSON(cJSON *subscr_condJSON)
{
    OpenAPI_subscr_cond_t *subscr_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *nf_instance_list = NULL;
    OpenAPI_list_t *nf_instance_listList = NULL;
    cJSON *nf_type = NULL;
    OpenAPI_nf_type_e nf_typeVariable = 0;
    cJSON *service_name = NULL;
    cJSON *service_name_list = NULL;
    OpenAPI_list_t *service_name_listList = NULL;
    cJSON *amf_set_id = NULL;
    cJSON *amf_region_id = NULL;
    cJSON *guami_list = NULL;
    OpenAPI_list_t *guami_listList = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *nsi_list = NULL;
    OpenAPI_list_t *nsi_listList = NULL;
    cJSON *nf_group_id = NULL;
    cJSON *nf_set_id = NULL;
    cJSON *nf_service_set_id = NULL;
    cJSON *smf_serving_area = NULL;
    OpenAPI_list_t *smf_serving_areaList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfInstanceId");
    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id) && !cJSON_IsNull(nf_instance_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    nf_instance_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfInstanceList");
    if (nf_instance_list) {
        cJSON *nf_instance_list_local = NULL;
        if (!cJSON_IsArray(nf_instance_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_instance_list]");
            goto end;
        }

        nf_instance_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instance_list_local, nf_instance_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_instance_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_instance_list]");
                goto end;
            }
            OpenAPI_list_add(nf_instance_listList, ogs_strdup(nf_instance_list_local->valuestring));
        }
    }

    nf_type = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfType");
    if (nf_type) {
    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);
    }

    service_name = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "serviceName");
    if (service_name) {
    if (!cJSON_IsString(service_name) && !cJSON_IsNull(service_name)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [service_name]");
        goto end;
    }
    }

    service_name_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "serviceNameList");
    if (service_name_list) {
        cJSON *service_name_list_local = NULL;
        if (!cJSON_IsArray(service_name_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [service_name_list]");
            goto end;
        }

        service_name_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(service_name_list_local, service_name_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(service_name_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [service_name_list]");
                goto end;
            }
            OpenAPI_list_add(service_name_listList, ogs_strdup(service_name_list_local->valuestring));
        }
    }

    amf_set_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "amfSetId");
    if (amf_set_id) {
    if (!cJSON_IsString(amf_set_id) && !cJSON_IsNull(amf_set_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [amf_set_id]");
        goto end;
    }
    }

    amf_region_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "amfRegionId");
    if (amf_region_id) {
    if (!cJSON_IsString(amf_region_id) && !cJSON_IsNull(amf_region_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [amf_region_id]");
        goto end;
    }
    }

    guami_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "guamiList");
    if (guami_list) {
        cJSON *guami_list_local = NULL;
        if (!cJSON_IsArray(guami_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [guami_list]");
            goto end;
        }

        guami_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(guami_list_local, guami_list) {
            if (!cJSON_IsObject(guami_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [guami_list]");
                goto end;
            }
            OpenAPI_guami_t *guami_listItem = OpenAPI_guami_parseFromJSON(guami_list_local);
            if (!guami_listItem) {
                ogs_error("No guami_listItem");
                goto end;
            }
            OpenAPI_list_add(guami_listList, guami_listItem);
        }
    }

    snssai_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "snssaiList");
    if (snssai_list) {
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }
    }

    nsi_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nsiList");
    if (nsi_list) {
        cJSON *nsi_list_local = NULL;
        if (!cJSON_IsArray(nsi_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nsi_list]");
            goto end;
        }

        nsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_list_local, nsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nsi_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nsi_list]");
                goto end;
            }
            OpenAPI_list_add(nsi_listList, ogs_strdup(nsi_list_local->valuestring));
        }
    }

    nf_group_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfGroupId");
    if (nf_group_id) {
    if (!cJSON_IsString(nf_group_id) && !cJSON_IsNull(nf_group_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_group_id]");
        goto end;
    }
    }

    nf_set_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfSetId");
    if (nf_set_id) {
    if (!cJSON_IsString(nf_set_id) && !cJSON_IsNull(nf_set_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_set_id]");
        goto end;
    }
    }

    nf_service_set_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfServiceSetId");
    if (nf_service_set_id) {
    if (!cJSON_IsString(nf_service_set_id) && !cJSON_IsNull(nf_service_set_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_service_set_id]");
        goto end;
    }
    }

    smf_serving_area = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "smfServingArea");
    if (smf_serving_area) {
        cJSON *smf_serving_area_local = NULL;
        if (!cJSON_IsArray(smf_serving_area)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [smf_serving_area]");
            goto end;
        }

        smf_serving_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_serving_area_local, smf_serving_area) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(smf_serving_area_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [smf_serving_area]");
                goto end;
            }
            OpenAPI_list_add(smf_serving_areaList, ogs_strdup(smf_serving_area_local->valuestring));
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    subscr_cond_local_var = OpenAPI_subscr_cond_create (
        nf_instance_id && !cJSON_IsNull(nf_instance_id) ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        nf_instance_list ? nf_instance_listList : NULL,
        nf_type ? nf_typeVariable : 0,
        service_name && !cJSON_IsNull(service_name) ? ogs_strdup(service_name->valuestring) : NULL,
        service_name_list ? service_name_listList : NULL,
        amf_set_id && !cJSON_IsNull(amf_set_id) ? ogs_strdup(amf_set_id->valuestring) : NULL,
        amf_region_id && !cJSON_IsNull(amf_region_id) ? ogs_strdup(amf_region_id->valuestring) : NULL,
        guami_list ? guami_listList : NULL,
        snssai_list ? snssai_listList : NULL,
        nsi_list ? nsi_listList : NULL,
        nf_group_id && !cJSON_IsNull(nf_group_id) ? ogs_strdup(nf_group_id->valuestring) : NULL,
        nf_set_id && !cJSON_IsNull(nf_set_id) ? ogs_strdup(nf_set_id->valuestring) : NULL,
        nf_service_set_id && !cJSON_IsNull(nf_service_set_id) ? ogs_strdup(nf_service_set_id->valuestring) : NULL,
        smf_serving_area ? smf_serving_areaList : NULL,
        tai_list ? tai_listList : NULL
    );

    return subscr_cond_local_var;
end:
    if (nf_instance_listList) {
        OpenAPI_list_for_each(nf_instance_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_instance_listList);
        nf_instance_listList = NULL;
    }
    if (service_name_listList) {
        OpenAPI_list_for_each(service_name_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_name_listList);
        service_name_listList = NULL;
    }
    if (guami_listList) {
        OpenAPI_list_for_each(guami_listList, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(guami_listList);
        guami_listList = NULL;
    }
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
    }
    if (nsi_listList) {
        OpenAPI_list_for_each(nsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsi_listList);
        nsi_listList = NULL;
    }
    if (smf_serving_areaList) {
        OpenAPI_list_for_each(smf_serving_areaList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smf_serving_areaList);
        smf_serving_areaList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    return NULL;
}

OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_copy(OpenAPI_subscr_cond_t *dst, OpenAPI_subscr_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscr_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed");
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

    OpenAPI_subscr_cond_free(dst);
    dst = OpenAPI_subscr_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

