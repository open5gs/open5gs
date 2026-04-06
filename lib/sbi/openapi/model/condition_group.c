
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "condition_group.h"

OpenAPI_condition_group_t *OpenAPI_condition_group_create(
    OpenAPI_list_t *_and,
    OpenAPI_list_t *_or
)
{
    OpenAPI_condition_group_t *condition_group_local_var = ogs_malloc(sizeof(OpenAPI_condition_group_t));
    ogs_assert(condition_group_local_var);

    condition_group_local_var->_and = _and;
    condition_group_local_var->_or = _or;

    return condition_group_local_var;
}

void OpenAPI_condition_group_free(OpenAPI_condition_group_t *condition_group)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == condition_group) {
        return;
    }
    if (condition_group->_and) {
        OpenAPI_list_for_each(condition_group->_and, node) {
            OpenAPI_selection_conditions_free(node->data);
        }
        OpenAPI_list_free(condition_group->_and);
        condition_group->_and = NULL;
    }
    if (condition_group->_or) {
        OpenAPI_list_for_each(condition_group->_or, node) {
            OpenAPI_selection_conditions_free(node->data);
        }
        OpenAPI_list_free(condition_group->_or);
        condition_group->_or = NULL;
    }
    ogs_free(condition_group);
}

cJSON *OpenAPI_condition_group_convertToJSON(OpenAPI_condition_group_t *condition_group)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (condition_group == NULL) {
        ogs_error("OpenAPI_condition_group_convertToJSON() failed [ConditionGroup]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (condition_group->_and) {
    cJSON *_andList = cJSON_AddArrayToObject(item, "and");
    if (_andList == NULL) {
        ogs_error("OpenAPI_condition_group_convertToJSON() failed [_and]");
        goto end;
    }
    OpenAPI_list_for_each(condition_group->_and, node) {
        cJSON *itemLocal = OpenAPI_selection_conditions_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_group_convertToJSON() failed [_and]");
            goto end;
        }
        cJSON_AddItemToArray(_andList, itemLocal);
    }
    }

    if (condition_group->_or) {
    cJSON *_orList = cJSON_AddArrayToObject(item, "or");
    if (_orList == NULL) {
        ogs_error("OpenAPI_condition_group_convertToJSON() failed [_or]");
        goto end;
    }
    OpenAPI_list_for_each(condition_group->_or, node) {
        cJSON *itemLocal = OpenAPI_selection_conditions_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_condition_group_convertToJSON() failed [_or]");
            goto end;
        }
        cJSON_AddItemToArray(_orList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_condition_group_t *OpenAPI_condition_group_parseFromJSON(cJSON *condition_groupJSON)
{
    OpenAPI_condition_group_t *condition_group_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_and = NULL;
    OpenAPI_list_t *_andList = NULL;
    cJSON *_or = NULL;
    OpenAPI_list_t *_orList = NULL;
    _and = cJSON_GetObjectItemCaseSensitive(condition_groupJSON, "and");
    if (_and) {
        cJSON *_and_local = NULL;
        if (!cJSON_IsArray(_and)) {
            ogs_error("OpenAPI_condition_group_parseFromJSON() failed [_and]");
            goto end;
        }

        _andList = OpenAPI_list_create();

        cJSON_ArrayForEach(_and_local, _and) {
            if (!cJSON_IsObject(_and_local)) {
                ogs_error("OpenAPI_condition_group_parseFromJSON() failed [_and]");
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

    _or = cJSON_GetObjectItemCaseSensitive(condition_groupJSON, "or");
    if (_or) {
        cJSON *_or_local = NULL;
        if (!cJSON_IsArray(_or)) {
            ogs_error("OpenAPI_condition_group_parseFromJSON() failed [_or]");
            goto end;
        }

        _orList = OpenAPI_list_create();

        cJSON_ArrayForEach(_or_local, _or) {
            if (!cJSON_IsObject(_or_local)) {
                ogs_error("OpenAPI_condition_group_parseFromJSON() failed [_or]");
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

    condition_group_local_var = OpenAPI_condition_group_create (
        _and ? _andList : NULL,
        _or ? _orList : NULL
    );

    return condition_group_local_var;
end:
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

OpenAPI_condition_group_t *OpenAPI_condition_group_copy(OpenAPI_condition_group_t *dst, OpenAPI_condition_group_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_condition_group_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_condition_group_convertToJSON() failed");
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

    OpenAPI_condition_group_free(dst);
    dst = OpenAPI_condition_group_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

