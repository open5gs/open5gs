
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_container.h"

OpenAPI_steering_container_t *OpenAPI_steering_container_create(
    OpenAPI_list_t *steering_info_list,
    char *secured_packet
)
{
    OpenAPI_steering_container_t *steering_container_local_var = ogs_malloc(sizeof(OpenAPI_steering_container_t));
    ogs_assert(steering_container_local_var);

    steering_container_local_var->steering_info_list = steering_info_list;
    steering_container_local_var->secured_packet = secured_packet;

    return steering_container_local_var;
}

void OpenAPI_steering_container_free(OpenAPI_steering_container_t *steering_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == steering_container) {
        return;
    }
    if (steering_container->steering_info_list) {
        OpenAPI_list_for_each(steering_container->steering_info_list, node) {
            OpenAPI_steering_info_free(node->data);
        }
        OpenAPI_list_free(steering_container->steering_info_list);
        steering_container->steering_info_list = NULL;
    }
    if (steering_container->secured_packet) {
        ogs_free(steering_container->secured_packet);
        steering_container->secured_packet = NULL;
    }
    ogs_free(steering_container);
}

/*
 * oneOf union (x-open5gs-union): the members are the alternatives of the
 * original oneOf. Exactly one is set, and it is the JSON value itself
 * rather than a property of a wrapper object.
 */
cJSON *OpenAPI_steering_container_convertToJSON(OpenAPI_steering_container_t *steering_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (steering_container == NULL) {
        ogs_error("OpenAPI_steering_container_convertToJSON() failed [SteeringContainer]");
        return NULL;
    }

    if ((steering_container->steering_info_list != NULL) +
            (steering_container->secured_packet != NULL) != 1) {
        ogs_error("OpenAPI_steering_container_convertToJSON() failed [SteeringContainer]: exactly one alternative must be set");
        return NULL;
    }

    if (steering_container->steering_info_list) {
        item = cJSON_CreateArray();
        if (item == NULL) {
            ogs_error("OpenAPI_steering_container_convertToJSON() failed [steering_info_list]");
            return NULL;
        }
        OpenAPI_list_for_each(steering_container->steering_info_list, node) {
            cJSON *itemLocal = OpenAPI_steering_info_convertToJSON(node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_steering_container_convertToJSON() failed [steering_info_list]");
                cJSON_Delete(item);
                return NULL;
            }
            cJSON_AddItemToArray(item, itemLocal);
        }
        return item;
    }
    if (steering_container->secured_packet) {
        return cJSON_CreateString(steering_container->secured_packet);
    }

    ogs_error("OpenAPI_steering_container_convertToJSON() failed [SteeringContainer]");
    return NULL;
}

OpenAPI_steering_container_t *OpenAPI_steering_container_parseFromJSON(cJSON *steering_containerJSON)
{
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_list_t *steering_info_list = NULL;
    char *secured_packet = NULL;

    if (cJSON_IsArray(steering_containerJSON)) {
        cJSON *steering_info_list_local = NULL;

        steering_info_list = OpenAPI_list_create();

        cJSON_ArrayForEach(steering_info_list_local, steering_containerJSON) {
            if (!cJSON_IsObject(steering_info_list_local)) {
                ogs_error("OpenAPI_steering_container_parseFromJSON() failed [steering_info_list]");
                goto end;
            }
            OpenAPI_steering_info_t *steering_info_listItem = OpenAPI_steering_info_parseFromJSON(steering_info_list_local);
            if (!steering_info_listItem) {
                ogs_error("No steering_info_listItem");
                goto end;
            }
            OpenAPI_list_add(steering_info_list, steering_info_listItem);
        }
    } else if (cJSON_IsString(steering_containerJSON)) {
        secured_packet = ogs_strdup(steering_containerJSON->valuestring);
        ogs_assert(secured_packet);
    } else {
        ogs_error("OpenAPI_steering_container_parseFromJSON() failed [SteeringContainer]");
        goto end;
    }

    return OpenAPI_steering_container_create(steering_info_list, secured_packet);
end:
    if (steering_info_list) {
        OpenAPI_list_for_each(steering_info_list, node) {
            OpenAPI_steering_info_free(node->data);
        }
        OpenAPI_list_free(steering_info_list);
        steering_info_list = NULL;
    }
    if (secured_packet) {
        ogs_free(secured_packet);
        secured_packet = NULL;
    }
    return NULL;
}

OpenAPI_steering_container_t *OpenAPI_steering_container_copy(OpenAPI_steering_container_t *dst, OpenAPI_steering_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_container_convertToJSON() failed");
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

    OpenAPI_steering_container_free(dst);
    dst = OpenAPI_steering_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

