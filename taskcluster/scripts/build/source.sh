#!/bin/bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

if [[ -n "$PULL_REQUEST_NUMBER" ]]; then
    GITREF="refs/pull/${PULL_REQUEST_NUMBER}/merge"
elif [[ "$MOZILLAVPN_HEAD_REF" =~ ^refs/heads/ ]]; then
    GITREF="$MOZILLAVPN_HEAD_REF"
elif [[ "$MOZILLAVPN_HEAD_REF" =~ ^releases.([0-9][^/]*) ]]; then
    GITREF="refs/heads/releases/${BASH_REMATCH[1]}"
else
    GITREF="refs/heads/${MOZILLAVPN_HEAD_REF}"
fi

# Extracted the vendored cargo dependencies ontop.
CARGO_OPTIONS=""
if [[ -d ${MOZ_FETCHES_DIR}/cargo-deps ]]; then
   CARGO_OPTIONS="--cargo-deps ${MOZ_FETCHES_DIR}/cargo-deps"
fi

./scripts/linux/script.sh -g ${GITREF} ${CARGO_OPTIONS}
