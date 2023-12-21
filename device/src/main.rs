use rumqttc::QoS;
use core::time::Duration;
use std::thread;

fn main() {
    let mut client = WorkshopPLCClient::new();

    thread::spawn(move || loop {
        client.client.publish(format!("/device/{}", client.id), QoS::AtLeastOnce, false, "true").unwrap();
        thread::sleep(Duration::from_millis(5000));
    });
    
    // Iterate to poll the eventloop for connection progress
    for (_, notification) in client.connection.iter().enumerate() {
        println!("Notification = {:?}", notification);
    }
}