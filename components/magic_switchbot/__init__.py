  
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, esp32_ble_tracker
from esphome.const import (
    CONF_ID, 
    CONF_MAC_ADDRESS, 
    CONF_BATTERY_LEVEL, 
    UNIT_PERCENT, 
    ICON_EMPTY, 
    DEVICE_CLASS_BATTERY, 
    STATE_CLASS_MEASUREMENT
)


DEPENDENCIES = ["esp32_ble_tracker"]

magic_switchbot_ns = cg.esphome_ns.namespace("magic_switchbot")
MagicSwitchBot = magic_switchbot_ns.class_(
    "MagicSwitchBot", esp32_ble_tracker.ESPBTDeviceListener, cg.Component
)

CONFIG_SCHEMA = ( 
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MagicSwitchBot),
            cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                UNIT_PERCENT,
                ICON_EMPTY,
                0,
                DEVICE_CLASS_BATTERY,
                STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await esp32_ble_tracker.register_ble_device(var, config)

    cg.add(var.set_address(config[CONF_MAC_ADDRESS].as_hex))

    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level(sens))
