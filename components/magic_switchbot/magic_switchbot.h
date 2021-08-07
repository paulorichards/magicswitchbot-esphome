#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "mbedtls/aes.h"

#ifdef ARDUINO_ARCH_ESP32
#include <esp_gattc_api.h>


namespace esphome {
namespace magic_switchbot {

namespace espbt = esphome::esp32_ble_tracker;


static const uint16_t MAGIC_SWITCHBOT_SERVICE_UUID = 0xFEE7;
static const uint16_t MAGIC_SWITCHBOT_CHARACTERISTIC_WRITE_UUID = 0x36F5;
static const uint16_t MAGIC_SWITCHBOT_CHARACTERISTIC_READ_UUID = 0x36F6;
static const unsigned char KEY[]	= { 0x2A, 0x61, 0x39, 0x5C, 0x40, 0x55, 0x49, 0x51, 0x3A, 0x5A, 0x4B, 0x62, 0x1B, 0x6D, 0x37, 0x35 };


class MagicSwitchbot : public Component, public esphome::ble_client::BLEClientNode {
 public:
  void dump_config() override;
  void setup() override;
  void loop() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
 private:
    uint16_t char_handle_;
    uint8_t current_request_;
    mbedtls_aes_context aes_context_;
    unsigned char token_[4];

    void get_token();
};

}  // namespace magic_switchbot
}  // namespace esphome

#endif