
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vlan_tag_handling.h"

OpenAPI_vlan_tag_handling_t *OpenAPI_vlan_tag_handling_create(
    OpenAPI_vlan_tag_value_t *vlan_tag,
    OpenAPI_vlan_tag_type_e vlan_tag_type
)
{
    OpenAPI_vlan_tag_handling_t *vlan_tag_handling_local_var = ogs_malloc(sizeof(OpenAPI_vlan_tag_handling_t));
    ogs_assert(vlan_tag_handling_local_var);

    vlan_tag_handling_local_var->vlan_tag = vlan_tag;
    vlan_tag_handling_local_var->vlan_tag_type = vlan_tag_type;

    return vlan_tag_handling_local_var;
}

void OpenAPI_vlan_tag_handling_free(OpenAPI_vlan_tag_handling_t *vlan_tag_handling)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vlan_tag_handling) {
        return;
    }
    if (vlan_tag_handling->vlan_tag) {
        OpenAPI_vlan_tag_value_free(vlan_tag_handling->vlan_tag);
        vlan_tag_handling->vlan_tag = NULL;
    }
    ogs_free(vlan_tag_handling);
}

cJSON *OpenAPI_vlan_tag_handling_convertToJSON(OpenAPI_vlan_tag_handling_t *vlan_tag_handling)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vlan_tag_handling == NULL) {
        ogs_error("OpenAPI_vlan_tag_handling_convertToJSON() failed [VlanTagHandling]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!vlan_tag_handling->vlan_tag) {
        ogs_error("OpenAPI_vlan_tag_handling_convertToJSON() failed [vlan_tag]");
        return NULL;
    }
    cJSON *vlan_tag_local_JSON = OpenAPI_vlan_tag_value_convertToJSON(vlan_tag_handling->vlan_tag);
    if (vlan_tag_local_JSON == NULL) {
        ogs_error("OpenAPI_vlan_tag_handling_convertToJSON() failed [vlan_tag]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vlanTag", vlan_tag_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vlan_tag_handling_convertToJSON() failed [vlan_tag]");
        goto end;
    }

    if (vlan_tag_handling->vlan_tag_type != OpenAPI_vlan_tag_type_NULL) {
    if (cJSON_AddStringToObject(item, "vlanTagType", OpenAPI_vlan_tag_type_ToString(vlan_tag_handling->vlan_tag_type)) == NULL) {
        ogs_error("OpenAPI_vlan_tag_handling_convertToJSON() failed [vlan_tag_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vlan_tag_handling_t *OpenAPI_vlan_tag_handling_parseFromJSON(cJSON *vlan_tag_handlingJSON)
{
    OpenAPI_vlan_tag_handling_t *vlan_tag_handling_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *vlan_tag = NULL;
    OpenAPI_vlan_tag_value_t *vlan_tag_local_nonprim = NULL;
    cJSON *vlan_tag_type = NULL;
    OpenAPI_vlan_tag_type_e vlan_tag_typeVariable = 0;
    vlan_tag = cJSON_GetObjectItemCaseSensitive(vlan_tag_handlingJSON, "vlanTag");
    if (!vlan_tag) {
        ogs_error("OpenAPI_vlan_tag_handling_parseFromJSON() failed [vlan_tag]");
        goto end;
    }
    vlan_tag_local_nonprim = OpenAPI_vlan_tag_value_parseFromJSON(vlan_tag);
    if (!vlan_tag_local_nonprim) {
        ogs_error("OpenAPI_vlan_tag_value_parseFromJSON failed [vlan_tag]");
        goto end;
    }

    vlan_tag_type = cJSON_GetObjectItemCaseSensitive(vlan_tag_handlingJSON, "vlanTagType");
    if (vlan_tag_type) {
    if (!cJSON_IsString(vlan_tag_type)) {
        ogs_error("OpenAPI_vlan_tag_handling_parseFromJSON() failed [vlan_tag_type]");
        goto end;
    }
    vlan_tag_typeVariable = OpenAPI_vlan_tag_type_FromString(vlan_tag_type->valuestring);
    }

    vlan_tag_handling_local_var = OpenAPI_vlan_tag_handling_create (
        vlan_tag_local_nonprim,
        vlan_tag_type ? vlan_tag_typeVariable : 0
    );

    return vlan_tag_handling_local_var;
end:
    if (vlan_tag_local_nonprim) {
        OpenAPI_vlan_tag_value_free(vlan_tag_local_nonprim);
        vlan_tag_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_vlan_tag_handling_t *OpenAPI_vlan_tag_handling_copy(OpenAPI_vlan_tag_handling_t *dst, OpenAPI_vlan_tag_handling_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vlan_tag_handling_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vlan_tag_handling_convertToJSON() failed");
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

    OpenAPI_vlan_tag_handling_free(dst);
    dst = OpenAPI_vlan_tag_handling_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

