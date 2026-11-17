
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mapdu_session_information.h"

OpenAPI_mapdu_session_information_t *OpenAPI_mapdu_session_information_create(
    OpenAPI_ma_pdu_indication_e m_apdu_session_indicator,
    OpenAPI_atsss_capability_t *a_tsss_capability
)
{
    OpenAPI_mapdu_session_information_t *mapdu_session_information_local_var = ogs_malloc(sizeof(OpenAPI_mapdu_session_information_t));
    ogs_assert(mapdu_session_information_local_var);

    mapdu_session_information_local_var->m_apdu_session_indicator = m_apdu_session_indicator;
    mapdu_session_information_local_var->a_tsss_capability = a_tsss_capability;

    return mapdu_session_information_local_var;
}

void OpenAPI_mapdu_session_information_free(OpenAPI_mapdu_session_information_t *mapdu_session_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mapdu_session_information) {
        return;
    }
    if (mapdu_session_information->a_tsss_capability) {
        OpenAPI_atsss_capability_free(mapdu_session_information->a_tsss_capability);
        mapdu_session_information->a_tsss_capability = NULL;
    }
    ogs_free(mapdu_session_information);
}

cJSON *OpenAPI_mapdu_session_information_convertToJSON(OpenAPI_mapdu_session_information_t *mapdu_session_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mapdu_session_information == NULL) {
        ogs_error("OpenAPI_mapdu_session_information_convertToJSON() failed [MAPDUSessionInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mapdu_session_information->m_apdu_session_indicator != OpenAPI_ma_pdu_indication_NULL) {
    if (cJSON_AddStringToObject(item, "mAPDUSessionIndicator", OpenAPI_ma_pdu_indication_ToString(mapdu_session_information->m_apdu_session_indicator)) == NULL) {
        ogs_error("OpenAPI_mapdu_session_information_convertToJSON() failed [m_apdu_session_indicator]");
        goto end;
    }
    }

    if (mapdu_session_information->a_tsss_capability) {
    cJSON *a_tsss_capability_local_JSON = OpenAPI_atsss_capability_convertToJSON(mapdu_session_information->a_tsss_capability);
    if (a_tsss_capability_local_JSON == NULL) {
        ogs_error("OpenAPI_mapdu_session_information_convertToJSON() failed [a_tsss_capability]");
        goto end;
    }
    cJSON_AddItemToObject(item, "aTSSSCapability", a_tsss_capability_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mapdu_session_information_convertToJSON() failed [a_tsss_capability]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mapdu_session_information_t *OpenAPI_mapdu_session_information_parseFromJSON(cJSON *mapdu_session_informationJSON)
{
    OpenAPI_mapdu_session_information_t *mapdu_session_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *m_apdu_session_indicator = NULL;
    OpenAPI_ma_pdu_indication_e m_apdu_session_indicatorVariable = 0;
    cJSON *a_tsss_capability = NULL;
    OpenAPI_atsss_capability_t *a_tsss_capability_local_nonprim = NULL;
    m_apdu_session_indicator = cJSON_GetObjectItemCaseSensitive(mapdu_session_informationJSON, "mAPDUSessionIndicator");
    if (m_apdu_session_indicator) {
    if (!cJSON_IsString(m_apdu_session_indicator)) {
        ogs_error("OpenAPI_mapdu_session_information_parseFromJSON() failed [m_apdu_session_indicator]");
        goto end;
    }
    m_apdu_session_indicatorVariable = OpenAPI_ma_pdu_indication_FromString(m_apdu_session_indicator->valuestring);
    }

    a_tsss_capability = cJSON_GetObjectItemCaseSensitive(mapdu_session_informationJSON, "aTSSSCapability");
    if (a_tsss_capability) {
    a_tsss_capability_local_nonprim = OpenAPI_atsss_capability_parseFromJSON(a_tsss_capability);
    if (!a_tsss_capability_local_nonprim) {
        ogs_error("OpenAPI_atsss_capability_parseFromJSON failed [a_tsss_capability]");
        goto end;
    }
    }

    mapdu_session_information_local_var = OpenAPI_mapdu_session_information_create (
        m_apdu_session_indicator ? m_apdu_session_indicatorVariable : 0,
        a_tsss_capability ? a_tsss_capability_local_nonprim : NULL
    );

    return mapdu_session_information_local_var;
end:
    if (a_tsss_capability_local_nonprim) {
        OpenAPI_atsss_capability_free(a_tsss_capability_local_nonprim);
        a_tsss_capability_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mapdu_session_information_t *OpenAPI_mapdu_session_information_copy(OpenAPI_mapdu_session_information_t *dst, OpenAPI_mapdu_session_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mapdu_session_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mapdu_session_information_convertToJSON() failed");
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

    OpenAPI_mapdu_session_information_free(dst);
    dst = OpenAPI_mapdu_session_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

