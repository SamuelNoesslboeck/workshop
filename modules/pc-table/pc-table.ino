// #####################################
// #    PC-TABLE - SMARTSHOP MODULE    #
// #####################################
//
// > Version 0.3.2
// 

# define VERSION "0.3.2"

# include "arduino_helpers.hpp"

// Libraries
# include <WiFi.h>
# include <PubSubClient.h>

// General module
# include "module.hpp"
# include "circlelab/mqtt_topics.hpp"
# include "circlelab/wifi_data.hpp"   // Hidden file

// Components
# include "rotary_encoder.hpp"
# include "rtrig.hpp"
# include "toff.hpp"

// Connection
/// @brief The hostname of the module, used in WiFi and MQTT connections
# define HOSTNAME "module-pc-table"
/// @brief The amount of milliseconds between reconnection tries when disconnected to the WiFi
# define WIFI_RECONN_INTERVAL 3000
/// @brief The amount of milliseconds between reconnection tries when disconnected to the MQTT server
# define MQTT_RECONN_INTERVAL 5000
/// @brief The amout of milliseconds between sending an active status information to the MQTT broker
# define MQTT_ACTIVE_INTERVAL 3000
/// @brief The amount of millisends between sending a measurement of the temperature and humidity
# define TEMP_READ_INTERVAL 5000

// Static variables
static RTrig ut, lt;
static TOff up(50), lp(50);

static WiFiClient esp_client;
static PubSubClient mqtt_client(esp_client);

// Static helper variables
static unsigned long wifi_reconn_helper = 0;
static unsigned long mqtt_reconn_helper = 0;
static unsigned long mqtt_active_helper = 0;
static unsigned long mqtt_temp_helper = 0;

static char payload_buffer[128];

// Helper functions

  /// @brief Prints out configuration data used for the WiFi connection
  void print_wifi_info() { 
    debug("| > SSID: '");
    debug(CIRCLELAB_WIFI_SSID);
    debugln("'");

    debug("| > PASSWORD: '");
    debug(CIRCLELAB_WIFI_PASSWD);
    debugln("'");

    debug("| > HOSTNAME: '");
    debug(HOSTNAME);
    debugln("'");
  }

  /// @brief Prints out configuration data used for the MQTT connection
  void print_mqtt_info() {
    debug("| > ADDR: 'mqtt://");
    debug(CIRCLELAB_WIFI_HUB_HOST); 
    debug(":");
    debug(CIRCLELAB_WIFI_HUB_MQTT_PORT);
    debugln("'");
  }
// 

// WiFi functions

  /// @brief Reconnection function for WiFi, will skip if already connected
  void wifi_reconnect() {
    if ((WiFi.status() != WL_CONNECTED) && ((millis() - wifi_reconn_helper) > WIFI_RECONN_INTERVAL)) {
      debugln("> WiFi disconnected! Attempting to reconnect ... ");
      WiFi.disconnect();
      WiFi.reconnect();
      wifi_reconn_helper = millis();
    } 
  }

//

// MQTT Functions
  /// @brief Callback function for incomming mqtt messages
  void mqtt_callback(char* _topic, byte* message, unsigned int length) {
    // Parse into strings
    String topic (_topic);
    String messageTemp;
    for (int i = 0; i < length; i++) {
      messageTemp += (char)message[i];
    }

    // React to messages
    if (topic == TOPIC_LIGHT_CHAIN_MAIN) {
      if (messageTemp == "true") {
        debugln("| | > Main light chain is activated!");
        smartshop::light::chain_main = true;
      } else if (messageTemp == "false") {
        debugln("| | > Main light chain is deactivated!");
        smartshop::light::chain_main = false;
      } else {
        debug("| | > Bad payload: ");
        debugln(messageTemp);
      }
    }
  }

  /// @brief Reconnection function for the MQTT client, skips if connection is still up
  void mqtt_reconnect() {
    if (!WiFi.isConnected()) {
      return;
    }

    while (!mqtt_client.connected() && ((mqtt_active_helper - millis()) > MQTT_RECONN_INTERVAL)) {
      debug("| > Connecting to MQTT-Server ... ");

      if (mqtt_client.connect(HOSTNAME)) {
        debugln("done!");
        
        // Subscribe to topics
        mqtt_client.subscribe(TOPIC_LIGHT_CHAIN_MAIN);
        mqtt_client.subscribe(TOPIC_LIGHT_CHAIN_LIVING);
      } else {
        debug("failed! Error-Code (");
        debug(mqtt_client.state());
        debugln(") trying again soon ... ");
      }

      mqtt_active_helper = millis();
    }
  }

  // Data
  void mqtt_send_active() {
    if ((millis() - mqtt_active_helper) > MQTT_ACTIVE_INTERVAL) {
      debug("> Sending active status ... ");

      sprintf(payload_buffer, "{\"uptime\":%u,\"version\":\"" VERSION "\"}", millis());
      mqtt_client.publish(JOIN_TOPICS(TOPIC_MODULES, HOSTNAME), payload_buffer);
      mqtt_active_helper = millis();

      debugln("done!");
    }
  }

  void mqtt_send_temp() {
    if ((millis() - mqtt_temp_helper) > TEMP_READ_INTERVAL) {
      debugln("> Sending tempature status ... ");

      smartshop::humid = pc_table::get_humid();
      smartshop::temp = pc_table::get_temp();

      if (pc_table::dht.getStatus()) {
        debug("| > Measurement failed! Error: ");
        debugln(pc_table::dht.getStatusString());
      } else {
        debug("| > Temperature: ");
        debugln(smartshop::temp);
        debug("| > Humidity: ");
        debugln(smartshop::humid);

        sprintf(payload_buffer, "%.2f", smartshop::temp);
        mqtt_client.publish(TOPIC_TEMP, payload_buffer, (bool)true);

        sprintf(payload_buffer, "%.2f", smartshop::humid);
        mqtt_client.publish(TOPIC_HUMID, payload_buffer, (bool)true);
      }

      mqtt_temp_helper = millis();
      
      debugln("| > Done!");
    }
  }
// 

void setup() {
  init_debug(115200);

  debugln("# PC-Table Module");
  debugln("> Initializing devices ... ");

  pc_table::setup();

  debugln("| > Done!");

  debugln("> Setting up Wifi ... ");
  print_wifi_info();

  // Setup WiFi
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(CIRCLELAB_WIFI_SSID, CIRCLELAB_WIFI_PASSWD);

  debug("| > Connecting ");
  while (WiFi.status() != WL_CONNECTED) {
    debug(".");
    delay(500);
  }
  debugln(" done!");
  
  debug("| | > IP: ");
  debugln(WiFi.localIP());

  debug("| | > RRSI: ");
  debugln(WiFi.RSSI());
  
  // MQTT
  debugln("> Setting up MQTT ... ");
  print_mqtt_info();
  
  mqtt_client.setServer(CIRCLELAB_WIFI_HUB_HOST, CIRCLELAB_WIFI_HUB_MQTT_PORT);
  mqtt_client.setCallback(mqtt_callback);

  mqtt_reconnect();
  
  debugln("| > Done!");

  delay(100);   // Wait before starting to recv msgs

  debugln("> Initialization complete!");
}


void loop() {
  // Rotary-Encoders
  RotaryMove upper_val = pc_table::upper_encoder.check_rotary();
  RotaryMove lower_val = pc_table::lower_encoder.check_rotary();

  if (ut.check(up.exec(pc_table::upper_encoder.check_switch()))) {
    debugln("| > Upper switch pressed!");
    mqtt_client.publish(TOPIC_LIGHT_CHAIN_MAIN, "true", true);
  }

  if (lt.check(lp.exec(pc_table::lower_encoder.check_switch()))) {
    debugln("| > Lower switch pressed!");
    mqtt_client.publish(TOPIC_LIGHT_CHAIN_MAIN, "false", true);
  }

  if (is_movement(upper_val)) {
    debug("| > Upper rotary encoder moved to: ");
    debugln(pc_table::upper_encoder.counter); 
  }

  if (is_movement(lower_val)) {
    debug("| > Lower rotary encoder moved to: ");
    debugln(pc_table::lower_encoder.counter); 
  }

  // Maintain Connections
  wifi_reconnect();
  mqtt_reconnect();

  mqtt_send_active();
  mqtt_send_temp();

  mqtt_client.loop();
}
