
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscr_cond.h"

char *OpenAPI_condition_typesubscr_cond_ToString(OpenAPI_subscr_cond_condition_type_e condition_type)
{
    const char *condition_typeArray[] =  { "NULL", "SERVICE_NAME_LIST_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    if (condition_type < sizeofArray)
        return (char *)condition_typeArray[condition_type];
    else
        return (char *)"Unknown";
}

OpenAPI_subscr_cond_condition_type_e OpenAPI_condition_typesubscr_cond_FromString(char* condition_type)
{
    int stringToReturn = 0;
    const char *condition_typeArray[] =  { "NULL", "SERVICE_NAME_LIST_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(condition_type, condition_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_create(
    char *nf_instance_id,
    OpenAPI_list_t *nf_instance_id_list,
    OpenAPI_nf_type_e nf_type,
    OpenAPI_service_name_e service_name,
    OpenAPI_subscr_cond_condition_type_e condition_type,
    OpenAPI_list_t *service_name_list,
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *nsi_list,
    char *nf_group_id,
    OpenAPI_list_t *nf_group_id_list,
    char *nf_set_id,
    char *nf_service_set_id,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *scp_domains,
    OpenAPI_list_t *nf_type_list,
    OpenAPI_list_t *analytics_ids,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *ml_analytics_list,
    OpenAPI_list_t *af_events,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *served_fqdn_list
)
{
    OpenAPI_subscr_cond_t *subscr_cond_local_var = ogs_malloc(sizeof(OpenAPI_subscr_cond_t));
    ogs_assert(subscr_cond_local_var);

    subscr_cond_local_var->nf_instance_id = nf_instance_id;
    subscr_cond_local_var->nf_instance_id_list = nf_instance_id_list;
    subscr_cond_local_var->nf_type = nf_type;
    subscr_cond_local_var->service_name = service_name;
    subscr_cond_local_var->condition_type = condition_type;
    subscr_cond_local_var->service_name_list = service_name_list;
    subscr_cond_local_var->amf_set_id = amf_set_id;
    subscr_cond_local_var->amf_region_id = amf_region_id;
    subscr_cond_local_var->guami_list = guami_list;
    subscr_cond_local_var->snssai_list = snssai_list;
    subscr_cond_local_var->nsi_list = nsi_list;
    subscr_cond_local_var->nf_group_id = nf_group_id;
    subscr_cond_local_var->nf_group_id_list = nf_group_id_list;
    subscr_cond_local_var->nf_set_id = nf_set_id;
    subscr_cond_local_var->nf_service_set_id = nf_service_set_id;
    subscr_cond_local_var->smf_serving_area = smf_serving_area;
    subscr_cond_local_var->tai_list = tai_list;
    subscr_cond_local_var->scp_domains = scp_domains;
    subscr_cond_local_var->nf_type_list = nf_type_list;
    subscr_cond_local_var->analytics_ids = analytics_ids;
    subscr_cond_local_var->tai_range_list = tai_range_list;
    subscr_cond_local_var->serving_nf_type_list = serving_nf_type_list;
    subscr_cond_local_var->serving_nf_set_id_list = serving_nf_set_id_list;
    subscr_cond_local_var->ml_analytics_list = ml_analytics_list;
    subscr_cond_local_var->af_events = af_events;
    subscr_cond_local_var->pfd_data = pfd_data;
    subscr_cond_local_var->gpsi_ranges = gpsi_ranges;
    subscr_cond_local_var->external_group_identifiers_ranges = external_group_identifiers_ranges;
    subscr_cond_local_var->served_fqdn_list = served_fqdn_list;

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
    if (subscr_cond->nf_instance_id_list) {
        OpenAPI_list_for_each(subscr_cond->nf_instance_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->nf_instance_id_list);
        subscr_cond->nf_instance_id_list = NULL;
    }
    if (subscr_cond->service_name_list) {
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
    if (subscr_cond->nf_group_id_list) {
        OpenAPI_list_for_each(subscr_cond->nf_group_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->nf_group_id_list);
        subscr_cond->nf_group_id_list = NULL;
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
    if (subscr_cond->scp_domains) {
        OpenAPI_list_for_each(subscr_cond->scp_domains, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->scp_domains);
        subscr_cond->scp_domains = NULL;
    }
    if (subscr_cond->nf_type_list) {
        OpenAPI_list_free(subscr_cond->nf_type_list);
        subscr_cond->nf_type_list = NULL;
    }
    if (subscr_cond->analytics_ids) {
        OpenAPI_list_for_each(subscr_cond->analytics_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->analytics_ids);
        subscr_cond->analytics_ids = NULL;
    }
    if (subscr_cond->tai_range_list) {
        OpenAPI_list_for_each(subscr_cond->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->tai_range_list);
        subscr_cond->tai_range_list = NULL;
    }
    if (subscr_cond->serving_nf_type_list) {
        OpenAPI_list_free(subscr_cond->serving_nf_type_list);
        subscr_cond->serving_nf_type_list = NULL;
    }
    if (subscr_cond->serving_nf_set_id_list) {
        OpenAPI_list_for_each(subscr_cond->serving_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->serving_nf_set_id_list);
        subscr_cond->serving_nf_set_id_list = NULL;
    }
    if (subscr_cond->ml_analytics_list) {
        OpenAPI_list_for_each(subscr_cond->ml_analytics_list, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->ml_analytics_list);
        subscr_cond->ml_analytics_list = NULL;
    }
    if (subscr_cond->af_events) {
        OpenAPI_list_free(subscr_cond->af_events);
        subscr_cond->af_events = NULL;
    }
    if (subscr_cond->pfd_data) {
        OpenAPI_pfd_data_free(subscr_cond->pfd_data);
        subscr_cond->pfd_data = NULL;
    }
    if (subscr_cond->gpsi_ranges) {
        OpenAPI_list_for_each(subscr_cond->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->gpsi_ranges);
        subscr_cond->gpsi_ranges = NULL;
    }
    if (subscr_cond->external_group_identifiers_ranges) {
        OpenAPI_list_for_each(subscr_cond->external_group_identifiers_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->external_group_identifiers_ranges);
        subscr_cond->external_group_identifiers_ranges = NULL;
    }
    if (subscr_cond->served_fqdn_list) {
        OpenAPI_list_for_each(subscr_cond->served_fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscr_cond->served_fqdn_list);
        subscr_cond->served_fqdn_list = NULL;
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

    if (subscr_cond->nf_instance_id_list) {
    cJSON *nf_instance_id_listList = cJSON_AddArrayToObject(item, "nfInstanceIdList");
    if (nf_instance_id_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_instance_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->nf_instance_id_list, node) {
        if (cJSON_AddStringToObject(nf_instance_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_instance_id_list]");
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

    if (subscr_cond->service_name != OpenAPI_service_name_NULL) {
    if (cJSON_AddStringToObject(item, "serviceName", OpenAPI_service_name_ToString(subscr_cond->service_name)) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [service_name]");
        goto end;
    }
    }

    if (subscr_cond->condition_type != OpenAPI_subscr_cond_CONDITIONTYPE_NULL) {
    if (cJSON_AddStringToObject(item, "conditionType", OpenAPI_condition_typesubscr_cond_ToString(subscr_cond->condition_type)) == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [condition_type]");
        goto end;
    }
    }

    if (subscr_cond->service_name_list != OpenAPI_service_name_NULL) {
    cJSON *service_name_listList = cJSON_AddArrayToObject(item, "serviceNameList");
    if (service_name_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [service_name_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->service_name_list, node) {
        if (cJSON_AddStringToObject(service_name_listList, "", OpenAPI_service_name_ToString((intptr_t)node->data)) == NULL) {
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

    if (subscr_cond->nf_group_id_list) {
    cJSON *nf_group_id_listList = cJSON_AddArrayToObject(item, "nfGroupIdList");
    if (nf_group_id_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_group_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->nf_group_id_list, node) {
        if (cJSON_AddStringToObject(nf_group_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_group_id_list]");
            goto end;
        }
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

    if (subscr_cond->scp_domains) {
    cJSON *scp_domainsList = cJSON_AddArrayToObject(item, "scpDomains");
    if (scp_domainsList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [scp_domains]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->scp_domains, node) {
        if (cJSON_AddStringToObject(scp_domainsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [scp_domains]");
            goto end;
        }
    }
    }

    if (subscr_cond->nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *nf_type_listList = cJSON_AddArrayToObject(item, "nfTypeList");
    if (nf_type_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->nf_type_list, node) {
        if (cJSON_AddStringToObject(nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [nf_type_list]");
            goto end;
        }
    }
    }

    if (subscr_cond->analytics_ids) {
    cJSON *analytics_idsList = cJSON_AddArrayToObject(item, "analyticsIds");
    if (analytics_idsList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->analytics_ids, node) {
        if (cJSON_AddStringToObject(analytics_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [analytics_ids]");
            goto end;
        }
    }
    }

    if (subscr_cond->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (subscr_cond->serving_nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *serving_nf_type_listList = cJSON_AddArrayToObject(item, "servingNfTypeList");
    if (serving_nf_type_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [serving_nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->serving_nf_type_list, node) {
        if (cJSON_AddStringToObject(serving_nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [serving_nf_type_list]");
            goto end;
        }
    }
    }

    if (subscr_cond->serving_nf_set_id_list) {
    cJSON *serving_nf_set_id_listList = cJSON_AddArrayToObject(item, "servingNfSetIdList");
    if (serving_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [serving_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->serving_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(serving_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [serving_nf_set_id_list]");
            goto end;
        }
    }
    }

    if (subscr_cond->ml_analytics_list) {
    cJSON *ml_analytics_listList = cJSON_AddArrayToObject(item, "mlAnalyticsList");
    if (ml_analytics_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [ml_analytics_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->ml_analytics_list, node) {
        cJSON *itemLocal = OpenAPI_ml_analytics_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [ml_analytics_list]");
            goto end;
        }
        cJSON_AddItemToArray(ml_analytics_listList, itemLocal);
    }
    }

    if (subscr_cond->af_events != OpenAPI_af_event_NULL) {
    cJSON *af_eventsList = cJSON_AddArrayToObject(item, "afEvents");
    if (af_eventsList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [af_events]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->af_events, node) {
        if (cJSON_AddStringToObject(af_eventsList, "", OpenAPI_af_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [af_events]");
            goto end;
        }
    }
    }

    if (subscr_cond->pfd_data) {
    cJSON *pfd_data_local_JSON = OpenAPI_pfd_data_convertToJSON(subscr_cond->pfd_data);
    if (pfd_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [pfd_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pfdData", pfd_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [pfd_data]");
        goto end;
    }
    }

    if (subscr_cond->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (subscr_cond->external_group_identifiers_ranges) {
    cJSON *external_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [external_group_identifiers_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->external_group_identifiers_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(external_group_identifiers_rangesList, itemLocal);
    }
    }

    if (subscr_cond->served_fqdn_list) {
    cJSON *served_fqdn_listList = cJSON_AddArrayToObject(item, "servedFqdnList");
    if (served_fqdn_listList == NULL) {
        ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [served_fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(subscr_cond->served_fqdn_list, node) {
        if (cJSON_AddStringToObject(served_fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscr_cond_convertToJSON() failed [served_fqdn_list]");
            goto end;
        }
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
    cJSON *nf_instance_id_list = NULL;
    OpenAPI_list_t *nf_instance_id_listList = NULL;
    cJSON *nf_type = NULL;
    OpenAPI_nf_type_e nf_typeVariable = 0;
    cJSON *service_name = NULL;
    OpenAPI_service_name_e service_nameVariable = 0;
    cJSON *condition_type = NULL;
    OpenAPI_subscr_cond_condition_type_e condition_typeVariable = 0;
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
    cJSON *nf_group_id_list = NULL;
    OpenAPI_list_t *nf_group_id_listList = NULL;
    cJSON *nf_set_id = NULL;
    cJSON *nf_service_set_id = NULL;
    cJSON *smf_serving_area = NULL;
    OpenAPI_list_t *smf_serving_areaList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *scp_domains = NULL;
    OpenAPI_list_t *scp_domainsList = NULL;
    cJSON *nf_type_list = NULL;
    OpenAPI_list_t *nf_type_listList = NULL;
    cJSON *analytics_ids = NULL;
    OpenAPI_list_t *analytics_idsList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *serving_nf_type_list = NULL;
    OpenAPI_list_t *serving_nf_type_listList = NULL;
    cJSON *serving_nf_set_id_list = NULL;
    OpenAPI_list_t *serving_nf_set_id_listList = NULL;
    cJSON *ml_analytics_list = NULL;
    OpenAPI_list_t *ml_analytics_listList = NULL;
    cJSON *af_events = NULL;
    OpenAPI_list_t *af_eventsList = NULL;
    cJSON *pfd_data = NULL;
    OpenAPI_pfd_data_t *pfd_data_local_nonprim = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *external_group_identifiers_ranges = NULL;
    OpenAPI_list_t *external_group_identifiers_rangesList = NULL;
    cJSON *served_fqdn_list = NULL;
    OpenAPI_list_t *served_fqdn_listList = NULL;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfInstanceId");
    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id) && !cJSON_IsNull(nf_instance_id)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    nf_instance_id_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfInstanceIdList");
    if (nf_instance_id_list) {
        cJSON *nf_instance_id_list_local = NULL;
        if (!cJSON_IsArray(nf_instance_id_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_instance_id_list]");
            goto end;
        }

        nf_instance_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instance_id_list_local, nf_instance_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_instance_id_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_instance_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_instance_id_listList, ogs_strdup(nf_instance_id_list_local->valuestring));
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
    if (!cJSON_IsString(service_name)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [service_name]");
        goto end;
    }
    service_nameVariable = OpenAPI_service_name_FromString(service_name->valuestring);
    }

    condition_type = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "conditionType");
    if (condition_type) {
    if (!cJSON_IsString(condition_type)) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    condition_typeVariable = OpenAPI_condition_typesubscr_cond_FromString(condition_type->valuestring);
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
            OpenAPI_service_name_e localEnum = OpenAPI_service_name_NULL;
            if (!cJSON_IsString(service_name_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [service_name_list]");
                goto end;
            }
            localEnum = OpenAPI_service_name_FromString(service_name_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"service_name_list\" is not supported. Ignoring it ...",
                         service_name_list_local->valuestring);
            } else {
                OpenAPI_list_add(service_name_listList, (void *)localEnum);
            }
        }
        if (service_name_listList->count == 0) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed: Expected service_name_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
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

    nf_group_id_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfGroupIdList");
    if (nf_group_id_list) {
        cJSON *nf_group_id_list_local = NULL;
        if (!cJSON_IsArray(nf_group_id_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_group_id_list]");
            goto end;
        }

        nf_group_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_group_id_list_local, nf_group_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_group_id_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_group_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_group_id_listList, ogs_strdup(nf_group_id_list_local->valuestring));
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

    scp_domains = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "scpDomains");
    if (scp_domains) {
        cJSON *scp_domains_local = NULL;
        if (!cJSON_IsArray(scp_domains)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [scp_domains]");
            goto end;
        }

        scp_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(scp_domains_local, scp_domains) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(scp_domains_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [scp_domains]");
                goto end;
            }
            OpenAPI_list_add(scp_domainsList, ogs_strdup(scp_domains_local->valuestring));
        }
    }

    nf_type_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "nfTypeList");
    if (nf_type_list) {
        cJSON *nf_type_list_local = NULL;
        if (!cJSON_IsArray(nf_type_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_type_list]");
            goto end;
        }

        nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_type_list_local, nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(nf_type_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [nf_type_list]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(nf_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"nf_type_list\" is not supported. Ignoring it ...",
                         nf_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(nf_type_listList, (void *)localEnum);
            }
        }
        if (nf_type_listList->count == 0) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed: Expected nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    analytics_ids = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "analyticsIds");
    if (analytics_ids) {
        cJSON *analytics_ids_local = NULL;
        if (!cJSON_IsArray(analytics_ids)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [analytics_ids]");
            goto end;
        }

        analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_ids_local, analytics_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(analytics_ids_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [analytics_ids]");
                goto end;
            }
            OpenAPI_list_add(analytics_idsList, ogs_strdup(analytics_ids_local->valuestring));
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    serving_nf_type_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "servingNfTypeList");
    if (serving_nf_type_list) {
        cJSON *serving_nf_type_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_type_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [serving_nf_type_list]");
            goto end;
        }

        serving_nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_type_list_local, serving_nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(serving_nf_type_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [serving_nf_type_list]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(serving_nf_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_nf_type_list\" is not supported. Ignoring it ...",
                         serving_nf_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(serving_nf_type_listList, (void *)localEnum);
            }
        }
        if (serving_nf_type_listList->count == 0) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed: Expected serving_nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "servingNfSetIdList");
    if (serving_nf_set_id_list) {
        cJSON *serving_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_id_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [serving_nf_set_id_list]");
            goto end;
        }

        serving_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_id_list_local, serving_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_id_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [serving_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_id_listList, ogs_strdup(serving_nf_set_id_list_local->valuestring));
        }
    }

    ml_analytics_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "mlAnalyticsList");
    if (ml_analytics_list) {
        cJSON *ml_analytics_list_local = NULL;
        if (!cJSON_IsArray(ml_analytics_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [ml_analytics_list]");
            goto end;
        }

        ml_analytics_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_list_local, ml_analytics_list) {
            if (!cJSON_IsObject(ml_analytics_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [ml_analytics_list]");
                goto end;
            }
            OpenAPI_ml_analytics_info_t *ml_analytics_listItem = OpenAPI_ml_analytics_info_parseFromJSON(ml_analytics_list_local);
            if (!ml_analytics_listItem) {
                ogs_error("No ml_analytics_listItem");
                goto end;
            }
            OpenAPI_list_add(ml_analytics_listList, ml_analytics_listItem);
        }
    }

    af_events = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "afEvents");
    if (af_events) {
        cJSON *af_events_local = NULL;
        if (!cJSON_IsArray(af_events)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [af_events]");
            goto end;
        }

        af_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_events_local, af_events) {
            OpenAPI_af_event_e localEnum = OpenAPI_af_event_NULL;
            if (!cJSON_IsString(af_events_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [af_events]");
                goto end;
            }
            localEnum = OpenAPI_af_event_FromString(af_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"af_events\" is not supported. Ignoring it ...",
                         af_events_local->valuestring);
            } else {
                OpenAPI_list_add(af_eventsList, (void *)localEnum);
            }
        }
        if (af_eventsList->count == 0) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed: Expected af_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pfd_data = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "pfdData");
    if (pfd_data) {
    pfd_data_local_nonprim = OpenAPI_pfd_data_parseFromJSON(pfd_data);
    if (!pfd_data_local_nonprim) {
        ogs_error("OpenAPI_pfd_data_parseFromJSON failed [pfd_data]");
        goto end;
    }
    }

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local);
            if (!gpsi_rangesItem) {
                ogs_error("No gpsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    external_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_ranges) {
        cJSON *external_group_identifiers_ranges_local = NULL;
        if (!cJSON_IsArray(external_group_identifiers_ranges)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }

        external_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(external_group_identifiers_ranges_local, external_group_identifiers_ranges) {
            if (!cJSON_IsObject(external_group_identifiers_ranges_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [external_group_identifiers_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *external_group_identifiers_rangesItem = OpenAPI_identity_range_parseFromJSON(external_group_identifiers_ranges_local);
            if (!external_group_identifiers_rangesItem) {
                ogs_error("No external_group_identifiers_rangesItem");
                goto end;
            }
            OpenAPI_list_add(external_group_identifiers_rangesList, external_group_identifiers_rangesItem);
        }
    }

    served_fqdn_list = cJSON_GetObjectItemCaseSensitive(subscr_condJSON, "servedFqdnList");
    if (served_fqdn_list) {
        cJSON *served_fqdn_list_local = NULL;
        if (!cJSON_IsArray(served_fqdn_list)) {
            ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [served_fqdn_list]");
            goto end;
        }

        served_fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(served_fqdn_list_local, served_fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(served_fqdn_list_local)) {
                ogs_error("OpenAPI_subscr_cond_parseFromJSON() failed [served_fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(served_fqdn_listList, ogs_strdup(served_fqdn_list_local->valuestring));
        }
    }

    subscr_cond_local_var = OpenAPI_subscr_cond_create (
        nf_instance_id && !cJSON_IsNull(nf_instance_id) ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        nf_instance_id_list ? nf_instance_id_listList : NULL,
        nf_type ? nf_typeVariable : 0,
        service_name ? service_nameVariable : 0,
        condition_type ? condition_typeVariable : 0,
        service_name_list ? service_name_listList : NULL,
        amf_set_id && !cJSON_IsNull(amf_set_id) ? ogs_strdup(amf_set_id->valuestring) : NULL,
        amf_region_id && !cJSON_IsNull(amf_region_id) ? ogs_strdup(amf_region_id->valuestring) : NULL,
        guami_list ? guami_listList : NULL,
        snssai_list ? snssai_listList : NULL,
        nsi_list ? nsi_listList : NULL,
        nf_group_id && !cJSON_IsNull(nf_group_id) ? ogs_strdup(nf_group_id->valuestring) : NULL,
        nf_group_id_list ? nf_group_id_listList : NULL,
        nf_set_id && !cJSON_IsNull(nf_set_id) ? ogs_strdup(nf_set_id->valuestring) : NULL,
        nf_service_set_id && !cJSON_IsNull(nf_service_set_id) ? ogs_strdup(nf_service_set_id->valuestring) : NULL,
        smf_serving_area ? smf_serving_areaList : NULL,
        tai_list ? tai_listList : NULL,
        scp_domains ? scp_domainsList : NULL,
        nf_type_list ? nf_type_listList : NULL,
        analytics_ids ? analytics_idsList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        serving_nf_type_list ? serving_nf_type_listList : NULL,
        serving_nf_set_id_list ? serving_nf_set_id_listList : NULL,
        ml_analytics_list ? ml_analytics_listList : NULL,
        af_events ? af_eventsList : NULL,
        pfd_data ? pfd_data_local_nonprim : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        external_group_identifiers_ranges ? external_group_identifiers_rangesList : NULL,
        served_fqdn_list ? served_fqdn_listList : NULL
    );

    return subscr_cond_local_var;
end:
    if (nf_instance_id_listList) {
        OpenAPI_list_for_each(nf_instance_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_instance_id_listList);
        nf_instance_id_listList = NULL;
    }
    if (service_name_listList) {
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
    if (nf_group_id_listList) {
        OpenAPI_list_for_each(nf_group_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_group_id_listList);
        nf_group_id_listList = NULL;
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
    if (scp_domainsList) {
        OpenAPI_list_for_each(scp_domainsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_domainsList);
        scp_domainsList = NULL;
    }
    if (nf_type_listList) {
        OpenAPI_list_free(nf_type_listList);
        nf_type_listList = NULL;
    }
    if (analytics_idsList) {
        OpenAPI_list_for_each(analytics_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_idsList);
        analytics_idsList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (serving_nf_type_listList) {
        OpenAPI_list_free(serving_nf_type_listList);
        serving_nf_type_listList = NULL;
    }
    if (serving_nf_set_id_listList) {
        OpenAPI_list_for_each(serving_nf_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(serving_nf_set_id_listList);
        serving_nf_set_id_listList = NULL;
    }
    if (ml_analytics_listList) {
        OpenAPI_list_for_each(ml_analytics_listList, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_listList);
        ml_analytics_listList = NULL;
    }
    if (af_eventsList) {
        OpenAPI_list_free(af_eventsList);
        af_eventsList = NULL;
    }
    if (pfd_data_local_nonprim) {
        OpenAPI_pfd_data_free(pfd_data_local_nonprim);
        pfd_data_local_nonprim = NULL;
    }
    if (gpsi_rangesList) {
        OpenAPI_list_for_each(gpsi_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_rangesList);
        gpsi_rangesList = NULL;
    }
    if (external_group_identifiers_rangesList) {
        OpenAPI_list_for_each(external_group_identifiers_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(external_group_identifiers_rangesList);
        external_group_identifiers_rangesList = NULL;
    }
    if (served_fqdn_listList) {
        OpenAPI_list_for_each(served_fqdn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(served_fqdn_listList);
        served_fqdn_listList = NULL;
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

