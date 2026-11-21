
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_session_charging_information.h"

OpenAPI_mbs_session_charging_information_t *OpenAPI_mbs_session_charging_information_create(
    OpenAPI_mbs_session_id_t *m_bs_session_id,
    OpenAPI_mbs_service_type_e m_bs_service_type,
    OpenAPI_service_area_t *service_area,
    char *m_bs_start_time,
    char *m_bs_end_time,
    OpenAPI_mbs_session_activity_status_e m_bs_service_activity_status,
    OpenAPI_serving_network_function_id_t *serving_network_function_id
)
{
    OpenAPI_mbs_session_charging_information_t *mbs_session_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_mbs_session_charging_information_t));
    ogs_assert(mbs_session_charging_information_local_var);

    mbs_session_charging_information_local_var->m_bs_session_id = m_bs_session_id;
    mbs_session_charging_information_local_var->m_bs_service_type = m_bs_service_type;
    mbs_session_charging_information_local_var->service_area = service_area;
    mbs_session_charging_information_local_var->m_bs_start_time = m_bs_start_time;
    mbs_session_charging_information_local_var->m_bs_end_time = m_bs_end_time;
    mbs_session_charging_information_local_var->m_bs_service_activity_status = m_bs_service_activity_status;
    mbs_session_charging_information_local_var->serving_network_function_id = serving_network_function_id;

    return mbs_session_charging_information_local_var;
}

void OpenAPI_mbs_session_charging_information_free(OpenAPI_mbs_session_charging_information_t *mbs_session_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_session_charging_information) {
        return;
    }
    if (mbs_session_charging_information->m_bs_session_id) {
        OpenAPI_mbs_session_id_free(mbs_session_charging_information->m_bs_session_id);
        mbs_session_charging_information->m_bs_session_id = NULL;
    }
    if (mbs_session_charging_information->service_area) {
        OpenAPI_service_area_free(mbs_session_charging_information->service_area);
        mbs_session_charging_information->service_area = NULL;
    }
    if (mbs_session_charging_information->m_bs_start_time) {
        ogs_free(mbs_session_charging_information->m_bs_start_time);
        mbs_session_charging_information->m_bs_start_time = NULL;
    }
    if (mbs_session_charging_information->m_bs_end_time) {
        ogs_free(mbs_session_charging_information->m_bs_end_time);
        mbs_session_charging_information->m_bs_end_time = NULL;
    }
    if (mbs_session_charging_information->serving_network_function_id) {
        OpenAPI_serving_network_function_id_free(mbs_session_charging_information->serving_network_function_id);
        mbs_session_charging_information->serving_network_function_id = NULL;
    }
    ogs_free(mbs_session_charging_information);
}

cJSON *OpenAPI_mbs_session_charging_information_convertToJSON(OpenAPI_mbs_session_charging_information_t *mbs_session_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_session_charging_information == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [MBSSessionChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mbs_session_charging_information->m_bs_session_id) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_session_id]");
        return NULL;
    }
    cJSON *m_bs_session_id_local_JSON = OpenAPI_mbs_session_id_convertToJSON(mbs_session_charging_information->m_bs_session_id);
    if (m_bs_session_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_session_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mBSSessionID", m_bs_session_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_session_id]");
        goto end;
    }

    if (mbs_session_charging_information->m_bs_service_type == OpenAPI_mbs_service_type_NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_service_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mBSServiceType", OpenAPI_mbs_service_type_ToString(mbs_session_charging_information->m_bs_service_type)) == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_service_type]");
        goto end;
    }

    if (mbs_session_charging_information->service_area) {
    cJSON *service_area_local_JSON = OpenAPI_service_area_convertToJSON(mbs_session_charging_information->service_area);
    if (service_area_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [service_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "serviceArea", service_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [service_area]");
        goto end;
    }
    }

    if (mbs_session_charging_information->m_bs_start_time) {
    if (cJSON_AddStringToObject(item, "mBSStartTime", mbs_session_charging_information->m_bs_start_time) == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_start_time]");
        goto end;
    }
    }

    if (mbs_session_charging_information->m_bs_end_time) {
    if (cJSON_AddStringToObject(item, "mBSEndTime", mbs_session_charging_information->m_bs_end_time) == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_end_time]");
        goto end;
    }
    }

    if (mbs_session_charging_information->m_bs_service_activity_status != OpenAPI_mbs_session_activity_status_NULL) {
    if (cJSON_AddStringToObject(item, "mBSServiceActivityStatus", OpenAPI_mbs_session_activity_status_ToString(mbs_session_charging_information->m_bs_service_activity_status)) == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [m_bs_service_activity_status]");
        goto end;
    }
    }

    if (mbs_session_charging_information->serving_network_function_id) {
    cJSON *serving_network_function_id_local_JSON = OpenAPI_serving_network_function_id_convertToJSON(mbs_session_charging_information->serving_network_function_id);
    if (serving_network_function_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [serving_network_function_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetworkFunctionID", serving_network_function_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed [serving_network_function_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mbs_session_charging_information_t *OpenAPI_mbs_session_charging_information_parseFromJSON(cJSON *mbs_session_charging_informationJSON)
{
    OpenAPI_mbs_session_charging_information_t *mbs_session_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *m_bs_session_id = NULL;
    OpenAPI_mbs_session_id_t *m_bs_session_id_local_nonprim = NULL;
    cJSON *m_bs_service_type = NULL;
    OpenAPI_mbs_service_type_e m_bs_service_typeVariable = 0;
    cJSON *service_area = NULL;
    OpenAPI_service_area_t *service_area_local_nonprim = NULL;
    cJSON *m_bs_start_time = NULL;
    cJSON *m_bs_end_time = NULL;
    cJSON *m_bs_service_activity_status = NULL;
    OpenAPI_mbs_session_activity_status_e m_bs_service_activity_statusVariable = 0;
    cJSON *serving_network_function_id = NULL;
    OpenAPI_serving_network_function_id_t *serving_network_function_id_local_nonprim = NULL;
    m_bs_session_id = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "mBSSessionID");
    if (!m_bs_session_id) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON() failed [m_bs_session_id]");
        goto end;
    }
    m_bs_session_id_local_nonprim = OpenAPI_mbs_session_id_parseFromJSON(m_bs_session_id);
    if (!m_bs_session_id_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_id_parseFromJSON failed [m_bs_session_id]");
        goto end;
    }

    m_bs_service_type = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "mBSServiceType");
    if (!m_bs_service_type) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON() failed [m_bs_service_type]");
        goto end;
    }
    if (!cJSON_IsString(m_bs_service_type)) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON() failed [m_bs_service_type]");
        goto end;
    }
    m_bs_service_typeVariable = OpenAPI_mbs_service_type_FromString(m_bs_service_type->valuestring);

    service_area = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "serviceArea");
    if (service_area) {
    service_area_local_nonprim = OpenAPI_service_area_parseFromJSON(service_area);
    if (!service_area_local_nonprim) {
        ogs_error("OpenAPI_service_area_parseFromJSON failed [service_area]");
        goto end;
    }
    }

    m_bs_start_time = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "mBSStartTime");
    if (m_bs_start_time) {
    if (!cJSON_IsString(m_bs_start_time) && !cJSON_IsNull(m_bs_start_time)) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON() failed [m_bs_start_time]");
        goto end;
    }
    }

    m_bs_end_time = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "mBSEndTime");
    if (m_bs_end_time) {
    if (!cJSON_IsString(m_bs_end_time) && !cJSON_IsNull(m_bs_end_time)) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON() failed [m_bs_end_time]");
        goto end;
    }
    }

    m_bs_service_activity_status = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "mBSServiceActivityStatus");
    if (m_bs_service_activity_status) {
    if (!cJSON_IsString(m_bs_service_activity_status)) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON() failed [m_bs_service_activity_status]");
        goto end;
    }
    m_bs_service_activity_statusVariable = OpenAPI_mbs_session_activity_status_FromString(m_bs_service_activity_status->valuestring);
    }

    serving_network_function_id = cJSON_GetObjectItemCaseSensitive(mbs_session_charging_informationJSON, "servingNetworkFunctionID");
    if (serving_network_function_id) {
    serving_network_function_id_local_nonprim = OpenAPI_serving_network_function_id_parseFromJSON(serving_network_function_id);
    if (!serving_network_function_id_local_nonprim) {
        ogs_error("OpenAPI_serving_network_function_id_parseFromJSON failed [serving_network_function_id]");
        goto end;
    }
    }

    mbs_session_charging_information_local_var = OpenAPI_mbs_session_charging_information_create (
        m_bs_session_id_local_nonprim,
        m_bs_service_typeVariable,
        service_area ? service_area_local_nonprim : NULL,
        m_bs_start_time && !cJSON_IsNull(m_bs_start_time) ? ogs_strdup(m_bs_start_time->valuestring) : NULL,
        m_bs_end_time && !cJSON_IsNull(m_bs_end_time) ? ogs_strdup(m_bs_end_time->valuestring) : NULL,
        m_bs_service_activity_status ? m_bs_service_activity_statusVariable : 0,
        serving_network_function_id ? serving_network_function_id_local_nonprim : NULL
    );

    return mbs_session_charging_information_local_var;
end:
    if (m_bs_session_id_local_nonprim) {
        OpenAPI_mbs_session_id_free(m_bs_session_id_local_nonprim);
        m_bs_session_id_local_nonprim = NULL;
    }
    if (service_area_local_nonprim) {
        OpenAPI_service_area_free(service_area_local_nonprim);
        service_area_local_nonprim = NULL;
    }
    if (serving_network_function_id_local_nonprim) {
        OpenAPI_serving_network_function_id_free(serving_network_function_id_local_nonprim);
        serving_network_function_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mbs_session_charging_information_t *OpenAPI_mbs_session_charging_information_copy(OpenAPI_mbs_session_charging_information_t *dst, OpenAPI_mbs_session_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_session_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_session_charging_information_convertToJSON() failed");
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

    OpenAPI_mbs_session_charging_information_free(dst);
    dst = OpenAPI_mbs_session_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

