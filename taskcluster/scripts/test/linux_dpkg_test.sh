#!/bin/bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set -e

# Install the mozillavpn.deb package
sudo apt-get update
sudo apt install -y $MOZ_FETCHES_DIR/mozillavpn.deb

echo "Installed version"
mozillavpn --version

# Install npm and prepare to run functional tests.
# TODO: This path manipulation should be Node.js version agnostic.
export PATH=${PATH}:${MOZ_FETCHES_DIR}/node-v20.17.0-linux-x64/bin
export MVPN_ADDONS_PATH=${MOZ_FETCHES_DIR}/addons
export MVPN_BIN=$(which mozillavpn)
(cd $VCS_PATH && npm install)

# Let's run a test
if [ $# -ge 1 ]; then
  (cd $VCS_PATH && xvfb-run -a npm run functionalTest -- --retries 3 $1)
fi
