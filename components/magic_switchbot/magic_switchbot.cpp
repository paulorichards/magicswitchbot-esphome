#include "magic_switchbot.h"
#include "esphome/core/log.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace magic_switchbot {

static const char *const TAG = "magic_switchbot";

void MagicSwitchbot::dump_config() {
  ESP_LOGCONFIG(TAG, "Magic Switchbot");
}


}

}  // namespace magic_switchbot
}  // namespace esphome

#endif