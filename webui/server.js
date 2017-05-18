const express = require('express');
const next = require('next');
const models = require('./models');
const auth = require('./routes/auth');

const dev = process.env.NODE_ENV != 'production';
const app = next({ dev });
const handle = app.getRequestHandler();

app.prepare()
.then(() => {
  const server = express();

  models.sequelize.sync()
  .then(() => {
    auth.configure({
      app: app,
      server: server,
      secret: process.env.SESSION_SECRET
    });

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
