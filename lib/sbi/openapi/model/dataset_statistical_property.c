
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dataset_statistical_property.h"

OpenAPI_dataset_statistical_property_t *OpenAPI_dataset_statistical_property_create(
)
{
    OpenAPI_dataset_statistical_property_t *dataset_statistical_property_local_var = ogs_malloc(sizeof(OpenAPI_dataset_statistical_property_t));
    ogs_assert(dataset_statistical_property_local_var);


    return dataset_statistical_property_local_var;
}

void OpenAPI_dataset_statistical_property_free(OpenAPI_dataset_statistical_property_t *dataset_statistical_property)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dataset_statistical_property) {
        return;
    }
    ogs_free(dataset_statistical_property);
}

cJSON *OpenAPI_dataset_statistical_property_convertToJSON(OpenAPI_dataset_statistical_property_t *dataset_statistical_property)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dataset_statistical_property == NULL) {
        ogs_error("OpenAPI_dataset_statistical_property_convertToJSON() failed [DatasetStatisticalProperty]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dataset_statistical_property_t *OpenAPI_dataset_statistical_property_parseFromJSON(cJSON *dataset_statistical_propertyJSON)
{
    OpenAPI_dataset_statistical_property_t *dataset_statistical_property_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    dataset_statistical_property_local_var = OpenAPI_dataset_statistical_property_create (
    );

    return dataset_statistical_property_local_var;
end:
    return NULL;
}

OpenAPI_dataset_statistical_property_t *OpenAPI_dataset_statistical_property_copy(OpenAPI_dataset_statistical_property_t *dst, OpenAPI_dataset_statistical_property_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dataset_statistical_property_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dataset_statistical_property_convertToJSON() failed");
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

    OpenAPI_dataset_statistical_property_free(dst);
    dst = OpenAPI_dataset_statistical_property_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

