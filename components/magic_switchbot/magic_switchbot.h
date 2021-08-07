#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#ifdef ARDUINO_ARCH_ESP32
#include <esp_gattc_api.h>

namespace esphome {
namespace magic_switchbot {

class MagicSwitchbot : public Component, public esphome::ble_client::BLEClientNode {
 public:
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
 protected:
};

}  // namespace magic_switchbot
}  // namespace esphome

#endif