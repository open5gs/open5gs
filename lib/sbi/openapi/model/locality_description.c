
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "locality_description.h"

OpenAPI_locality_description_t *OpenAPI_locality_description_create(
    OpenAPI_locality_type_e locality_type,
    char *locality_value,
    OpenAPI_list_t *addl_loc_descr_items
)
{
    OpenAPI_locality_description_t *locality_description_local_var = ogs_malloc(sizeof(OpenAPI_locality_description_t));
    ogs_assert(locality_description_local_var);

    locality_description_local_var->locality_type = locality_type;
    locality_description_local_var->locality_value = locality_value;
    locality_description_local_var->addl_loc_descr_items = addl_loc_descr_items;

    return locality_description_local_var;
}

void OpenAPI_locality_description_free(OpenAPI_locality_description_t *locality_description)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == locality_description) {
        return;
    }
    if (locality_description->locality_value) {
        ogs_free(locality_description->locality_value);
        locality_description->locality_value = NULL;
    }
    if (locality_description->addl_loc_descr_items) {
        OpenAPI_list_for_each(locality_description->addl_loc_descr_items, node) {
            OpenAPI_locality_description_item_free(node->data);
        }
        OpenAPI_list_free(locality_description->addl_loc_descr_items);
        locality_description->addl_loc_descr_items = NULL;
    }
    ogs_free(locality_description);
}

cJSON *OpenAPI_locality_description_convertToJSON(OpenAPI_locality_description_t *locality_description)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (locality_description == NULL) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed [LocalityDescription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (locality_description->locality_type == OpenAPI_locality_type_NULL) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed [locality_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "localityType", OpenAPI_locality_type_ToString(locality_description->locality_type)) == NULL) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed [locality_type]");
        goto end;
    }

    if (!locality_description->locality_value) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed [locality_value]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "localityValue", locality_description->locality_value) == NULL) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed [locality_value]");
        goto end;
    }

    if (locality_description->addl_loc_descr_items) {
    cJSON *addl_loc_descr_itemsList = cJSON_AddArrayToObject(item, "addlLocDescrItems");
    if (addl_loc_descr_itemsList == NULL) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed [addl_loc_descr_items]");
        goto end;
    }
    OpenAPI_list_for_each(locality_description->addl_loc_descr_items, node) {
        cJSON *itemLocal = OpenAPI_locality_description_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_locality_description_convertToJSON() failed [addl_loc_descr_items]");
            goto end;
        }
        cJSON_AddItemToArray(addl_loc_descr_itemsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_locality_description_t *OpenAPI_locality_description_parseFromJSON(cJSON *locality_descriptionJSON)
{
    OpenAPI_locality_description_t *locality_description_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *locality_type = NULL;
    OpenAPI_locality_type_e locality_typeVariable = 0;
    cJSON *locality_value = NULL;
    cJSON *addl_loc_descr_items = NULL;
    OpenAPI_list_t *addl_loc_descr_itemsList = NULL;
    locality_type = cJSON_GetObjectItemCaseSensitive(locality_descriptionJSON, "localityType");
    if (!locality_type) {
        ogs_error("OpenAPI_locality_description_parseFromJSON() failed [locality_type]");
        goto end;
    }
    if (!cJSON_IsString(locality_type)) {
        ogs_error("OpenAPI_locality_description_parseFromJSON() failed [locality_type]");
        goto end;
    }
    locality_typeVariable = OpenAPI_locality_type_FromString(locality_type->valuestring);

    locality_value = cJSON_GetObjectItemCaseSensitive(locality_descriptionJSON, "localityValue");
    if (!locality_value) {
        ogs_error("OpenAPI_locality_description_parseFromJSON() failed [locality_value]");
        goto end;
    }
    if (!cJSON_IsString(locality_value)) {
        ogs_error("OpenAPI_locality_description_parseFromJSON() failed [locality_value]");
        goto end;
    }

    addl_loc_descr_items = cJSON_GetObjectItemCaseSensitive(locality_descriptionJSON, "addlLocDescrItems");
    if (addl_loc_descr_items) {
        cJSON *addl_loc_descr_items_local = NULL;
        if (!cJSON_IsArray(addl_loc_descr_items)) {
            ogs_error("OpenAPI_locality_description_parseFromJSON() failed [addl_loc_descr_items]");
            goto end;
        }

        addl_loc_descr_itemsList = OpenAPI_list_create();

        cJSON_ArrayForEach(addl_loc_descr_items_local, addl_loc_descr_items) {
            if (!cJSON_IsObject(addl_loc_descr_items_local)) {
                ogs_error("OpenAPI_locality_description_parseFromJSON() failed [addl_loc_descr_items]");
                goto end;
            }
            OpenAPI_locality_description_item_t *addl_loc_descr_itemsItem = OpenAPI_locality_description_item_parseFromJSON(addl_loc_descr_items_local);
            if (!addl_loc_descr_itemsItem) {
                ogs_error("No addl_loc_descr_itemsItem");
                goto end;
            }
            OpenAPI_list_add(addl_loc_descr_itemsList, addl_loc_descr_itemsItem);
        }
    }

    locality_description_local_var = OpenAPI_locality_description_create (
        locality_typeVariable,
        ogs_strdup(locality_value->valuestring),
        addl_loc_descr_items ? addl_loc_descr_itemsList : NULL
    );

    return locality_description_local_var;
end:
    if (addl_loc_descr_itemsList) {
        OpenAPI_list_for_each(addl_loc_descr_itemsList, node) {
            OpenAPI_locality_description_item_free(node->data);
        }
        OpenAPI_list_free(addl_loc_descr_itemsList);
        addl_loc_descr_itemsList = NULL;
    }
    return NULL;
}

OpenAPI_locality_description_t *OpenAPI_locality_description_copy(OpenAPI_locality_description_t *dst, OpenAPI_locality_description_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_locality_description_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_locality_description_convertToJSON() failed");
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

    OpenAPI_locality_description_free(dst);
    dst = OpenAPI_locality_description_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

