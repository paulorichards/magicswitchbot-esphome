  
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import ble_client
from esphome.const import (
    CONF_ID
)


DEPENDENCIES = ["ble_client"]

magic_switchbot_ns = cg.esphome_ns.namespace("magic_switchbot")
MagicSwitchbot = magic_switchbot_ns.class_(
    "MagicSwitchbot", ble_client.BLEClientNode
)

CONFIG_SCHEMA = ( 
    cv.Schema(
        {
            cv.COMPONENT_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(MagicSwitchbot),
                }
            )
            
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
 
