/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


import express from 'express';
import { register } from './servers/ports.js';

let server;
let port;

function createServer(app) {
  return new Promise(r => {
    port = register('wasm');
    server = app.listen(port);
    server.on('error', err => {
      if (err.code === 'EADDRINUSE') {
        createServer(app).then(r);
        return;
      }

      throw new Error(err.code);
    });
    server.on('listening', r);
  });
}

export default {
  async start() {
    const app = express();

    const build_directory = process.env["WASM_BUILD_DIRECTORY"]
      ? process.env["WASM_BUILD_DIRECTORY"]
      : path.join(__dirname, '..', '..', 'build', 'wasm_build');

    if (!fs.existsSync(build_directory)) {
      throw new Error(`Provided build directory: ${path.format(build_directory)}  doesn't exist.`)
    }

    app.use((req, res, next) => {
      // Required headers to enable the SharedArrayBuffer API,
      // which in turn is a required API to run the WASM client.
      //
      // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SharedArrayBuffer#security_requirements
      res.setHeader('Cross-origin-Embedder-Policy', 'require-corp');
      res.setHeader('Cross-origin-Opener-Policy','same-origin');

      if (req.method === 'OPTIONS') {
        res.sendStatus(200)
      } else {
        next()
      }
    });

    app.use(express.static(build_directory));

    await createServer(app);
  },

  stop() {
    if (server) {
      server.close();
      server = null;
    }
  },

  get port() {
    return port;
  },

  get url() {
    return `http://localhost:${port}`;
  }
};

if (typeof require !== 'undefined' && require.main === module) {
  start();
  console.log(`Starting Mozilla VPN on ${url}.`)
}
