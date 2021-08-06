#include "magic_switchbot.h"
#include "esphome/core/log.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace magic_switchbot {

static const char *const TAG = "magic_switchbot";

void MagicSwitchbot::dump_config() {
  ESP_LOGCONFIG(TAG, "Magic Switchbot");
}

void MagicSwitchbot::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      if (param->open.status == ESP_GATT_OK) {
        ESP_LOGI(TAG, "[%s] Connected successfully!", this->get_name().c_str());
        break;
      }
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->battery_level_ = NAN;
      this->publish_state();
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto chr = this->parent_->get_characteristic(0xFEE7, 0x36F6);
      if (chr == nullptr) {
        ESP_LOGW(TAG, "[%s] No control service found at device, not an Anova..?", this->get_name().c_str());
        ESP_LOGW(TAG, "[%s] Note, this component does not currently support Anova Nano.", this->get_name().c_str());
        break;
      }
      ESP_LOGW(TAG, "[%s] Connected to something", this->get_name().c_str());
      this->char_handle_ = chr->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent_->gattc_if, this->parent_->remote_bda, chr->handle);
      if (status) {
        ESP_LOGW(TAG, "[%s] esp_ble_gattc_register_for_notify failed, status=%d", this->get_name().c_str(), status);
      }
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      ESP_LOGW(TAG, "[%s] ESP_GATTC_REG_FOR_NOTIFY_EVT", this->get_name().c_str());
      this->node_state = espbt::ClientState::Established;
      this->current_request_ = 0;
      ESP_LOGW(TAG, "[%s] ESP_GATTC_REG_FOR_NOTIFY_EVT", this->get_name().c_str());
      this->update();
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGW(TAG, "[%s] Notify event", this->get_name().c_str());
      
      if (param->notify.handle != this->char_handle_)
        break;
      ESP_LOGW(TAG, "[%s] Notify event", this->get_name().c_str());
      this->publish_state();

      break;
    }
    default:
      break;
  }
}

}  // namespace magic_switchbot
}  // namespace esphome

#endif