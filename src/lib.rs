use std::thread::JoinHandle;
use std::time::UNIX_EPOCH;

use rumqttc::{Client, MqttOptions};
use sysinfo::{CpuRefreshKind, RefreshKind, System};

// Constants
pub const CLIENT_ID_DEFAULT : &str = "rust_mqtt_client";
pub const PLC_DEFAULT_HOST : &str = "plc.local";

pub struct PLCMQTTConnection {
    id : String,
    client : Client,
    system : System,

    // Threads
    _thr : JoinHandle<()>,

    // Fields
    last_update : u128
}

impl PLCMQTTConnection {
    pub fn from_env() -> Self {
        let id = std::env::var("WORKSHOP_NET_CLIENT_ID").unwrap_or(String::from(CLIENT_ID_DEFAULT));

        let mqtt_options = MqttOptions::new(
            id.clone(), 
            std::env::var("PLC_ADDR").unwrap_or(String::from("plc.local")), 
            1883
        );

        let (client, mut connection) = Client::new(mqtt_options, 10);

        Self {
            id,
            client,
            system: System::new_with_specifics(RefreshKind::new().with_cpu(CpuRefreshKind::new())),

            _thr: std::thread::spawn(move || {
                for _ in connection.iter() {
                    // dbg!(node).unwrap();
                }
            }), 

            last_update: 0
        }
    }

    pub fn update_active(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        let stamp = std::time::SystemTime::now().duration_since(UNIX_EPOCH)?.as_millis();
        self.client.publish(
            format!("devices/{}/active", self.id), 
            rumqttc::QoS::AtLeastOnce, 
            true, 
            stamp.to_be_bytes()
        )?;
        self.last_update = stamp;
        Ok(())
    }

    pub fn update_cpu_usage(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        self.system.refresh_cpu();

        let count = self.system.cpus().len() as f32; 
        let mut sum = 0.0;

        for cpu in self.system.cpus() {
            sum += cpu.cpu_usage();
        }

        self.client.publish(
            format!("devices/{}/cpu_usage", self.id), 
            rumqttc::QoS::AtLeastOnce, 
            false, 
            (sum / count).to_string()
        )?;
        Ok(())
    }
}