// SPDX-License-Identifier: MIT
#pragma once
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <sstream>
#include <cstring>
#include <cmath>
#include <mutex>
#include "Print.h"
using String = std::string;
using byte = uint8_t;
using boolean = bool;
using std::isfinite;
inline unsigned long fakeMillis = 0;
inline unsigned long millis() { return fakeMillis; }
inline void delay(unsigned long ms) { fakeMillis += ms; }
template<class T> T constrain(T v, T low, T high) { return std::min(high, std::max(low, v)); }
constexpr int HIGH = 1, LOW = 0, OUTPUT = 1, INPUT_PULLUP = 2, OUTPUT_OPEN_DRAIN = 3, BIN = 2;
inline std::array<int, 64> inputLevels = [] { std::array<int, 64> a{}; a.fill(HIGH); return a; }();
inline int digitalRead(int pin) { return inputLevels.at(pin); }
inline int analogRead(int) { return 0; }
inline void noInterrupts() {}
inline void interrupts() {}
using portMUX_TYPE = std::mutex;
#define portMUX_INITIALIZER_UNLOCKED {}
inline void portENTER_CRITICAL(portMUX_TYPE *mux) { mux->lock(); }
inline void portEXIT_CRITICAL(portMUX_TYPE *mux) { mux->unlock(); }
struct PinEvent { int pin, value; bool mode; };
inline std::vector<PinEvent> pinEvents;
inline void digitalWrite(int pin, int value) { pinEvents.push_back({pin, value, false}); }
inline void pinMode(int pin, int value) { pinEvents.push_back({pin, value, true}); }
struct FakeSerial {
  std::vector<std::string> messages;
  std::deque<char> input;
  std::string partial;
  void begin(int) {}
  int available() { return input.size(); }
  int read() { char c = input.front(); input.pop_front(); return c; }
  void inject(const std::string &s) { input.insert(input.end(), s.begin(), s.end()); }
  template<class T> void print(const T &value) { std::ostringstream s; s << value; partial += s.str(); }
  void println() { messages.push_back(partial); partial.clear(); }
  template<class T> void println(const T &value) { print(value); println(); }
  template<class T> void println(const T &value, int) { println(value); }
};
inline FakeSerial Serial;
