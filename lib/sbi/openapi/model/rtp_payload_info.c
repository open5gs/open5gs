
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rtp_payload_info.h"

OpenAPI_rtp_payload_info_t *OpenAPI_rtp_payload_info_create(
    OpenAPI_list_t *rtp_payload_type_list,
    OpenAPI_rtp_payload_format_e rtp_payload_format
)
{
    OpenAPI_rtp_payload_info_t *rtp_payload_info_local_var = ogs_malloc(sizeof(OpenAPI_rtp_payload_info_t));
    ogs_assert(rtp_payload_info_local_var);

    rtp_payload_info_local_var->rtp_payload_type_list = rtp_payload_type_list;
    rtp_payload_info_local_var->rtp_payload_format = rtp_payload_format;

    return rtp_payload_info_local_var;
}

void OpenAPI_rtp_payload_info_free(OpenAPI_rtp_payload_info_t *rtp_payload_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rtp_payload_info) {
        return;
    }
    if (rtp_payload_info->rtp_payload_type_list) {
        OpenAPI_list_for_each(rtp_payload_info->rtp_payload_type_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rtp_payload_info->rtp_payload_type_list);
        rtp_payload_info->rtp_payload_type_list = NULL;
    }
    ogs_free(rtp_payload_info);
}

cJSON *OpenAPI_rtp_payload_info_convertToJSON(OpenAPI_rtp_payload_info_t *rtp_payload_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rtp_payload_info == NULL) {
        ogs_error("OpenAPI_rtp_payload_info_convertToJSON() failed [RtpPayloadInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rtp_payload_info->rtp_payload_type_list) {
    cJSON *rtp_payload_type_listList = cJSON_AddArrayToObject(item, "rtpPayloadTypeList");
    if (rtp_payload_type_listList == NULL) {
        ogs_error("OpenAPI_rtp_payload_info_convertToJSON() failed [rtp_payload_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(rtp_payload_info->rtp_payload_type_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_rtp_payload_info_convertToJSON() failed [rtp_payload_type_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(rtp_payload_type_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_rtp_payload_info_convertToJSON() failed [rtp_payload_type_list]");
            goto end;
        }
    }
    }

    if (rtp_payload_info->rtp_payload_format != OpenAPI_rtp_payload_format_NULL) {
    if (cJSON_AddStringToObject(item, "rtpPayloadFormat", OpenAPI_rtp_payload_format_ToString(rtp_payload_info->rtp_payload_format)) == NULL) {
        ogs_error("OpenAPI_rtp_payload_info_convertToJSON() failed [rtp_payload_format]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rtp_payload_info_t *OpenAPI_rtp_payload_info_parseFromJSON(cJSON *rtp_payload_infoJSON)
{
    OpenAPI_rtp_payload_info_t *rtp_payload_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rtp_payload_type_list = NULL;
    OpenAPI_list_t *rtp_payload_type_listList = NULL;
    cJSON *rtp_payload_format = NULL;
    OpenAPI_rtp_payload_format_e rtp_payload_formatVariable = 0;
    rtp_payload_type_list = cJSON_GetObjectItemCaseSensitive(rtp_payload_infoJSON, "rtpPayloadTypeList");
    if (rtp_payload_type_list) {
        cJSON *rtp_payload_type_list_local = NULL;
        if (!cJSON_IsArray(rtp_payload_type_list)) {
            ogs_error("OpenAPI_rtp_payload_info_parseFromJSON() failed [rtp_payload_type_list]");
            goto end;
        }

        rtp_payload_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(rtp_payload_type_list_local, rtp_payload_type_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(rtp_payload_type_list_local)) {
                ogs_error("OpenAPI_rtp_payload_info_parseFromJSON() failed [rtp_payload_type_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_rtp_payload_info_parseFromJSON() failed [rtp_payload_type_list]");
                goto end;
            }
            *localDouble = rtp_payload_type_list_local->valuedouble;
            OpenAPI_list_add(rtp_payload_type_listList, localDouble);
        }
    }

    rtp_payload_format = cJSON_GetObjectItemCaseSensitive(rtp_payload_infoJSON, "rtpPayloadFormat");
    if (rtp_payload_format) {
    if (!cJSON_IsString(rtp_payload_format)) {
        ogs_error("OpenAPI_rtp_payload_info_parseFromJSON() failed [rtp_payload_format]");
        goto end;
    }
    rtp_payload_formatVariable = OpenAPI_rtp_payload_format_FromString(rtp_payload_format->valuestring);
    }

    rtp_payload_info_local_var = OpenAPI_rtp_payload_info_create (
        rtp_payload_type_list ? rtp_payload_type_listList : NULL,
        rtp_payload_format ? rtp_payload_formatVariable : 0
    );

    return rtp_payload_info_local_var;
end:
    if (rtp_payload_type_listList) {
        OpenAPI_list_for_each(rtp_payload_type_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rtp_payload_type_listList);
        rtp_payload_type_listList = NULL;
    }
    return NULL;
}

OpenAPI_rtp_payload_info_t *OpenAPI_rtp_payload_info_copy(OpenAPI_rtp_payload_info_t *dst, OpenAPI_rtp_payload_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rtp_payload_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rtp_payload_info_convertToJSON() failed");
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

    OpenAPI_rtp_payload_info_free(dst);
    dst = OpenAPI_rtp_payload_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

