use core::time::Duration;

use rumqttc::{Client, Connection, MqttOptions};

pub const PLC_ADDR : &str = env!("PLC_ADDR");
pub const PLC_MQTT_PORT : u16 = 1883;

pub struct WorkshopPLCClient {
    pub id : String,
    pub client : Client,
    pub connection : Connection
}

impl WorkshopPLCClient {
    pub fn new() -> Self {
        let id = sys_info::hostname().unwrap();     // TODO: Remove unwrap
        let mut mqttoptions = MqttOptions::new(id.clone(), PLC_ADDR, PLC_MQTT_PORT);
        mqttoptions.set_keep_alive(Duration::from_secs(5));
        
        let (client, connection) = Client::new(mqttoptions, 10);

        Self {
            id, 
            client, 
            connection
        }
    }
}