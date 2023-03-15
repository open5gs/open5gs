
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nef_info.h"

OpenAPI_nef_info_t *OpenAPI_nef_info_create(
    char *nef_id,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_af_event_exposure_data_t *af_ee_data,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *served_fqdn_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *dnai_list,
    OpenAPI_list_t *un_trust_af_info_list,
    bool is_uas_nf_functionality_ind,
    int uas_nf_functionality_ind
)
{
    OpenAPI_nef_info_t *nef_info_local_var = ogs_malloc(sizeof(OpenAPI_nef_info_t));
    ogs_assert(nef_info_local_var);

    nef_info_local_var->nef_id = nef_id;
    nef_info_local_var->pfd_data = pfd_data;
    nef_info_local_var->af_ee_data = af_ee_data;
    nef_info_local_var->gpsi_ranges = gpsi_ranges;
    nef_info_local_var->external_group_identifiers_ranges = external_group_identifiers_ranges;
    nef_info_local_var->served_fqdn_list = served_fqdn_list;
    nef_info_local_var->tai_list = tai_list;
    nef_info_local_var->tai_range_list = tai_range_list;
    nef_info_local_var->dnai_list = dnai_list;
    nef_info_local_var->un_trust_af_info_list = un_trust_af_info_list;
    nef_info_local_var->is_uas_nf_functionality_ind = is_uas_nf_functionality_ind;
    nef_info_local_var->uas_nf_functionality_ind = uas_nf_functionality_ind;

    return nef_info_local_var;
}

void OpenAPI_nef_info_free(OpenAPI_nef_info_t *nef_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nef_info) {
        return;
    }
    if (nef_info->nef_id) {
        ogs_free(nef_info->nef_id);
        nef_info->nef_id = NULL;
    }
    if (nef_info->pfd_data) {
        OpenAPI_pfd_data_free(nef_info->pfd_data);
        nef_info->pfd_data = NULL;
    }
    if (nef_info->af_ee_data) {
        OpenAPI_af_event_exposure_data_free(nef_info->af_ee_data);
        nef_info->af_ee_data = NULL;
    }
    if (nef_info->gpsi_ranges) {
        OpenAPI_list_for_each(nef_info->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(nef_info->gpsi_ranges);
        nef_info->gpsi_ranges = NULL;
    }
    if (nef_info->external_group_identifiers_ranges) {
        OpenAPI_list_for_each(nef_info->external_group_identifiers_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(nef_info->external_group_identifiers_ranges);
        nef_info->external_group_identifiers_ranges = NULL;
    }
    if (nef_info->served_fqdn_list) {
        OpenAPI_list_for_each(nef_info->served_fqdn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nef_info->served_fqdn_list);
        nef_info->served_fqdn_list = NULL;
    }
    if (nef_info->tai_list) {
        OpenAPI_list_for_each(nef_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nef_info->tai_list);
        nef_info->tai_list = NULL;
    }
    if (nef_info->tai_range_list) {
        OpenAPI_list_for_each(nef_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nef_info->tai_range_list);
        nef_info->tai_range_list = NULL;
    }
    if (nef_info->dnai_list) {
        OpenAPI_list_for_each(nef_info->dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nef_info->dnai_list);
        nef_info->dnai_list = NULL;
    }
    if (nef_info->un_trust_af_info_list) {
        OpenAPI_list_for_each(nef_info->un_trust_af_info_list, node) {
            OpenAPI_un_trust_af_info_free(node->data);
        }
        OpenAPI_list_free(nef_info->un_trust_af_info_list);
        nef_info->un_trust_af_info_list = NULL;
    }
    ogs_free(nef_info);
}

cJSON *OpenAPI_nef_info_convertToJSON(OpenAPI_nef_info_t *nef_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nef_info == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [NefInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nef_info->nef_id) {
    if (cJSON_AddStringToObject(item, "nefId", nef_info->nef_id) == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [nef_id]");
        goto end;
    }
    }

    if (nef_info->pfd_data) {
    cJSON *pfd_data_local_JSON = OpenAPI_pfd_data_convertToJSON(nef_info->pfd_data);
    if (pfd_data_local_JSON == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [pfd_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pfdData", pfd_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [pfd_data]");
        goto end;
    }
    }

    if (nef_info->af_ee_data) {
    cJSON *af_ee_data_local_JSON = OpenAPI_af_event_exposure_data_convertToJSON(nef_info->af_ee_data);
    if (af_ee_data_local_JSON == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [af_ee_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afEeData", af_ee_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [af_ee_data]");
        goto end;
    }
    }

    if (nef_info->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (nef_info->external_group_identifiers_ranges) {
    cJSON *external_group_identifiers_rangesList = cJSON_AddArrayToObject(item, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_rangesList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [external_group_identifiers_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->external_group_identifiers_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(external_group_identifiers_rangesList, itemLocal);
    }
    }

    if (nef_info->served_fqdn_list) {
    cJSON *served_fqdn_listList = cJSON_AddArrayToObject(item, "servedFqdnList");
    if (served_fqdn_listList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [served_fqdn_list]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->served_fqdn_list, node) {
        if (cJSON_AddStringToObject(served_fqdn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [served_fqdn_list]");
            goto end;
        }
    }
    }

    if (nef_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nef_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (nef_info->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->dnai_list, node) {
        if (cJSON_AddStringToObject(dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [dnai_list]");
            goto end;
        }
    }
    }

    if (nef_info->un_trust_af_info_list) {
    cJSON *un_trust_af_info_listList = cJSON_AddArrayToObject(item, "unTrustAfInfoList");
    if (un_trust_af_info_listList == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [un_trust_af_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(nef_info->un_trust_af_info_list, node) {
        cJSON *itemLocal = OpenAPI_un_trust_af_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [un_trust_af_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(un_trust_af_info_listList, itemLocal);
    }
    }

    if (nef_info->is_uas_nf_functionality_ind) {
    if (cJSON_AddBoolToObject(item, "uasNfFunctionalityInd", nef_info->uas_nf_functionality_ind) == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [uas_nf_functionality_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nef_info_t *OpenAPI_nef_info_parseFromJSON(cJSON *nef_infoJSON)
{
    OpenAPI_nef_info_t *nef_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nef_id = NULL;
    cJSON *pfd_data = NULL;
    OpenAPI_pfd_data_t *pfd_data_local_nonprim = NULL;
    cJSON *af_ee_data = NULL;
    OpenAPI_af_event_exposure_data_t *af_ee_data_local_nonprim = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *external_group_identifiers_ranges = NULL;
    OpenAPI_list_t *external_group_identifiers_rangesList = NULL;
    cJSON *served_fqdn_list = NULL;
    OpenAPI_list_t *served_fqdn_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    cJSON *un_trust_af_info_list = NULL;
    OpenAPI_list_t *un_trust_af_info_listList = NULL;
    cJSON *uas_nf_functionality_ind = NULL;
    nef_id = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "nefId");
    if (nef_id) {
    if (!cJSON_IsString(nef_id) && !cJSON_IsNull(nef_id)) {
        ogs_error("OpenAPI_nef_info_parseFromJSON() failed [nef_id]");
        goto end;
    }
    }

    pfd_data = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "pfdData");
    if (pfd_data) {
    pfd_data_local_nonprim = OpenAPI_pfd_data_parseFromJSON(pfd_data);
    if (!pfd_data_local_nonprim) {
        ogs_error("OpenAPI_pfd_data_parseFromJSON failed [pfd_data]");
        goto end;
    }
    }

    af_ee_data = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "afEeData");
    if (af_ee_data) {
    af_ee_data_local_nonprim = OpenAPI_af_event_exposure_data_parseFromJSON(af_ee_data);
    if (!af_ee_data_local_nonprim) {
        ogs_error("OpenAPI_af_event_exposure_data_parseFromJSON failed [af_ee_data]");
        goto end;
    }
    }

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [gpsi_ranges]");
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

    external_group_identifiers_ranges = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "externalGroupIdentifiersRanges");
    if (external_group_identifiers_ranges) {
        cJSON *external_group_identifiers_ranges_local = NULL;
        if (!cJSON_IsArray(external_group_identifiers_ranges)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [external_group_identifiers_ranges]");
            goto end;
        }

        external_group_identifiers_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(external_group_identifiers_ranges_local, external_group_identifiers_ranges) {
            if (!cJSON_IsObject(external_group_identifiers_ranges_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [external_group_identifiers_ranges]");
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

    served_fqdn_list = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "servedFqdnList");
    if (served_fqdn_list) {
        cJSON *served_fqdn_list_local = NULL;
        if (!cJSON_IsArray(served_fqdn_list)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [served_fqdn_list]");
            goto end;
        }

        served_fqdn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(served_fqdn_list_local, served_fqdn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(served_fqdn_list_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [served_fqdn_list]");
                goto end;
            }
            OpenAPI_list_add(served_fqdn_listList, ogs_strdup(served_fqdn_list_local->valuestring));
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [tai_range_list]");
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

    dnai_list = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    un_trust_af_info_list = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "unTrustAfInfoList");
    if (un_trust_af_info_list) {
        cJSON *un_trust_af_info_list_local = NULL;
        if (!cJSON_IsArray(un_trust_af_info_list)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [un_trust_af_info_list]");
            goto end;
        }

        un_trust_af_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(un_trust_af_info_list_local, un_trust_af_info_list) {
            if (!cJSON_IsObject(un_trust_af_info_list_local)) {
                ogs_error("OpenAPI_nef_info_parseFromJSON() failed [un_trust_af_info_list]");
                goto end;
            }
            OpenAPI_un_trust_af_info_t *un_trust_af_info_listItem = OpenAPI_un_trust_af_info_parseFromJSON(un_trust_af_info_list_local);
            if (!un_trust_af_info_listItem) {
                ogs_error("No un_trust_af_info_listItem");
                goto end;
            }
            OpenAPI_list_add(un_trust_af_info_listList, un_trust_af_info_listItem);
        }
    }

    uas_nf_functionality_ind = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "uasNfFunctionalityInd");
    if (uas_nf_functionality_ind) {
    if (!cJSON_IsBool(uas_nf_functionality_ind)) {
        ogs_error("OpenAPI_nef_info_parseFromJSON() failed [uas_nf_functionality_ind]");
        goto end;
    }
    }

    nef_info_local_var = OpenAPI_nef_info_create (
        nef_id && !cJSON_IsNull(nef_id) ? ogs_strdup(nef_id->valuestring) : NULL,
        pfd_data ? pfd_data_local_nonprim : NULL,
        af_ee_data ? af_ee_data_local_nonprim : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        external_group_identifiers_ranges ? external_group_identifiers_rangesList : NULL,
        served_fqdn_list ? served_fqdn_listList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        dnai_list ? dnai_listList : NULL,
        un_trust_af_info_list ? un_trust_af_info_listList : NULL,
        uas_nf_functionality_ind ? true : false,
        uas_nf_functionality_ind ? uas_nf_functionality_ind->valueint : 0
    );

    return nef_info_local_var;
end:
    if (pfd_data_local_nonprim) {
        OpenAPI_pfd_data_free(pfd_data_local_nonprim);
        pfd_data_local_nonprim = NULL;
    }
    if (af_ee_data_local_nonprim) {
        OpenAPI_af_event_exposure_data_free(af_ee_data_local_nonprim);
        af_ee_data_local_nonprim = NULL;
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
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    if (un_trust_af_info_listList) {
        OpenAPI_list_for_each(un_trust_af_info_listList, node) {
            OpenAPI_un_trust_af_info_free(node->data);
        }
        OpenAPI_list_free(un_trust_af_info_listList);
        un_trust_af_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_nef_info_t *OpenAPI_nef_info_copy(OpenAPI_nef_info_t *dst, OpenAPI_nef_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nef_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed");
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

    OpenAPI_nef_info_free(dst);
    dst = OpenAPI_nef_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

