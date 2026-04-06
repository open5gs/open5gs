
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecs_addr_data.h"

OpenAPI_ecs_addr_data_t *OpenAPI_ecs_addr_data_create(
    char *self,
    OpenAPI_ecs_server_addr_t *ecs_server_addr,
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond,
    OpenAPI_list_t *ecs_auth_methods,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *supp_feat
)
{
    OpenAPI_ecs_addr_data_t *ecs_addr_data_local_var = ogs_malloc(sizeof(OpenAPI_ecs_addr_data_t));
    ogs_assert(ecs_addr_data_local_var);

    ecs_addr_data_local_var->self = self;
    ecs_addr_data_local_var->ecs_server_addr = ecs_server_addr;
    ecs_addr_data_local_var->spatial_validity_cond = spatial_validity_cond;
    ecs_addr_data_local_var->ecs_auth_methods = ecs_auth_methods;
    ecs_addr_data_local_var->dnn = dnn;
    ecs_addr_data_local_var->snssai = snssai;
    ecs_addr_data_local_var->supp_feat = supp_feat;

    return ecs_addr_data_local_var;
}

void OpenAPI_ecs_addr_data_free(OpenAPI_ecs_addr_data_t *ecs_addr_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecs_addr_data) {
        return;
    }
    if (ecs_addr_data->self) {
        ogs_free(ecs_addr_data->self);
        ecs_addr_data->self = NULL;
    }
    if (ecs_addr_data->ecs_server_addr) {
        OpenAPI_ecs_server_addr_free(ecs_addr_data->ecs_server_addr);
        ecs_addr_data->ecs_server_addr = NULL;
    }
    if (ecs_addr_data->spatial_validity_cond) {
        OpenAPI_spatial_validity_cond_1_free(ecs_addr_data->spatial_validity_cond);
        ecs_addr_data->spatial_validity_cond = NULL;
    }
    if (ecs_addr_data->ecs_auth_methods) {
        OpenAPI_list_free(ecs_addr_data->ecs_auth_methods);
        ecs_addr_data->ecs_auth_methods = NULL;
    }
    if (ecs_addr_data->dnn) {
        ogs_free(ecs_addr_data->dnn);
        ecs_addr_data->dnn = NULL;
    }
    if (ecs_addr_data->snssai) {
        OpenAPI_snssai_free(ecs_addr_data->snssai);
        ecs_addr_data->snssai = NULL;
    }
    if (ecs_addr_data->supp_feat) {
        ogs_free(ecs_addr_data->supp_feat);
        ecs_addr_data->supp_feat = NULL;
    }
    ogs_free(ecs_addr_data);
}

cJSON *OpenAPI_ecs_addr_data_convertToJSON(OpenAPI_ecs_addr_data_t *ecs_addr_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecs_addr_data == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [EcsAddrData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ecs_addr_data->self) {
    if (cJSON_AddStringToObject(item, "self", ecs_addr_data->self) == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [self]");
        goto end;
    }
    }

    if (!ecs_addr_data->ecs_server_addr) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [ecs_server_addr]");
        return NULL;
    }
    cJSON *ecs_server_addr_local_JSON = OpenAPI_ecs_server_addr_convertToJSON(ecs_addr_data->ecs_server_addr);
    if (ecs_server_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [ecs_server_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecsServerAddr", ecs_server_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [ecs_server_addr]");
        goto end;
    }

    if (ecs_addr_data->spatial_validity_cond) {
    cJSON *spatial_validity_cond_local_JSON = OpenAPI_spatial_validity_cond_1_convertToJSON(ecs_addr_data->spatial_validity_cond);
    if (spatial_validity_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidityCond", spatial_validity_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    }

    if (ecs_addr_data->ecs_auth_methods != OpenAPI_ecs_auth_method_NULL) {
    cJSON *ecs_auth_methodsList = cJSON_AddArrayToObject(item, "ecsAuthMethods");
    if (ecs_auth_methodsList == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [ecs_auth_methods]");
        goto end;
    }
    OpenAPI_list_for_each(ecs_addr_data->ecs_auth_methods, node) {
        if (cJSON_AddStringToObject(ecs_auth_methodsList, "", OpenAPI_ecs_auth_method_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [ecs_auth_methods]");
            goto end;
        }
    }
    }

    if (ecs_addr_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", ecs_addr_data->dnn) == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (ecs_addr_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(ecs_addr_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (ecs_addr_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", ecs_addr_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ecs_addr_data_t *OpenAPI_ecs_addr_data_parseFromJSON(cJSON *ecs_addr_dataJSON)
{
    OpenAPI_ecs_addr_data_t *ecs_addr_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *self = NULL;
    cJSON *ecs_server_addr = NULL;
    OpenAPI_ecs_server_addr_t *ecs_server_addr_local_nonprim = NULL;
    cJSON *spatial_validity_cond = NULL;
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond_local_nonprim = NULL;
    cJSON *ecs_auth_methods = NULL;
    OpenAPI_list_t *ecs_auth_methodsList = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *supp_feat = NULL;
    self = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "self");
    if (self) {
    if (!cJSON_IsString(self) && !cJSON_IsNull(self)) {
        ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed [self]");
        goto end;
    }
    }

    ecs_server_addr = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "ecsServerAddr");
    if (!ecs_server_addr) {
        ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed [ecs_server_addr]");
        goto end;
    }
    ecs_server_addr_local_nonprim = OpenAPI_ecs_server_addr_parseFromJSON(ecs_server_addr);
    if (!ecs_server_addr_local_nonprim) {
        ogs_error("OpenAPI_ecs_server_addr_parseFromJSON failed [ecs_server_addr]");
        goto end;
    }

    spatial_validity_cond = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "spatialValidityCond");
    if (spatial_validity_cond) {
    spatial_validity_cond_local_nonprim = OpenAPI_spatial_validity_cond_1_parseFromJSON(spatial_validity_cond);
    if (!spatial_validity_cond_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_cond_1_parseFromJSON failed [spatial_validity_cond]");
        goto end;
    }
    }

    ecs_auth_methods = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "ecsAuthMethods");
    if (ecs_auth_methods) {
        cJSON *ecs_auth_methods_local = NULL;
        if (!cJSON_IsArray(ecs_auth_methods)) {
            ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed [ecs_auth_methods]");
            goto end;
        }

        ecs_auth_methodsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecs_auth_methods_local, ecs_auth_methods) {
            OpenAPI_ecs_auth_method_e localEnum = OpenAPI_ecs_auth_method_NULL;
            if (!cJSON_IsString(ecs_auth_methods_local)) {
                ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed [ecs_auth_methods]");
                goto end;
            }
            localEnum = OpenAPI_ecs_auth_method_FromString(ecs_auth_methods_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ecs_auth_methods\" is not supported. Ignoring it ...",
                         ecs_auth_methods_local->valuestring);
            } else {
                OpenAPI_list_add(ecs_auth_methodsList, (void *)localEnum);
            }
        }
        if (ecs_auth_methodsList->count == 0) {
            ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed: Expected ecs_auth_methodsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(ecs_addr_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_ecs_addr_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    ecs_addr_data_local_var = OpenAPI_ecs_addr_data_create (
        self && !cJSON_IsNull(self) ? ogs_strdup(self->valuestring) : NULL,
        ecs_server_addr_local_nonprim,
        spatial_validity_cond ? spatial_validity_cond_local_nonprim : NULL,
        ecs_auth_methods ? ecs_auth_methodsList : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return ecs_addr_data_local_var;
end:
    if (ecs_server_addr_local_nonprim) {
        OpenAPI_ecs_server_addr_free(ecs_server_addr_local_nonprim);
        ecs_server_addr_local_nonprim = NULL;
    }
    if (spatial_validity_cond_local_nonprim) {
        OpenAPI_spatial_validity_cond_1_free(spatial_validity_cond_local_nonprim);
        spatial_validity_cond_local_nonprim = NULL;
    }
    if (ecs_auth_methodsList) {
        OpenAPI_list_free(ecs_auth_methodsList);
        ecs_auth_methodsList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ecs_addr_data_t *OpenAPI_ecs_addr_data_copy(OpenAPI_ecs_addr_data_t *dst, OpenAPI_ecs_addr_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecs_addr_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecs_addr_data_convertToJSON() failed");
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

    OpenAPI_ecs_addr_data_free(dst);
    dst = OpenAPI_ecs_addr_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

