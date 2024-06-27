use core::time::Duration;

use workshop::PLCMQTTConnection;

fn main() {
    let mut conn = PLCMQTTConnection::from_env();

    loop {
        std::thread::sleep(Duration::from_millis(3000));
        conn.update_active().unwrap();
        conn.update_cpu_usage().unwrap();
    }
}