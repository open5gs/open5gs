
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "unrelated_class.h"

OpenAPI_unrelated_class_t *OpenAPI_unrelated_class_create(
    OpenAPI_default_unrelated_class_t *default_unrelated_class,
    OpenAPI_external_unrelated_class_t *external_unrelated_class,
    OpenAPI_list_t *service_type_unrelated_classes
)
{
    OpenAPI_unrelated_class_t *unrelated_class_local_var = ogs_malloc(sizeof(OpenAPI_unrelated_class_t));
    ogs_assert(unrelated_class_local_var);

    unrelated_class_local_var->default_unrelated_class = default_unrelated_class;
    unrelated_class_local_var->external_unrelated_class = external_unrelated_class;
    unrelated_class_local_var->service_type_unrelated_classes = service_type_unrelated_classes;

    return unrelated_class_local_var;
}

void OpenAPI_unrelated_class_free(OpenAPI_unrelated_class_t *unrelated_class)
{
    if (NULL == unrelated_class) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_default_unrelated_class_free(unrelated_class->default_unrelated_class);
    OpenAPI_external_unrelated_class_free(unrelated_class->external_unrelated_class);
    OpenAPI_list_for_each(unrelated_class->service_type_unrelated_classes, node) {
        OpenAPI_service_type_unrelated_class_free(node->data);
    }
    OpenAPI_list_free(unrelated_class->service_type_unrelated_classes);
    ogs_free(unrelated_class);
}

cJSON *OpenAPI_unrelated_class_convertToJSON(OpenAPI_unrelated_class_t *unrelated_class)
{
    cJSON *item = NULL;

    if (unrelated_class == NULL) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [UnrelatedClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *default_unrelated_class_local_JSON = OpenAPI_default_unrelated_class_convertToJSON(unrelated_class->default_unrelated_class);
    if (default_unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [default_unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "defaultUnrelatedClass", default_unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [default_unrelated_class]");
        goto end;
    }

    if (unrelated_class->external_unrelated_class) {
    cJSON *external_unrelated_class_local_JSON = OpenAPI_external_unrelated_class_convertToJSON(unrelated_class->external_unrelated_class);
    if (external_unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [external_unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "externalUnrelatedClass", external_unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [external_unrelated_class]");
        goto end;
    }
    }

    if (unrelated_class->service_type_unrelated_classes) {
    cJSON *service_type_unrelated_classesList = cJSON_AddArrayToObject(item, "serviceTypeUnrelatedClasses");
    if (service_type_unrelated_classesList == NULL) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [service_type_unrelated_classes]");
        goto end;
    }

    OpenAPI_lnode_t *service_type_unrelated_classes_node;
    if (unrelated_class->service_type_unrelated_classes) {
        OpenAPI_list_for_each(unrelated_class->service_type_unrelated_classes, service_type_unrelated_classes_node) {
            cJSON *itemLocal = OpenAPI_service_type_unrelated_class_convertToJSON(service_type_unrelated_classes_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_unrelated_class_convertToJSON() failed [service_type_unrelated_classes]");
                goto end;
            }
            cJSON_AddItemToArray(service_type_unrelated_classesList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_unrelated_class_t *OpenAPI_unrelated_class_parseFromJSON(cJSON *unrelated_classJSON)
{
    OpenAPI_unrelated_class_t *unrelated_class_local_var = NULL;
    cJSON *default_unrelated_class = cJSON_GetObjectItemCaseSensitive(unrelated_classJSON, "defaultUnrelatedClass");
    if (!default_unrelated_class) {
        ogs_error("OpenAPI_unrelated_class_parseFromJSON() failed [default_unrelated_class]");
        goto end;
    }

    OpenAPI_default_unrelated_class_t *default_unrelated_class_local_nonprim = NULL;
    default_unrelated_class_local_nonprim = OpenAPI_default_unrelated_class_parseFromJSON(default_unrelated_class);

    cJSON *external_unrelated_class = cJSON_GetObjectItemCaseSensitive(unrelated_classJSON, "externalUnrelatedClass");

    OpenAPI_external_unrelated_class_t *external_unrelated_class_local_nonprim = NULL;
    if (external_unrelated_class) {
    external_unrelated_class_local_nonprim = OpenAPI_external_unrelated_class_parseFromJSON(external_unrelated_class);
    }

    cJSON *service_type_unrelated_classes = cJSON_GetObjectItemCaseSensitive(unrelated_classJSON, "serviceTypeUnrelatedClasses");

    OpenAPI_list_t *service_type_unrelated_classesList;
    if (service_type_unrelated_classes) {
    cJSON *service_type_unrelated_classes_local_nonprimitive;
    if (!cJSON_IsArray(service_type_unrelated_classes)){
        ogs_error("OpenAPI_unrelated_class_parseFromJSON() failed [service_type_unrelated_classes]");
        goto end;
    }

    service_type_unrelated_classesList = OpenAPI_list_create();

    cJSON_ArrayForEach(service_type_unrelated_classes_local_nonprimitive, service_type_unrelated_classes ) {
        if (!cJSON_IsObject(service_type_unrelated_classes_local_nonprimitive)) {
            ogs_error("OpenAPI_unrelated_class_parseFromJSON() failed [service_type_unrelated_classes]");
            goto end;
        }
        OpenAPI_service_type_unrelated_class_t *service_type_unrelated_classesItem = OpenAPI_service_type_unrelated_class_parseFromJSON(service_type_unrelated_classes_local_nonprimitive);

        if (!service_type_unrelated_classesItem) {
            ogs_error("No service_type_unrelated_classesItem");
            OpenAPI_list_free(service_type_unrelated_classesList);
            goto end;
        }

        OpenAPI_list_add(service_type_unrelated_classesList, service_type_unrelated_classesItem);
    }
    }

    unrelated_class_local_var = OpenAPI_unrelated_class_create (
        default_unrelated_class_local_nonprim,
        external_unrelated_class ? external_unrelated_class_local_nonprim : NULL,
        service_type_unrelated_classes ? service_type_unrelated_classesList : NULL
    );

    return unrelated_class_local_var;
end:
    return NULL;
}

OpenAPI_unrelated_class_t *OpenAPI_unrelated_class_copy(OpenAPI_unrelated_class_t *dst, OpenAPI_unrelated_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_unrelated_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_unrelated_class_convertToJSON() failed");
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

    OpenAPI_unrelated_class_free(dst);
    dst = OpenAPI_unrelated_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

