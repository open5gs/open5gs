
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slrb_configurations.h"

OpenAPI_slrb_configurations_t *OpenAPI_slrb_configurations_create(
    OpenAPI_pc5_qo_s_para_t *pc5_qos_para,
    char *slrb
    )
{
    OpenAPI_slrb_configurations_t *slrb_configurations_local_var = OpenAPI_malloc(sizeof(OpenAPI_slrb_configurations_t));
    if (!slrb_configurations_local_var) {
        return NULL;
    }
    slrb_configurations_local_var->pc5_qos_para = pc5_qos_para;
    slrb_configurations_local_var->slrb = slrb;

    return slrb_configurations_local_var;
}

void OpenAPI_slrb_configurations_free(OpenAPI_slrb_configurations_t *slrb_configurations)
{
    if (NULL == slrb_configurations) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_pc5_qo_s_para_free(slrb_configurations->pc5_qos_para);
    ogs_free(slrb_configurations->slrb);
    ogs_free(slrb_configurations);
}

cJSON *OpenAPI_slrb_configurations_convertToJSON(OpenAPI_slrb_configurations_t *slrb_configurations)
{
    cJSON *item = NULL;

    if (slrb_configurations == NULL) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed [SlrbConfigurations]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slrb_configurations->pc5_qos_para) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed [pc5_qos_para]");
        goto end;
    }
    cJSON *pc5_qos_para_local_JSON = OpenAPI_pc5_qo_s_para_convertToJSON(slrb_configurations->pc5_qos_para);
    if (pc5_qos_para_local_JSON == NULL) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed [pc5_qos_para]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pc5QosPara", pc5_qos_para_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed [pc5_qos_para]");
        goto end;
    }

    if (!slrb_configurations->slrb) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed [slrb]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "slrb", slrb_configurations->slrb) == NULL) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed [slrb]");
        goto end;
    }

end:
    return item;
}

OpenAPI_slrb_configurations_t *OpenAPI_slrb_configurations_parseFromJSON(cJSON *slrb_configurationsJSON)
{
    OpenAPI_slrb_configurations_t *slrb_configurations_local_var = NULL;
    cJSON *pc5_qos_para = cJSON_GetObjectItemCaseSensitive(slrb_configurationsJSON, "pc5QosPara");
    if (!pc5_qos_para) {
        ogs_error("OpenAPI_slrb_configurations_parseFromJSON() failed [pc5_qos_para]");
        goto end;
    }

    OpenAPI_pc5_qo_s_para_t *pc5_qos_para_local_nonprim = NULL;

    pc5_qos_para_local_nonprim = OpenAPI_pc5_qo_s_para_parseFromJSON(pc5_qos_para);

    cJSON *slrb = cJSON_GetObjectItemCaseSensitive(slrb_configurationsJSON, "slrb");
    if (!slrb) {
        ogs_error("OpenAPI_slrb_configurations_parseFromJSON() failed [slrb]");
        goto end;
    }


    if (!cJSON_IsString(slrb)) {
        ogs_error("OpenAPI_slrb_configurations_parseFromJSON() failed [slrb]");
        goto end;
    }

    slrb_configurations_local_var = OpenAPI_slrb_configurations_create (
        pc5_qos_para_local_nonprim,
        ogs_strdup(slrb->valuestring)
        );

    return slrb_configurations_local_var;
end:
    return NULL;
}

OpenAPI_slrb_configurations_t *OpenAPI_slrb_configurations_copy(OpenAPI_slrb_configurations_t *dst, OpenAPI_slrb_configurations_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slrb_configurations_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slrb_configurations_convertToJSON() failed");
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

    OpenAPI_slrb_configurations_free(dst);
    dst = OpenAPI_slrb_configurations_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

