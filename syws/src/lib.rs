use core::time::Duration;

use ftp::FtpStream;
use rumqttc::{Client, Connection, MqttOptions};

pub type Error = Box<dyn std::error::Error>;

pub const PLC_USERNAME : &str = include_str!("../../assets/username.key");
pub const PLC_PASSWORD : &str = include_str!("../../assets/password.key");

pub const PLC_ADDR : &str = env!("PLC_ADDR");
pub const PLC_FTP_PORT : u16 = 21;
pub const PLC_MQTT_PORT : u16 = 1883;

pub struct PlcMqttClient {
    pub id : String,
    pub client : Client,
    pub connection : Connection
}

impl PlcMqttClient {
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

pub struct PlcFtpClient {
    stream : FtpStream
}

impl PlcFtpClient {
    pub fn new() -> Result<Self, crate::Error> {
        let mut stream = FtpStream::connect(format!("{}:{}", PLC_ADDR, PLC_FTP_PORT))?;
        stream.login(PLC_USERNAME, PLC_PASSWORD)?;

        Ok(Self { 
            stream
        })
    }

    pub fn pwd(&mut self) -> Result<String, crate::Error> {
        self.stream.pwd().map_err(|err| err.into())
    }
}