
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_unrelated_class_1.h"

OpenAPI_ranging_sl_unrelated_class_1_t *OpenAPI_ranging_sl_unrelated_class_1_create(
    OpenAPI_ranging_sl_default_unrelated_class_1_t *ranging_sl_default_unrelated_class,
    OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class,
    OpenAPI_list_t *ranging_sl_app_id_unrelated_classes
)
{
    OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class_1_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_unrelated_class_1_t));
    ogs_assert(ranging_sl_unrelated_class_1_local_var);

    ranging_sl_unrelated_class_1_local_var->ranging_sl_default_unrelated_class = ranging_sl_default_unrelated_class;
    ranging_sl_unrelated_class_1_local_var->ranging_sl_external_unrelated_class = ranging_sl_external_unrelated_class;
    ranging_sl_unrelated_class_1_local_var->ranging_sl_app_id_unrelated_classes = ranging_sl_app_id_unrelated_classes;

    return ranging_sl_unrelated_class_1_local_var;
}

void OpenAPI_ranging_sl_unrelated_class_1_free(OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_unrelated_class_1) {
        return;
    }
    if (ranging_sl_unrelated_class_1->ranging_sl_default_unrelated_class) {
        OpenAPI_ranging_sl_default_unrelated_class_1_free(ranging_sl_unrelated_class_1->ranging_sl_default_unrelated_class);
        ranging_sl_unrelated_class_1->ranging_sl_default_unrelated_class = NULL;
    }
    if (ranging_sl_unrelated_class_1->ranging_sl_external_unrelated_class) {
        OpenAPI_ranging_sl_external_unrelated_class_free(ranging_sl_unrelated_class_1->ranging_sl_external_unrelated_class);
        ranging_sl_unrelated_class_1->ranging_sl_external_unrelated_class = NULL;
    }
    if (ranging_sl_unrelated_class_1->ranging_sl_app_id_unrelated_classes) {
        OpenAPI_list_for_each(ranging_sl_unrelated_class_1->ranging_sl_app_id_unrelated_classes, node) {
            OpenAPI_ranging_sl_app_id_unrelated_class_1_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_unrelated_class_1->ranging_sl_app_id_unrelated_classes);
        ranging_sl_unrelated_class_1->ranging_sl_app_id_unrelated_classes = NULL;
    }
    ogs_free(ranging_sl_unrelated_class_1);
}

cJSON *OpenAPI_ranging_sl_unrelated_class_1_convertToJSON(OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_unrelated_class_1 == NULL) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [RangingSlUnrelatedClass_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ranging_sl_unrelated_class_1->ranging_sl_default_unrelated_class) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_default_unrelated_class]");
        return NULL;
    }
    cJSON *ranging_sl_default_unrelated_class_local_JSON = OpenAPI_ranging_sl_default_unrelated_class_1_convertToJSON(ranging_sl_unrelated_class_1->ranging_sl_default_unrelated_class);
    if (ranging_sl_default_unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_default_unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlDefaultUnrelatedClass", ranging_sl_default_unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_default_unrelated_class]");
        goto end;
    }

    if (ranging_sl_unrelated_class_1->ranging_sl_external_unrelated_class) {
    cJSON *ranging_sl_external_unrelated_class_local_JSON = OpenAPI_ranging_sl_external_unrelated_class_convertToJSON(ranging_sl_unrelated_class_1->ranging_sl_external_unrelated_class);
    if (ranging_sl_external_unrelated_class_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_external_unrelated_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlExternalUnrelatedClass", ranging_sl_external_unrelated_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_external_unrelated_class]");
        goto end;
    }
    }

    if (ranging_sl_unrelated_class_1->ranging_sl_app_id_unrelated_classes) {
    cJSON *ranging_sl_app_id_unrelated_classesList = cJSON_AddArrayToObject(item, "rangingSlAppIDUnrelatedClasses");
    if (ranging_sl_app_id_unrelated_classesList == NULL) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_app_id_unrelated_classes]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_unrelated_class_1->ranging_sl_app_id_unrelated_classes, node) {
        cJSON *itemLocal = OpenAPI_ranging_sl_app_id_unrelated_class_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed [ranging_sl_app_id_unrelated_classes]");
            goto end;
        }
        cJSON_AddItemToArray(ranging_sl_app_id_unrelated_classesList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_unrelated_class_1_t *OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON(cJSON *ranging_sl_unrelated_class_1JSON)
{
    OpenAPI_ranging_sl_unrelated_class_1_t *ranging_sl_unrelated_class_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ranging_sl_default_unrelated_class = NULL;
    OpenAPI_ranging_sl_default_unrelated_class_1_t *ranging_sl_default_unrelated_class_local_nonprim = NULL;
    cJSON *ranging_sl_external_unrelated_class = NULL;
    OpenAPI_ranging_sl_external_unrelated_class_t *ranging_sl_external_unrelated_class_local_nonprim = NULL;
    cJSON *ranging_sl_app_id_unrelated_classes = NULL;
    OpenAPI_list_t *ranging_sl_app_id_unrelated_classesList = NULL;
    ranging_sl_default_unrelated_class = cJSON_GetObjectItemCaseSensitive(ranging_sl_unrelated_class_1JSON, "rangingSlDefaultUnrelatedClass");
    if (!ranging_sl_default_unrelated_class) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON() failed [ranging_sl_default_unrelated_class]");
        goto end;
    }
    ranging_sl_default_unrelated_class_local_nonprim = OpenAPI_ranging_sl_default_unrelated_class_1_parseFromJSON(ranging_sl_default_unrelated_class);
    if (!ranging_sl_default_unrelated_class_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_default_unrelated_class_1_parseFromJSON failed [ranging_sl_default_unrelated_class]");
        goto end;
    }

    ranging_sl_external_unrelated_class = cJSON_GetObjectItemCaseSensitive(ranging_sl_unrelated_class_1JSON, "rangingSlExternalUnrelatedClass");
    if (ranging_sl_external_unrelated_class) {
    ranging_sl_external_unrelated_class_local_nonprim = OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON(ranging_sl_external_unrelated_class);
    if (!ranging_sl_external_unrelated_class_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_external_unrelated_class_parseFromJSON failed [ranging_sl_external_unrelated_class]");
        goto end;
    }
    }

    ranging_sl_app_id_unrelated_classes = cJSON_GetObjectItemCaseSensitive(ranging_sl_unrelated_class_1JSON, "rangingSlAppIDUnrelatedClasses");
    if (ranging_sl_app_id_unrelated_classes) {
        cJSON *ranging_sl_app_id_unrelated_classes_local = NULL;
        if (!cJSON_IsArray(ranging_sl_app_id_unrelated_classes)) {
            ogs_error("OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON() failed [ranging_sl_app_id_unrelated_classes]");
            goto end;
        }

        ranging_sl_app_id_unrelated_classesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ranging_sl_app_id_unrelated_classes_local, ranging_sl_app_id_unrelated_classes) {
            if (!cJSON_IsObject(ranging_sl_app_id_unrelated_classes_local)) {
                ogs_error("OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON() failed [ranging_sl_app_id_unrelated_classes]");
                goto end;
            }
            OpenAPI_ranging_sl_app_id_unrelated_class_1_t *ranging_sl_app_id_unrelated_classesItem = OpenAPI_ranging_sl_app_id_unrelated_class_1_parseFromJSON(ranging_sl_app_id_unrelated_classes_local);
            if (!ranging_sl_app_id_unrelated_classesItem) {
                ogs_error("No ranging_sl_app_id_unrelated_classesItem");
                goto end;
            }
            OpenAPI_list_add(ranging_sl_app_id_unrelated_classesList, ranging_sl_app_id_unrelated_classesItem);
        }
    }

    ranging_sl_unrelated_class_1_local_var = OpenAPI_ranging_sl_unrelated_class_1_create (
        ranging_sl_default_unrelated_class_local_nonprim,
        ranging_sl_external_unrelated_class ? ranging_sl_external_unrelated_class_local_nonprim : NULL,
        ranging_sl_app_id_unrelated_classes ? ranging_sl_app_id_unrelated_classesList : NULL
    );

    return ranging_sl_unrelated_class_1_local_var;
end:
    if (ranging_sl_default_unrelated_class_local_nonprim) {
        OpenAPI_ranging_sl_default_unrelated_class_1_free(ranging_sl_default_unrelated_class_local_nonprim);
        ranging_sl_default_unrelated_class_local_nonprim = NULL;
    }
    if (ranging_sl_external_unrelated_class_local_nonprim) {
        OpenAPI_ranging_sl_external_unrelated_class_free(ranging_sl_external_unrelated_class_local_nonprim);
        ranging_sl_external_unrelated_class_local_nonprim = NULL;
    }
    if (ranging_sl_app_id_unrelated_classesList) {
        OpenAPI_list_for_each(ranging_sl_app_id_unrelated_classesList, node) {
            OpenAPI_ranging_sl_app_id_unrelated_class_1_free(node->data);
        }
        OpenAPI_list_free(ranging_sl_app_id_unrelated_classesList);
        ranging_sl_app_id_unrelated_classesList = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_unrelated_class_1_t *OpenAPI_ranging_sl_unrelated_class_1_copy(OpenAPI_ranging_sl_unrelated_class_1_t *dst, OpenAPI_ranging_sl_unrelated_class_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_unrelated_class_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_unrelated_class_1_convertToJSON() failed");
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

    OpenAPI_ranging_sl_unrelated_class_1_free(dst);
    dst = OpenAPI_ranging_sl_unrelated_class_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

