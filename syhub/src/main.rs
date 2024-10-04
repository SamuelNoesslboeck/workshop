use rocket::{get, launch, routes};
use rocket::response::stream::{Event, EventStream};
// use rocket::tokio::time;
use rocket::tokio::time::Duration;

use rumqttc::{MqttOptions, Client, QoS};

#[get("/events")]
fn stream() -> EventStream![] {
    EventStream! {
        let mut mqttoptions = MqttOptions::new("syhub", "hub.local", 1883);
        mqttoptions.set_keep_alive(Duration::from_secs(5));

        let (client, mut connection) = Client::new(mqttoptions, 10);
        client.subscribe("#", QoS::AtMostOnce).unwrap();

        loop {
            if let Ok(Ok(not)) = connection.try_recv() {
                println!("{:?}", not);
                yield Event::data(format!("{:?}", not));
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

}