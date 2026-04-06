
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vlan_tag_value.h"

OpenAPI_vlan_tag_value_t *OpenAPI_vlan_tag_value_create(
    char *vlan_tag_value,
    char *vid,
    char *pcp,
    char *dei
)
{
    OpenAPI_vlan_tag_value_t *vlan_tag_value_local_var = ogs_malloc(sizeof(OpenAPI_vlan_tag_value_t));
    ogs_assert(vlan_tag_value_local_var);

    vlan_tag_value_local_var->vlan_tag_value = vlan_tag_value;
    vlan_tag_value_local_var->vid = vid;
    vlan_tag_value_local_var->pcp = pcp;
    vlan_tag_value_local_var->dei = dei;

    return vlan_tag_value_local_var;
}

void OpenAPI_vlan_tag_value_free(OpenAPI_vlan_tag_value_t *vlan_tag_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vlan_tag_value) {
        return;
    }
    if (vlan_tag_value->vlan_tag_value) {
        ogs_free(vlan_tag_value->vlan_tag_value);
        vlan_tag_value->vlan_tag_value = NULL;
    }
    if (vlan_tag_value->vid) {
        ogs_free(vlan_tag_value->vid);
        vlan_tag_value->vid = NULL;
    }
    if (vlan_tag_value->pcp) {
        ogs_free(vlan_tag_value->pcp);
        vlan_tag_value->pcp = NULL;
    }
    if (vlan_tag_value->dei) {
        ogs_free(vlan_tag_value->dei);
        vlan_tag_value->dei = NULL;
    }
    ogs_free(vlan_tag_value);
}

cJSON *OpenAPI_vlan_tag_value_convertToJSON(OpenAPI_vlan_tag_value_t *vlan_tag_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vlan_tag_value == NULL) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed [VlanTagValue]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!vlan_tag_value->vlan_tag_value) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed [vlan_tag_value]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "vlanTagValue", vlan_tag_value->vlan_tag_value) == NULL) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed [vlan_tag_value]");
        goto end;
    }

    if (vlan_tag_value->vid) {
    if (cJSON_AddStringToObject(item, "vid", vlan_tag_value->vid) == NULL) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed [vid]");
        goto end;
    }
    }

    if (vlan_tag_value->pcp) {
    if (cJSON_AddStringToObject(item, "pcp", vlan_tag_value->pcp) == NULL) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed [pcp]");
        goto end;
    }
    }

    if (vlan_tag_value->dei) {
    if (cJSON_AddStringToObject(item, "dei", vlan_tag_value->dei) == NULL) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed [dei]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vlan_tag_value_t *OpenAPI_vlan_tag_value_parseFromJSON(cJSON *vlan_tag_valueJSON)
{
    OpenAPI_vlan_tag_value_t *vlan_tag_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *vlan_tag_value = NULL;
    cJSON *vid = NULL;
    cJSON *pcp = NULL;
    cJSON *dei = NULL;
    vlan_tag_value = cJSON_GetObjectItemCaseSensitive(vlan_tag_valueJSON, "vlanTagValue");
    if (!vlan_tag_value) {
        ogs_error("OpenAPI_vlan_tag_value_parseFromJSON() failed [vlan_tag_value]");
        goto end;
    }
    if (!cJSON_IsString(vlan_tag_value)) {
        ogs_error("OpenAPI_vlan_tag_value_parseFromJSON() failed [vlan_tag_value]");
        goto end;
    }

    vid = cJSON_GetObjectItemCaseSensitive(vlan_tag_valueJSON, "vid");
    if (vid) {
    if (!cJSON_IsString(vid) && !cJSON_IsNull(vid)) {
        ogs_error("OpenAPI_vlan_tag_value_parseFromJSON() failed [vid]");
        goto end;
    }
    }

    pcp = cJSON_GetObjectItemCaseSensitive(vlan_tag_valueJSON, "pcp");
    if (pcp) {
    if (!cJSON_IsString(pcp) && !cJSON_IsNull(pcp)) {
        ogs_error("OpenAPI_vlan_tag_value_parseFromJSON() failed [pcp]");
        goto end;
    }
    }

    dei = cJSON_GetObjectItemCaseSensitive(vlan_tag_valueJSON, "dei");
    if (dei) {
    if (!cJSON_IsString(dei) && !cJSON_IsNull(dei)) {
        ogs_error("OpenAPI_vlan_tag_value_parseFromJSON() failed [dei]");
        goto end;
    }
    }

    vlan_tag_value_local_var = OpenAPI_vlan_tag_value_create (
        ogs_strdup(vlan_tag_value->valuestring),
        vid && !cJSON_IsNull(vid) ? ogs_strdup(vid->valuestring) : NULL,
        pcp && !cJSON_IsNull(pcp) ? ogs_strdup(pcp->valuestring) : NULL,
        dei && !cJSON_IsNull(dei) ? ogs_strdup(dei->valuestring) : NULL
    );

    return vlan_tag_value_local_var;
end:
    return NULL;
}

OpenAPI_vlan_tag_value_t *OpenAPI_vlan_tag_value_copy(OpenAPI_vlan_tag_value_t *dst, OpenAPI_vlan_tag_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vlan_tag_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vlan_tag_value_convertToJSON() failed");
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

    OpenAPI_vlan_tag_value_free(dst);
    dst = OpenAPI_vlan_tag_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

