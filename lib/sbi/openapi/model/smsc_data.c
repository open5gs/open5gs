
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smsc_data.h"

OpenAPI_smsc_data_t *OpenAPI_smsc_data_create(
    char *smsc_map_address,
    OpenAPI_network_node_diameter_address_1_t *smsc_diameter_address
)
{
    OpenAPI_smsc_data_t *smsc_data_local_var = ogs_malloc(sizeof(OpenAPI_smsc_data_t));
    ogs_assert(smsc_data_local_var);

    smsc_data_local_var->smsc_map_address = smsc_map_address;
    smsc_data_local_var->smsc_diameter_address = smsc_diameter_address;

    return smsc_data_local_var;
}

void OpenAPI_smsc_data_free(OpenAPI_smsc_data_t *smsc_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smsc_data) {
        return;
    }
    if (smsc_data->smsc_map_address) {
        ogs_free(smsc_data->smsc_map_address);
        smsc_data->smsc_map_address = NULL;
    }
    if (smsc_data->smsc_diameter_address) {
        OpenAPI_network_node_diameter_address_1_free(smsc_data->smsc_diameter_address);
        smsc_data->smsc_diameter_address = NULL;
    }
    ogs_free(smsc_data);
}

cJSON *OpenAPI_smsc_data_convertToJSON(OpenAPI_smsc_data_t *smsc_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smsc_data == NULL) {
        ogs_error("OpenAPI_smsc_data_convertToJSON() failed [SmscData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smsc_data->smsc_map_address) {
    if (cJSON_AddStringToObject(item, "smscMapAddress", smsc_data->smsc_map_address) == NULL) {
        ogs_error("OpenAPI_smsc_data_convertToJSON() failed [smsc_map_address]");
        goto end;
    }
    }

    if (smsc_data->smsc_diameter_address) {
    cJSON *smsc_diameter_address_local_JSON = OpenAPI_network_node_diameter_address_1_convertToJSON(smsc_data->smsc_diameter_address);
    if (smsc_diameter_address_local_JSON == NULL) {
        ogs_error("OpenAPI_smsc_data_convertToJSON() failed [smsc_diameter_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smscDiameterAddress", smsc_diameter_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smsc_data_convertToJSON() failed [smsc_diameter_address]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smsc_data_t *OpenAPI_smsc_data_parseFromJSON(cJSON *smsc_dataJSON)
{
    OpenAPI_smsc_data_t *smsc_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smsc_map_address = NULL;
    cJSON *smsc_diameter_address = NULL;
    OpenAPI_network_node_diameter_address_1_t *smsc_diameter_address_local_nonprim = NULL;
    smsc_map_address = cJSON_GetObjectItemCaseSensitive(smsc_dataJSON, "smscMapAddress");
    if (smsc_map_address) {
    if (!cJSON_IsString(smsc_map_address) && !cJSON_IsNull(smsc_map_address)) {
        ogs_error("OpenAPI_smsc_data_parseFromJSON() failed [smsc_map_address]");
        goto end;
    }
    }

    smsc_diameter_address = cJSON_GetObjectItemCaseSensitive(smsc_dataJSON, "smscDiameterAddress");
    if (smsc_diameter_address) {
    smsc_diameter_address_local_nonprim = OpenAPI_network_node_diameter_address_1_parseFromJSON(smsc_diameter_address);
    if (!smsc_diameter_address_local_nonprim) {
        ogs_error("OpenAPI_network_node_diameter_address_1_parseFromJSON failed [smsc_diameter_address]");
        goto end;
    }
    }

    smsc_data_local_var = OpenAPI_smsc_data_create (
        smsc_map_address && !cJSON_IsNull(smsc_map_address) ? ogs_strdup(smsc_map_address->valuestring) : NULL,
        smsc_diameter_address ? smsc_diameter_address_local_nonprim : NULL
    );

    return smsc_data_local_var;
end:
    if (smsc_diameter_address_local_nonprim) {
        OpenAPI_network_node_diameter_address_1_free(smsc_diameter_address_local_nonprim);
        smsc_diameter_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_smsc_data_t *OpenAPI_smsc_data_copy(OpenAPI_smsc_data_t *dst, OpenAPI_smsc_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smsc_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smsc_data_convertToJSON() failed");
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

    OpenAPI_smsc_data_free(dst);
    dst = OpenAPI_smsc_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

