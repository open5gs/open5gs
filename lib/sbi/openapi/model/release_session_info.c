
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "release_session_info.h"

OpenAPI_release_session_info_t *OpenAPI_release_session_info_create(
    OpenAPI_list_t *release_session_list,
    OpenAPI_release_cause_e release_cause
)
{
    OpenAPI_release_session_info_t *release_session_info_local_var = ogs_malloc(sizeof(OpenAPI_release_session_info_t));
    ogs_assert(release_session_info_local_var);

    release_session_info_local_var->release_session_list = release_session_list;
    release_session_info_local_var->release_cause = release_cause;

    return release_session_info_local_var;
}

void OpenAPI_release_session_info_free(OpenAPI_release_session_info_t *release_session_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == release_session_info) {
        return;
    }
    if (release_session_info->release_session_list) {
        OpenAPI_list_for_each(release_session_info->release_session_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(release_session_info->release_session_list);
        release_session_info->release_session_list = NULL;
    }
    ogs_free(release_session_info);
}

cJSON *OpenAPI_release_session_info_convertToJSON(OpenAPI_release_session_info_t *release_session_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (release_session_info == NULL) {
        ogs_error("OpenAPI_release_session_info_convertToJSON() failed [ReleaseSessionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!release_session_info->release_session_list) {
        ogs_error("OpenAPI_release_session_info_convertToJSON() failed [release_session_list]");
        return NULL;
    }
    cJSON *release_session_listList = cJSON_AddArrayToObject(item, "releaseSessionList");
    if (release_session_listList == NULL) {
        ogs_error("OpenAPI_release_session_info_convertToJSON() failed [release_session_list]");
        goto end;
    }
    OpenAPI_list_for_each(release_session_info->release_session_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_release_session_info_convertToJSON() failed [release_session_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(release_session_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_release_session_info_convertToJSON() failed [release_session_list]");
            goto end;
        }
    }

    if (release_session_info->release_cause == OpenAPI_release_cause_NULL) {
        ogs_error("OpenAPI_release_session_info_convertToJSON() failed [release_cause]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "releaseCause", OpenAPI_release_cause_ToString(release_session_info->release_cause)) == NULL) {
        ogs_error("OpenAPI_release_session_info_convertToJSON() failed [release_cause]");
        goto end;
    }

end:
    return item;
}

OpenAPI_release_session_info_t *OpenAPI_release_session_info_parseFromJSON(cJSON *release_session_infoJSON)
{
    OpenAPI_release_session_info_t *release_session_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *release_session_list = NULL;
    OpenAPI_list_t *release_session_listList = NULL;
    cJSON *release_cause = NULL;
    OpenAPI_release_cause_e release_causeVariable = 0;
    release_session_list = cJSON_GetObjectItemCaseSensitive(release_session_infoJSON, "releaseSessionList");
    if (!release_session_list) {
        ogs_error("OpenAPI_release_session_info_parseFromJSON() failed [release_session_list]");
        goto end;
    }
        cJSON *release_session_list_local = NULL;
        if (!cJSON_IsArray(release_session_list)) {
            ogs_error("OpenAPI_release_session_info_parseFromJSON() failed [release_session_list]");
            goto end;
        }

        release_session_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(release_session_list_local, release_session_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(release_session_list_local)) {
                ogs_error("OpenAPI_release_session_info_parseFromJSON() failed [release_session_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_release_session_info_parseFromJSON() failed [release_session_list]");
                goto end;
            }
            *localDouble = release_session_list_local->valuedouble;
            OpenAPI_list_add(release_session_listList, localDouble);
        }

    release_cause = cJSON_GetObjectItemCaseSensitive(release_session_infoJSON, "releaseCause");
    if (!release_cause) {
        ogs_error("OpenAPI_release_session_info_parseFromJSON() failed [release_cause]");
        goto end;
    }
    if (!cJSON_IsString(release_cause)) {
        ogs_error("OpenAPI_release_session_info_parseFromJSON() failed [release_cause]");
        goto end;
    }
    release_causeVariable = OpenAPI_release_cause_FromString(release_cause->valuestring);

    release_session_info_local_var = OpenAPI_release_session_info_create (
        release_session_listList,
        release_causeVariable
    );

    return release_session_info_local_var;
end:
    if (release_session_listList) {
        OpenAPI_list_for_each(release_session_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(release_session_listList);
        release_session_listList = NULL;
    }
    return NULL;
}

OpenAPI_release_session_info_t *OpenAPI_release_session_info_copy(OpenAPI_release_session_info_t *dst, OpenAPI_release_session_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_release_session_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_release_session_info_convertToJSON() failed");
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

    OpenAPI_release_session_info_free(dst);
    dst = OpenAPI_release_session_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

