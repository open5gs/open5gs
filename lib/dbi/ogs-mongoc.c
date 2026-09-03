/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

    BSON_APPEND_INT32 (&cmd, "ping", 1);
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
    array[0] = ogs_strtok_r(tmp, "@", &saveptr);
    if (array[0])
        array[1] = ogs_strtok_r(NULL, "@", &saveptr);

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

#if MONGOC_CHECK_VERSION(1, 4, 0)
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

/*
 * Guarantees uniqueness of `eir` records at the database level, since the
 * collection allows both a generic (PEI-only) record and a more specific
 * (PEI+SUPI) record for the same PEI, and ogs_dbi_eir_check_equipment()
 * only detects duplicates it happens to encounter during a lookup.
 */
static void eir_create_indexes(mongoc_collection_t *collection)
{
    bson_t *keys1, *partial1;
    bson_t *keys2, *partial2;
    mongoc_index_opt_t opt1, opt2;
    bson_t reply;
    bson_error_t error;

    ogs_assert(collection);

    keys1 = BCON_NEW("pei", BCON_INT32(1), "supi", BCON_INT32(1));
    partial1 = BCON_NEW("supi", "{", "$type", BCON_UTF8("string"), "}");
    mongoc_index_opt_init(&opt1);
    opt1.unique = true;
    opt1.name = "eir_specific_unique";
    opt1.partial_filter_expression = partial1;

    keys2 = BCON_NEW("pei", BCON_INT32(1));
    partial2 = BCON_NEW("supi", "{", "$eq", BCON_NULL, "}");
    mongoc_index_opt_init(&opt2);
    opt2.unique = true;
    opt2.name = "eir_generic_unique";
    opt2.partial_filter_expression = partial2;

    if (!mongoc_collection_create_index_with_opts(
            collection, keys1, &opt1, NULL, &reply, &error))
        ogs_error("eir_create_indexes(specific) failed: %s", error.message);
    bson_destroy(&reply);

    if (!mongoc_collection_create_index_with_opts(
            collection, keys2, &opt2, NULL, &reply, &error))
        ogs_error("eir_create_indexes(generic) failed: %s", error.message);
    bson_destroy(&reply);

    bson_destroy(keys1);
    bson_destroy(partial1);
    bson_destroy(keys2);
    bson_destroy(partial2);
}

/*
 * Rejects malformed `eir` documents (missing `pei`/`status`, wrong types,
 * unrecognized `status` values) at write time, on top of the application
 * level checks in lib/dbi/eir.c and src/eir/n5geir-handler.c.
 */
static void eir_apply_schema_validation(mongoc_database_t *database)
{
    static const char *validator_json =
        "{"
        "  \"collMod\": \"eir\","
        "  \"validator\": {"
        "    \"$jsonSchema\": {"
        "      \"bsonType\": \"object\","
        "      \"required\": [\"pei\", \"status\"],"
        "      \"properties\": {"
        "        \"pei\": { \"bsonType\": \"string\" },"
        "        \"supi\": { \"bsonType\": [\"string\", \"null\"] },"
        "        \"status\": {"
        "          \"enum\": "
        "            [\"WHITELISTED\", \"BLACKLISTED\", \"GREYLISTED\"]"
        "        }"
        "      }"
        "    }"
        "  },"
        "  \"validationLevel\": \"strict\","
        "  \"validationAction\": \"error\""
        "}";
    bson_t *cmd;
    bson_t reply;
    bson_error_t error;

    ogs_assert(database);

    cmd = bson_new_from_json((const uint8_t *)validator_json, -1, &error);
    if (!cmd) {
        ogs_error("eir_apply_schema_validation() parse failed: %s",
                error.message);
        return;
    }

    if (!mongoc_database_write_command_with_opts(
            database, cmd, NULL, &reply, &error))
        ogs_error("eir_apply_schema_validation() failed: %s", error.message);

    bson_destroy(&reply);
    bson_destroy(cmd);
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

        self.collection.eir = mongoc_client_get_collection(
            ogs_mongoc()->client, ogs_mongoc()->name, "eir");
        ogs_assert(self.collection.eir);

        eir_create_indexes(self.collection.eir);
        eir_apply_schema_validation(
                (mongoc_database_t *)ogs_mongoc()->database);
    }

    return OGS_OK;
}

void ogs_dbi_final(void)
{
    if (self.collection.subscriber) {
        mongoc_collection_destroy(self.collection.subscriber);
    }
    if (self.collection.eir) {
        mongoc_collection_destroy(self.collection.eir);
    }

#if MONGOC_CHECK_VERSION(1, 9, 0)
    if (self.stream) {
        mongoc_change_stream_destroy(self.stream);
    }
#endif

    ogs_mongoc_final();
}

int ogs_dbi_collection_watch_init(void)
{
#if MONGOC_CHECK_VERSION(1, 9, 0)
    bson_t empty = BSON_INITIALIZER;    
    const bson_t *err_doc;
    bson_error_t error;
    bson_t *options = BCON_NEW("fullDocument", "updateLookup");
   
    ogs_mongoc()->stream = mongoc_collection_watch(self.collection.subscriber,
        &empty, options);

    if (mongoc_change_stream_error_document(ogs_mongoc()->stream, &error,
            &err_doc)) {
        if (!bson_empty (err_doc)) {
            ogs_error("Change Stream Error.  Enable replica sets to "
                "enable database updates to be sent to MME.");
        } else {
            ogs_error("Client Error: %s\n", error.message);
        }
        return OGS_ERROR;
    } else {
        ogs_info("Change Streams are Enabled.");
    }

    return OGS_OK;
# else
    return OGS_ERROR;
#endif
}
