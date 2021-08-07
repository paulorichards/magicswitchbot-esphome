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

static const uint16_t COMMAND_SIZE = 16;

static const uint16_t MAGIC_SWITCHBOT_SERVICE_UUID = 0xFEE7;
static const uint16_t MAGIC_SWITCHBOT_CHARACTERISTIC_WRITE_UUID = 0x36F5;
static const uint16_t MAGIC_SWITCHBOT_CHARACTERISTIC_READ_UUID = 0x36F6;
static const uint8_t KEY[]	         = { 42, 97, 57, 92, 64, 85, 73, 81, 58, 90, 75, 98, 27, 109, 55, 53 };
static const uint8_t LOGIN_COMMAND[] = { 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t CHECK_BATTERY_COMMAND[] = { 0x02, 0x01, 0x01, 0x01};
static const uint8_t PUSH_BUTTON_COMMAND[] = { 0x05, 0x01, 0x01, 0x02};


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

    uint8_t token_[4];
    uint8_t notification_[16];
    bool is_logged_in_;
    void login();
    void check_battery();
    void push_button();
    void decode(uint8_t *value, uint16_t length, uint8_t *output );
};

}  // namespace magic_switchbot
}  // namespace esphome

#endif