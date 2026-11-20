
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_slicing_info.h"

OpenAPI_network_slicing_info_t *OpenAPI_network_slicing_info_create(
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *h_plmn_snssai,
    OpenAPI_snssai_t *alternative_snssai
)
{
    OpenAPI_network_slicing_info_t *network_slicing_info_local_var = ogs_malloc(sizeof(OpenAPI_network_slicing_info_t));
    ogs_assert(network_slicing_info_local_var);

    network_slicing_info_local_var->s_nssai = s_nssai;
    network_slicing_info_local_var->h_plmn_snssai = h_plmn_snssai;
    network_slicing_info_local_var->alternative_snssai = alternative_snssai;

    return network_slicing_info_local_var;
}

void OpenAPI_network_slicing_info_free(OpenAPI_network_slicing_info_t *network_slicing_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_slicing_info) {
        return;
    }
    if (network_slicing_info->s_nssai) {
        OpenAPI_snssai_free(network_slicing_info->s_nssai);
        network_slicing_info->s_nssai = NULL;
    }
    if (network_slicing_info->h_plmn_snssai) {
        OpenAPI_snssai_free(network_slicing_info->h_plmn_snssai);
        network_slicing_info->h_plmn_snssai = NULL;
    }
    if (network_slicing_info->alternative_snssai) {
        OpenAPI_snssai_free(network_slicing_info->alternative_snssai);
        network_slicing_info->alternative_snssai = NULL;
    }
    ogs_free(network_slicing_info);
}

cJSON *OpenAPI_network_slicing_info_convertToJSON(OpenAPI_network_slicing_info_t *network_slicing_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_slicing_info == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [NetworkSlicingInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!network_slicing_info->s_nssai) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [s_nssai]");
        return NULL;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(network_slicing_info->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNSSAI", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (network_slicing_info->h_plmn_snssai) {
    cJSON *h_plmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(network_slicing_info->h_plmn_snssai);
    if (h_plmn_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [h_plmn_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hPlmnSNSSAI", h_plmn_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [h_plmn_snssai]");
        goto end;
    }
    }

    if (network_slicing_info->alternative_snssai) {
    cJSON *alternative_snssai_local_JSON = OpenAPI_snssai_convertToJSON(network_slicing_info->alternative_snssai);
    if (alternative_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [alternative_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "alternativeSNSSAI", alternative_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed [alternative_snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_network_slicing_info_t *OpenAPI_network_slicing_info_parseFromJSON(cJSON *network_slicing_infoJSON)
{
    OpenAPI_network_slicing_info_t *network_slicing_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *h_plmn_snssai = NULL;
    OpenAPI_snssai_t *h_plmn_snssai_local_nonprim = NULL;
    cJSON *alternative_snssai = NULL;
    OpenAPI_snssai_t *alternative_snssai_local_nonprim = NULL;
    s_nssai = cJSON_GetObjectItemCaseSensitive(network_slicing_infoJSON, "sNSSAI");
    if (!s_nssai) {
        ogs_error("OpenAPI_network_slicing_info_parseFromJSON() failed [s_nssai]");
        goto end;
    }
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }

    h_plmn_snssai = cJSON_GetObjectItemCaseSensitive(network_slicing_infoJSON, "hPlmnSNSSAI");
    if (h_plmn_snssai) {
    h_plmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(h_plmn_snssai);
    if (!h_plmn_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [h_plmn_snssai]");
        goto end;
    }
    }

    alternative_snssai = cJSON_GetObjectItemCaseSensitive(network_slicing_infoJSON, "alternativeSNSSAI");
    if (alternative_snssai) {
    alternative_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alternative_snssai);
    if (!alternative_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alternative_snssai]");
        goto end;
    }
    }

    network_slicing_info_local_var = OpenAPI_network_slicing_info_create (
        s_nssai_local_nonprim,
        h_plmn_snssai ? h_plmn_snssai_local_nonprim : NULL,
        alternative_snssai ? alternative_snssai_local_nonprim : NULL
    );

    return network_slicing_info_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (h_plmn_snssai_local_nonprim) {
        OpenAPI_snssai_free(h_plmn_snssai_local_nonprim);
        h_plmn_snssai_local_nonprim = NULL;
    }
    if (alternative_snssai_local_nonprim) {
        OpenAPI_snssai_free(alternative_snssai_local_nonprim);
        alternative_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_network_slicing_info_t *OpenAPI_network_slicing_info_copy(OpenAPI_network_slicing_info_t *dst, OpenAPI_network_slicing_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_slicing_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_slicing_info_convertToJSON() failed");
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

    OpenAPI_network_slicing_info_free(dst);
    dst = OpenAPI_network_slicing_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

