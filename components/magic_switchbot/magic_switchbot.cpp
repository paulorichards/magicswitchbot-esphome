#include "magic_switchbot.h"
#include "esphome/core/log.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace magic_switchbot {

static const char *const TAG = "magic_switchbot";

void MagicSwitchbot::dump_config() {
  ESP_LOGCONFIG(TAG, "Magic Switchbot");
}

void MagicSwitchbot::setup(){
  
  mbedtls_aes_init(&aes_context_);
	
  mbedtls_aes_setkey_enc(&aes_context_, KEY, 128);
}

void MagicSwitchbot::loop(){}

void MagicSwitchbot::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    ESP_LOGW(TAG, "Event recived");
  }
}

}  // namespace magic_switchbot
}  // namespace esphome

#endif