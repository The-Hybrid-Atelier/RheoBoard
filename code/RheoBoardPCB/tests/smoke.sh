#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
test_dir="$(cd "$(dirname "$0")" && pwd)"
bash "$test_dir/run-tests.sh"
bash "$test_dir/run-application-tests.sh"
RHEO_TEST_ALTERNATE=1 bash "$test_dir/run-application-tests.sh"
