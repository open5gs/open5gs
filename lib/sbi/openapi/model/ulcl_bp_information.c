
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ulcl_bp_information.h"

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_create(
    char *ulcl_bp_upf_id,
    OpenAPI_list_t *upf_events
)
{
    OpenAPI_ulcl_bp_information_t *ulcl_bp_information_local_var = ogs_malloc(sizeof(OpenAPI_ulcl_bp_information_t));
    ogs_assert(ulcl_bp_information_local_var);

    ulcl_bp_information_local_var->ulcl_bp_upf_id = ulcl_bp_upf_id;
    ulcl_bp_information_local_var->upf_events = upf_events;

    return ulcl_bp_information_local_var;
}

void OpenAPI_ulcl_bp_information_free(OpenAPI_ulcl_bp_information_t *ulcl_bp_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ulcl_bp_information) {
        return;
    }
    if (ulcl_bp_information->ulcl_bp_upf_id) {
        ogs_free(ulcl_bp_information->ulcl_bp_upf_id);
        ulcl_bp_information->ulcl_bp_upf_id = NULL;
    }
    if (ulcl_bp_information->upf_events) {
        OpenAPI_list_free(ulcl_bp_information->upf_events);
        ulcl_bp_information->upf_events = NULL;
    }
    ogs_free(ulcl_bp_information);
}

cJSON *OpenAPI_ulcl_bp_information_convertToJSON(OpenAPI_ulcl_bp_information_t *ulcl_bp_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ulcl_bp_information == NULL) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed [UlclBpInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ulcl_bp_information->ulcl_bp_upf_id) {
    if (cJSON_AddStringToObject(item, "ulclBpUpfId", ulcl_bp_information->ulcl_bp_upf_id) == NULL) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed [ulcl_bp_upf_id]");
        goto end;
    }
    }

    if (ulcl_bp_information->upf_events != OpenAPI_event_type_NULL) {
    cJSON *upf_eventsList = cJSON_AddArrayToObject(item, "upfEvents");
    if (upf_eventsList == NULL) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed [upf_events]");
        goto end;
    }
    OpenAPI_list_for_each(ulcl_bp_information->upf_events, node) {
        if (cJSON_AddStringToObject(upf_eventsList, "", OpenAPI_event_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed [upf_events]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_parseFromJSON(cJSON *ulcl_bp_informationJSON)
{
    OpenAPI_ulcl_bp_information_t *ulcl_bp_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ulcl_bp_upf_id = NULL;
    cJSON *upf_events = NULL;
    OpenAPI_list_t *upf_eventsList = NULL;
    ulcl_bp_upf_id = cJSON_GetObjectItemCaseSensitive(ulcl_bp_informationJSON, "ulclBpUpfId");
    if (ulcl_bp_upf_id) {
    if (!cJSON_IsString(ulcl_bp_upf_id) && !cJSON_IsNull(ulcl_bp_upf_id)) {
        ogs_error("OpenAPI_ulcl_bp_information_parseFromJSON() failed [ulcl_bp_upf_id]");
        goto end;
    }
    }

    upf_events = cJSON_GetObjectItemCaseSensitive(ulcl_bp_informationJSON, "upfEvents");
    if (upf_events) {
        cJSON *upf_events_local = NULL;
        if (!cJSON_IsArray(upf_events)) {
            ogs_error("OpenAPI_ulcl_bp_information_parseFromJSON() failed [upf_events]");
            goto end;
        }

        upf_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(upf_events_local, upf_events) {
            OpenAPI_event_type_e localEnum = OpenAPI_event_type_NULL;
            if (!cJSON_IsString(upf_events_local)) {
                ogs_error("OpenAPI_ulcl_bp_information_parseFromJSON() failed [upf_events]");
                goto end;
            }
            localEnum = OpenAPI_event_type_FromString(upf_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"upf_events\" is not supported. Ignoring it ...",
                         upf_events_local->valuestring);
            } else {
                OpenAPI_list_add(upf_eventsList, (void *)localEnum);
            }
        }
        if (upf_eventsList->count == 0) {
            ogs_error("OpenAPI_ulcl_bp_information_parseFromJSON() failed: Expected upf_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ulcl_bp_information_local_var = OpenAPI_ulcl_bp_information_create (
        ulcl_bp_upf_id && !cJSON_IsNull(ulcl_bp_upf_id) ? ogs_strdup(ulcl_bp_upf_id->valuestring) : NULL,
        upf_events ? upf_eventsList : NULL
    );

    return ulcl_bp_information_local_var;
end:
    if (upf_eventsList) {
        OpenAPI_list_free(upf_eventsList);
        upf_eventsList = NULL;
    }
    return NULL;
}

OpenAPI_ulcl_bp_information_t *OpenAPI_ulcl_bp_information_copy(OpenAPI_ulcl_bp_information_t *dst, OpenAPI_ulcl_bp_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ulcl_bp_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ulcl_bp_information_convertToJSON() failed");
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

    OpenAPI_ulcl_bp_information_free(dst);
    dst = OpenAPI_ulcl_bp_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

