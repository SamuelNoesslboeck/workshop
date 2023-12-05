use std::borrow::Cow;
use paho_mqtt as mqtt;

// Submodules
    mod workshop;
    pub use workshop::Workshop;
// 

const BROKER : &str = "mqtt://localhost:1883";
const CLIENT_NAME : &str = "plc";

fn handle_msg(ws : &mut Workshop, topic : &str, pl : Cow<'_, str>) {
    match topic {
        "ws/light/chain_main" => {
            if let Ok(b) = pl.parse::<bool>() {
                ws.light_chain_main.set_state(b).unwrap()
            }
        },
        _ => { }
    }
}

fn main() {
    // Create workshop 
        let mut ws = Workshop::new().unwrap();
    // 

    // MQTT Connection
        let client_ops = mqtt::CreateOptionsBuilder::new()
            .server_uri(BROKER)
            .client_id(CLIENT_NAME)
            .finalize();

        let connect_ops = mqtt::ConnectOptionsBuilder::new()
            .keep_alive_interval(core::time::Duration::from_secs(20))
            .clean_session(true)
            .finalize();

        let client = mqtt::Client::new(client_ops).unwrap();
        client.connect(connect_ops).unwrap();
    // 

    let rx = client.start_consuming();

    for msg_opt in rx.iter() {
        if let Some(msg) = msg_opt {
            handle_msg(&mut ws, msg.topic(), msg.payload_str())
        }
    }
}