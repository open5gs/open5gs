/*
 * Settings the CHF publishes for the WebUI.
 *
 * Collection: chf.settings.  The CHF writes { _id: 'pool', maxMembers } at
 * start-up from charging.pool.max_members in chf.yaml (src/chf/db-handler.c,
 * chf_db_publish_settings), so the limit set there is enforced where members
 * are added.  Read-only from this side.
 *
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
 */

'use strict';

const mongoose = require('mongoose');

const ChfSettings = new mongoose.Schema(
  {
    _id: String,
    maxMembers: Number,
    publishedBy: String,
    updatedAt: Date,
  },
  {
    collection: 'chf.settings',
    versionKey: false,
  }
);

module.exports = mongoose.model('ChfSettings', ChfSettings);
