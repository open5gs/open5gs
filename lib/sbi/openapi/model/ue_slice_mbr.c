
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_slice_mbr.h"

OpenAPI_ue_slice_mbr_t *OpenAPI_ue_slice_mbr_create(
    OpenAPI_list_t* slice_mbr,
    OpenAPI_snssai_t *serving_snssai,
    OpenAPI_snssai_t *mapped_home_snssai
)
{
    OpenAPI_ue_slice_mbr_t *ue_slice_mbr_local_var = ogs_malloc(sizeof(OpenAPI_ue_slice_mbr_t));
    ogs_assert(ue_slice_mbr_local_var);

    ue_slice_mbr_local_var->slice_mbr = slice_mbr;
    ue_slice_mbr_local_var->serving_snssai = serving_snssai;
    ue_slice_mbr_local_var->mapped_home_snssai = mapped_home_snssai;

    return ue_slice_mbr_local_var;
}

void OpenAPI_ue_slice_mbr_free(OpenAPI_ue_slice_mbr_t *ue_slice_mbr)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_slice_mbr) {
        return;
    }
    if (ue_slice_mbr->slice_mbr) {
        OpenAPI_list_for_each(ue_slice_mbr->slice_mbr, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_mbr_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_slice_mbr->slice_mbr);
        ue_slice_mbr->slice_mbr = NULL;
    }
    if (ue_slice_mbr->serving_snssai) {
        OpenAPI_snssai_free(ue_slice_mbr->serving_snssai);
        ue_slice_mbr->serving_snssai = NULL;
    }
    if (ue_slice_mbr->mapped_home_snssai) {
        OpenAPI_snssai_free(ue_slice_mbr->mapped_home_snssai);
        ue_slice_mbr->mapped_home_snssai = NULL;
    }
    ogs_free(ue_slice_mbr);
}

cJSON *OpenAPI_ue_slice_mbr_convertToJSON(OpenAPI_ue_slice_mbr_t *ue_slice_mbr)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_slice_mbr == NULL) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [UeSliceMbr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_slice_mbr->slice_mbr) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [slice_mbr]");
        return NULL;
    }
    cJSON *slice_mbr = cJSON_AddObjectToObject(item, "sliceMbr");
    if (slice_mbr == NULL) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [slice_mbr]");
        goto end;
    }
    cJSON *localMapObject = slice_mbr;
    if (ue_slice_mbr->slice_mbr) {
        OpenAPI_list_for_each(ue_slice_mbr->slice_mbr, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [slice_mbr]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [slice_mbr]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_slice_mbr_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

    if (!ue_slice_mbr->serving_snssai) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [serving_snssai]");
        return NULL;
    }
    cJSON *serving_snssai_local_JSON = OpenAPI_snssai_convertToJSON(ue_slice_mbr->serving_snssai);
    if (serving_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [serving_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingSnssai", serving_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [serving_snssai]");
        goto end;
    }

    if (ue_slice_mbr->mapped_home_snssai) {
    cJSON *mapped_home_snssai_local_JSON = OpenAPI_snssai_convertToJSON(ue_slice_mbr->mapped_home_snssai);
    if (mapped_home_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [mapped_home_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mappedHomeSnssai", mapped_home_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed [mapped_home_snssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_slice_mbr_t *OpenAPI_ue_slice_mbr_parseFromJSON(cJSON *ue_slice_mbrJSON)
{
    OpenAPI_ue_slice_mbr_t *ue_slice_mbr_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *slice_mbr = NULL;
    OpenAPI_list_t *slice_mbrList = NULL;
    cJSON *serving_snssai = NULL;
    OpenAPI_snssai_t *serving_snssai_local_nonprim = NULL;
    cJSON *mapped_home_snssai = NULL;
    OpenAPI_snssai_t *mapped_home_snssai_local_nonprim = NULL;
    slice_mbr = cJSON_GetObjectItemCaseSensitive(ue_slice_mbrJSON, "sliceMbr");
    if (!slice_mbr) {
        ogs_error("OpenAPI_ue_slice_mbr_parseFromJSON() failed [slice_mbr]");
        goto end;
    }
        cJSON *slice_mbr_local_map = NULL;
        if (!cJSON_IsObject(slice_mbr) && !cJSON_IsNull(slice_mbr)) {
            ogs_error("OpenAPI_ue_slice_mbr_parseFromJSON() failed [slice_mbr]");
            goto end;
        }
        if (cJSON_IsObject(slice_mbr)) {
            slice_mbrList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(slice_mbr_local_map, slice_mbr) {
                cJSON *localMapObject = slice_mbr_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_slice_mbr_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_slice_mbr_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(slice_mbrList, localMapKeyPair);
            }
        }

    serving_snssai = cJSON_GetObjectItemCaseSensitive(ue_slice_mbrJSON, "servingSnssai");
    if (!serving_snssai) {
        ogs_error("OpenAPI_ue_slice_mbr_parseFromJSON() failed [serving_snssai]");
        goto end;
    }
    serving_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(serving_snssai);
    if (!serving_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [serving_snssai]");
        goto end;
    }

    mapped_home_snssai = cJSON_GetObjectItemCaseSensitive(ue_slice_mbrJSON, "mappedHomeSnssai");
    if (mapped_home_snssai) {
    mapped_home_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(mapped_home_snssai);
    if (!mapped_home_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [mapped_home_snssai]");
        goto end;
    }
    }

    ue_slice_mbr_local_var = OpenAPI_ue_slice_mbr_create (
        slice_mbrList,
        serving_snssai_local_nonprim,
        mapped_home_snssai ? mapped_home_snssai_local_nonprim : NULL
    );

    return ue_slice_mbr_local_var;
end:
    if (slice_mbrList) {
        OpenAPI_list_for_each(slice_mbrList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_mbr_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(slice_mbrList);
        slice_mbrList = NULL;
    }
    if (serving_snssai_local_nonprim) {
        OpenAPI_snssai_free(serving_snssai_local_nonprim);
        serving_snssai_local_nonprim = NULL;
    }
    if (mapped_home_snssai_local_nonprim) {
        OpenAPI_snssai_free(mapped_home_snssai_local_nonprim);
        mapped_home_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_slice_mbr_t *OpenAPI_ue_slice_mbr_copy(OpenAPI_ue_slice_mbr_t *dst, OpenAPI_ue_slice_mbr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_slice_mbr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_slice_mbr_convertToJSON() failed");
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

    OpenAPI_ue_slice_mbr_free(dst);
    dst = OpenAPI_ue_slice_mbr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

