
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "available_bitrate_monitoring_request.h"

OpenAPI_available_bitrate_monitoring_request_t *OpenAPI_available_bitrate_monitoring_request_create(
    OpenAPI_available_bitrate_request_e avail_bitrate_req,
    OpenAPI_list_t *avail_bitrate_ul_thrs,
    OpenAPI_list_t *avail_bitrate_dl_thrs
)
{
    OpenAPI_available_bitrate_monitoring_request_t *available_bitrate_monitoring_request_local_var = ogs_malloc(sizeof(OpenAPI_available_bitrate_monitoring_request_t));
    ogs_assert(available_bitrate_monitoring_request_local_var);

    available_bitrate_monitoring_request_local_var->avail_bitrate_req = avail_bitrate_req;
    available_bitrate_monitoring_request_local_var->avail_bitrate_ul_thrs = avail_bitrate_ul_thrs;
    available_bitrate_monitoring_request_local_var->avail_bitrate_dl_thrs = avail_bitrate_dl_thrs;

    return available_bitrate_monitoring_request_local_var;
}

void OpenAPI_available_bitrate_monitoring_request_free(OpenAPI_available_bitrate_monitoring_request_t *available_bitrate_monitoring_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == available_bitrate_monitoring_request) {
        return;
    }
    if (available_bitrate_monitoring_request->avail_bitrate_ul_thrs) {
        OpenAPI_list_for_each(available_bitrate_monitoring_request->avail_bitrate_ul_thrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(available_bitrate_monitoring_request->avail_bitrate_ul_thrs);
        available_bitrate_monitoring_request->avail_bitrate_ul_thrs = NULL;
    }
    if (available_bitrate_monitoring_request->avail_bitrate_dl_thrs) {
        OpenAPI_list_for_each(available_bitrate_monitoring_request->avail_bitrate_dl_thrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(available_bitrate_monitoring_request->avail_bitrate_dl_thrs);
        available_bitrate_monitoring_request->avail_bitrate_dl_thrs = NULL;
    }
    ogs_free(available_bitrate_monitoring_request);
}

cJSON *OpenAPI_available_bitrate_monitoring_request_convertToJSON(OpenAPI_available_bitrate_monitoring_request_t *available_bitrate_monitoring_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (available_bitrate_monitoring_request == NULL) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [AvailableBitrateMonitoringRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (available_bitrate_monitoring_request->avail_bitrate_req == OpenAPI_available_bitrate_request_NULL) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [avail_bitrate_req]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "availBitrateReq", OpenAPI_available_bitrate_request_ToString(available_bitrate_monitoring_request->avail_bitrate_req)) == NULL) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [avail_bitrate_req]");
        goto end;
    }

    if (available_bitrate_monitoring_request->avail_bitrate_ul_thrs) {
    cJSON *avail_bitrate_ul_thrsList = cJSON_AddArrayToObject(item, "availBitrateUlThrs");
    if (avail_bitrate_ul_thrsList == NULL) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [avail_bitrate_ul_thrs]");
        goto end;
    }
    OpenAPI_list_for_each(available_bitrate_monitoring_request->avail_bitrate_ul_thrs, node) {
        if (cJSON_AddStringToObject(avail_bitrate_ul_thrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [avail_bitrate_ul_thrs]");
            goto end;
        }
    }
    }

    if (available_bitrate_monitoring_request->avail_bitrate_dl_thrs) {
    cJSON *avail_bitrate_dl_thrsList = cJSON_AddArrayToObject(item, "availBitrateDlThrs");
    if (avail_bitrate_dl_thrsList == NULL) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [avail_bitrate_dl_thrs]");
        goto end;
    }
    OpenAPI_list_for_each(available_bitrate_monitoring_request->avail_bitrate_dl_thrs, node) {
        if (cJSON_AddStringToObject(avail_bitrate_dl_thrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed [avail_bitrate_dl_thrs]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_available_bitrate_monitoring_request_t *OpenAPI_available_bitrate_monitoring_request_parseFromJSON(cJSON *available_bitrate_monitoring_requestJSON)
{
    OpenAPI_available_bitrate_monitoring_request_t *available_bitrate_monitoring_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *avail_bitrate_req = NULL;
    OpenAPI_available_bitrate_request_e avail_bitrate_reqVariable = 0;
    cJSON *avail_bitrate_ul_thrs = NULL;
    OpenAPI_list_t *avail_bitrate_ul_thrsList = NULL;
    cJSON *avail_bitrate_dl_thrs = NULL;
    OpenAPI_list_t *avail_bitrate_dl_thrsList = NULL;
    avail_bitrate_req = cJSON_GetObjectItemCaseSensitive(available_bitrate_monitoring_requestJSON, "availBitrateReq");
    if (!avail_bitrate_req) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_parseFromJSON() failed [avail_bitrate_req]");
        goto end;
    }
    if (!cJSON_IsString(avail_bitrate_req)) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_parseFromJSON() failed [avail_bitrate_req]");
        goto end;
    }
    avail_bitrate_reqVariable = OpenAPI_available_bitrate_request_FromString(avail_bitrate_req->valuestring);

    avail_bitrate_ul_thrs = cJSON_GetObjectItemCaseSensitive(available_bitrate_monitoring_requestJSON, "availBitrateUlThrs");
    if (avail_bitrate_ul_thrs) {
        cJSON *avail_bitrate_ul_thrs_local = NULL;
        if (!cJSON_IsArray(avail_bitrate_ul_thrs)) {
            ogs_error("OpenAPI_available_bitrate_monitoring_request_parseFromJSON() failed [avail_bitrate_ul_thrs]");
            goto end;
        }

        avail_bitrate_ul_thrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(avail_bitrate_ul_thrs_local, avail_bitrate_ul_thrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(avail_bitrate_ul_thrs_local)) {
                ogs_error("OpenAPI_available_bitrate_monitoring_request_parseFromJSON() failed [avail_bitrate_ul_thrs]");
                goto end;
            }
            OpenAPI_list_add(avail_bitrate_ul_thrsList, ogs_strdup(avail_bitrate_ul_thrs_local->valuestring));
        }
    }

    avail_bitrate_dl_thrs = cJSON_GetObjectItemCaseSensitive(available_bitrate_monitoring_requestJSON, "availBitrateDlThrs");
    if (avail_bitrate_dl_thrs) {
        cJSON *avail_bitrate_dl_thrs_local = NULL;
        if (!cJSON_IsArray(avail_bitrate_dl_thrs)) {
            ogs_error("OpenAPI_available_bitrate_monitoring_request_parseFromJSON() failed [avail_bitrate_dl_thrs]");
            goto end;
        }

        avail_bitrate_dl_thrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(avail_bitrate_dl_thrs_local, avail_bitrate_dl_thrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(avail_bitrate_dl_thrs_local)) {
                ogs_error("OpenAPI_available_bitrate_monitoring_request_parseFromJSON() failed [avail_bitrate_dl_thrs]");
                goto end;
            }
            OpenAPI_list_add(avail_bitrate_dl_thrsList, ogs_strdup(avail_bitrate_dl_thrs_local->valuestring));
        }
    }

    available_bitrate_monitoring_request_local_var = OpenAPI_available_bitrate_monitoring_request_create (
        avail_bitrate_reqVariable,
        avail_bitrate_ul_thrs ? avail_bitrate_ul_thrsList : NULL,
        avail_bitrate_dl_thrs ? avail_bitrate_dl_thrsList : NULL
    );

    return available_bitrate_monitoring_request_local_var;
end:
    if (avail_bitrate_ul_thrsList) {
        OpenAPI_list_for_each(avail_bitrate_ul_thrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(avail_bitrate_ul_thrsList);
        avail_bitrate_ul_thrsList = NULL;
    }
    if (avail_bitrate_dl_thrsList) {
        OpenAPI_list_for_each(avail_bitrate_dl_thrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(avail_bitrate_dl_thrsList);
        avail_bitrate_dl_thrsList = NULL;
    }
    return NULL;
}

OpenAPI_available_bitrate_monitoring_request_t *OpenAPI_available_bitrate_monitoring_request_copy(OpenAPI_available_bitrate_monitoring_request_t *dst, OpenAPI_available_bitrate_monitoring_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_available_bitrate_monitoring_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_available_bitrate_monitoring_request_convertToJSON() failed");
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

    OpenAPI_available_bitrate_monitoring_request_free(dst);
    dst = OpenAPI_available_bitrate_monitoring_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

