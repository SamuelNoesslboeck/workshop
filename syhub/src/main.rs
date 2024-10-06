use rocket::{get, launch, routes};
use rocket::fs::{FileServer, Options};
use rocket::response::stream::{Event, EventStream};
use rocket::tokio::time;
use rocket::tokio::time::Duration;

use rumqttc::{MqttOptions, AsyncClient, QoS};
use rumqttc::Event::Incoming;
use rumqttc::Packet::Publish;
use core::str;
use std::error::Error;

#[get("/events")]
fn stream() -> EventStream![] {
    EventStream! {
        let mut mqttoptions = MqttOptions::new("syhub", "hub.local", 1883);
        mqttoptions.set_keep_alive(Duration::from_secs(5));
        
        let (client, mut eventloop) = AsyncClient::new(mqttoptions, 10);
        client.subscribe("#", QoS::AtMostOnce).await.unwrap();
    
        loop {        
            if let Incoming(Publish(msg)) = eventloop.poll().await.unwrap() {
                yield Event::data(String::from_utf8(msg.payload.to_vec()).expect("Invalid UTF-8 data")).id(msg.topic);
            }
        }        
    }
}

#[get("/")]
fn index() -> &'static str {
    "Hello, world!"
}

#[launch]
fn rocket() -> _ {
    rocket::build()
        .mount("/", routes![index, stream])
        .mount("/test", FileServer::new("../www", Options::Index))

}