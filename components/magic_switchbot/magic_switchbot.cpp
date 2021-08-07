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
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::Established;
      this->current_request_ = 0;
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto chr = this->parent_->get_characteristic(MAGIC_SWITCHBOT_SERVICE_UUID, MAGIC_SWITCHBOT_CHARACTERISTIC_READ_UUID);
      if (chr == nullptr) {
        ESP_LOGW(TAG, "[%s] No control service found at device, not an Anova..?", this->get_name().c_str());
        ESP_LOGW(TAG, "[%s] Note, this component does not currently support Anova Nano.", this->get_name().c_str());
        break;
      }
      this->char_handle_ = chr->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent_->gattc_if, this->parent_->remote_bda, chr->handle);
      if (status) {
        ESP_LOGW(TAG, "[%s] esp_ble_gattc_register_for_notify failed, status=%d", this->get_name().c_str(), status);
      }
      break;
    }
  }
}

void MagicSwitchbot::get_token(){
  unsigned char command[16] = {0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  unsigned char output[16];

  mbedtls_aes_crypt_cbc( &aes_context_, MBEDTLS_AES_ENCRYPT, command, &output );
  
}

}  // namespace magic_switchbot
}  // namespace esphome

#endif