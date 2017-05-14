const express = require('express');
const next = require('next');

const dev = process.env.NODE_ENV != 'production';
const app = next({ dev });
const handle = app.getRequestHandler();

const models = require('./models');

app.prepare()
.then(() => {
    const server = express();

    models.sequelize.sync()
    .then(() => {
        server.get('*', (req, res) => {
            return handle(req, res);
        });

        server.listen(3000, err => {
            if (err) throw err;
            console.log('> Ready on http://localhost:3000');
        });
    })
})
.catch(err => console.log(err));
