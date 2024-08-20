# include "module.hpp"

# define DEBUG true
# include "arduino_helpers.hpp"
# include "circlelab/mqtt_topics.hpp"

void pc_table::setup() {
    debug("| > Setting up DHT-Temp sensor ... ");
    dht.setup(DHT_PIN, DHTesp::DHT11);
    debugln("done!");
}

float pc_table::get_temp() {
    return dht.getTemperature();
}

float pc_table::get_humid() {
    return dht.getHumidity();
}