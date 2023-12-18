use rumqttc::{MqttOptions, Client, QoS};
use std::time::Duration;

mod syws;
use syws::*;

fn main() {
    let mut mqttoptions = MqttOptions::new("test", PLC_ADDR, 1883);
    mqttoptions.set_keep_alive(Duration::from_secs(5));
    
    let (mut client, mut connection) = Client::new(mqttoptions, 10);
    client.publish("/test/test", QoS::AtMostOnce, false, "true").unwrap();
    
    // Iterate to poll the eventloop for connection progress
    for (_, notification) in connection.iter().enumerate() {
        println!("Notification = {:?}", notification);
    }
}
