  
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, ble_client
from esphome.const import (
    CONF_ID, 
    CONF_BATTERY_LEVEL, 
    UNIT_PERCENT, 
    ICON_EMPTY, 
    DEVICE_CLASS_BATTERY, 
    STATE_CLASS_MEASUREMENT
)


DEPENDENCIES = ["ble_client"]

magic_switchbot_ns = cg.esphome_ns.namespace("magic_switchbot")
MagicSwitchBot = magic_switchbot_ns.class_(
    "MagicSwitchBot", sensor.Sensor, ble_client.BLEClientNode, cg.Component
)

CONFIG_SCHEMA = ( 
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MagicSwitchBot),
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                UNIT_PERCENT,
                ICON_EMPTY,
                0,
                DEVICE_CLASS_BATTERY,
                STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
 
    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level(sens))
