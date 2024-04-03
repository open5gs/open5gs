/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_DBI_INSIDE) && !defined(OGS_DBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_MONGOC_H
#define OGS_MONGOC_H

#include <mongoc.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct ogs_mongoc_s {
    bool initialized;
    const char *name;
    void *uri;
    void *client;
    void *database;

#if MONGOC_CHECK_VERSION(1, 9, 0)
    mongoc_change_stream_t *stream;
#endif

    char *masked_db_uri;

    struct {
        void *subscriber;
    } collection;
} ogs_mongoc_t;

int ogs_mongoc_init(const char *db_uri);
void ogs_mongoc_final(void);
ogs_mongoc_t *ogs_mongoc(void);

int ogs_dbi_init(const char *db_uri);
void ogs_dbi_final(void);

int ogs_dbi_collection_watch_init(void);
int ogs_dbi_poll_change_stream(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_MONGOC_H */
