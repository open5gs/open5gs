
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_sharing_charging_information.h"

OpenAPI_network_sharing_charging_information_t *OpenAPI_network_sharing_charging_information_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_snssai_t *single_nssai
)
{
    OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_network_sharing_charging_information_t));
    ogs_assert(network_sharing_charging_information_local_var);

    network_sharing_charging_information_local_var->plmn_id = plmn_id;
    network_sharing_charging_information_local_var->single_nssai = single_nssai;

    return network_sharing_charging_information_local_var;
}

void OpenAPI_network_sharing_charging_information_free(OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_sharing_charging_information) {
        return;
    }
    if (network_sharing_charging_information->plmn_id) {
        OpenAPI_plmn_id_free(network_sharing_charging_information->plmn_id);
        network_sharing_charging_information->plmn_id = NULL;
    }
    if (network_sharing_charging_information->single_nssai) {
        OpenAPI_snssai_free(network_sharing_charging_information->single_nssai);
        network_sharing_charging_information->single_nssai = NULL;
    }
    ogs_free(network_sharing_charging_information);
}

cJSON *OpenAPI_network_sharing_charging_information_convertToJSON(OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_sharing_charging_information == NULL) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed [NetworkSharingChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!network_sharing_charging_information->plmn_id) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(network_sharing_charging_information->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (network_sharing_charging_information->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(network_sharing_charging_information->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNSSAI", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_network_sharing_charging_information_t *OpenAPI_network_sharing_charging_information_parseFromJSON(cJSON *network_sharing_charging_informationJSON)
{
    OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(network_sharing_charging_informationJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_network_sharing_charging_information_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(network_sharing_charging_informationJSON, "singleNSSAI");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    network_sharing_charging_information_local_var = OpenAPI_network_sharing_charging_information_create (
        plmn_id_local_nonprim,
        single_nssai ? single_nssai_local_nonprim : NULL
    );

    return network_sharing_charging_information_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_network_sharing_charging_information_t *OpenAPI_network_sharing_charging_information_copy(OpenAPI_network_sharing_charging_information_t *dst, OpenAPI_network_sharing_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_sharing_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_sharing_charging_information_convertToJSON() failed");
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

    OpenAPI_network_sharing_charging_information_free(dst);
    dst = OpenAPI_network_sharing_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

