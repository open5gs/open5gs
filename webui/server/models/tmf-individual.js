/*
 * TMF632 Individual — persistence for the TM Forum Party Management API.
 *
 * Collection: tmf.individuals.  A BSS record only; the CHF NF never reads it.
 * Attribute validation lives in server/tmf/party.js; the schema is
 * deliberately permissive (strict: false) so the optional TMF632 structures
 * (contactMedium, externalReference, ...) persist as sent.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');

const Individual = new mongoose.Schema(
  {
    id: { type: String, required: true, unique: true },
    givenName: { type: String, index: true },
    familyName: { type: String, index: true },
  },
  {
    collection: 'tmf.individuals',
    strict: false,
    id: false,          /* 'id' is a real attribute, not Mongoose's virtual */
    versionKey: false,
  }
);

module.exports = mongoose.model('TmfIndividual', Individual);
