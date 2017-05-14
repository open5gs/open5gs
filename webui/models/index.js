process.env.DB_CONNECTION_STRING = process.env.DB_CONNECTION_STRING || 'sqlite:///tmp/nextepc.db';

const Sequelize = require('sequelize');
const sequelize = new Sequelize('db', null, null, {
        "dialect": "sqlite",
        "storage": "./db.development.sqlite"
});

function sync(...args) {
    return sequelize.sync(...args);
}

module.exports = { sync };