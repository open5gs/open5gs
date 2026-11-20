
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "recipient_address.h"

OpenAPI_recipient_address_t *OpenAPI_recipient_address_create(
    OpenAPI_sm_address_info_t *recipient_address_info,
    OpenAPI_sm_addressee_type_e s_maddressee_type
)
{
    OpenAPI_recipient_address_t *recipient_address_local_var = ogs_malloc(sizeof(OpenAPI_recipient_address_t));
    ogs_assert(recipient_address_local_var);

    recipient_address_local_var->recipient_address_info = recipient_address_info;
    recipient_address_local_var->s_maddressee_type = s_maddressee_type;

    return recipient_address_local_var;
}

void OpenAPI_recipient_address_free(OpenAPI_recipient_address_t *recipient_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == recipient_address) {
        return;
    }
    if (recipient_address->recipient_address_info) {
        OpenAPI_sm_address_info_free(recipient_address->recipient_address_info);
        recipient_address->recipient_address_info = NULL;
    }
    ogs_free(recipient_address);
}

cJSON *OpenAPI_recipient_address_convertToJSON(OpenAPI_recipient_address_t *recipient_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (recipient_address == NULL) {
        ogs_error("OpenAPI_recipient_address_convertToJSON() failed [RecipientAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (recipient_address->recipient_address_info) {
    cJSON *recipient_address_info_local_JSON = OpenAPI_sm_address_info_convertToJSON(recipient_address->recipient_address_info);
    if (recipient_address_info_local_JSON == NULL) {
        ogs_error("OpenAPI_recipient_address_convertToJSON() failed [recipient_address_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recipientAddressInfo", recipient_address_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_recipient_address_convertToJSON() failed [recipient_address_info]");
        goto end;
    }
    }

    if (recipient_address->s_maddressee_type != OpenAPI_sm_addressee_type_NULL) {
    if (cJSON_AddStringToObject(item, "sMaddresseeType", OpenAPI_sm_addressee_type_ToString(recipient_address->s_maddressee_type)) == NULL) {
        ogs_error("OpenAPI_recipient_address_convertToJSON() failed [s_maddressee_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_recipient_address_t *OpenAPI_recipient_address_parseFromJSON(cJSON *recipient_addressJSON)
{
    OpenAPI_recipient_address_t *recipient_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *recipient_address_info = NULL;
    OpenAPI_sm_address_info_t *recipient_address_info_local_nonprim = NULL;
    cJSON *s_maddressee_type = NULL;
    OpenAPI_sm_addressee_type_e s_maddressee_typeVariable = 0;
    recipient_address_info = cJSON_GetObjectItemCaseSensitive(recipient_addressJSON, "recipientAddressInfo");
    if (recipient_address_info) {
    recipient_address_info_local_nonprim = OpenAPI_sm_address_info_parseFromJSON(recipient_address_info);
    if (!recipient_address_info_local_nonprim) {
        ogs_error("OpenAPI_sm_address_info_parseFromJSON failed [recipient_address_info]");
        goto end;
    }
    }

    s_maddressee_type = cJSON_GetObjectItemCaseSensitive(recipient_addressJSON, "sMaddresseeType");
    if (s_maddressee_type) {
    if (!cJSON_IsString(s_maddressee_type)) {
        ogs_error("OpenAPI_recipient_address_parseFromJSON() failed [s_maddressee_type]");
        goto end;
    }
    s_maddressee_typeVariable = OpenAPI_sm_addressee_type_FromString(s_maddressee_type->valuestring);
    }

    recipient_address_local_var = OpenAPI_recipient_address_create (
        recipient_address_info ? recipient_address_info_local_nonprim : NULL,
        s_maddressee_type ? s_maddressee_typeVariable : 0
    );

    return recipient_address_local_var;
end:
    if (recipient_address_info_local_nonprim) {
        OpenAPI_sm_address_info_free(recipient_address_info_local_nonprim);
        recipient_address_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_recipient_address_t *OpenAPI_recipient_address_copy(OpenAPI_recipient_address_t *dst, OpenAPI_recipient_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_recipient_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_recipient_address_convertToJSON() failed");
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

    OpenAPI_recipient_address_free(dst);
    dst = OpenAPI_recipient_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

