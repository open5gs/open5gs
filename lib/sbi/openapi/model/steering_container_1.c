
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "steering_container_1.h"

OpenAPI_steering_container_1_t *OpenAPI_steering_container_1_create(
    OpenAPI_list_t *steering_info_list,
    char *secured_packet
)
{
    OpenAPI_steering_container_1_t *steering_container_1_local_var = ogs_malloc(sizeof(OpenAPI_steering_container_1_t));
    ogs_assert(steering_container_1_local_var);

    steering_container_1_local_var->steering_info_list = steering_info_list;
    steering_container_1_local_var->secured_packet = secured_packet;

    return steering_container_1_local_var;
}

void OpenAPI_steering_container_1_free(OpenAPI_steering_container_1_t *steering_container_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == steering_container_1) {
        return;
    }
    if (steering_container_1->steering_info_list) {
        OpenAPI_list_for_each(steering_container_1->steering_info_list, node) {
            OpenAPI_steering_info_1_free(node->data);
        }
        OpenAPI_list_free(steering_container_1->steering_info_list);
        steering_container_1->steering_info_list = NULL;
    }
    if (steering_container_1->secured_packet) {
        ogs_free(steering_container_1->secured_packet);
        steering_container_1->secured_packet = NULL;
    }
    ogs_free(steering_container_1);
}

/*
 * oneOf union (x-open5gs-union): the members are the alternatives of the
 * original oneOf. Exactly one is set, and it is the JSON value itself
 * rather than a property of a wrapper object.
 */
cJSON *OpenAPI_steering_container_1_convertToJSON(OpenAPI_steering_container_1_t *steering_container_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (steering_container_1 == NULL) {
        ogs_error("OpenAPI_steering_container_1_convertToJSON() failed [SteeringContainer_1]");
        return NULL;
    }

    if ((steering_container_1->steering_info_list != NULL) +
            (steering_container_1->secured_packet != NULL) != 1) {
        ogs_error("OpenAPI_steering_container_1_convertToJSON() failed [SteeringContainer_1]: exactly one alternative must be set");
        return NULL;
    }

    if (steering_container_1->steering_info_list) {
        item = cJSON_CreateArray();
        if (item == NULL) {
            ogs_error("OpenAPI_steering_container_1_convertToJSON() failed [steering_info_list]");
            return NULL;
        }
        OpenAPI_list_for_each(steering_container_1->steering_info_list, node) {
            cJSON *itemLocal = OpenAPI_steering_info_1_convertToJSON(node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_steering_container_1_convertToJSON() failed [steering_info_list]");
                cJSON_Delete(item);
                return NULL;
            }
            cJSON_AddItemToArray(item, itemLocal);
        }
        return item;
    }
    if (steering_container_1->secured_packet) {
        return cJSON_CreateString(steering_container_1->secured_packet);
    }

    ogs_error("OpenAPI_steering_container_1_convertToJSON() failed [SteeringContainer_1]");
    return NULL;
}

OpenAPI_steering_container_1_t *OpenAPI_steering_container_1_parseFromJSON(cJSON *steering_container_1JSON)
{
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_list_t *steering_info_list = NULL;
    char *secured_packet = NULL;

    if (cJSON_IsArray(steering_container_1JSON)) {
        cJSON *steering_info_list_local = NULL;

        steering_info_list = OpenAPI_list_create();

        cJSON_ArrayForEach(steering_info_list_local, steering_container_1JSON) {
            if (!cJSON_IsObject(steering_info_list_local)) {
                ogs_error("OpenAPI_steering_container_1_parseFromJSON() failed [steering_info_list]");
                goto end;
            }
            OpenAPI_steering_info_1_t *steering_info_listItem = OpenAPI_steering_info_1_parseFromJSON(steering_info_list_local);
            if (!steering_info_listItem) {
                ogs_error("No steering_info_listItem");
                goto end;
            }
            OpenAPI_list_add(steering_info_list, steering_info_listItem);
        }
    } else if (cJSON_IsString(steering_container_1JSON)) {
        secured_packet = ogs_strdup(steering_container_1JSON->valuestring);
        ogs_assert(secured_packet);
    } else {
        ogs_error("OpenAPI_steering_container_1_parseFromJSON() failed [SteeringContainer_1]");
        goto end;
    }

    return OpenAPI_steering_container_1_create(steering_info_list, secured_packet);
end:
    if (steering_info_list) {
        OpenAPI_list_for_each(steering_info_list, node) {
            OpenAPI_steering_info_1_free(node->data);
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

OpenAPI_steering_container_1_t *OpenAPI_steering_container_1_copy(OpenAPI_steering_container_1_t *dst, OpenAPI_steering_container_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_steering_container_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_steering_container_1_convertToJSON() failed");
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

    OpenAPI_steering_container_1_free(dst);
    dst = OpenAPI_steering_container_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

