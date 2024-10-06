use rocket::{get, launch, routes};
use rocket::fs::{FileServer, NamedFile, Options};
use rocket::response::stream::{Event, EventStream};
use rocket::tokio::time::Duration;

use rumqttc::{MqttOptions, AsyncClient, QoS};
use rumqttc::Event::Incoming;
use rumqttc::Packet::Publish;

#[get("/events")]
pub fn stream() -> EventStream![] {
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
pub async fn index() -> Option<NamedFile> {
    NamedFile::open("www/index.html").await.ok()
}

#[launch]
fn rocket() -> _ {
    rocket::build()
        .mount("/", routes![index, stream])
        .mount("/", FileServer::new("./www", Options::Index))

}