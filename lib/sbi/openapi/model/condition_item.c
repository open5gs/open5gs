
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "condition_item.h"

OpenAPI_condition_item_t *OpenAPI_condition_item_create(
    OpenAPI_list_t *consumer_nf_types,
    bool is_service_feature,
    int service_feature,
    bool is_vs_service_feature,
    int vs_service_feature,
    OpenAPI_list_t *supi_range_list,
    OpenAPI_list_t *gpsi_range_list,
    OpenAPI_list_t *impu_range_list,
    OpenAPI_list_t *impi_range_list,
    OpenAPI_list_t *pei_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *dnn_list
)
{
    OpenAPI_condition_item_t *condition_item_local_var = ogs_malloc(sizeof(OpenAPI_condition_item_t));
    ogs_assert(condition_item_local_var);

    condition_item_local_var->consumer_nf_types = consumer_nf_types;
    condition_item_local_var->is_service_feature = is_service_feature;
    condition_item_local_var->service_feature = service_feature;
    condition_item_local_var->is_vs_service_feature = is_vs_service_feature;
    condition_item_local_var->vs_service_feature = vs_service_feature;
    condition_item_local_var->supi_range_list = supi_range_list;
    condition_item_local_var->gpsi_range_list = gpsi_range_list;
    condition_item_local_var->impu_range_list = impu_range_list;
    condition_item_local_var->impi_range_list = impi_range_list;
    condition_item_local_var->pei_list = pei_list;
    condition_item_local_var->tai_range_list = tai_range_list;
    condition_item_local_var->dnn_list = dnn_list;

    return condition_item_local_var;
}

void OpenAPI_condition_item_free(OpenAPI_condition_item_t *condition_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == condition_item) {
        return;
    }
    if (condition_item->consumer_nf_types) {
        OpenAPI_list_free(condition_item->consumer_nf_types);
        condition_item->consumer_nf_types = NULL;
    }
    if (condition_item->supi_range_list) {
        OpenAPI_list_for_each(condition_item->supi_range_list, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(condition_item->supi_range_list);
        condition_item->supi_range_list = NULL;
    }
    if (condition_item->gpsi_range_list) {
        OpenAPI_list_for_each(condition_item->gpsi_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(condition_item->gpsi_range_list);
        condition_item->gpsi_range_list = NULL;
    }
    if (condition_item->impu_range_list) {
        OpenAPI_list_for_each(condition_item->impu_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(condition_item->impu_range_list);
        condition_item->impu_range_list = NULL;
    }
    if (condition_item->impi_range_list) {
        OpenAPI_list_for_each(condition_item->impi_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(condition_item->impi_range_list);
        condition_item->impi_range_list = NULL;
    }
    if (condition_item->pei_list) {
        OpenAPI_list_for_each(condition_item->pei_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(condition_item->pei_list);
        condition_item->pei_list = NULL;
    }
    if (condition_item->tai_range_list) {
        OpenAPI_list_for_each(condition_item->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(condition_item->tai_range_list);
        condition_item->tai_range_list = NULL;
    }
    if (condition_item->dnn_list) {
        OpenAPI_list_for_each(condition_item->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(condition_item->dnn_list);
        condition_item->dnn_list = NULL;
    }
    ogs_free(condition_item);
}

cJSON *OpenAPI_condition_item_convertToJSON(OpenAPI_condition_item_t *condition_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (condition_item == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [ConditionItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (condition_item->consumer_nf_types != OpenAPI_nf_type_NULL) {
    cJSON *consumer_nf_typesList = cJSON_AddArrayToObject(item, "consumerNfTypes");
    if (consumer_nf_typesList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [consumer_nf_types]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->consumer_nf_types, node) {
        if (cJSON_AddStringToObject(consumer_nf_typesList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [consumer_nf_types]");
            goto end;
        }
    }
    }

    if (condition_item->is_service_feature) {
    if (cJSON_AddNumberToObject(item, "serviceFeature", condition_item->service_feature) == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [service_feature]");
        goto end;
    }
    }

    if (condition_item->is_vs_service_feature) {
    if (cJSON_AddNumberToObject(item, "vsServiceFeature", condition_item->vs_service_feature) == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [vs_service_feature]");
        goto end;
    }
    }

    if (condition_item->supi_range_list) {
    cJSON *supi_range_listList = cJSON_AddArrayToObject(item, "supiRangeList");
    if (supi_range_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [supi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->supi_range_list, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [supi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(supi_range_listList, itemLocal);
    }
    }

    if (condition_item->gpsi_range_list) {
    cJSON *gpsi_range_listList = cJSON_AddArrayToObject(item, "gpsiRangeList");
    if (gpsi_range_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [gpsi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->gpsi_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [gpsi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_range_listList, itemLocal);
    }
    }

    if (condition_item->impu_range_list) {
    cJSON *impu_range_listList = cJSON_AddArrayToObject(item, "impuRangeList");
    if (impu_range_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [impu_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->impu_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [impu_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(impu_range_listList, itemLocal);
    }
    }

    if (condition_item->impi_range_list) {
    cJSON *impi_range_listList = cJSON_AddArrayToObject(item, "impiRangeList");
    if (impi_range_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [impi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->impi_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [impi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(impi_range_listList, itemLocal);
    }
    }

    if (condition_item->pei_list) {
    cJSON *pei_listList = cJSON_AddArrayToObject(item, "peiList");
    if (pei_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [pei_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->pei_list, node) {
        if (cJSON_AddStringToObject(pei_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [pei_list]");
            goto end;
        }
    }
    }

    if (condition_item->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (condition_item->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(condition_item->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_condition_item_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_condition_item_t *OpenAPI_condition_item_parseFromJSON(cJSON *condition_itemJSON)
{
    OpenAPI_condition_item_t *condition_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *consumer_nf_types = NULL;
    OpenAPI_list_t *consumer_nf_typesList = NULL;
    cJSON *service_feature = NULL;
    cJSON *vs_service_feature = NULL;
    cJSON *supi_range_list = NULL;
    OpenAPI_list_t *supi_range_listList = NULL;
    cJSON *gpsi_range_list = NULL;
    OpenAPI_list_t *gpsi_range_listList = NULL;
    cJSON *impu_range_list = NULL;
    OpenAPI_list_t *impu_range_listList = NULL;
    cJSON *impi_range_list = NULL;
    OpenAPI_list_t *impi_range_listList = NULL;
    cJSON *pei_list = NULL;
    OpenAPI_list_t *pei_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    consumer_nf_types = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "consumerNfTypes");
    if (consumer_nf_types) {
        cJSON *consumer_nf_types_local = NULL;
        if (!cJSON_IsArray(consumer_nf_types)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [consumer_nf_types]");
            goto end;
        }

        consumer_nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(consumer_nf_types_local, consumer_nf_types) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(consumer_nf_types_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [consumer_nf_types]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(consumer_nf_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"consumer_nf_types\" is not supported. Ignoring it ...",
                         consumer_nf_types_local->valuestring);
            } else {
                OpenAPI_list_add(consumer_nf_typesList, (void *)localEnum);
            }
        }
        if (consumer_nf_typesList->count == 0) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed: Expected consumer_nf_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    service_feature = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "serviceFeature");
    if (service_feature) {
    if (!cJSON_IsNumber(service_feature)) {
        ogs_error("OpenAPI_condition_item_parseFromJSON() failed [service_feature]");
        goto end;
    }
    }

    vs_service_feature = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "vsServiceFeature");
    if (vs_service_feature) {
    if (!cJSON_IsNumber(vs_service_feature)) {
        ogs_error("OpenAPI_condition_item_parseFromJSON() failed [vs_service_feature]");
        goto end;
    }
    }

    supi_range_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "supiRangeList");
    if (supi_range_list) {
        cJSON *supi_range_list_local = NULL;
        if (!cJSON_IsArray(supi_range_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [supi_range_list]");
            goto end;
        }

        supi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_range_list_local, supi_range_list) {
            if (!cJSON_IsObject(supi_range_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [supi_range_list]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_range_listItem = OpenAPI_supi_range_parseFromJSON(supi_range_list_local);
            if (!supi_range_listItem) {
                ogs_error("No supi_range_listItem");
                goto end;
            }
            OpenAPI_list_add(supi_range_listList, supi_range_listItem);
        }
    }

    gpsi_range_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "gpsiRangeList");
    if (gpsi_range_list) {
        cJSON *gpsi_range_list_local = NULL;
        if (!cJSON_IsArray(gpsi_range_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [gpsi_range_list]");
            goto end;
        }

        gpsi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_range_list_local, gpsi_range_list) {
            if (!cJSON_IsObject(gpsi_range_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [gpsi_range_list]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_range_listItem = OpenAPI_identity_range_parseFromJSON(gpsi_range_list_local);
            if (!gpsi_range_listItem) {
                ogs_error("No gpsi_range_listItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_range_listList, gpsi_range_listItem);
        }
    }

    impu_range_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "impuRangeList");
    if (impu_range_list) {
        cJSON *impu_range_list_local = NULL;
        if (!cJSON_IsArray(impu_range_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [impu_range_list]");
            goto end;
        }

        impu_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(impu_range_list_local, impu_range_list) {
            if (!cJSON_IsObject(impu_range_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [impu_range_list]");
                goto end;
            }
            OpenAPI_identity_range_t *impu_range_listItem = OpenAPI_identity_range_parseFromJSON(impu_range_list_local);
            if (!impu_range_listItem) {
                ogs_error("No impu_range_listItem");
                goto end;
            }
            OpenAPI_list_add(impu_range_listList, impu_range_listItem);
        }
    }

    impi_range_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "impiRangeList");
    if (impi_range_list) {
        cJSON *impi_range_list_local = NULL;
        if (!cJSON_IsArray(impi_range_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [impi_range_list]");
            goto end;
        }

        impi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(impi_range_list_local, impi_range_list) {
            if (!cJSON_IsObject(impi_range_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [impi_range_list]");
                goto end;
            }
            OpenAPI_identity_range_t *impi_range_listItem = OpenAPI_identity_range_parseFromJSON(impi_range_list_local);
            if (!impi_range_listItem) {
                ogs_error("No impi_range_listItem");
                goto end;
            }
            OpenAPI_list_add(impi_range_listList, impi_range_listItem);
        }
    }

    pei_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "peiList");
    if (pei_list) {
        cJSON *pei_list_local = NULL;
        if (!cJSON_IsArray(pei_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [pei_list]");
            goto end;
        }

        pei_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pei_list_local, pei_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pei_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [pei_list]");
                goto end;
            }
            OpenAPI_list_add(pei_listList, ogs_strdup(pei_list_local->valuestring));
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    dnn_list = cJSON_GetObjectItemCaseSensitive(condition_itemJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_condition_item_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_condition_item_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    condition_item_local_var = OpenAPI_condition_item_create (
        consumer_nf_types ? consumer_nf_typesList : NULL,
        service_feature ? true : false,
        service_feature ? service_feature->valuedouble : 0,
        vs_service_feature ? true : false,
        vs_service_feature ? vs_service_feature->valuedouble : 0,
        supi_range_list ? supi_range_listList : NULL,
        gpsi_range_list ? gpsi_range_listList : NULL,
        impu_range_list ? impu_range_listList : NULL,
        impi_range_list ? impi_range_listList : NULL,
        pei_list ? pei_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        dnn_list ? dnn_listList : NULL
    );

    return condition_item_local_var;
end:
    if (consumer_nf_typesList) {
        OpenAPI_list_free(consumer_nf_typesList);
        consumer_nf_typesList = NULL;
    }
    if (supi_range_listList) {
        OpenAPI_list_for_each(supi_range_listList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_range_listList);
        supi_range_listList = NULL;
    }
    if (gpsi_range_listList) {
        OpenAPI_list_for_each(gpsi_range_listList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_range_listList);
        gpsi_range_listList = NULL;
    }
    if (impu_range_listList) {
        OpenAPI_list_for_each(impu_range_listList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(impu_range_listList);
        impu_range_listList = NULL;
    }
    if (impi_range_listList) {
        OpenAPI_list_for_each(impi_range_listList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(impi_range_listList);
        impi_range_listList = NULL;
    }
    if (pei_listList) {
        OpenAPI_list_for_each(pei_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pei_listList);
        pei_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    return NULL;
}

OpenAPI_condition_item_t *OpenAPI_condition_item_copy(OpenAPI_condition_item_t *dst, OpenAPI_condition_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_condition_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_condition_item_convertToJSON() failed");
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

    OpenAPI_condition_item_free(dst);
    dst = OpenAPI_condition_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

