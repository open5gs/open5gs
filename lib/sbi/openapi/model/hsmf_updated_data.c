
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hsmf_updated_data.h"

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_create(
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_list_t *dnai_list,
    char *supported_features,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    int ipv6_multi_homing_ind
    )
{
    OpenAPI_hsmf_updated_data_t *hsmf_updated_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_hsmf_updated_data_t));
    if (!hsmf_updated_data_local_var) {
        return NULL;
    }
    hsmf_updated_data_local_var->n1_sm_info_to_ue = n1_sm_info_to_ue;
    hsmf_updated_data_local_var->n4_info = n4_info;
    hsmf_updated_data_local_var->n4_info_ext1 = n4_info_ext1;
    hsmf_updated_data_local_var->n4_info_ext2 = n4_info_ext2;
    hsmf_updated_data_local_var->dnai_list = dnai_list;
    hsmf_updated_data_local_var->supported_features = supported_features;
    hsmf_updated_data_local_var->roaming_charging_profile = roaming_charging_profile;
    hsmf_updated_data_local_var->ipv6_multi_homing_ind = ipv6_multi_homing_ind;

    return hsmf_updated_data_local_var;
}

void OpenAPI_hsmf_updated_data_free(OpenAPI_hsmf_updated_data_t *hsmf_updated_data)
{
    if (NULL == hsmf_updated_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ref_to_binary_data_free(hsmf_updated_data->n1_sm_info_to_ue);
    OpenAPI_n4_information_free(hsmf_updated_data->n4_info);
    OpenAPI_n4_information_free(hsmf_updated_data->n4_info_ext1);
    OpenAPI_n4_information_free(hsmf_updated_data->n4_info_ext2);
    OpenAPI_list_for_each(hsmf_updated_data->dnai_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(hsmf_updated_data->dnai_list);
    ogs_free(hsmf_updated_data->supported_features);
    OpenAPI_roaming_charging_profile_free(hsmf_updated_data->roaming_charging_profile);
    ogs_free(hsmf_updated_data);
}

cJSON *OpenAPI_hsmf_updated_data_convertToJSON(OpenAPI_hsmf_updated_data_t *hsmf_updated_data)
{
    cJSON *item = NULL;

    if (hsmf_updated_data == NULL) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [HsmfUpdatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hsmf_updated_data->n1_sm_info_to_ue) {
        cJSON *n1_sm_info_to_ue_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(hsmf_updated_data->n1_sm_info_to_ue);
        if (n1_sm_info_to_ue_local_JSON == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n1_sm_info_to_ue]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n1SmInfoToUe", n1_sm_info_to_ue_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n1_sm_info_to_ue]");
            goto end;
        }
    }

    if (hsmf_updated_data->n4_info) {
        cJSON *n4_info_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_updated_data->n4_info);
        if (n4_info_local_JSON == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n4Info", n4_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info]");
            goto end;
        }
    }

    if (hsmf_updated_data->n4_info_ext1) {
        cJSON *n4_info_ext1_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_updated_data->n4_info_ext1);
        if (n4_info_ext1_local_JSON == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext1]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n4InfoExt1", n4_info_ext1_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext1]");
            goto end;
        }
    }

    if (hsmf_updated_data->n4_info_ext2) {
        cJSON *n4_info_ext2_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_updated_data->n4_info_ext2);
        if (n4_info_ext2_local_JSON == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext2]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n4InfoExt2", n4_info_ext2_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [n4_info_ext2]");
            goto end;
        }
    }

    if (hsmf_updated_data->dnai_list) {
        cJSON *dnai_list = cJSON_AddArrayToObject(item, "dnaiList");
        if (dnai_list == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [dnai_list]");
            goto end;
        }

        OpenAPI_lnode_t *dnai_list_node;
        OpenAPI_list_for_each(hsmf_updated_data->dnai_list, dnai_list_node)  {
            if (cJSON_AddStringToObject(dnai_list, "", (char*)dnai_list_node->data) == NULL) {
                ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [dnai_list]");
                goto end;
            }
        }
    }

    if (hsmf_updated_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", hsmf_updated_data->supported_features) == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (hsmf_updated_data->roaming_charging_profile) {
        cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(hsmf_updated_data->roaming_charging_profile);
        if (roaming_charging_profile_local_JSON == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [roaming_charging_profile]");
            goto end;
        }
        cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [roaming_charging_profile]");
            goto end;
        }
    }

    if (hsmf_updated_data->ipv6_multi_homing_ind) {
        if (cJSON_AddBoolToObject(item, "ipv6MultiHomingInd", hsmf_updated_data->ipv6_multi_homing_ind) == NULL) {
            ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed [ipv6_multi_homing_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_parseFromJSON(cJSON *hsmf_updated_dataJSON)
{
    OpenAPI_hsmf_updated_data_t *hsmf_updated_data_local_var = NULL;
    cJSON *n1_sm_info_to_ue = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n1SmInfoToUe");

    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue_local_nonprim = NULL;
    if (n1_sm_info_to_ue) {
        n1_sm_info_to_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_to_ue);
    }

    cJSON *n4_info = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n4Info");

    OpenAPI_n4_information_t *n4_info_local_nonprim = NULL;
    if (n4_info) {
        n4_info_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info);
    }

    cJSON *n4_info_ext1 = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n4InfoExt1");

    OpenAPI_n4_information_t *n4_info_ext1_local_nonprim = NULL;
    if (n4_info_ext1) {
        n4_info_ext1_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext1);
    }

    cJSON *n4_info_ext2 = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "n4InfoExt2");

    OpenAPI_n4_information_t *n4_info_ext2_local_nonprim = NULL;
    if (n4_info_ext2) {
        n4_info_ext2_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext2);
    }

    cJSON *dnai_list = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "dnaiList");

    OpenAPI_list_t *dnai_listList;
    if (dnai_list) {
        cJSON *dnai_list_local;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [dnai_list]");
            goto end;
        }
        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "roamingChargingProfile");

    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    if (roaming_charging_profile) {
        roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    }

    cJSON *ipv6_multi_homing_ind = cJSON_GetObjectItemCaseSensitive(hsmf_updated_dataJSON, "ipv6MultiHomingInd");

    if (ipv6_multi_homing_ind) {
        if (!cJSON_IsBool(ipv6_multi_homing_ind)) {
            ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON() failed [ipv6_multi_homing_ind]");
            goto end;
        }
    }

    hsmf_updated_data_local_var = OpenAPI_hsmf_updated_data_create (
        n1_sm_info_to_ue ? n1_sm_info_to_ue_local_nonprim : NULL,
        n4_info ? n4_info_local_nonprim : NULL,
        n4_info_ext1 ? n4_info_ext1_local_nonprim : NULL,
        n4_info_ext2 ? n4_info_ext2_local_nonprim : NULL,
        dnai_list ? dnai_listList : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        ipv6_multi_homing_ind ? ipv6_multi_homing_ind->valueint : 0
        );

    return hsmf_updated_data_local_var;
end:
    return NULL;
}

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_copy(OpenAPI_hsmf_updated_data_t *dst, OpenAPI_hsmf_updated_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hsmf_updated_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hsmf_updated_data_convertToJSON() failed");
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

    OpenAPI_hsmf_updated_data_free(dst);
    dst = OpenAPI_hsmf_updated_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

