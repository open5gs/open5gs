
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "early_media_description.h"

OpenAPI_early_media_description_t *OpenAPI_early_media_description_create(
    OpenAPI_sdp_time_stamps_t *s_dp_time_stamps,
    OpenAPI_list_t *s_dp_media_component,
    OpenAPI_list_t *s_dp_session_description
)
{
    OpenAPI_early_media_description_t *early_media_description_local_var = ogs_malloc(sizeof(OpenAPI_early_media_description_t));
    ogs_assert(early_media_description_local_var);

    early_media_description_local_var->s_dp_time_stamps = s_dp_time_stamps;
    early_media_description_local_var->s_dp_media_component = s_dp_media_component;
    early_media_description_local_var->s_dp_session_description = s_dp_session_description;

    return early_media_description_local_var;
}

void OpenAPI_early_media_description_free(OpenAPI_early_media_description_t *early_media_description)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == early_media_description) {
        return;
    }
    if (early_media_description->s_dp_time_stamps) {
        OpenAPI_sdp_time_stamps_free(early_media_description->s_dp_time_stamps);
        early_media_description->s_dp_time_stamps = NULL;
    }
    if (early_media_description->s_dp_media_component) {
        OpenAPI_list_for_each(early_media_description->s_dp_media_component, node) {
            OpenAPI_sdp_media_component_free(node->data);
        }
        OpenAPI_list_free(early_media_description->s_dp_media_component);
        early_media_description->s_dp_media_component = NULL;
    }
    if (early_media_description->s_dp_session_description) {
        OpenAPI_list_for_each(early_media_description->s_dp_session_description, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(early_media_description->s_dp_session_description);
        early_media_description->s_dp_session_description = NULL;
    }
    ogs_free(early_media_description);
}

cJSON *OpenAPI_early_media_description_convertToJSON(OpenAPI_early_media_description_t *early_media_description)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (early_media_description == NULL) {
        ogs_error("OpenAPI_early_media_description_convertToJSON() failed [EarlyMediaDescription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (early_media_description->s_dp_time_stamps) {
    cJSON *s_dp_time_stamps_local_JSON = OpenAPI_sdp_time_stamps_convertToJSON(early_media_description->s_dp_time_stamps);
    if (s_dp_time_stamps_local_JSON == NULL) {
        ogs_error("OpenAPI_early_media_description_convertToJSON() failed [s_dp_time_stamps]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sDPTimeStamps", s_dp_time_stamps_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_early_media_description_convertToJSON() failed [s_dp_time_stamps]");
        goto end;
    }
    }

    if (early_media_description->s_dp_media_component) {
    cJSON *s_dp_media_componentList = cJSON_AddArrayToObject(item, "sDPMediaComponent");
    if (s_dp_media_componentList == NULL) {
        ogs_error("OpenAPI_early_media_description_convertToJSON() failed [s_dp_media_component]");
        goto end;
    }
    OpenAPI_list_for_each(early_media_description->s_dp_media_component, node) {
        cJSON *itemLocal = OpenAPI_sdp_media_component_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_early_media_description_convertToJSON() failed [s_dp_media_component]");
            goto end;
        }
        cJSON_AddItemToArray(s_dp_media_componentList, itemLocal);
    }
    }

    if (early_media_description->s_dp_session_description) {
    cJSON *s_dp_session_descriptionList = cJSON_AddArrayToObject(item, "sDPSessionDescription");
    if (s_dp_session_descriptionList == NULL) {
        ogs_error("OpenAPI_early_media_description_convertToJSON() failed [s_dp_session_description]");
        goto end;
    }
    OpenAPI_list_for_each(early_media_description->s_dp_session_description, node) {
        if (cJSON_AddStringToObject(s_dp_session_descriptionList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_early_media_description_convertToJSON() failed [s_dp_session_description]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_early_media_description_t *OpenAPI_early_media_description_parseFromJSON(cJSON *early_media_descriptionJSON)
{
    OpenAPI_early_media_description_t *early_media_description_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_dp_time_stamps = NULL;
    OpenAPI_sdp_time_stamps_t *s_dp_time_stamps_local_nonprim = NULL;
    cJSON *s_dp_media_component = NULL;
    OpenAPI_list_t *s_dp_media_componentList = NULL;
    cJSON *s_dp_session_description = NULL;
    OpenAPI_list_t *s_dp_session_descriptionList = NULL;
    s_dp_time_stamps = cJSON_GetObjectItemCaseSensitive(early_media_descriptionJSON, "sDPTimeStamps");
    if (s_dp_time_stamps) {
    s_dp_time_stamps_local_nonprim = OpenAPI_sdp_time_stamps_parseFromJSON(s_dp_time_stamps);
    if (!s_dp_time_stamps_local_nonprim) {
        ogs_error("OpenAPI_sdp_time_stamps_parseFromJSON failed [s_dp_time_stamps]");
        goto end;
    }
    }

    s_dp_media_component = cJSON_GetObjectItemCaseSensitive(early_media_descriptionJSON, "sDPMediaComponent");
    if (s_dp_media_component) {
        cJSON *s_dp_media_component_local = NULL;
        if (!cJSON_IsArray(s_dp_media_component)) {
            ogs_error("OpenAPI_early_media_description_parseFromJSON() failed [s_dp_media_component]");
            goto end;
        }

        s_dp_media_componentList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_dp_media_component_local, s_dp_media_component) {
            if (!cJSON_IsObject(s_dp_media_component_local)) {
                ogs_error("OpenAPI_early_media_description_parseFromJSON() failed [s_dp_media_component]");
                goto end;
            }
            OpenAPI_sdp_media_component_t *s_dp_media_componentItem = OpenAPI_sdp_media_component_parseFromJSON(s_dp_media_component_local);
            if (!s_dp_media_componentItem) {
                ogs_error("No s_dp_media_componentItem");
                goto end;
            }
            OpenAPI_list_add(s_dp_media_componentList, s_dp_media_componentItem);
        }
    }

    s_dp_session_description = cJSON_GetObjectItemCaseSensitive(early_media_descriptionJSON, "sDPSessionDescription");
    if (s_dp_session_description) {
        cJSON *s_dp_session_description_local = NULL;
        if (!cJSON_IsArray(s_dp_session_description)) {
            ogs_error("OpenAPI_early_media_description_parseFromJSON() failed [s_dp_session_description]");
            goto end;
        }

        s_dp_session_descriptionList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_dp_session_description_local, s_dp_session_description) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(s_dp_session_description_local)) {
                ogs_error("OpenAPI_early_media_description_parseFromJSON() failed [s_dp_session_description]");
                goto end;
            }
            OpenAPI_list_add(s_dp_session_descriptionList, ogs_strdup(s_dp_session_description_local->valuestring));
        }
    }

    early_media_description_local_var = OpenAPI_early_media_description_create (
        s_dp_time_stamps ? s_dp_time_stamps_local_nonprim : NULL,
        s_dp_media_component ? s_dp_media_componentList : NULL,
        s_dp_session_description ? s_dp_session_descriptionList : NULL
    );

    return early_media_description_local_var;
end:
    if (s_dp_time_stamps_local_nonprim) {
        OpenAPI_sdp_time_stamps_free(s_dp_time_stamps_local_nonprim);
        s_dp_time_stamps_local_nonprim = NULL;
    }
    if (s_dp_media_componentList) {
        OpenAPI_list_for_each(s_dp_media_componentList, node) {
            OpenAPI_sdp_media_component_free(node->data);
        }
        OpenAPI_list_free(s_dp_media_componentList);
        s_dp_media_componentList = NULL;
    }
    if (s_dp_session_descriptionList) {
        OpenAPI_list_for_each(s_dp_session_descriptionList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(s_dp_session_descriptionList);
        s_dp_session_descriptionList = NULL;
    }
    return NULL;
}

OpenAPI_early_media_description_t *OpenAPI_early_media_description_copy(OpenAPI_early_media_description_t *dst, OpenAPI_early_media_description_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_early_media_description_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_early_media_description_convertToJSON() failed");
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

    OpenAPI_early_media_description_free(dst);
    dst = OpenAPI_early_media_description_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

