//! # Services
//! 
//! Services and devices watchdog for the workshop
use base64::Engine;
use base64::engine::{GeneralPurpose, GeneralPurposeConfig};
use mqtt::Message;
use reqwest::blocking::Client;
use reqwest::header::AUTHORIZATION;

use paho_mqtt as mqtt;

// Submodules
mod data;
use data::*;

// Addresses 
const ROUTER_DHCP_TABLE_ADDR : &str = "http://192.168.1.1/DHCPTable.asp";
const BROKER : &str = "mqtt://plc.local:1883";

// Data
const ROUTER_AUTH : &str = include_str!("../router_basic_auth.key");
const MQTT_CLIENT_NAME : &str = "plc";

fn main() {
    // Encoding and parsing
        let engine = GeneralPurpose::new(
            &base64::alphabet::STANDARD, 
            GeneralPurposeConfig::new()
        );

        let pattern = regex::Regex::new(r"'(.*?)'").unwrap();
    // 

    // Connections
        let client = Client::new();

        let client_ops = mqtt::CreateOptionsBuilder::new()
            .server_uri(BROKER)
            .client_id(MQTT_CLIENT_NAME)
            .finalize();

        let connect_ops = mqtt::ConnectOptionsBuilder::new()
            .keep_alive_interval(core::time::Duration::from_secs(20))
            .clean_session(true)
            .finalize();

        let mclient = mqtt::Client::new(client_ops).unwrap();
        mclient.connect(connect_ops).unwrap();
    // 

    let req = client.get(ROUTER_DHCP_TABLE_ADDR)
        .header(AUTHORIZATION, format!("Basic {}", engine.encode(ROUTER_AUTH)))
        .send()
        .unwrap();
        
    if req.status().is_success() {
        let text = req.text().unwrap();

        // Getting the array out of the java code
        let array_part = text
            .split_once("var table = new Array(").unwrap().1
            .split_once(");\nvar i = 0;").unwrap().0;

        let mut rclients = vec![];
        let mut matches = vec![];

        for (_, [val]) in pattern.captures_iter(array_part).map(|c| c.extract()) {
            matches.push(String::from(val));

            // Each client has 5 strings defining them
            if matches.len() == 5 {
                rclients.push(RouterClient::from_vec(matches).unwrap());
                matches = vec![];
            }
        }

        dbg!(rclients);

        // Publish to broker
        mclient.publish(Message::new_retained("", payload, qos))
    }
}
