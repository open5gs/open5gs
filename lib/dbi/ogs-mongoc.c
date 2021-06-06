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

#include <mongoc.h>

#include "ogs-dbi.h"

int __ogs_dbi_domain;

static ogs_mongoc_t self;

/*
 * We've added it 
 * Because the following function is deprecated in the mongo-c-driver
 */
static bool
ogs_mongoc_mongoc_client_get_server_status (mongoc_client_t *client, /* IN */
                                 mongoc_read_prefs_t *read_prefs, /* IN */
                                 bson_t *reply,                   /* OUT */
                                 bson_error_t *error)             /* OUT */
{
   bson_t cmd = BSON_INITIALIZER;
   bool ret = false;

   BSON_ASSERT (client);

   BSON_APPEND_INT32 (&cmd, "serverStatus", 1);
   ret = mongoc_client_command_simple (
      client, "admin", &cmd, read_prefs, reply, error);
   bson_destroy (&cmd);

   return ret;
}

static char *masked_db_uri(const char *db_uri)
{
    char *tmp;
    char *array[2], *saveptr = NULL;
    char *masked = NULL;

    ogs_assert(db_uri);

    tmp = ogs_strdup(db_uri);
    ogs_assert(tmp);

    memset(array, 0, sizeof(array));
    array[0] = strtok_r(tmp, "@", &saveptr);
    if (array[0])
        array[1] = strtok_r(NULL, "@", &saveptr);

    if (array[1]) {
        masked = ogs_msprintf("mongodb://*****:*****@%s", array[1]);
        ogs_assert(masked);
    } else {
        masked = ogs_strdup(array[0]);
        ogs_assert(masked);
    }

    ogs_free(tmp);

    return masked;
}

int ogs_mongoc_init(const char *db_uri)
{
    bson_t reply;
    bson_error_t error;
    bson_iter_t iter;

    const mongoc_uri_t *uri;

    if (!db_uri) {
        ogs_error("No DB_URI");
        return OGS_ERROR;
    }

    memset(&self, 0, sizeof(ogs_mongoc_t));

    self.masked_db_uri = masked_db_uri(db_uri);

    mongoc_init();

    self.initialized = true;

    self.client = mongoc_client_new(db_uri);
    if (!self.client) {
        ogs_error("Failed to parse DB URI [%s]", self.masked_db_uri);
        return OGS_ERROR;
    }

#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 4
    mongoc_client_set_error_api(self.client, 2);
#endif

    uri = mongoc_client_get_uri(self.client);
    ogs_assert(uri);

    self.name = mongoc_uri_get_database(uri);
    ogs_assert(self.name);

    self.database = mongoc_client_get_database(self.client, self.name);
    ogs_assert(self.database);

    if (!ogs_mongoc_mongoc_client_get_server_status(
                self.client, NULL, &reply, &error)) {
        ogs_warn("Failed to connect to server [%s]", self.masked_db_uri);
        return OGS_RETRY;
    }

    ogs_assert(bson_iter_init_find(&iter, &reply, "ok"));

    bson_destroy(&reply);

    ogs_info("MongoDB URI: '%s'", self.masked_db_uri);

    return OGS_OK;
}

void ogs_mongoc_final(void)
{
    if (self.database) {
        mongoc_database_destroy(self.database);
        self.database = NULL;
    }
    if (self.client) {
        mongoc_client_destroy(self.client);
        self.client = NULL;
    }
    if (self.masked_db_uri) {
        ogs_free(self.masked_db_uri);
        self.masked_db_uri = NULL;
    }

    if (self.initialized) {
        mongoc_cleanup();
        self.initialized = false;
    }
}

ogs_mongoc_t *ogs_mongoc(void)
{
    return &self;
}

int ogs_dbi_init(const char *db_uri)
{
    int rv;

    ogs_assert(db_uri);

    rv = ogs_mongoc_init(db_uri);
    if (rv != OGS_OK) return rv;

    if (ogs_mongoc()->client && ogs_mongoc()->name) {
        self.collection.subscriber = mongoc_client_get_collection(
            ogs_mongoc()->client, ogs_mongoc()->name, "subscribers");
        ogs_assert(self.collection.subscriber);
    }

    return OGS_OK;
}

void ogs_dbi_final()
{
    if (self.collection.subscriber) {
        mongoc_collection_destroy(self.collection.subscriber);
    }

    ogs_mongoc_final();
}
