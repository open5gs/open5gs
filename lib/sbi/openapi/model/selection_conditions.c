
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "selection_conditions.h"

OpenAPI_selection_conditions_t *OpenAPI_selection_conditions_create(
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
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *_and,
    OpenAPI_list_t *_or
)
{
    OpenAPI_selection_conditions_t *selection_conditions_local_var = ogs_malloc(sizeof(OpenAPI_selection_conditions_t));
    ogs_assert(selection_conditions_local_var);

    selection_conditions_local_var->consumer_nf_types = consumer_nf_types;
    selection_conditions_local_var->is_service_feature = is_service_feature;
    selection_conditions_local_var->service_feature = service_feature;
    selection_conditions_local_var->is_vs_service_feature = is_vs_service_feature;
    selection_conditions_local_var->vs_service_feature = vs_service_feature;
    selection_conditions_local_var->supi_range_list = supi_range_list;
    selection_conditions_local_var->gpsi_range_list = gpsi_range_list;
    selection_conditions_local_var->impu_range_list = impu_range_list;
    selection_conditions_local_var->impi_range_list = impi_range_list;
    selection_conditions_local_var->pei_list = pei_list;
    selection_conditions_local_var->tai_range_list = tai_range_list;
    selection_conditions_local_var->dnn_list = dnn_list;
    selection_conditions_local_var->_and = _and;
    selection_conditions_local_var->_or = _or;

    return selection_conditions_local_var;
}

void OpenAPI_selection_conditions_free(OpenAPI_selection_conditions_t *selection_conditions)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == selection_conditions) {
        return;
    }
    if (selection_conditions->consumer_nf_types) {
        OpenAPI_list_free(selection_conditions->consumer_nf_types);
        selection_conditions->consumer_nf_types = NULL;
    }
    if (selection_conditions->supi_range_list) {
        OpenAPI_list_for_each(selection_conditions->supi_range_list, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->supi_range_list);
        selection_conditions->supi_range_list = NULL;
    }
    if (selection_conditions->gpsi_range_list) {
        OpenAPI_list_for_each(selection_conditions->gpsi_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->gpsi_range_list);
        selection_conditions->gpsi_range_list = NULL;
    }
    if (selection_conditions->impu_range_list) {
        OpenAPI_list_for_each(selection_conditions->impu_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->impu_range_list);
        selection_conditions->impu_range_list = NULL;
    }
    if (selection_conditions->impi_range_list) {
        OpenAPI_list_for_each(selection_conditions->impi_range_list, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->impi_range_list);
        selection_conditions->impi_range_list = NULL;
    }
    if (selection_conditions->pei_list) {
        OpenAPI_list_for_each(selection_conditions->pei_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->pei_list);
        selection_conditions->pei_list = NULL;
    }
    if (selection_conditions->tai_range_list) {
        OpenAPI_list_for_each(selection_conditions->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->tai_range_list);
        selection_conditions->tai_range_list = NULL;
    }
    if (selection_conditions->dnn_list) {
        OpenAPI_list_for_each(selection_conditions->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->dnn_list);
        selection_conditions->dnn_list = NULL;
    }
    if (selection_conditions->_and) {
        OpenAPI_list_for_each(selection_conditions->_and, node) {
            OpenAPI_selection_conditions_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->_and);
        selection_conditions->_and = NULL;
    }
    if (selection_conditions->_or) {
        OpenAPI_list_for_each(selection_conditions->_or, node) {
            OpenAPI_selection_conditions_free(node->data);
        }
        OpenAPI_list_free(selection_conditions->_or);
        selection_conditions->_or = NULL;
    }
    ogs_free(selection_conditions);
}

cJSON *OpenAPI_selection_conditions_convertToJSON(OpenAPI_selection_conditions_t *selection_conditions)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (selection_conditions == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [SelectionConditions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (selection_conditions->consumer_nf_types != OpenAPI_nf_type_NULL) {
    cJSON *consumer_nf_typesList = cJSON_AddArrayToObject(item, "consumerNfTypes");
    if (consumer_nf_typesList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [consumer_nf_types]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->consumer_nf_types, node) {
        if (cJSON_AddStringToObject(consumer_nf_typesList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [consumer_nf_types]");
            goto end;
        }
    }
    }

    if (selection_conditions->is_service_feature) {
    if (cJSON_AddNumberToObject(item, "serviceFeature", selection_conditions->service_feature) == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [service_feature]");
        goto end;
    }
    }

    if (selection_conditions->is_vs_service_feature) {
    if (cJSON_AddNumberToObject(item, "vsServiceFeature", selection_conditions->vs_service_feature) == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [vs_service_feature]");
        goto end;
    }
    }

    if (selection_conditions->supi_range_list) {
    cJSON *supi_range_listList = cJSON_AddArrayToObject(item, "supiRangeList");
    if (supi_range_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [supi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->supi_range_list, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [supi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(supi_range_listList, itemLocal);
    }
    }

    if (selection_conditions->gpsi_range_list) {
    cJSON *gpsi_range_listList = cJSON_AddArrayToObject(item, "gpsiRangeList");
    if (gpsi_range_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [gpsi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->gpsi_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [gpsi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_range_listList, itemLocal);
    }
    }

    if (selection_conditions->impu_range_list) {
    cJSON *impu_range_listList = cJSON_AddArrayToObject(item, "impuRangeList");
    if (impu_range_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [impu_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->impu_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [impu_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(impu_range_listList, itemLocal);
    }
    }

    if (selection_conditions->impi_range_list) {
    cJSON *impi_range_listList = cJSON_AddArrayToObject(item, "impiRangeList");
    if (impi_range_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [impi_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->impi_range_list, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [impi_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(impi_range_listList, itemLocal);
    }
    }

    if (selection_conditions->pei_list) {
    cJSON *pei_listList = cJSON_AddArrayToObject(item, "peiList");
    if (pei_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [pei_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->pei_list, node) {
        if (cJSON_AddStringToObject(pei_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [pei_list]");
            goto end;
        }
    }
    }

    if (selection_conditions->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (selection_conditions->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (selection_conditions->_and) {
    cJSON *_andList = cJSON_AddArrayToObject(item, "and");
    if (_andList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [_and]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->_and, node) {
        cJSON *itemLocal = OpenAPI_selection_conditions_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [_and]");
            goto end;
        }
        cJSON_AddItemToArray(_andList, itemLocal);
    }
    }

    if (selection_conditions->_or) {
    cJSON *_orList = cJSON_AddArrayToObject(item, "or");
    if (_orList == NULL) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [_or]");
        goto end;
    }
    OpenAPI_list_for_each(selection_conditions->_or, node) {
        cJSON *itemLocal = OpenAPI_selection_conditions_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_selection_conditions_convertToJSON() failed [_or]");
            goto end;
        }
        cJSON_AddItemToArray(_orList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_selection_conditions_t *OpenAPI_selection_conditions_parseFromJSON(cJSON *selection_conditionsJSON)
{
    OpenAPI_selection_conditions_t *selection_conditions_local_var = NULL;
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
    cJSON *_and = NULL;
    OpenAPI_list_t *_andList = NULL;
    cJSON *_or = NULL;
    OpenAPI_list_t *_orList = NULL;
    consumer_nf_types = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "consumerNfTypes");
    if (consumer_nf_types) {
        cJSON *consumer_nf_types_local = NULL;
        if (!cJSON_IsArray(consumer_nf_types)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [consumer_nf_types]");
            goto end;
        }

        consumer_nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(consumer_nf_types_local, consumer_nf_types) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(consumer_nf_types_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [consumer_nf_types]");
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
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed: Expected consumer_nf_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    service_feature = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "serviceFeature");
    if (service_feature) {
    if (!cJSON_IsNumber(service_feature)) {
        ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [service_feature]");
        goto end;
    }
    }

    vs_service_feature = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "vsServiceFeature");
    if (vs_service_feature) {
    if (!cJSON_IsNumber(vs_service_feature)) {
        ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [vs_service_feature]");
        goto end;
    }
    }

    supi_range_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "supiRangeList");
    if (supi_range_list) {
        cJSON *supi_range_list_local = NULL;
        if (!cJSON_IsArray(supi_range_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [supi_range_list]");
            goto end;
        }

        supi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_range_list_local, supi_range_list) {
            if (!cJSON_IsObject(supi_range_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [supi_range_list]");
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

    gpsi_range_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "gpsiRangeList");
    if (gpsi_range_list) {
        cJSON *gpsi_range_list_local = NULL;
        if (!cJSON_IsArray(gpsi_range_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [gpsi_range_list]");
            goto end;
        }

        gpsi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_range_list_local, gpsi_range_list) {
            if (!cJSON_IsObject(gpsi_range_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [gpsi_range_list]");
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

    impu_range_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "impuRangeList");
    if (impu_range_list) {
        cJSON *impu_range_list_local = NULL;
        if (!cJSON_IsArray(impu_range_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [impu_range_list]");
            goto end;
        }

        impu_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(impu_range_list_local, impu_range_list) {
            if (!cJSON_IsObject(impu_range_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [impu_range_list]");
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

    impi_range_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "impiRangeList");
    if (impi_range_list) {
        cJSON *impi_range_list_local = NULL;
        if (!cJSON_IsArray(impi_range_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [impi_range_list]");
            goto end;
        }

        impi_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(impi_range_list_local, impi_range_list) {
            if (!cJSON_IsObject(impi_range_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [impi_range_list]");
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

    pei_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "peiList");
    if (pei_list) {
        cJSON *pei_list_local = NULL;
        if (!cJSON_IsArray(pei_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [pei_list]");
            goto end;
        }

        pei_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pei_list_local, pei_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pei_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [pei_list]");
                goto end;
            }
            OpenAPI_list_add(pei_listList, ogs_strdup(pei_list_local->valuestring));
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [tai_range_list]");
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

    dnn_list = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    _and = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "and");
    if (_and) {
        cJSON *_and_local = NULL;
        if (!cJSON_IsArray(_and)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [_and]");
            goto end;
        }

        _andList = OpenAPI_list_create();

        cJSON_ArrayForEach(_and_local, _and) {
            if (!cJSON_IsObject(_and_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [_and]");
                goto end;
            }
            OpenAPI_selection_conditions_t *_andItem = OpenAPI_selection_conditions_parseFromJSON(_and_local);
            if (!_andItem) {
                ogs_error("No _andItem");
                goto end;
            }
            OpenAPI_list_add(_andList, _andItem);
        }
    }

    _or = cJSON_GetObjectItemCaseSensitive(selection_conditionsJSON, "or");
    if (_or) {
        cJSON *_or_local = NULL;
        if (!cJSON_IsArray(_or)) {
            ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [_or]");
            goto end;
        }

        _orList = OpenAPI_list_create();

        cJSON_ArrayForEach(_or_local, _or) {
            if (!cJSON_IsObject(_or_local)) {
                ogs_error("OpenAPI_selection_conditions_parseFromJSON() failed [_or]");
                goto end;
            }
            OpenAPI_selection_conditions_t *_orItem = OpenAPI_selection_conditions_parseFromJSON(_or_local);
            if (!_orItem) {
                ogs_error("No _orItem");
                goto end;
            }
            OpenAPI_list_add(_orList, _orItem);
        }
    }

    selection_conditions_local_var = OpenAPI_selection_conditions_create (
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
        dnn_list ? dnn_listList : NULL,
        _and ? _andList : NULL,
        _or ? _orList : NULL
    );

    return selection_conditions_local_var;
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
    if (_andList) {
        OpenAPI_list_for_each(_andList, node) {
            OpenAPI_selection_conditions_free(node->data);
        }
        OpenAPI_list_free(_andList);
        _andList = NULL;
    }
    if (_orList) {
        OpenAPI_list_for_each(_orList, node) {
            OpenAPI_selection_conditions_free(node->data);
        }
        OpenAPI_list_free(_orList);
        _orList = NULL;
    }
    return NULL;
}

OpenAPI_selection_conditions_t *OpenAPI_selection_conditions_copy(OpenAPI_selection_conditions_t *dst, OpenAPI_selection_conditions_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_selection_conditions_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_selection_conditions_convertToJSON() failed");
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

    OpenAPI_selection_conditions_free(dst);
    dst = OpenAPI_selection_conditions_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

