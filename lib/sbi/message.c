/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-sbi.h"

#include "sbi-private.h"

static OGS_POOL(request_pool, ogs_sbi_request_t);
static OGS_POOL(response_pool, ogs_sbi_response_t);

void ogs_sbi_message_init(int num_of_request_pool, int num_of_response_pool)
{
    ogs_pool_init(&request_pool, num_of_request_pool);
    ogs_pool_init(&response_pool, num_of_response_pool);
}

void ogs_sbi_message_final(void)
{
    ogs_pool_final(&request_pool);
    ogs_pool_final(&response_pool);
}

void ogs_sbi_message_free(ogs_sbi_message_t *message)
{
    ogs_assert(message);

    if (message->NFProfile)
        OpenAPI_nf_profile_free(message->NFProfile);
    if (message->ProblemDetails)
        OpenAPI_problem_details_free(message->ProblemDetails);
    if (message->PatchItemList) {
        OpenAPI_lnode_t *node = NULL;
        OpenAPI_list_for_each(message->PatchItemList, node)
            OpenAPI_patch_item_free(node->data);
        OpenAPI_list_free(message->PatchItemList);
    }

    if (message->SubscriptionData)
        OpenAPI_subscription_data_free(message->SubscriptionData);
    if (message->NotificationData)
        OpenAPI_notification_data_free(message->NotificationData);
    if (message->SearchResult)
        OpenAPI_search_result_free(message->SearchResult);
}

ogs_sbi_request_t *ogs_sbi_request_new(void)
{
    ogs_sbi_request_t *request = NULL;

    ogs_pool_alloc(&request_pool, &request);

    ogs_assert(request);
    memset(request, 0, sizeof(ogs_sbi_request_t));

    request->http.params = ogs_hash_make();
    request->http.headers = ogs_hash_make();

    return request;
}

ogs_sbi_response_t *ogs_sbi_response_new(void)
{
    ogs_sbi_response_t *response = NULL;

    ogs_pool_alloc(&response_pool, &response);
    ogs_assert(response);
    memset(response, 0, sizeof(ogs_sbi_response_t));

    response->http.params = ogs_hash_make();
    response->http.headers = ogs_hash_make();

    return response;
}

static void sbi_header_free(ogs_sbi_header_t *h);
static void http_message_free(http_message_t *http);

void ogs_sbi_request_free(ogs_sbi_request_t *request)
{
    ogs_assert(request);

    if (request->h.url)
        ogs_free(request->h.url);

    sbi_header_free(&request->h);
    http_message_free(&request->http);

    ogs_pool_free(&request_pool, request);
}

void ogs_sbi_response_free(ogs_sbi_response_t *response)
{
    ogs_assert(response);

    if (response->h.url)
        ogs_free(response->h.url);

    sbi_header_free(&response->h);
    http_message_free(&response->http);

    ogs_pool_free(&response_pool, response);
}

static void sbi_header_free(ogs_sbi_header_t *h)
{
    ogs_assert(h);

    if (h->method) ogs_free(h->method);
    if (h->service.name) ogs_free(h->service.name);
    if (h->api.version) ogs_free(h->api.version);
    if (h->resource.name) ogs_free(h->resource.name);
    if (h->resource.id) ogs_free(h->resource.id);
}

static void http_message_free(http_message_t *http)
{
    ogs_assert(http);

    if (http->params) {
        ogs_hash_index_t *hi;
        for (hi = ogs_hash_first(http->params); hi; hi = ogs_hash_next(hi)) {
            char *val = ogs_hash_this_val(hi);
            ogs_free(val);
        }
        ogs_hash_destroy(http->params);
    }

    if (http->headers) {
        ogs_hash_index_t *hi;
        for (hi = ogs_hash_first(http->headers); hi; hi = ogs_hash_next(hi)) {
            char *val = ogs_hash_this_val(hi);
            ogs_free(val);
        }
        ogs_hash_destroy(http->headers);
    }
    if (http->content) ogs_free(http->content);
}

static char *build_content(ogs_sbi_message_t *message);

ogs_sbi_request_t *ogs_sbi_build_request(ogs_sbi_message_t *message)
{
    ogs_sbi_request_t *request = NULL;

    ogs_assert(message);

    request = ogs_sbi_request_new();
    ogs_assert(request);

    ogs_assert(message->h.method);
    request->h.method = ogs_strdup(message->h.method);
    if (message->h.url) {
        request->h.url = ogs_strdup(message->h.url);
    } else {
        ogs_assert(message->h.service.name);
        request->h.service.name = ogs_strdup(message->h.service.name);
        ogs_assert(message->h.api.version);
        request->h.api.version = ogs_strdup(message->h.api.version);
        ogs_assert(message->h.resource.name);
        request->h.resource.name = ogs_strdup(message->h.resource.name);
        if (message->h.resource.id)
            request->h.resource.id = ogs_strdup(message->h.resource.id);
    }

    /* URL Param */
    if (message->param.nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.nf_type);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_NF_TYPE, v);
    }
    if (message->param.requester_nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.requester_nf_type);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_REQUESTER_NF_TYPE, v);
    }
    if (message->param.target_nf_type) {
        char *v = OpenAPI_nf_type_ToString(message->param.target_nf_type);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params,
                OGS_SBI_PARAM_TARGET_NF_TYPE, v);
    }
    if (message->param.limit) {
        char *v = ogs_msprintf("%d", message->param.limit);
        ogs_assert(v);
        ogs_sbi_header_set(request->http.params, OGS_SBI_PARAM_LIMIT, v);
    }

    /* HTTP Message */
    request->http.content = build_content(message);
    if (request->http.content) {
        if (message->http.content_type)
            ogs_sbi_header_set(request->http.headers,
                    OGS_SBI_CONTENT_TYPE, message->http.content_type);
        else
            ogs_sbi_header_set(request->http.headers,
                    OGS_SBI_CONTENT_TYPE, OGS_SBI_CONTENT_JSON_TYPE);
    }

    if (message->http.content_encoding)
        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_ACCEPT_ENCODING, message->http.content_encoding);

    return request;
}

ogs_sbi_response_t *ogs_sbi_build_response(ogs_sbi_message_t *message)
{
    ogs_sbi_response_t *response = NULL;

    ogs_assert(message);

    response = ogs_sbi_response_new();
    ogs_assert(response);

    /* HTTP Message */
    response->http.content = build_content(message);
    if (response->http.content) {
        if (message->http.content_type)
            ogs_sbi_header_set(response->http.headers,
                    OGS_SBI_CONTENT_TYPE, message->http.content_type);
        else
            ogs_sbi_header_set(response->http.headers,
                    OGS_SBI_CONTENT_TYPE, OGS_SBI_CONTENT_JSON_TYPE);
    }

    if (message->http.location == true)
        ogs_sbi_header_set(response->http.headers, "Location", message->h.url);
    if (message->http.cache_control)
        ogs_sbi_header_set(response->http.headers, "Cache-Control",
                message->http.cache_control);

    return response;
}

static char *build_content(ogs_sbi_message_t *message)
{
    char *content = NULL;
    cJSON *item = NULL;
    ogs_assert(message);

    if (message->ProblemDetails) {
        item = OpenAPI_problem_details_convertToJSON(message->ProblemDetails);
        ogs_assert(item);
    } else if (message->NFProfile) {
        item = OpenAPI_nf_profile_convertToJSON(message->NFProfile);
        ogs_assert(item);
    } else if (message->PatchItemList) {
        OpenAPI_lnode_t *node = NULL;

        item = cJSON_CreateArray();
        ogs_assert(item);

        OpenAPI_list_for_each(message->PatchItemList, node) {
            cJSON *patchItem = OpenAPI_patch_item_convertToJSON(node->data);
            ogs_assert(patchItem);
            cJSON_AddItemToArray(item, patchItem);
        }
    } else if (message->SubscriptionData) {
        item = OpenAPI_subscription_data_convertToJSON(
                message->SubscriptionData);
        ogs_assert(item);
    } else if (message->NotificationData) {
        item = OpenAPI_notification_data_convertToJSON(
                message->NotificationData);
        ogs_assert(item);
    } else if (message->SearchResult) {
        item = OpenAPI_search_result_convertToJSON(message->SearchResult);
        ogs_assert(item);
    } else if (message->links) {
        item = ogs_sbi_links_convertToJSON(message->links);
        ogs_assert(item);
    }

    if (item) {
        content = cJSON_Print(item);
        ogs_assert(content);
        cJSON_Delete(item);
    }

    return content;
}

static int parse_sbi_header(
        ogs_sbi_message_t *message, ogs_sbi_header_t *header);
static int parse_content(ogs_sbi_message_t *message, char *content);

int ogs_sbi_parse_request(
        ogs_sbi_message_t *message, ogs_sbi_request_t *request)
{
    int rv;
    ogs_hash_index_t *hi;

    ogs_assert(request);
    ogs_assert(message);

    rv = parse_sbi_header(message, &request->h);
    if (rv != OGS_OK) {
        ogs_error("sbi_parse_header() failed");
        return OGS_ERROR;
    }

    for (hi = ogs_hash_first(request->http.params);
            hi; hi = ogs_hash_next(hi)) {
        if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_PARAM_NF_TYPE)) {
            message->param.nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_TARGET_NF_TYPE)) {
            message->param.target_nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_REQUESTER_NF_TYPE)) {
            message->param.requester_nf_type =
                OpenAPI_nf_type_FromString(ogs_hash_this_val(hi));
        } else if (!strcmp(ogs_hash_this_key(hi),
                    OGS_SBI_PARAM_LIMIT)) {
            message->param.limit = atoi(ogs_hash_this_val(hi));
        }
    }

    for (hi = ogs_hash_first(request->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_ACCEPT_ENCODING)) {
            message->http.content_encoding = ogs_hash_this_val(hi);
        } else if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_CONTENT_TYPE)) {
            message->http.content_type = ogs_hash_this_val(hi);
        }
    }

    if (parse_content(message, request->http.content) != OGS_OK) {
        ogs_error("parse_content() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sbi_parse_response(
        ogs_sbi_message_t *message, ogs_sbi_response_t *response)
{
    int rv;
    ogs_hash_index_t *hi;

    ogs_assert(response);
    ogs_assert(message);

    rv = parse_sbi_header(message, &response->h);
    if (rv != OGS_OK) {
        ogs_error("sbi_parse_header() failed");
        return OGS_ERROR;
    }

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        if (!strcmp(ogs_hash_this_key(hi), OGS_SBI_CONTENT_TYPE)) {
            message->http.content_type = ogs_hash_this_val(hi);
        }
    }

    if (parse_content(message, response->http.content) != OGS_OK) {
        ogs_error("parse_content() failed");
        return OGS_ERROR;
    }

    message->res_status = response->status;

    return OGS_OK;
}

static int parse_sbi_header(
        ogs_sbi_message_t *message, ogs_sbi_header_t *header)
{
    struct yuarel yuarel;
    char *saveptr = NULL;
    char *url = NULL, *p = NULL;;

    ogs_assert(message);
    ogs_assert(header);

    memset(message, 0, sizeof(*message));

    message->h.method = header->method;
    ogs_assert(message->h.method);
    message->h.url = header->url;
    ogs_assert(message->h.url);

    url = ogs_strdup(header->url);
    ogs_assert(url);
    p = url;

    if (p[0] != '/') {
        int rv = yuarel_parse(&yuarel, p);
        if (rv != OGS_OK) {
            ogs_error("yuarel_parse() failed");
            ogs_free(url);
            return OGS_ERROR;
        }

        p = yuarel.path;
    }

    header->service.name = ogs_sbi_parse_url(p, "/", &saveptr);
    if (!header->service.name) {
        ogs_error("ogs_sbi_parse_url() failed");
        ogs_free(url);
        return OGS_ERROR;
    }
    message->h.service.name = header->service.name;

    header->api.version = ogs_sbi_parse_url(NULL, "/", &saveptr);
    if (!header->api.version) {
        ogs_error("ogs_sbi_parse_url() failed");
        ogs_free(url);
        return OGS_ERROR;
    }
    message->h.api.version = header->api.version;

    header->resource.name = ogs_sbi_parse_url(NULL, "/", &saveptr);
    if (!header->resource.name) {
        ogs_error("ogs_sbi_parse_url() failed");
        ogs_free(url);
        return OGS_ERROR;
    }
    message->h.resource.name = header->resource.name;

    header->resource.id = ogs_sbi_parse_url(NULL, "/", &saveptr);
    message->h.resource.id = header->resource.id;

    ogs_free(url);

    return OGS_OK;
}

static int parse_content(ogs_sbi_message_t *message, char *content)
{
    int rv = OGS_OK;
    cJSON *item = NULL;

    ogs_assert(message);

    if (!content)
        return OGS_OK;

    if (strcmp(message->h.api.version, OGS_SBI_API_VERSION) != 0) {
        ogs_error("Not supported version - %s", message->h.api.version);
        return OGS_ERROR;
    }

    item = cJSON_Parse(content);
    if (!item) {
        ogs_error("JSON parse error");
        return OGS_ERROR;
    }

    if (message->http.content_type) {
        if (!strncmp(message->http.content_type,
                OGS_SBI_CONTENT_PROBLEM_TYPE,
                strlen(OGS_SBI_CONTENT_PROBLEM_TYPE))) {
            message->ProblemDetails =
                OpenAPI_problem_details_parseFromJSON(item);
        } else if (!strncmp(message->http.content_type,
                OGS_SBI_CONTENT_PATCH_TYPE,
                strlen(OGS_SBI_CONTENT_PATCH_TYPE))) {
            if (item) {
                OpenAPI_patch_item_t *patch_item = NULL;
                cJSON *patchJSON = NULL;
                message->PatchItemList = OpenAPI_list_create();
                cJSON_ArrayForEach(patchJSON, item) {
                    if (!cJSON_IsObject(patchJSON)) {
                        rv = OGS_ERROR;
                        ogs_error("Unknown JSON");
                        goto cleanup;
                    }

                    patch_item = OpenAPI_patch_item_parseFromJSON(patchJSON);
                    OpenAPI_list_add(message->PatchItemList, patch_item);
                }
            }
        } else {
            SWITCH(message->h.service.name)
            CASE(OGS_SBI_SERVICE_NAME_NRF_NFM)

                SWITCH(message->h.resource.name)
                CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                    message->NFProfile =
                        OpenAPI_nf_profile_parseFromJSON(item);
                    if (!message->NFProfile) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                    message->SubscriptionData =
                        OpenAPI_subscription_data_parseFromJSON(item);
                    if (!message->SubscriptionData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                    message->NotificationData =
                        OpenAPI_notification_data_parseFromJSON(item);
                    if (!message->NotificationData) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                    break;

                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown resource name [%s]",
                            message->h.resource.name);
                END
                break;

            CASE(OGS_SBI_SERVICE_NAME_NRF_DISC)
                SWITCH(message->h.resource.name)
                CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                    message->SearchResult =
                        OpenAPI_search_result_parseFromJSON(item);
                    if (!message->SearchResult) {
                        rv = OGS_ERROR;
                        ogs_error("JSON parse error");
                    }
                    break;

                DEFAULT
                    rv = OGS_ERROR;
                    ogs_error("Unknown resource name [%s]",
                            message->h.resource.name);
                END
                break;

            DEFAULT
                rv = OGS_ERROR;
                ogs_error("Not implemented API name [%s]",
                        message->h.service.name);
            END
        }
    }

cleanup:

    cJSON_Delete(item);
    return rv;
}

void ogs_sbi_header_set(ogs_hash_t *ht, const void *key, const void *val)
{
    ogs_hash_set(ht, key, strlen(key), ogs_strdup(val));
}

void *ogs_sbi_header_get(ogs_hash_t *ht, const void *key)
{
    return ogs_hash_get(ht, key, strlen(key));
}

