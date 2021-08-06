#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#ifdef ARDUINO_ARCH_ESP32
#include <esp_gattc_api.h>

namespace esphome {
namespace magic_switchbot {

class MagicSwitchbot : public esphome::ble_client::BLEClientNode {
 public:
  void setup() override;
  void loop() override;
  void update() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
 protected:
};

}  // namespace magic_switchbot
}  // namespace esphome

#endif