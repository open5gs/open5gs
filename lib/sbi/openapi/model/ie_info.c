
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ie_info.h"

OpenAPI_ie_info_t *OpenAPI_ie_info_create(
    OpenAPI_ie_location_e ie_loc,
    OpenAPI_ie_type_e ie_type,
    char *req_ie,
    char *rsp_ie,
    bool is_is_modifiable,
    int is_modifiable,
    OpenAPI_list_t* is_modifiable_by_ipx
)
{
    OpenAPI_ie_info_t *ie_info_local_var = ogs_malloc(sizeof(OpenAPI_ie_info_t));
    ogs_assert(ie_info_local_var);

    ie_info_local_var->ie_loc = ie_loc;
    ie_info_local_var->ie_type = ie_type;
    ie_info_local_var->req_ie = req_ie;
    ie_info_local_var->rsp_ie = rsp_ie;
    ie_info_local_var->is_is_modifiable = is_is_modifiable;
    ie_info_local_var->is_modifiable = is_modifiable;
    ie_info_local_var->is_modifiable_by_ipx = is_modifiable_by_ipx;

    return ie_info_local_var;
}

void OpenAPI_ie_info_free(OpenAPI_ie_info_t *ie_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ie_info) {
        return;
    }
    if (ie_info->req_ie) {
        ogs_free(ie_info->req_ie);
        ie_info->req_ie = NULL;
    }
    if (ie_info->rsp_ie) {
        ogs_free(ie_info->rsp_ie);
        ie_info->rsp_ie = NULL;
    }
    if (ie_info->is_modifiable_by_ipx) {
        OpenAPI_list_for_each(ie_info->is_modifiable_by_ipx, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ie_info->is_modifiable_by_ipx);
        ie_info->is_modifiable_by_ipx = NULL;
    }
    ogs_free(ie_info);
}

cJSON *OpenAPI_ie_info_convertToJSON(OpenAPI_ie_info_t *ie_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ie_info == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [IeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ie_info->ie_loc == OpenAPI_ie_location_NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [ie_loc]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ieLoc", OpenAPI_ie_location_ToString(ie_info->ie_loc)) == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [ie_loc]");
        goto end;
    }

    if (ie_info->ie_type == OpenAPI_ie_type_NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [ie_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ieType", OpenAPI_ie_type_ToString(ie_info->ie_type)) == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [ie_type]");
        goto end;
    }

    if (ie_info->req_ie) {
    if (cJSON_AddStringToObject(item, "reqIe", ie_info->req_ie) == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [req_ie]");
        goto end;
    }
    }

    if (ie_info->rsp_ie) {
    if (cJSON_AddStringToObject(item, "rspIe", ie_info->rsp_ie) == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [rsp_ie]");
        goto end;
    }
    }

    if (ie_info->is_is_modifiable) {
    if (cJSON_AddBoolToObject(item, "isModifiable", ie_info->is_modifiable) == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [is_modifiable]");
        goto end;
    }
    }

    if (ie_info->is_modifiable_by_ipx) {
    cJSON *is_modifiable_by_ipx = cJSON_AddObjectToObject(item, "isModifiableByIpx");
    if (is_modifiable_by_ipx == NULL) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed [is_modifiable_by_ipx]");
        goto end;
    }
    cJSON *localMapObject = is_modifiable_by_ipx;
    if (ie_info->is_modifiable_by_ipx) {
        OpenAPI_list_for_each(ie_info->is_modifiable_by_ipx, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ie_info_convertToJSON() failed [is_modifiable_by_ipx]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ie_info_convertToJSON() failed [is_modifiable_by_ipx]");
                goto end;
            }
            if (cJSON_AddBoolToObject(localMapObject, localKeyValue->key, (uintptr_t)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_ie_info_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_ie_info_t *OpenAPI_ie_info_parseFromJSON(cJSON *ie_infoJSON)
{
    OpenAPI_ie_info_t *ie_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ie_loc = NULL;
    OpenAPI_ie_location_e ie_locVariable = 0;
    cJSON *ie_type = NULL;
    OpenAPI_ie_type_e ie_typeVariable = 0;
    cJSON *req_ie = NULL;
    cJSON *rsp_ie = NULL;
    cJSON *is_modifiable = NULL;
    cJSON *is_modifiable_by_ipx = NULL;
    OpenAPI_list_t *is_modifiable_by_ipxList = NULL;
    ie_loc = cJSON_GetObjectItemCaseSensitive(ie_infoJSON, "ieLoc");
    if (!ie_loc) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [ie_loc]");
        goto end;
    }
    if (!cJSON_IsString(ie_loc)) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [ie_loc]");
        goto end;
    }
    ie_locVariable = OpenAPI_ie_location_FromString(ie_loc->valuestring);

    ie_type = cJSON_GetObjectItemCaseSensitive(ie_infoJSON, "ieType");
    if (!ie_type) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [ie_type]");
        goto end;
    }
    if (!cJSON_IsString(ie_type)) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [ie_type]");
        goto end;
    }
    ie_typeVariable = OpenAPI_ie_type_FromString(ie_type->valuestring);

    req_ie = cJSON_GetObjectItemCaseSensitive(ie_infoJSON, "reqIe");
    if (req_ie) {
    if (!cJSON_IsString(req_ie) && !cJSON_IsNull(req_ie)) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [req_ie]");
        goto end;
    }
    }

    rsp_ie = cJSON_GetObjectItemCaseSensitive(ie_infoJSON, "rspIe");
    if (rsp_ie) {
    if (!cJSON_IsString(rsp_ie) && !cJSON_IsNull(rsp_ie)) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [rsp_ie]");
        goto end;
    }
    }

    is_modifiable = cJSON_GetObjectItemCaseSensitive(ie_infoJSON, "isModifiable");
    if (is_modifiable) {
    if (!cJSON_IsBool(is_modifiable)) {
        ogs_error("OpenAPI_ie_info_parseFromJSON() failed [is_modifiable]");
        goto end;
    }
    }

    is_modifiable_by_ipx = cJSON_GetObjectItemCaseSensitive(ie_infoJSON, "isModifiableByIpx");
    if (is_modifiable_by_ipx) {
        cJSON *is_modifiable_by_ipx_local_map = NULL;
        if (!cJSON_IsObject(is_modifiable_by_ipx) && !cJSON_IsNull(is_modifiable_by_ipx)) {
            ogs_error("OpenAPI_ie_info_parseFromJSON() failed [is_modifiable_by_ipx]");
            goto end;
        }
        if (cJSON_IsObject(is_modifiable_by_ipx)) {
            is_modifiable_by_ipxList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(is_modifiable_by_ipx_local_map, is_modifiable_by_ipx) {
                cJSON *localMapObject = is_modifiable_by_ipx_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsBool(localMapObject)) {
                    ogs_error("OpenAPI_ie_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                localInt = (int *)ogs_calloc(1, sizeof(int));
                if (!localInt) {
                    ogs_error("OpenAPI_ie_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localInt = localMapObject->valueint;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localInt);
                OpenAPI_list_add(is_modifiable_by_ipxList, localMapKeyPair);
            }
        }
    }

    ie_info_local_var = OpenAPI_ie_info_create (
        ie_locVariable,
        ie_typeVariable,
        req_ie && !cJSON_IsNull(req_ie) ? ogs_strdup(req_ie->valuestring) : NULL,
        rsp_ie && !cJSON_IsNull(rsp_ie) ? ogs_strdup(rsp_ie->valuestring) : NULL,
        is_modifiable ? true : false,
        is_modifiable ? is_modifiable->valueint : 0,
        is_modifiable_by_ipx ? is_modifiable_by_ipxList : NULL
    );

    return ie_info_local_var;
end:
    if (is_modifiable_by_ipxList) {
        OpenAPI_list_for_each(is_modifiable_by_ipxList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(is_modifiable_by_ipxList);
        is_modifiable_by_ipxList = NULL;
    }
    return NULL;
}

OpenAPI_ie_info_t *OpenAPI_ie_info_copy(OpenAPI_ie_info_t *dst, OpenAPI_ie_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ie_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ie_info_convertToJSON() failed");
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

    OpenAPI_ie_info_free(dst);
    dst = OpenAPI_ie_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

