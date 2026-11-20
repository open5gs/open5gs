/*
 * Persistence for the TMF routers, behind one small interface.
 *
 * The routers never talk to Mongoose directly.  That keeps the HTTP layer
 * testable end to end without a database (tests inject memoryRepo), and it
 * confines the storage details - including the atomic, conditional updates
 * the balance top-up depends on - to one place.
 *
 * Interface (all methods async, all documents plain objects):
 *   list(filter, offset, limit)   -> { items, total }
 *   get(key)                      -> doc | null
 *   exists(filter)                -> boolean
 *   create(doc)                   -> doc
 *   replace(key, doc)             -> doc | null     whole-document replace
 *   update(filter, update)        -> number matched  ($set / $inc / $unset)
 *   remove(key)                   -> boolean
 *
 * `filter` is an equality map on (possibly dotted) paths; a null value
 * matches a missing or null field, as in MongoDB.  A path that ends at an
 * array of scalars matches when any element equals the value, or when the
 * whole array equals an array value.  `$or: [filter, ...]` matches when any
 * of its filters does.  An update may use MongoDB's positional operator,
 * `{$set: {'members.$': x}}`, to replace the element the filter matched.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

/* ------------------------------------------------------------------ */
/* MongoDB (Mongoose)                                                   */
/* ------------------------------------------------------------------ */

function mongooseRepo(Model, keyField) {
  const byKey = key => ({ [keyField]: key });
  const strip = doc => {
    if (!doc) return doc;
    const out = Object.assign({}, doc);
    delete out._id;
    delete out.__v;
    return out;
  };

  return {
    keyField,

    async list(filter, offset, limit) {
      const [items, total] = await Promise.all([
        Model.find(filter).sort({ [keyField]: 1 })
          .skip(offset).limit(limit).lean(),
        Model.countDocuments(filter),
      ]);
      return { items: items.map(strip), total };
    },

    async get(key) {
      return strip(await Model.findOne(byKey(key)).lean());
    },

    async exists(filter) {
      return (await Model.countDocuments(filter).limit(1)) > 0;
    },

    async create(doc) {
      const created = await Model.create(doc);
      return strip(created.toObject());
    },

    async replace(key, doc) {
      return strip(await Model.findOneAndReplace(byKey(key), doc,
          { new: true, runValidators: true }).lean());
    },

    async update(filter, update) {
      const res = await Model.updateOne(filter, update);
      /* Mongoose 5 reports n; newer drivers report matchedCount. */
      return res.matchedCount != null ? res.matchedCount : (res.n || 0);
    },

    async remove(key) {
      const res = await Model.deleteOne(byKey(key));
      return (res.deletedCount || 0) > 0;
    },
  };
}

/* ------------------------------------------------------------------ */
/* In-memory (tests)                                                    */
/* ------------------------------------------------------------------ */

function getPath(obj, path) {
  return path.split('.').reduce(
    (o, k) => (o == null ? undefined : o[k]), obj);
}

function setPath(obj, path, value) {
  const keys = path.split('.');
  let o = obj;
  keys.slice(0, -1).forEach(k => {
    if (o[k] == null || typeof o[k] !== 'object') o[k] = {};
    o = o[k];
  });
  o[keys[keys.length - 1]] = value;
}

function unsetPath(obj, path) {
  const keys = path.split('.');
  const parent = keys.slice(0, -1).reduce(
    (o, k) => (o == null ? undefined : o[k]), obj);
  if (parent) delete parent[keys[keys.length - 1]];
}

function sameValue(a, b) {
  if (a instanceof Date || b instanceof Date)
    return new Date(a).getTime() === new Date(b).getTime();
  /* An array value matches an array field only if they are equal, element
   * for element and in order, as in MongoDB. */
  if (Array.isArray(a) || Array.isArray(b))
    return Array.isArray(a) && Array.isArray(b) &&
        JSON.stringify(a) === JSON.stringify(b);
  return a === b;
}

/*
 * Every value a dotted path can reach, descending into arrays the way a
 * MongoDB query does: {'relatedParty.id': x} matches when ANY element of the
 * relatedParty array has id x.  Without this the fake would silently never
 * match array paths, and tests of the delete guards would pass vacuously.
 */
function valuesAt(value, keys) {
  if (!keys.length) return Array.isArray(value) ? value.concat([value]) : [value];
  if (Array.isArray(value))
    return value.reduce((acc, el) => acc.concat(valuesAt(el, keys)), []);
  if (value == null || typeof value !== 'object') return [undefined];
  return valuesAt(value[keys[0]], keys.slice(1));
}

function matches(doc, filter) {
  return Object.keys(filter).every(path => {
    if (path === '$or') return filter.$or.some(f => matches(doc, f));
    const want = filter[path];
    const haves = valuesAt(doc, path.split('.'));
    if (want === null) return haves.every(h => h == null);
    return haves.some(h => sameValue(h, want));
  });
}

const clone = v => (v === undefined ? v : JSON.parse(JSON.stringify(v)));

/*
 * `unique` lists further paths with a unique index.  As in MongoDB, a
 * document without the field is not indexed (a sparse index), and for an
 * array every element has to be unique across documents.
 */
function memoryRepo(keyField, seed, unique) {
  const docs = (seed || []).map(clone);
  const taken = (doc, path) => valuesAt(doc, path.split('.'))
    .filter(v => v != null && !Array.isArray(v));
  const collides = doc => (unique || []).some(path => {
    const mine = taken(doc, path);
    return mine.length && docs.some(d =>
      taken(d, path).some(v => mine.indexOf(v) !== -1));
  });

  return {
    keyField,
    docs, /* exposed for test assertions */

    async list(filter, offset, limit) {
      const hit = docs.filter(d => matches(d, filter))
        .sort((a, b) => String(a[keyField]).localeCompare(String(b[keyField])));
      return { items: hit.slice(offset, offset + limit).map(clone),
               total: hit.length };
    },

    async get(key) {
      return clone(docs.find(d => d[keyField] === key) || null);
    },

    async exists(filter) {
      return docs.some(d => matches(d, filter));
    },

    async create(doc) {
      if (docs.some(d => d[keyField] === doc[keyField]) || collides(doc)) {
        const err = new Error('duplicate key');
        err.code = 11000;
        throw err;
      }
      docs.push(clone(doc));
      return clone(doc);
    },

    async replace(key, doc) {
      const i = docs.findIndex(d => d[keyField] === key);
      if (i < 0) return null;
      docs[i] = clone(doc);
      return clone(docs[i]);
    },

    async update(filter, update) {
      const i = docs.findIndex(d => matches(d, filter));
      if (i < 0) return 0;
      /* Applied to a copy first, so a write that would break a unique
       * index changes nothing, as in MongoDB. */
      const doc = clone(docs[i]);
      Object.keys(update.$set || {}).forEach(p => {
        if (p.endsWith('.$')) {
          /* The positional operator: the element of that array the filter
           * matched on. */
          const field = p.slice(0, -2);
          const arr = getPath(doc, field);
          const at = Array.isArray(arr) ? arr.indexOf(filter[field]) : -1;
          if (at < 0) throw new Error(`positional update on ${field} did not match`);
          arr[at] = clone(update.$set[p]);
        } else {
          setPath(doc, p, clone(update.$set[p]));
        }
      });
      Object.keys(update.$inc || {}).forEach(
        p => setPath(doc, p, (Number(getPath(doc, p)) || 0) + update.$inc[p]));
      Object.keys(update.$unset || {}).forEach(p => unsetPath(doc, p));
      const others = docs.filter((d, k) => k !== i);
      if ((unique || []).some(path => {
        const mine = taken(doc, path);
        return mine.length && others.some(d =>
          taken(d, path).some(v => mine.indexOf(v) !== -1));
      })) {
        const err = new Error('duplicate key');
        err.code = 11000;
        throw err;
      }
      /* In place: tests hold references to stored documents. */
      Object.keys(docs[i]).forEach(k => delete docs[i][k]);
      Object.assign(docs[i], doc);
      return 1;
    },

    async remove(key) {
      const i = docs.findIndex(d => d[keyField] === key);
      if (i < 0) return false;
      docs.splice(i, 1);
      return true;
    },
  };
}

module.exports = { mongooseRepo, memoryRepo, getPath };
