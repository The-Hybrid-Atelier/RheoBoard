// SPDX-License-Identifier: MIT
// Only the BLE radio/transport is replaced. OSC parsing/encoding uses the
// installed production libraries, including ThingPlusBLEOSC/OSCHandler.cpp.
#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H
#include <Arduino.h>
#include <memory>
class BLECharacteristic;
class BLECharacteristicCallbacks {
public:
  virtual ~BLECharacteristicCallbacks() = default;
  virtual void onWrite(BLECharacteristic *) {}
};
class BLECharacteristic {
  std::unique_ptr<BLECharacteristicCallbacks> callbacks;
  std::string value;
public:
  std::vector<std::string> notifications;
  void setCallbacks(BLECharacteristicCallbacks *cb) { callbacks.reset(cb); }
  std::string getValue() { return value; }
  void setValue(uint8_t *data, size_t n) { value.assign(reinterpret_cast<char *>(data), n); }
  void notify() { notifications.push_back(value); }
  void receive(const std::string &text) { value = text; callbacks->onWrite(this); }
};
struct BLEServer { void startAdvertising() {} };
inline BLECharacteristic fakeTx, fakeRx;
inline BLEServer fakeServer;
inline BLECharacteristic *pTxCharacteristic = &fakeTx, *pRxCharacteristic = &fakeRx;
inline BLEServer *pServer = &fakeServer;
inline bool deviceConnected = false, oldDeviceConnected = false;
inline void setupBLE(const char *, const char *, const char *, const char *) {}
#endif
