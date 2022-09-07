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

#include "hss-context.h"
#include "hss-fd-path.h"
#include "hss-s6a-path.h"

static ogs_thread_t *thread;
static void hss_main(void *data);

static int initialized = 0;

int hss_initialize(void)
{
    int rv;

    hss_context_init();

    rv = hss_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_dbi_init(ogs_app()->db_uri);
    if (rv != OGS_OK) return rv;

    rv = hss_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

    thread = ogs_thread_create(hss_main, NULL);
    if (!thread) return OGS_ERROR;        

    initialized = 1;

    return OGS_OK;
}

void hss_terminate(void)
{
    if (!initialized) return;

    ogs_thread_destroy(thread);

    hss_fd_final();

    ogs_dbi_final();
    hss_context_final();

    return;
}

static void hss_main(void *data)
{
    bson_t empty = BSON_INITIALIZER;
    const bson_t *doc;
    const bson_t *err_doc;
    bson_error_t error;
    mongoc_collection_t *coll;
    mongoc_change_stream_t *stream;
    mongoc_uri_t *uri;
    mongoc_client_t *client;
    bson_t *options = NULL;

    bson_iter_t iter;
    bson_iter_t child1_iter;
    bson_iter_t child2_iter;    

    char *utf8 = NULL;
    uint32_t length = 0;
    bool send_clr_flag = false;
    bool send_idr_flag = false;
    uint32_t subdatamask;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    mongoc_init();

    uri = mongoc_uri_new_with_error (ogs_app()->db_uri, &error);
    if (!uri) {
        fprintf (stderr,
                "failed to parse URI: %s\n"
                "error message:       %s\n",
                ogs_app()->db_uri,
                error.message);
    }

    client = mongoc_client_new_from_uri (uri);
    if (!client) {
        ogs_error("Client Error.");
    }

    options = BCON_NEW("fullDocument", "updateLookup");
   
    coll = mongoc_client_get_collection (client, "open5gs", "subscribers");
    stream = mongoc_collection_watch (coll, &empty, options);

    if (mongoc_change_stream_error_document (stream, &error, &err_doc)) {
        if (!bson_empty (err_doc)) {
            ogs_error("Change Stream Error.  Enable replica sets to \
                enable database updates to be sent to MME.");
        } else {
            ogs_error("Client Error: %s\n", error.message);
        }
        goto end;
    }

    ogs_info("Change Stream Available.");

    while (1) {
        while (mongoc_change_stream_next (stream, &doc)) {
                char *as_json = bson_as_relaxed_extended_json (doc, NULL);
            ogs_debug("Got document: %s\n", as_json);
            if (!bson_iter_init_find(&iter, doc, "fullDocument")) {
                ogs_debug("No 'imsi' field in this document.");
                goto out;
            } else {
                bson_iter_recurse(&iter, &child1_iter);
                while (bson_iter_next(&child1_iter)) {
                    const char *key = bson_iter_key(&child1_iter);
                    if (!strcmp(key, "imsi") && 
                        BSON_ITER_HOLDS_UTF8(&child1_iter)) {
                        utf8 = (char *)bson_iter_utf8(&child1_iter, &length);
                        memcpy(imsi_bcd, utf8, 15);
                    }
                }
            }

            if (bson_iter_init_find(&iter, doc, "updateDescription")) {
                bson_iter_recurse(&iter, &child1_iter);
                while (bson_iter_next(&child1_iter)) {
                    const char *key = bson_iter_key(&child1_iter);
                    if (!strcmp(key, "updatedFields") && 
                            BSON_ITER_HOLDS_DOCUMENT(&child1_iter)) {
                        bson_iter_recurse(&child1_iter, &child2_iter);
                        while (bson_iter_next(&child2_iter)) {
                            const char *child2_key = bson_iter_key(
                                &child2_iter);
                            if (!strcmp(child2_key, 
                                    "request_cancel_location") && 
                                    BSON_ITER_HOLDS_BOOL(&child2_iter)) {
                                send_clr_flag = 
                                    (char *)bson_iter_bool(&child2_iter);
                            } else if (!strncmp(child2_key, "msisdn", 
                                    strlen("msisdn"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_MSISDN);
                            } else if (!strncmp(child2_key, 
                                    "access_restriction_data", 
                                    strlen("access_restriction_data"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_ARD);
                            } else if (!strncmp(child2_key, 
                                    "subscriber_status", 
                                    strlen("subscriber_status"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_SUB_STATUS);
                            } else if (!strncmp(child2_key, 
                                    "network_access_mode", 
                                    strlen("network_access_mode"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_NAM);
                            } else if (!strncmp(child2_key, "ambr", 
                                    strlen("ambr"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_UEAMBR);
                            } else if (!strncmp(child2_key, 
                                    "subscribed_rau_tau_timer", 
                                    strlen("subscribed_rau_tau_timer"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_RAU_TAU_TIMER);
                            } else if (!strncmp(child2_key, "slice", 
                                    strlen("slice"))) {
                                send_idr_flag = true;
                                subdatamask = (subdatamask | 
                                    OGS_HSS_SUBDATA_SLICE);
                            }
                        }
                    }
                }
            } else {
                ogs_debug("No 'updateDescription' field in this document");
            }

            if (send_idr_flag) {
                ogs_info("[%s] Subscription-Data Changed", imsi_bcd);
                hss_s6a_send_idr(imsi_bcd, 0, subdatamask);
                send_idr_flag = false;
                subdatamask = OGS_HSS_SUBDATA_NO_UPDATE;
            }

            if (send_clr_flag) {
                ogs_info("[%s] Cancel Location Requested", imsi_bcd);
                hss_s6a_send_clr(imsi_bcd, NULL, NULL, 
                    OGS_DIAM_S6A_CT_SUBSCRIPTION_WITHDRAWL);            
                send_clr_flag = false;
            }
            
            bson_free (as_json);
        }
out:
        if (mongoc_change_stream_error_document (stream, &error, &err_doc)) {
            if (!bson_empty (err_doc)) {
                ogs_debug("Server Error: %s\n",
                bson_as_relaxed_extended_json (err_doc, NULL));
            } else {
                ogs_debug("Client Error: %s\n", error.message);
            }
        }
        sleep(1);
    }
end:
    mongoc_client_destroy (client);
}
