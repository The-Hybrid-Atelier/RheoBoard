#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
sketch_dir="$(cd "$(dirname "$0")" && pwd)"
command -v arduino-cli >/dev/null || { echo 'Install arduino-cli first.' >&2; exit 1; }
# Uses the installed libraries; never replaces a local library automatically.
arduino-cli compile --fqbn esp32:esp32:esp32thing_plus "$sketch_dir" "$@"
