#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
set -euo pipefail
test_dir="$(cd "$(dirname "$0")" && pwd)"
library_dir="${ARDUINO_LIBRARIES:-$HOME/Documents/Arduino/libraries}"
test_build="$(mktemp -d "${TMPDIR:-/tmp}/rheo-app-tests.XXXXXX")"
trap 'rm -rf "$test_build"' EXIT
for library_file in OSC/OSCMessage.cpp ThingPlusBLEOSC/src/OSCHandler.cpp; do
  [[ -f "$library_dir/$library_file" ]] || { echo "Missing $library_file; set ARDUINO_LIBRARIES." >&2; exit 1; }
done
# Compile the production OSC library and the installed ThingPlus parser.
# Pre-including the BLE stub suppresses only the real radio header.
profile_flags=(-DRHEO_HARDWARE_PROFILE=1)
if [[ "${RHEO_TEST_ALTERNATE:-0}" == 1 ]]; then
  profile_flags+=(-DRHEO_V1_VACUUM_CHANNEL=0 -DRHEO_V1_VALVE_PORT=0 -DRHEO_V1_SENSOR_RESET_PIN=21)
fi
"${CC:-cc}" -c "$library_dir/OSC/OSCMatch.c" -o "$test_build/match.o"
"${CXX:-c++}" -std=c++17 -g -fsanitize=address,undefined,float-cast-overflow \
  -DESP32 -include "$test_dir/fakes/BLEHandler.h" \
  "${profile_flags[@]}" \
  -include "$library_dir/ThingPlusBLEOSC/src/MemoryStream.h" \
  -I"$test_dir/fakes" -I"$test_dir/../RheoBoardOTS" \
  -I"$library_dir/OSC" -I"$library_dir/ThingPlusBLEOSC/src" \
  "$test_dir/v1_application_test.cpp" \
  "$test_dir/../RheoBoardOTS/V1Hardware.cpp" \
  "$test_dir/../RheoBoardOTS/PneumaticSystem.cpp" \
  "$test_dir/../RheoBoardOTS/RheoSystem.cpp" \
  "$test_dir/../RheoBoardOTS/RoutinePlayer.cpp" \
  "$library_dir/OSC/OSCMessage.cpp" "$library_dir/OSC/OSCData.cpp" \
  "$library_dir/OSC/OSCTiming.cpp" "$library_dir/ThingPlusBLEOSC/src/OSCHandler.cpp" \
  "$test_build/match.o" -o "$test_build/application"
failed=0
if [[ $# == 0 ]]; then
  set -- serial rep system_stop pump_zero disconnect program triad_stop sensor_fault bubble_fault button button_stop stop_matrix queue_stop program_complete
fi
for scenario in "$@"; do
  if ! UBSAN_OPTIONS=halt_on_error=1 "$test_build/application" "$scenario"; then failed=1; fi
done
exit "$failed"
