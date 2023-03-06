
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "psa_information.h"

OpenAPI_psa_information_t *OpenAPI_psa_information_create(
    OpenAPI_psa_indication_e psa_ind,
    OpenAPI_list_t *dnai_list,
    char *ue_ipv6_prefix,
    char *psa_upf_id
)
{
    OpenAPI_psa_information_t *psa_information_local_var = ogs_malloc(sizeof(OpenAPI_psa_information_t));
    ogs_assert(psa_information_local_var);

    psa_information_local_var->psa_ind = psa_ind;
    psa_information_local_var->dnai_list = dnai_list;
    psa_information_local_var->ue_ipv6_prefix = ue_ipv6_prefix;
    psa_information_local_var->psa_upf_id = psa_upf_id;

    return psa_information_local_var;
}

void OpenAPI_psa_information_free(OpenAPI_psa_information_t *psa_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == psa_information) {
        return;
    }
    if (psa_information->dnai_list) {
        OpenAPI_list_for_each(psa_information->dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(psa_information->dnai_list);
        psa_information->dnai_list = NULL;
    }
    if (psa_information->ue_ipv6_prefix) {
        ogs_free(psa_information->ue_ipv6_prefix);
        psa_information->ue_ipv6_prefix = NULL;
    }
    if (psa_information->psa_upf_id) {
        ogs_free(psa_information->psa_upf_id);
        psa_information->psa_upf_id = NULL;
    }
    ogs_free(psa_information);
}

cJSON *OpenAPI_psa_information_convertToJSON(OpenAPI_psa_information_t *psa_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (psa_information == NULL) {
        ogs_error("OpenAPI_psa_information_convertToJSON() failed [PsaInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (psa_information->psa_ind != OpenAPI_psa_indication_NULL) {
    if (cJSON_AddStringToObject(item, "psaInd", OpenAPI_psa_indication_ToString(psa_information->psa_ind)) == NULL) {
        ogs_error("OpenAPI_psa_information_convertToJSON() failed [psa_ind]");
        goto end;
    }
    }

    if (psa_information->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_psa_information_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(psa_information->dnai_list, node) {
        if (cJSON_AddStringToObject(dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_psa_information_convertToJSON() failed [dnai_list]");
            goto end;
        }
    }
    }

    if (psa_information->ue_ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ueIpv6Prefix", psa_information->ue_ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_psa_information_convertToJSON() failed [ue_ipv6_prefix]");
        goto end;
    }
    }

    if (psa_information->psa_upf_id) {
    if (cJSON_AddStringToObject(item, "psaUpfId", psa_information->psa_upf_id) == NULL) {
        ogs_error("OpenAPI_psa_information_convertToJSON() failed [psa_upf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_psa_information_t *OpenAPI_psa_information_parseFromJSON(cJSON *psa_informationJSON)
{
    OpenAPI_psa_information_t *psa_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *psa_ind = NULL;
    OpenAPI_psa_indication_e psa_indVariable = 0;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    cJSON *ue_ipv6_prefix = NULL;
    cJSON *psa_upf_id = NULL;
    psa_ind = cJSON_GetObjectItemCaseSensitive(psa_informationJSON, "psaInd");
    if (psa_ind) {
    if (!cJSON_IsString(psa_ind)) {
        ogs_error("OpenAPI_psa_information_parseFromJSON() failed [psa_ind]");
        goto end;
    }
    psa_indVariable = OpenAPI_psa_indication_FromString(psa_ind->valuestring);
    }

    dnai_list = cJSON_GetObjectItemCaseSensitive(psa_informationJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_psa_information_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_psa_information_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    ue_ipv6_prefix = cJSON_GetObjectItemCaseSensitive(psa_informationJSON, "ueIpv6Prefix");
    if (ue_ipv6_prefix) {
    if (!cJSON_IsString(ue_ipv6_prefix) && !cJSON_IsNull(ue_ipv6_prefix)) {
        ogs_error("OpenAPI_psa_information_parseFromJSON() failed [ue_ipv6_prefix]");
        goto end;
    }
    }

    psa_upf_id = cJSON_GetObjectItemCaseSensitive(psa_informationJSON, "psaUpfId");
    if (psa_upf_id) {
    if (!cJSON_IsString(psa_upf_id) && !cJSON_IsNull(psa_upf_id)) {
        ogs_error("OpenAPI_psa_information_parseFromJSON() failed [psa_upf_id]");
        goto end;
    }
    }

    psa_information_local_var = OpenAPI_psa_information_create (
        psa_ind ? psa_indVariable : 0,
        dnai_list ? dnai_listList : NULL,
        ue_ipv6_prefix && !cJSON_IsNull(ue_ipv6_prefix) ? ogs_strdup(ue_ipv6_prefix->valuestring) : NULL,
        psa_upf_id && !cJSON_IsNull(psa_upf_id) ? ogs_strdup(psa_upf_id->valuestring) : NULL
    );

    return psa_information_local_var;
end:
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    return NULL;
}

OpenAPI_psa_information_t *OpenAPI_psa_information_copy(OpenAPI_psa_information_t *dst, OpenAPI_psa_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_psa_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_psa_information_convertToJSON() failed");
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

    OpenAPI_psa_information_free(dst);
    dst = OpenAPI_psa_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

