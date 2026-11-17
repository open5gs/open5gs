
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_container_information.h"

OpenAPI_mbs_container_information_t *OpenAPI_mbs_container_information_create(
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_established_connection_info_t *established_connection_info
)
{
    OpenAPI_mbs_container_information_t *mbs_container_information_local_var = ogs_malloc(sizeof(OpenAPI_mbs_container_information_t));
    ogs_assert(mbs_container_information_local_var);

    mbs_container_information_local_var->timeof_first_usage = timeof_first_usage;
    mbs_container_information_local_var->timeof_last_usage = timeof_last_usage;
    mbs_container_information_local_var->is_qo_s_information_null = is_qo_s_information_null;
    mbs_container_information_local_var->qo_s_information = qo_s_information;
    mbs_container_information_local_var->established_connection_info = established_connection_info;

    return mbs_container_information_local_var;
}

void OpenAPI_mbs_container_information_free(OpenAPI_mbs_container_information_t *mbs_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_container_information) {
        return;
    }
    if (mbs_container_information->timeof_first_usage) {
        ogs_free(mbs_container_information->timeof_first_usage);
        mbs_container_information->timeof_first_usage = NULL;
    }
    if (mbs_container_information->timeof_last_usage) {
        ogs_free(mbs_container_information->timeof_last_usage);
        mbs_container_information->timeof_last_usage = NULL;
    }
    if (mbs_container_information->qo_s_information) {
        OpenAPI_qos_data_free(mbs_container_information->qo_s_information);
        mbs_container_information->qo_s_information = NULL;
    }
    if (mbs_container_information->established_connection_info) {
        OpenAPI_established_connection_info_free(mbs_container_information->established_connection_info);
        mbs_container_information->established_connection_info = NULL;
    }
    ogs_free(mbs_container_information);
}

cJSON *OpenAPI_mbs_container_information_convertToJSON(OpenAPI_mbs_container_information_t *mbs_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_container_information == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [MBSContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbs_container_information->timeof_first_usage) {
    if (cJSON_AddStringToObject(item, "timeofFirstUsage", mbs_container_information->timeof_first_usage) == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [timeof_first_usage]");
        goto end;
    }
    }

    if (mbs_container_information->timeof_last_usage) {
    if (cJSON_AddStringToObject(item, "timeofLastUsage", mbs_container_information->timeof_last_usage) == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [timeof_last_usage]");
        goto end;
    }
    }

    if (mbs_container_information->qo_s_information) {
    cJSON *qo_s_information_local_JSON = OpenAPI_qos_data_convertToJSON(mbs_container_information->qo_s_information);
    if (qo_s_information_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [qo_s_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoSInformation", qo_s_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [qo_s_information]");
        goto end;
    }
    } else if (mbs_container_information->is_qo_s_information_null) {
        if (cJSON_AddNullToObject(item, "qoSInformation") == NULL) {
            ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [qo_s_information]");
            goto end;
        }
    }

    if (mbs_container_information->established_connection_info) {
    cJSON *established_connection_info_local_JSON = OpenAPI_established_connection_info_convertToJSON(mbs_container_information->established_connection_info);
    if (established_connection_info_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [established_connection_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "establishedConnectionInfo", established_connection_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed [established_connection_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mbs_container_information_t *OpenAPI_mbs_container_information_parseFromJSON(cJSON *mbs_container_informationJSON)
{
    OpenAPI_mbs_container_information_t *mbs_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *timeof_first_usage = NULL;
    cJSON *timeof_last_usage = NULL;
    cJSON *qo_s_information = NULL;
    OpenAPI_qos_data_t *qo_s_information_local_nonprim = NULL;
    cJSON *established_connection_info = NULL;
    OpenAPI_established_connection_info_t *established_connection_info_local_nonprim = NULL;
    timeof_first_usage = cJSON_GetObjectItemCaseSensitive(mbs_container_informationJSON, "timeofFirstUsage");
    if (timeof_first_usage) {
    if (!cJSON_IsString(timeof_first_usage) && !cJSON_IsNull(timeof_first_usage)) {
        ogs_error("OpenAPI_mbs_container_information_parseFromJSON() failed [timeof_first_usage]");
        goto end;
    }
    }

    timeof_last_usage = cJSON_GetObjectItemCaseSensitive(mbs_container_informationJSON, "timeofLastUsage");
    if (timeof_last_usage) {
    if (!cJSON_IsString(timeof_last_usage) && !cJSON_IsNull(timeof_last_usage)) {
        ogs_error("OpenAPI_mbs_container_information_parseFromJSON() failed [timeof_last_usage]");
        goto end;
    }
    }

    qo_s_information = cJSON_GetObjectItemCaseSensitive(mbs_container_informationJSON, "qoSInformation");
    if (qo_s_information) {
    if (!cJSON_IsNull(qo_s_information)) {
    qo_s_information_local_nonprim = OpenAPI_qos_data_parseFromJSON(qo_s_information);
    if (!qo_s_information_local_nonprim) {
        ogs_error("OpenAPI_qos_data_parseFromJSON failed [qo_s_information]");
        goto end;
    }
    }
    }

    established_connection_info = cJSON_GetObjectItemCaseSensitive(mbs_container_informationJSON, "establishedConnectionInfo");
    if (established_connection_info) {
    established_connection_info_local_nonprim = OpenAPI_established_connection_info_parseFromJSON(established_connection_info);
    if (!established_connection_info_local_nonprim) {
        ogs_error("OpenAPI_established_connection_info_parseFromJSON failed [established_connection_info]");
        goto end;
    }
    }

    mbs_container_information_local_var = OpenAPI_mbs_container_information_create (
        timeof_first_usage && !cJSON_IsNull(timeof_first_usage) ? ogs_strdup(timeof_first_usage->valuestring) : NULL,
        timeof_last_usage && !cJSON_IsNull(timeof_last_usage) ? ogs_strdup(timeof_last_usage->valuestring) : NULL,
        qo_s_information && cJSON_IsNull(qo_s_information) ? true : false,
        qo_s_information ? qo_s_information_local_nonprim : NULL,
        established_connection_info ? established_connection_info_local_nonprim : NULL
    );

    return mbs_container_information_local_var;
end:
    if (qo_s_information_local_nonprim) {
        OpenAPI_qos_data_free(qo_s_information_local_nonprim);
        qo_s_information_local_nonprim = NULL;
    }
    if (established_connection_info_local_nonprim) {
        OpenAPI_established_connection_info_free(established_connection_info_local_nonprim);
        established_connection_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mbs_container_information_t *OpenAPI_mbs_container_information_copy(OpenAPI_mbs_container_information_t *dst, OpenAPI_mbs_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_container_information_convertToJSON() failed");
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

    OpenAPI_mbs_container_information_free(dst);
    dst = OpenAPI_mbs_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

