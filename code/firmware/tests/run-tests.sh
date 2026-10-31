#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
test_dir="$(cd "$(dirname "$0")" && pwd)"
test_build="$(mktemp -d "${TMPDIR:-/tmp}/rheo-v1-tests.XXXXXX")"
trap 'rm -rf "$test_build"' EXIT
for variant in default alternate; do
  flags=(-DRHEO_HARDWARE_PROFILE=1)
  if [[ "$variant" == alternate ]]; then
    flags+=(-DRHEO_V1_VACUUM_CHANNEL=0 -DRHEO_V1_VALVE_PORT=0 -DRHEO_V1_SENSOR_RESET_PIN=21)
  fi
  "${CXX:-c++}" -std=c++17 -Wall -Wextra -Werror -fsanitize=address,undefined \
    "${flags[@]}" -I"$test_dir/fakes" -I"$test_dir/../RheoBoardOTS" \
    "$test_dir/v1_hardware_test.cpp" "$test_dir/../RheoBoardOTS/V1Hardware.cpp" \
    -o "$test_build/$variant"
  "$test_build/$variant"
done
