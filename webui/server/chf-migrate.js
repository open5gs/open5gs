/*
 * Bring chf.subscribers up to the bucket model.
 *
 * A subscriber used to have exactly one allowance, so the collection had a
 * unique index on supi.  A subscriber can now hold several buckets - one
 * per slice, DNN, home PLMN or roaming state - and they are told apart by
 * bucketId, so that uniqueness has to move.  Mongoose creates the new
 * indexes but never drops one it no longer declares, so the old unique
 * index would keep rejecting the second bucket with E11000.
 *
 * Runs once at start-up and does nothing on a collection that is already
 * in the new shape, so it is safe on every boot.  A failure is logged and
 * ignored: the WebUI still serves the buckets that exist.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const COLLECTION = 'chf.subscribers';

async function migrateChfBuckets(connection) {
  const db = connection && connection.db;
  if (!db) return;

  try {
    const names = await db.listCollections({ name: COLLECTION }).toArray();
    if (!names.length) return;      /* nothing provisioned yet */

    const collection = db.collection(COLLECTION);
    const indexes = await collection.indexes();

    /* The old unique index on supi: drop it, so a subscriber can hold
     * more than one bucket.  Mongoose recreates it as a plain index. */
    const supiUnique = indexes.find(
        ix => ix.unique && ix.key && ix.key.supi === 1 &&
              Object.keys(ix.key).length === 1);
    if (supiUnique) {
      await collection.dropIndex(supiUnique.name);
      console.log(
          `[chf] ${COLLECTION}: dropped the unique index on supi ` +
          '(a subscriber can now hold several buckets)');
    }

    /* Documents written before buckets existed: their id is their SUPI,
     * which is what makes them the subscriber's catch-all bucket. */
    const result = await collection.updateMany(
        { bucketId: { $exists: false } },
        [{ $set: { bucketId: '$supi' } }]);
    const changed = result.modifiedCount || 0;
    if (changed)
      console.log(`[chf] ${COLLECTION}: gave ${changed} bucket(s) an id`);
  } catch (err) {
    console.error(`[chf] ${COLLECTION} migration skipped: ${err.message}`);
  }
}

module.exports = { migrateChfBuckets };
