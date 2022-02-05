
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nef_cond.h"

char *OpenAPI_condition_typenef_cond_ToString(OpenAPI_nef_cond_condition_type_e condition_type)
{
    const char *condition_typeArray[] =  { "NULL", "NEF_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    if (condition_type < sizeofArray)
        return (char *)condition_typeArray[condition_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nef_cond_condition_type_e OpenAPI_condition_typenef_cond_FromString(char* condition_type)
{
    int stringToReturn = 0;
    const char *condition_typeArray[] =  { "NULL", "NEF_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(condition_type, condition_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_nef_cond_t *OpenAPI_nef_cond_create(
    OpenAPI_nef_cond_condition_type_e condition_type,
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *snssai_list,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *served_fqdn_list
)
{
    OpenAPI_nef_cond_t *nef_cond_local_var = ogs_malloc(sizeof(OpenAPI_nef_cond_t));
    ogs_assert(nef_cond_local_var);

    nef_cond_local_var->condition_type = condition_type;
    nef_cond_local_var->af_events = af_events;
    nef_cond_local_var->snssai_list = snssai_list;
    nef_cond_local_var->pfd_data = pfd_data;
    nef_cond_local_var->gpsi_ranges = gpsi_ranges;
    nef_cond_local_var->external_group_identifiers_ranges = external_group_identifiers_ranges;
    nef_cond_local_var->served_fqdn_list = served_fqdn_list;

    return nef_cond_local_var;
}

void OpenAPI_nef_cond_free(OpenAPI_nef_cond_t *nef_cond)
{
    if (NULL == nef_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(nef_cond->af_events);
    OpenAPI_list_for_each(nef_cond->snssai_list, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nef_cond->snssai_list);
    OpenAPI_pfd_data_free(nef_cond->pfd_data);
    OpenAPI_list_for_each(nef_cond->gpsi_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(nef_cond->gpsi_ranges);
    OpenAPI_list_for_each(nef_cond->external_group_identifiers_ranges, node) {
        OpenAPI_identity_range_free(node->data);
    }
    OpenAPI_list_free(nef_cond->external_group_identifiers_ranges);
    OpenAPI_list_for_each(nef_cond->served_fqdn_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nef_cond->served_fqdn_list);
    ogs_free(nef_cond);
}

cJSON *OpenAPI_nef_cond_convertToJSON(OpenAPI_nef_cond_t *nef_cond)
{
    cJSON *item = NULL;

    if (nef_cond == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [NefCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "conditionType", OpenAPI_condition_typenef_cond_ToString(nef_cond->condition_type)) == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [condition_type]");
        goto end;
    }

    if (nef_cond->af_events) {
    cJSON *af_events = cJSON_AddArrayToObject(item, "afEvents");
    if (af_events == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [af_events]");
        goto end;
    }
    OpenAPI_lnode_t *af_events_node;
    OpenAPI_list_for_each(nef_cond->af_events, af_events_node) {
        if (cJSON_AddStringToObject(af_events, "", OpenAPI_af_event_ToString((intptr_t)af_events_node->data)) == NULL) {
            ogs_error("OpenAPI_nef_cond_convertToJSON() failed [af_events]");
            goto end;
        }
    }
    }

    if (nef_cond->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [snssai_list]");
        goto end;
    }

    OpenAPI_lnode_t *snssai_list_node;
    if (nef_cond->snssai_list) {
        OpenAPI_list_for_each(nef_cond->snssai_list, snssai_list_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(snssai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nef_cond_convertToJSON() failed [snssai_list]");
                goto end;
            }
            cJSON_AddItemToArray(snssai_listList, itemLocal);
        }
    }
    }

    if (nef_cond->pfd_data) {
    cJSON *pfd_data_local_JSON = OpenAPI_pfd_data_convertToJSON(nef_cond->pfd_data);
    if (pfd_data_local_JSON == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [pfd_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pfdData", pfd_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [pfd_data]");
        goto end;
    }
    }

    if (nef_cond->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *gpsi_ranges_node;
    if (nef_cond->gpsi_ranges) {
        OpenAPI_list_for_each(nef_cond->gpsi_ranges, gpsi_ranges_node) {
            cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(gpsi_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nef_cond_convertToJSON() failed [gpsi_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
        }
    }
    }

    if (nef_cond->external_group_identifiers_ranges) {
    cJSON *external_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [external_group_identifiers_ranges]");
        goto end;
    }

    OpenAPI_lnode_t *external_group_identifiers_ranges_node;
    if (nef_cond->external_group_identifiers_ranges) {
        OpenAPI_list_for_each(nef_cond->external_group_identifiers_ranges, external_group_identifiers_ranges_node) {
            cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(external_group_identifiers_ranges_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nef_cond_convertToJSON() failed [external_group_identifiers_ranges]");
                goto end;
            }
            cJSON_AddItemToArray(external_group_identifiers_rangesList, itemLocal);
        }
    }
    }

    if (nef_cond->served_fqdn_list) {
    cJSON *served_fqdn_list = cJSON_AddArrayToObject(item, "servedFqdnList");
    if (served_fqdn_list == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [served_fqdn_list]");
        goto end;
    }

    OpenAPI_lnode_t *served_fqdn_list_node;
    OpenAPI_list_for_each(nef_cond->served_fqdn_list, served_fqdn_list_node)  {
    if (cJSON_AddStringToObject(served_fqdn_list, "", (char*)served_fqdn_list_node->data) == NULL) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed [served_fqdn_list]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_nef_cond_t *OpenAPI_nef_cond_parseFromJSON(cJSON *nef_condJSON)
{
    OpenAPI_nef_cond_t *nef_cond_local_var = NULL;
    cJSON *condition_type = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "conditionType");
    if (!condition_type) {
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }

    OpenAPI_nef_cond_condition_type_e condition_typeVariable;
    if (!cJSON_IsString(condition_type)) {
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    condition_typeVariable = OpenAPI_condition_typenef_cond_FromString(condition_type->valuestring);

    cJSON *af_events = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "afEvents");

    OpenAPI_list_t *af_eventsList;
    if (af_events) {
    cJSON *af_events_local_nonprimitive;
    if (!cJSON_IsArray(af_events)) {
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [af_events]");
        goto end;
    }

    af_eventsList = OpenAPI_list_create();

    cJSON_ArrayForEach(af_events_local_nonprimitive, af_events ) {
        if (!cJSON_IsString(af_events_local_nonprimitive)){
            ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [af_events]");
            goto end;
        }

        OpenAPI_list_add(af_eventsList, (void *)OpenAPI_af_event_FromString(af_events_local_nonprimitive->valuestring));
    }
    }

    cJSON *snssai_list = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "snssaiList");

    OpenAPI_list_t *snssai_listList;
    if (snssai_list) {
    cJSON *snssai_list_local_nonprimitive;
    if (!cJSON_IsArray(snssai_list)){
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [snssai_list]");
        goto end;
    }

    snssai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssai_list_local_nonprimitive, snssai_list ) {
        if (!cJSON_IsObject(snssai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [snssai_list]");
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

    cJSON *pfd_data = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "pfdData");

    OpenAPI_pfd_data_t *pfd_data_local_nonprim = NULL;
    if (pfd_data) {
    pfd_data_local_nonprim = OpenAPI_pfd_data_parseFromJSON(pfd_data);
    }

    cJSON *gpsi_ranges = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "gpsiRanges");

    OpenAPI_list_t *gpsi_rangesList;
    if (gpsi_ranges) {
    cJSON *gpsi_ranges_local_nonprimitive;
    if (!cJSON_IsArray(gpsi_ranges)){
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [gpsi_ranges]");
        goto end;
    }

    gpsi_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(gpsi_ranges_local_nonprimitive, gpsi_ranges ) {
        if (!cJSON_IsObject(gpsi_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }
        OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local_nonprimitive);

        if (!gpsi_rangesItem) {
            ogs_error("No gpsi_rangesItem");
            OpenAPI_list_free(gpsi_rangesList);
            goto end;
        }

        OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
    }
    }

    cJSON *external_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "externalGroupIdentifiersRanges");

    OpenAPI_list_t *external_group_identifiers_rangesList;
    if (external_group_identifiers_ranges) {
    cJSON *external_group_identifiers_ranges_local_nonprimitive;
    if (!cJSON_IsArray(external_group_identifiers_ranges)){
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [external_group_identifiers_ranges]");
        goto end;
    }

    external_group_identifiers_rangesList = OpenAPI_list_create();

    cJSON_ArrayForEach(external_group_identifiers_ranges_local_nonprimitive, external_group_identifiers_ranges ) {
        if (!cJSON_IsObject(external_group_identifiers_ranges_local_nonprimitive)) {
            ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }
        OpenAPI_identity_range_t *external_group_identifiers_rangesItem = OpenAPI_identity_range_parseFromJSON(external_group_identifiers_ranges_local_nonprimitive);

        if (!external_group_identifiers_rangesItem) {
            ogs_error("No external_group_identifiers_rangesItem");
            OpenAPI_list_free(external_group_identifiers_rangesList);
            goto end;
        }

        OpenAPI_list_add(external_group_identifiers_rangesList, external_group_identifiers_rangesItem);
    }
    }

    cJSON *served_fqdn_list = cJSON_GetObjectItemCaseSensitive(nef_condJSON, "servedFqdnList");

    OpenAPI_list_t *served_fqdn_listList;
    if (served_fqdn_list) {
    cJSON *served_fqdn_list_local;
    if (!cJSON_IsArray(served_fqdn_list)) {
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [served_fqdn_list]");
        goto end;
    }
    served_fqdn_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(served_fqdn_list_local, served_fqdn_list) {
    if (!cJSON_IsString(served_fqdn_list_local)) {
        ogs_error("OpenAPI_nef_cond_parseFromJSON() failed [served_fqdn_list]");
        goto end;
    }
    OpenAPI_list_add(served_fqdn_listList , ogs_strdup(served_fqdn_list_local->valuestring));
    }
    }

    nef_cond_local_var = OpenAPI_nef_cond_create (
        condition_typeVariable,
        af_events ? af_eventsList : NULL,
        snssai_list ? snssai_listList : NULL,
        pfd_data ? pfd_data_local_nonprim : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        external_group_identifiers_ranges ? external_group_identifiers_rangesList : NULL,
        served_fqdn_list ? served_fqdn_listList : NULL
    );

    return nef_cond_local_var;
end:
    return NULL;
}

OpenAPI_nef_cond_t *OpenAPI_nef_cond_copy(OpenAPI_nef_cond_t *dst, OpenAPI_nef_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nef_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nef_cond_convertToJSON() failed");
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

    OpenAPI_nef_cond_free(dst);
    dst = OpenAPI_nef_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

