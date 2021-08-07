#include <cstring>

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

    this->current_request_ = 0;

}

void MagicSwitchbot::loop(){}

void MagicSwitchbot::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {

  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      if (param->open.status == ESP_GATT_OK) {
        ESP_LOGI(TAG, "Connected successfully!");
        break;
      }
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      ESP_LOGW(TAG, "Disconnected!");
      this->is_logged_in_ = false;
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::Established;
      this->current_request_ = 0;
      ESP_LOGI(TAG, "Registered for notification");
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {      
      auto chr = this->parent_->get_characteristic(MAGIC_SWITCHBOT_SERVICE_UUID, MAGIC_SWITCHBOT_CHARACTERISTIC_READ_UUID);
      if (chr == nullptr) {
        ESP_LOGI(TAG, "No control service found at device, not an Magic Switchbot?");
        break;
      }
      if (this->is_logged_in_ == false){
        this->login();
        auto status = esp_ble_gattc_register_for_notify(this->parent_->gattc_if, this->parent_->remote_bda, chr->handle);
        if (status) {
          ESP_LOGI(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
        }
        this->check_battery();

      }
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGI(TAG, "Got Notification event");
      this->decode(param->notify.value, param->notify.value_len, this->notification_);
      for (int i = 0; i < 16; i++)
        ESP_LOGI(TAG, "Notification data %02X", this->notification_[i]);
      break; 
    }
  }
}


void MagicSwitchbot::login(){
  //TODO: Password support
  unsigned char command[16];
  unsigned char iv[16];
  unsigned char output[16];

  std::memcpy(command, LOGIN_COMMAND, COMMAND_SIZE);
  mbedtls_aes_crypt_cbc( &aes_context_, MBEDTLS_AES_ENCRYPT, 16, iv, command, output );
  auto chr = this->parent_->get_characteristic(MAGIC_SWITCHBOT_SERVICE_UUID, MAGIC_SWITCHBOT_CHARACTERISTIC_WRITE_UUID);
  auto status = esp_ble_gattc_write_char(this->parent_->gattc_if, this->parent_->conn_id, chr->handle, COMMAND_SIZE, output, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  
  if (status) {
    ESP_LOGI(TAG, "esp_ble_gattc_write_char failed, status=%d",  status);
    this->is_logged_in_ = false;
  }
  else {
    ESP_LOGI(TAG, "session established");
    this->is_logged_in_ = true;
    std::memcpy(output + 3, this->token_, 4);
  }
}

void MagicSwitchbot::check_battery(){
  ESP_LOGI(TAG, "Checking battery");

  unsigned char command[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };;
  unsigned char iv[16];
  unsigned char output[16];

  std::memcpy(command, CHECK_BATTERY_COMMAND, 4);
  std::memcpy(command + 4, CHECK_BATTERY_COMMAND, 4);

  mbedtls_aes_crypt_cbc( &aes_context_, MBEDTLS_AES_ENCRYPT, 16, iv, command, output );
  auto chr = this->parent_->get_characteristic(MAGIC_SWITCHBOT_SERVICE_UUID, MAGIC_SWITCHBOT_CHARACTERISTIC_WRITE_UUID);
  auto status = esp_ble_gattc_write_char(this->parent_->gattc_if, this->parent_->conn_id, chr->handle, COMMAND_SIZE, output, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  
  if (status) {
    ESP_LOGI(TAG, "esp_ble_gattc_write_char failed, status=%d",  status);  
  }

  
   chr = this->parent_->get_characteristic(MAGIC_SWITCHBOT_SERVICE_UUID, MAGIC_SWITCHBOT_CHARACTERISTIC_READ_UUID);
   status = esp_ble_gattc_read_char(this->parent_->gattc_if, this->parent_->conn_id, chr->handle, ESP_GATT_AUTH_REQ_NONE);

ESP_LOGI(TAG, "esp_ble_gattc_read_char Reading char");

  if (status) {
    ESP_LOGI(TAG, "esp_ble_gattc_read_char failed, status=%d",  status);  
  }
  
}

void MagicSwitchbot::decode(uint8_t *value, uint16_t length, uint8_t *output ){
  uint8_t iv[16];
  mbedtls_aes_crypt_cbc( &aes_context_, MBEDTLS_AES_DECRYPT, length, iv, value, output );
  
}

}  // namespace magic_switchbot
}  // namespace esphome

#endif