#include "magic_switchbot.h"
#include "esphome/core/log.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace magic_switchbot {

static const char *const TAG = "magic_switchbot";

void MagicSwitchBot::dump_config() {
  ESP_LOGCONFIG(TAG, "Magic SwitchBot");
  LOG_SENSOR("  ", "Battery Level", this->battery_level_);
}

bool MagicSwitchBot::parse_device(const esp32_ble_tracker::ESPBTDevice &device) {
  if (device.address_uint64() != this->address_) {
    ESP_LOGVV(TAG, "parse_device(): unknown MAC address.");
    return false;
  }
  ESP_LOGVV(TAG, "parse_device(): MAC address %s found.", device.address_str().c_str());

  bool success = false;
  for (auto &service_data : device.get_service_datas()) {

    success = true;
  }

  if (!success) {
    return false;
  }

  return true;
}


}  // namespace magic_switchbot
}  // namespace esphome

#endif