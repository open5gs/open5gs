process.env.DB_STORAGE = process.env.DB_STORAGE || '/tmp/nextepc.db';

const fs = require('fs');
const path = require('path');
const Sequelize = require('sequelize');
const sequelize = new Sequelize(null, null, null, {
  "dialect": "sqlite",
  "storage": process.env.DB_STORAGE
});

var db = {};

fs
  .readdirSync(__dirname)
  .filter(file => {
    return file.indexOf('.') !== 0 && file !== 'index.js';
  })
  .forEach(file => {
    const model = sequelize.import(path.join(__dirname, file));
    db[model.name] = model;
  });

Object.keys(db).forEach(modelName => {
  if ('associate' in db[modelName]) db[modelName].associate(db);
});

db.sequelize = sequelize;
db.Sequelize = Sequelize;

module.exports = db;