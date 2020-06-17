
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "atsss_capability.h"

OpenAPI_atsss_capability_t *OpenAPI_atsss_capability_create(
    int atsss_ll,
    int mptcp
    )
{
    OpenAPI_atsss_capability_t *atsss_capability_local_var = OpenAPI_malloc(sizeof(OpenAPI_atsss_capability_t));
    if (!atsss_capability_local_var) {
        return NULL;
    }
    atsss_capability_local_var->atsss_ll = atsss_ll;
    atsss_capability_local_var->mptcp = mptcp;

    return atsss_capability_local_var;
}

void OpenAPI_atsss_capability_free(OpenAPI_atsss_capability_t *atsss_capability)
{
    if (NULL == atsss_capability) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(atsss_capability);
}

cJSON *OpenAPI_atsss_capability_convertToJSON(OpenAPI_atsss_capability_t *atsss_capability)
{
    cJSON *item = NULL;

    if (atsss_capability == NULL) {
        ogs_error("OpenAPI_atsss_capability_convertToJSON() failed [AtsssCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (atsss_capability->atsss_ll) {
        if (cJSON_AddBoolToObject(item, "atsssLL", atsss_capability->atsss_ll) == NULL) {
            ogs_error("OpenAPI_atsss_capability_convertToJSON() failed [atsss_ll]");
            goto end;
        }
    }

    if (atsss_capability->mptcp) {
        if (cJSON_AddBoolToObject(item, "mptcp", atsss_capability->mptcp) == NULL) {
            ogs_error("OpenAPI_atsss_capability_convertToJSON() failed [mptcp]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_atsss_capability_t *OpenAPI_atsss_capability_parseFromJSON(cJSON *atsss_capabilityJSON)
{
    OpenAPI_atsss_capability_t *atsss_capability_local_var = NULL;
    cJSON *atsss_ll = cJSON_GetObjectItemCaseSensitive(atsss_capabilityJSON, "atsssLL");

    if (atsss_ll) {
        if (!cJSON_IsBool(atsss_ll)) {
            ogs_error("OpenAPI_atsss_capability_parseFromJSON() failed [atsss_ll]");
            goto end;
        }
    }

    cJSON *mptcp = cJSON_GetObjectItemCaseSensitive(atsss_capabilityJSON, "mptcp");

    if (mptcp) {
        if (!cJSON_IsBool(mptcp)) {
            ogs_error("OpenAPI_atsss_capability_parseFromJSON() failed [mptcp]");
            goto end;
        }
    }

    atsss_capability_local_var = OpenAPI_atsss_capability_create (
        atsss_ll ? atsss_ll->valueint : 0,
        mptcp ? mptcp->valueint : 0
        );

    return atsss_capability_local_var;
end:
    return NULL;
}

OpenAPI_atsss_capability_t *OpenAPI_atsss_capability_copy(OpenAPI_atsss_capability_t *dst, OpenAPI_atsss_capability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_atsss_capability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_atsss_capability_convertToJSON() failed");
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

    OpenAPI_atsss_capability_free(dst);
    dst = OpenAPI_atsss_capability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

