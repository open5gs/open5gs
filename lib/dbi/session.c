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

#include "ogs-dbi.h"

int ogs_dbi_session_data(char *supi, ogs_s_nssai_t *s_nssai, char *dnn,
        ogs_session_data_t *session_data)
{
    int rv = OGS_OK;
    mongoc_cursor_t *cursor = NULL;
    bson_t *query = NULL;
    bson_t *opts = NULL;
    bson_error_t error;
    const bson_t *document;
    bson_iter_t iter;
    bson_iter_t child1_iter, child2_iter, child3_iter, child4_iter, child5_iter;
    bson_iter_t child6_iter, child7_iter, child8_iter, child9_iter;
    const char *utf8 = NULL;
    uint32_t length = 0;
    bool found = false;

    ogs_session_t *session = NULL;

    char *supi_type = NULL;
    char *supi_id = NULL;

    ogs_session_data_t zero_data;

    ogs_assert(supi);
    ogs_assert(dnn);
    ogs_assert(session_data);

    memset(&zero_data, 0, sizeof(zero_data));

    /* session_data should be initialized to zero */
    ogs_assert(memcmp(session_data, &zero_data, sizeof(zero_data)) == 0);

    supi_type = ogs_id_get_type(supi);
    ogs_assert(supi_type);
    supi_id = ogs_id_get_value(supi);
    ogs_assert(supi_id);

    query = BCON_NEW(supi_type, BCON_UTF8(supi_id));
#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 5
    cursor = mongoc_collection_find_with_opts(
            ogs_mongoc()->collection.subscriber, query, NULL, NULL);
#else
    cursor = mongoc_collection_find(ogs_mongoc()->collection.subscriber,
            MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
#endif

    if (!mongoc_cursor_next(cursor, &document)) {
        ogs_error("[%s] Cannot find IMSI in DB", supi);

        rv = OGS_ERROR;
        goto out;
    }

    if (mongoc_cursor_error(cursor, &error)) {
        ogs_error("Cursor Failure: %s", error.message);

        rv = OGS_ERROR;
        goto out;
    }

    /* Finding Session for S_NSSAI+DNN */
    if (!bson_iter_init(&iter, document)) {
        ogs_error("bson_iter_init failed in this document");

        rv = OGS_ERROR;
        goto out;
    }

    while (bson_iter_next(&iter)) {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "slice") && BSON_ITER_HOLDS_ARRAY(&iter)) {
            bson_iter_recurse(&iter, &child1_iter);
            while (bson_iter_next(&child1_iter)) {
                uint8_t sst;
                ogs_uint24_t sd;

                sst = 0;
                sd.v = OGS_S_NSSAI_NO_SD_VALUE;

                bson_iter_recurse(&child1_iter, &child2_iter);
                while (bson_iter_next(&child2_iter)) {
                    const char *child2_key = bson_iter_key(&child2_iter);

                    if (!strcmp(child2_key, "sst") &&
                        BSON_ITER_HOLDS_INT32(&child2_iter)) {
                        sst = bson_iter_int32(&child2_iter);
                    } else if (!strcmp(child2_key, "sd") &&
                        BSON_ITER_HOLDS_UTF8(&child2_iter)) {
                        utf8 = bson_iter_utf8(&child2_iter, &length);
                        ogs_assert(utf8);
                        sd = ogs_s_nssai_sd_from_string(utf8);
                    } else if (!strcmp(child2_key, "session") &&
                        BSON_ITER_HOLDS_ARRAY(&child2_iter)) {
                        bson_iter_recurse(&child2_iter, &child3_iter);
                    }
                }

                if (!sst) {
                    ogs_error("No SST");
                    continue;
                }

                if (s_nssai && s_nssai->sst != sst) continue;

                if (s_nssai &&
                    s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE &&
                    sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                    if (s_nssai->sd.v != sd.v) continue;
                }

                while (bson_iter_next(&child3_iter)) {
                    bson_iter_recurse(&child3_iter, &child4_iter);
                    while (bson_iter_next(&child4_iter)) {
                        const char *child4_key = bson_iter_key(&child4_iter);
                        if (!strcmp(child4_key, "name") &&
                            BSON_ITER_HOLDS_UTF8(&child4_iter)) {
                            utf8 = bson_iter_utf8(&child4_iter, &length);
                            if (ogs_strncasecmp(utf8, dnn, length) == 0) {
                                found = true;
                                goto done;
                            }
                        }
                    }
                }
            }
        }
    }

done:
    if (found == false) {
        ogs_error("Cannot find SUPI[%s] S_NSSAI[SST:%d SD:0x%x] DNN[%s] in DB",
                supi_id,
                s_nssai ? s_nssai->sst : 0,
                s_nssai ? s_nssai->sd.v : 0,
                dnn);

        rv = OGS_ERROR;
        goto out;
    }

    session = &session_data->session;
    bson_iter_recurse(&child3_iter, &child4_iter);
    while (bson_iter_next(&child4_iter)) {
        const char *child4_key = bson_iter_key(&child4_iter);
        if (!strcmp(child4_key, "name") &&
            BSON_ITER_HOLDS_UTF8(&child4_iter)) {
            utf8 = bson_iter_utf8(&child4_iter, &length);
            session->name = ogs_strndup(utf8, length);
            ogs_assert(session->name);
        } else if (!strcmp(child4_key, "type") &&
            BSON_ITER_HOLDS_INT32(&child4_iter)) {
            session->session_type = bson_iter_int32(&child4_iter);
        } else if (!strcmp(child4_key, "qos") &&
            BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
            bson_iter_recurse(&child4_iter, &child5_iter);
            while (bson_iter_next(&child5_iter)) {
                const char *child5_key = bson_iter_key(&child5_iter);
                if (!strcmp(child5_key, "index") &&
                    BSON_ITER_HOLDS_INT32(&child5_iter)) {
                    session->qos.index = bson_iter_int32(&child5_iter);
                } else if (!strcmp(child5_key, "arp") &&
                    BSON_ITER_HOLDS_DOCUMENT(&child5_iter)) {
                    bson_iter_recurse(&child5_iter, &child6_iter);
                    while (bson_iter_next(&child6_iter)) {
                        const char *child6_key = bson_iter_key(&child6_iter);
                        if (!strcmp(child6_key, "priority_level") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            session->qos.arp.priority_level =
                                bson_iter_int32(&child6_iter);
                        } else if (!strcmp(child6_key,
                                    "pre_emption_capability") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            session->qos.arp.pre_emption_capability =
                                bson_iter_int32(&child6_iter);
                        } else if (!strcmp(child6_key,
                                    "pre_emption_vulnerability") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            session->qos.arp.pre_emption_vulnerability =
                                bson_iter_int32(&child6_iter);
                        }
                    }
                }
            }
        } else if (!strcmp(child4_key, "ambr") &&
            BSON_ITER_HOLDS_DOCUMENT(&child4_iter)) {
            bson_iter_recurse(&child4_iter, &child5_iter);
            while (bson_iter_next(&child5_iter)) {
                const char *child5_key = bson_iter_key(&child5_iter);
                if (!strcmp(child5_key, "downlink") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child5_iter)) {
                    uint8_t unit = 0;
                    int n;

                    bson_iter_recurse(&child5_iter, &child6_iter);
                    while (bson_iter_next(&child6_iter)) {
                        const char *child6_key = bson_iter_key(&child6_iter);
                        if (!strcmp(child6_key, "value") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            session->ambr.downlink =
                                bson_iter_int32(&child6_iter);
                        } else if (!strcmp(child6_key, "unit") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            unit = bson_iter_int32(&child6_iter);
                        }
                    }

                    for (n = 0; n < unit; n++)
                        session->ambr.downlink *= 1024;
                } else if (!strcmp(child5_key, "uplink") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child5_iter)) {
                    uint8_t unit = 0;
                    int n;

                    bson_iter_recurse(&child5_iter, &child6_iter);
                    while (bson_iter_next(&child6_iter)) {
                        const char *child6_key = bson_iter_key(&child6_iter);
                        if (!strcmp(child6_key, "value") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            session->ambr.uplink =
                                bson_iter_int32(&child6_iter);
                        } else if (!strcmp(child6_key, "unit") &&
                            BSON_ITER_HOLDS_INT32(&child6_iter)) {
                            unit = bson_iter_int32(&child6_iter);
                        }
                    }

                    for (n = 0; n < unit; n++)
                        session->ambr.uplink *= 1024;
                }

            }
        } else if (!strcmp(child4_key, "pcc_rule") &&
            BSON_ITER_HOLDS_ARRAY(&child4_iter)) {
            int pcc_rule_index = 0;

            bson_iter_recurse(&child4_iter, &child5_iter);
            while (bson_iter_next(&child5_iter)) {
                ogs_pcc_rule_t *pcc_rule = NULL;

                ogs_assert(pcc_rule_index < OGS_MAX_NUM_OF_PCC_RULE);
                pcc_rule = &session_data->pcc_rule[pcc_rule_index];

                bson_iter_recurse(&child5_iter, &child6_iter);
                while (bson_iter_next(&child6_iter)) {
                    const char *child6_key = bson_iter_key(&child6_iter);

                    if (!strcmp(child6_key, "qos") &&
                        BSON_ITER_HOLDS_DOCUMENT(&child6_iter)) {
                        bson_iter_recurse(&child6_iter, &child7_iter);
                        while (bson_iter_next(&child7_iter)) {
                            const char *child7_key =
                                bson_iter_key(&child7_iter);
                            if (!strcmp(child7_key, "index") &&
                                BSON_ITER_HOLDS_INT32(&child7_iter)) {
                                pcc_rule->qos.index =
                                    bson_iter_int32(&child7_iter);
                            } else if (!strcmp(child7_key, "arp") &&
                                BSON_ITER_HOLDS_DOCUMENT(&child7_iter)) {
                                bson_iter_recurse(&child7_iter, &child8_iter);
                                while (bson_iter_next(&child8_iter)) {
                                    const char *child8_key =
                                        bson_iter_key(&child8_iter);
                                    if (!strcmp(child8_key, "priority_level") &&
                                        BSON_ITER_HOLDS_INT32(&child8_iter)) {
                                        pcc_rule->qos.arp.priority_level =
                                            bson_iter_int32(&child8_iter);
                                    } else if (!strcmp(child8_key,
                                        "pre_emption_capability") &&
                                        BSON_ITER_HOLDS_INT32(&child8_iter)) {
                                        pcc_rule->qos.arp.
                                            pre_emption_capability =
                                                bson_iter_int32(&child8_iter);
                                    } else if (!strcmp(child8_key,
                                        "pre_emption_vulnerability") &&
                                        BSON_ITER_HOLDS_INT32(&child8_iter)) {
                                        pcc_rule->qos.arp.
                                            pre_emption_vulnerability =
                                            bson_iter_int32(&child8_iter);
                                    }
                                }
                            } else if (!strcmp(child7_key, "mbr") &&
                                BSON_ITER_HOLDS_DOCUMENT(&child7_iter)) {
                                bson_iter_recurse(&child7_iter, &child8_iter);
                                while (bson_iter_next(&child8_iter)) {
                                    const char *child8_key =
                                        bson_iter_key(&child8_iter);
                                    if (!strcmp(child8_key, "downlink") &&
                                        BSON_ITER_HOLDS_DOCUMENT(
                                            &child8_iter)) {
                                        uint8_t unit = 0;
                                        int n;

                                        bson_iter_recurse(
                                                &child8_iter, &child9_iter);
                                        while (bson_iter_next(&child9_iter)) {
                                            const char *child9_key =
                                                bson_iter_key(&child9_iter);
                                            if (!strcmp(child9_key, "value") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                pcc_rule->qos.mbr.downlink =
                                                    bson_iter_int32(
                                                            &child9_iter);
                                            } else if (!strcmp(
                                                        child9_key, "unit") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                unit = bson_iter_int32(
                                                        &child9_iter);
                                            }
                                        }

                                        for (n = 0; n < unit; n++)
                                            pcc_rule->qos.mbr.downlink *= 1024;

                                    } else if (!strcmp(child8_key, "uplink") &&
                                        BSON_ITER_HOLDS_DOCUMENT(
                                            &child8_iter)) {
                                        uint8_t unit = 0;
                                        int n;

                                        bson_iter_recurse(
                                                &child8_iter, &child9_iter);
                                        while (bson_iter_next(&child9_iter)) {
                                            const char *child9_key =
                                                bson_iter_key(&child9_iter);
                                            if (!strcmp(child9_key, "value") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                pcc_rule->qos.mbr.uplink =
                                                    bson_iter_int32(
                                                            &child9_iter);
                                            } else if (!strcmp(
                                                        child9_key, "unit") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                unit = bson_iter_int32(
                                                        &child9_iter);
                                            }
                                        }

                                        for (n = 0; n < unit; n++)
                                            pcc_rule->qos.mbr.uplink *= 1024;
                                    }
                                }
                            } else if (!strcmp(child7_key, "gbr") &&
                                BSON_ITER_HOLDS_DOCUMENT(&child7_iter)) {
                                bson_iter_recurse(&child7_iter, &child8_iter);
                                while (bson_iter_next(&child8_iter)) {
                                    const char *child8_key =
                                        bson_iter_key(&child8_iter);
                                    if (!strcmp(child8_key, "downlink") &&
                                        BSON_ITER_HOLDS_DOCUMENT(
                                            &child8_iter)) {
                                        uint8_t unit = 0;
                                        int n;

                                        bson_iter_recurse(
                                                &child8_iter, &child9_iter);
                                        while (bson_iter_next(&child9_iter)) {
                                            const char *child9_key =
                                                bson_iter_key(&child9_iter);
                                            if (!strcmp(child9_key, "value") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                pcc_rule->qos.gbr.downlink =
                                                    bson_iter_int32(
                                                            &child9_iter);
                                            } else if (!strcmp(
                                                        child9_key, "unit") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                unit = bson_iter_int32(
                                                        &child9_iter);
                                            }
                                        }

                                        for (n = 0; n < unit; n++)
                                            pcc_rule->qos.gbr.downlink *= 1024;

                                    } else if (!strcmp(child8_key, "uplink") &&
                                        BSON_ITER_HOLDS_DOCUMENT(
                                            &child8_iter)) {
                                        uint8_t unit = 0;
                                        int n;

                                        bson_iter_recurse(
                                                &child8_iter, &child9_iter);
                                        while (bson_iter_next(&child9_iter)) {
                                            const char *child9_key =
                                                bson_iter_key(&child9_iter);
                                            if (!strcmp(child9_key, "value") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                pcc_rule->qos.gbr.uplink =
                                                    bson_iter_int32(
                                                            &child9_iter);
                                            } else if (!strcmp(
                                                        child9_key, "unit") &&
                                                BSON_ITER_HOLDS_INT32(
                                                    &child9_iter)) {
                                                unit = bson_iter_int32(
                                                        &child9_iter);
                                            }
                                        }

                                        for (n = 0; n < unit; n++)
                                            pcc_rule->qos.gbr.uplink *= 1024;
                                    }
                                }
                            }
                        }
                    } else if (!strcmp(child6_key, "flow") &&
                        BSON_ITER_HOLDS_ARRAY(&child6_iter)) {
                        int flow_index = 0;

                        bson_iter_recurse(&child6_iter, &child7_iter);
                        while (bson_iter_next(&child7_iter)) {
                            ogs_flow_t *flow = NULL;

                            ogs_assert(
                                flow_index < OGS_MAX_NUM_OF_FLOW_IN_PCC_RULE);
                            flow = &pcc_rule->flow[flow_index];

                            bson_iter_recurse(&child7_iter, &child8_iter);
                            while (bson_iter_next(&child8_iter)) {
                                const char *child8_key =
                                    bson_iter_key(&child8_iter);
                                if (!strcmp(child8_key, "direction") &&
                                    BSON_ITER_HOLDS_INT32(&child8_iter)) {
                                    flow->direction =
                                        bson_iter_int32(&child8_iter);
                                } else if (!strcmp(child8_key, "description") &&
                                    BSON_ITER_HOLDS_UTF8(&child8_iter)) {
                                    utf8 = bson_iter_utf8(
                                            &child8_iter, &length);
                                    flow->description = ogs_calloc(1, length+1);
                                    ogs_assert(flow->description);
                                    ogs_cpystrn((char*)flow->description,
                                        utf8, length+1);
                                }
                            }
                            flow_index++;
                        }
                        pcc_rule->num_of_flow = flow_index;
                    }
                }

                /* EPC: Charing-Rule-Name */
                if (pcc_rule->name) {
                    ogs_error("PCC Rule Name has already been defined");
                    ogs_free(pcc_rule->name);
                }
                pcc_rule->name = ogs_msprintf("%s-g%d", dnn, pcc_rule_index+1);
                ogs_assert(pcc_rule->name);

                /* 5GC: PCC-Rule-Id */
                if (pcc_rule->id) {
                    ogs_error("PCC Rule Id has already been defined");
                    ogs_free(pcc_rule->id);
                }
                pcc_rule->id = ogs_msprintf("%s-n%d", dnn, pcc_rule_index+1);
                ogs_assert(pcc_rule->id);

                pcc_rule->precedence = pcc_rule_index+1;
                pcc_rule_index++;
            }
            session_data->num_of_pcc_rule = pcc_rule_index;
        }
    }

out:
    if (query) bson_destroy(query);
    if (opts) bson_destroy(opts);
    if (cursor) mongoc_cursor_destroy(cursor);

    ogs_free(supi_type);
    ogs_free(supi_id);

    return rv;
}
