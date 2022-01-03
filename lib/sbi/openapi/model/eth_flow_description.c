
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eth_flow_description.h"

OpenAPI_eth_flow_description_t *OpenAPI_eth_flow_description_create(
    char *dest_mac_addr,
    char *eth_type,
    char *f_desc,
    OpenAPI_flow_direction_e f_dir,
    char *source_mac_addr,
    OpenAPI_list_t *vlan_tags,
    char *src_mac_addr_end,
    char *dest_mac_addr_end
)
{
    OpenAPI_eth_flow_description_t *eth_flow_description_local_var = ogs_malloc(sizeof(OpenAPI_eth_flow_description_t));
    ogs_assert(eth_flow_description_local_var);

    eth_flow_description_local_var->dest_mac_addr = dest_mac_addr;
    eth_flow_description_local_var->eth_type = eth_type;
    eth_flow_description_local_var->f_desc = f_desc;
    eth_flow_description_local_var->f_dir = f_dir;
    eth_flow_description_local_var->source_mac_addr = source_mac_addr;
    eth_flow_description_local_var->vlan_tags = vlan_tags;
    eth_flow_description_local_var->src_mac_addr_end = src_mac_addr_end;
    eth_flow_description_local_var->dest_mac_addr_end = dest_mac_addr_end;

    return eth_flow_description_local_var;
}

void OpenAPI_eth_flow_description_free(OpenAPI_eth_flow_description_t *eth_flow_description)
{
    if (NULL == eth_flow_description) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(eth_flow_description->dest_mac_addr);
    ogs_free(eth_flow_description->eth_type);
    ogs_free(eth_flow_description->f_desc);
    ogs_free(eth_flow_description->source_mac_addr);
    OpenAPI_list_for_each(eth_flow_description->vlan_tags, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(eth_flow_description->vlan_tags);
    ogs_free(eth_flow_description->src_mac_addr_end);
    ogs_free(eth_flow_description->dest_mac_addr_end);
    ogs_free(eth_flow_description);
}

cJSON *OpenAPI_eth_flow_description_convertToJSON(OpenAPI_eth_flow_description_t *eth_flow_description)
{
    cJSON *item = NULL;

    if (eth_flow_description == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [EthFlowDescription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eth_flow_description->dest_mac_addr) {
    if (cJSON_AddStringToObject(item, "destMacAddr", eth_flow_description->dest_mac_addr) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [dest_mac_addr]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "ethType", eth_flow_description->eth_type) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [eth_type]");
        goto end;
    }

    if (eth_flow_description->f_desc) {
    if (cJSON_AddStringToObject(item, "fDesc", eth_flow_description->f_desc) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [f_desc]");
        goto end;
    }
    }

    if (eth_flow_description->f_dir) {
    if (cJSON_AddStringToObject(item, "fDir", OpenAPI_flow_direction_ToString(eth_flow_description->f_dir)) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [f_dir]");
        goto end;
    }
    }

    if (eth_flow_description->source_mac_addr) {
    if (cJSON_AddStringToObject(item, "sourceMacAddr", eth_flow_description->source_mac_addr) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [source_mac_addr]");
        goto end;
    }
    }

    if (eth_flow_description->vlan_tags) {
    cJSON *vlan_tags = cJSON_AddArrayToObject(item, "vlanTags");
    if (vlan_tags == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [vlan_tags]");
        goto end;
    }

    OpenAPI_lnode_t *vlan_tags_node;
    OpenAPI_list_for_each(eth_flow_description->vlan_tags, vlan_tags_node)  {
    if (cJSON_AddStringToObject(vlan_tags, "", (char*)vlan_tags_node->data) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [vlan_tags]");
        goto end;
    }
                    }
    }

    if (eth_flow_description->src_mac_addr_end) {
    if (cJSON_AddStringToObject(item, "srcMacAddrEnd", eth_flow_description->src_mac_addr_end) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [src_mac_addr_end]");
        goto end;
    }
    }

    if (eth_flow_description->dest_mac_addr_end) {
    if (cJSON_AddStringToObject(item, "destMacAddrEnd", eth_flow_description->dest_mac_addr_end) == NULL) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed [dest_mac_addr_end]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eth_flow_description_t *OpenAPI_eth_flow_description_parseFromJSON(cJSON *eth_flow_descriptionJSON)
{
    OpenAPI_eth_flow_description_t *eth_flow_description_local_var = NULL;
    cJSON *dest_mac_addr = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "destMacAddr");

    if (dest_mac_addr) {
    if (!cJSON_IsString(dest_mac_addr)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [dest_mac_addr]");
        goto end;
    }
    }

    cJSON *eth_type = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "ethType");
    if (!eth_type) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [eth_type]");
        goto end;
    }

    if (!cJSON_IsString(eth_type)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [eth_type]");
        goto end;
    }

    cJSON *f_desc = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "fDesc");

    if (f_desc) {
    if (!cJSON_IsString(f_desc)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [f_desc]");
        goto end;
    }
    }

    cJSON *f_dir = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "fDir");

    OpenAPI_flow_direction_e f_dirVariable;
    if (f_dir) {
    if (!cJSON_IsString(f_dir)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [f_dir]");
        goto end;
    }
    f_dirVariable = OpenAPI_flow_direction_FromString(f_dir->valuestring);
    }

    cJSON *source_mac_addr = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "sourceMacAddr");

    if (source_mac_addr) {
    if (!cJSON_IsString(source_mac_addr)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [source_mac_addr]");
        goto end;
    }
    }

    cJSON *vlan_tags = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "vlanTags");

    OpenAPI_list_t *vlan_tagsList;
    if (vlan_tags) {
    cJSON *vlan_tags_local;
    if (!cJSON_IsArray(vlan_tags)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [vlan_tags]");
        goto end;
    }
    vlan_tagsList = OpenAPI_list_create();

    cJSON_ArrayForEach(vlan_tags_local, vlan_tags) {
    if (!cJSON_IsString(vlan_tags_local)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [vlan_tags]");
        goto end;
    }
    OpenAPI_list_add(vlan_tagsList , ogs_strdup(vlan_tags_local->valuestring));
    }
    }

    cJSON *src_mac_addr_end = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "srcMacAddrEnd");

    if (src_mac_addr_end) {
    if (!cJSON_IsString(src_mac_addr_end)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [src_mac_addr_end]");
        goto end;
    }
    }

    cJSON *dest_mac_addr_end = cJSON_GetObjectItemCaseSensitive(eth_flow_descriptionJSON, "destMacAddrEnd");

    if (dest_mac_addr_end) {
    if (!cJSON_IsString(dest_mac_addr_end)) {
        ogs_error("OpenAPI_eth_flow_description_parseFromJSON() failed [dest_mac_addr_end]");
        goto end;
    }
    }

    eth_flow_description_local_var = OpenAPI_eth_flow_description_create (
        dest_mac_addr ? ogs_strdup(dest_mac_addr->valuestring) : NULL,
        ogs_strdup(eth_type->valuestring),
        f_desc ? ogs_strdup(f_desc->valuestring) : NULL,
        f_dir ? f_dirVariable : 0,
        source_mac_addr ? ogs_strdup(source_mac_addr->valuestring) : NULL,
        vlan_tags ? vlan_tagsList : NULL,
        src_mac_addr_end ? ogs_strdup(src_mac_addr_end->valuestring) : NULL,
        dest_mac_addr_end ? ogs_strdup(dest_mac_addr_end->valuestring) : NULL
    );

    return eth_flow_description_local_var;
end:
    return NULL;
}

OpenAPI_eth_flow_description_t *OpenAPI_eth_flow_description_copy(OpenAPI_eth_flow_description_t *dst, OpenAPI_eth_flow_description_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eth_flow_description_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eth_flow_description_convertToJSON() failed");
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

    OpenAPI_eth_flow_description_free(dst);
    dst = OpenAPI_eth_flow_description_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

