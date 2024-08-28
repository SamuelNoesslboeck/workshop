// ###################################
// #    LIVING - SMARTSHOP MODULE    #
// ###################################
//
// > Version 0.3.0
// 

# define VERSION "0.3.0"

# include "arduino_helpers.hpp"

// Libraries
# include <WiFi.h>
# include <PubSubClient.h>

# include "module.hpp"
# include "circlelab/wifi_data.hpp"
# include "circlelab/mqtt_topics.hpp"

// Components
# include "rgb_led.hpp"

// Connection
/// @brief The hostname of the module, used in WiFi and MQTT connections
# define HOSTNAME "module-living"
/// @brief The amount of milliseconds between reconnection tries when disconnected to the WiFi
# define WIFI_RECONN_INTERVAL 3000
/// @brief The amount of milliseconds between reconnection tries when disconnected to the MQTT server
# define MQTT_RECONN_INTERVAL 5000
/// @brief The amout of milliseconds between sending an active status information to the MQTT broker
# define MQTT_ACTIVE_INTERVAL 3000
/// @brief The amount of millisends between sending a measurement of the temperature and humidity
# define TEMP_READ_INTERVAL 5000

/// @brief Reduced brightness of the LED
# define LED_BRIGHT 100

// Static
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
    debug(CIRCLELAB_WIFI_PLC_HOST); 
    debug(":");
    debug(CIRCLELAB_WIFI_PLC_MQTT_PORT);
    debugln("'");
  }
// 

// WiFi functions

  /// @brief Reconnection function for WiFi, will skip if already connected
  void wifi_reconnect() {
    if ((WiFi.status() != WL_CONNECTED) && ((millis() - wifi_reconn_helper) > WIFI_RECONN_INTERVAL)) {
      living::rgb_led.set_red(LED_BRIGHT);
      living::rgb_led.set_blue(0);
      living::rgb_led.set_green(0); 

      debugln("> WiFi disconnected! Attempting to reconnect ... ");
      WiFi.disconnect();
      WiFi.reconnect();
      wifi_reconn_helper = millis();

      if (WiFi.status() == WL_CONNECTED) {
        living::rgb_led.set_red(0);
        living::rgb_led.set_blue(LED_BRIGHT);
      }
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
        debugln("> Activating main light chain!");
        living::set_light_chain_main(true);
        smartshop::light::chain_main = true;
      } else if (messageTemp == "false") {
        debugln("> Deactivating main light chain!");
        living::set_light_chain_main(false);
        smartshop::light::chain_main = false;
      } else {
        debugln("> Bad payload, no action was taken!");
      }
    }

    if (topic == TOPIC_LIGHT_CHAIN_LIVING) {
      if (messageTemp == "true") {
        debugln("> Activating living light chain!");
        living::set_light_chain_living(true);
      } else if (messageTemp == "false") {
        debugln("> Deactivating living light chain!");
        living::set_light_chain_living(false);
      } else {
        debugln("> Bad payload, no action was taken!");
      }
    }
  }

  /// @brief Reconnection function for the MQTT client, skips if connection is still up
  void mqtt_reconnect() {
    if (!WiFi.isConnected()) {
      return;
    }

    while (!mqtt_client.connected() && ((mqtt_active_helper - millis()) > MQTT_RECONN_INTERVAL)) {
      living::rgb_led.set_green(0);
      living::rgb_led.set_blue(LED_BRIGHT);
      debug("| > Connecting to MQTT-Server ... ");

      if (mqtt_client.connect(HOSTNAME)) {
        living::rgb_led.set_green(LED_BRIGHT);
        living::rgb_led.set_blue(0);
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
//

void setup() {
  init_debug(115200);

  debugln("\n# Bedside module");
  debug("> Version: ");
  debugln(VERSION);
  debug("> Initializing module ... ");

  living::setup();

  debugln("done!");

  debugln("> Setting up Wifi ... ");
  print_wifi_info();

  // Configure WiFi
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(CIRCLELAB_WIFI_SSID, CIRCLELAB_WIFI_PASSWD);

  debug("| > Connecting ");
  while (WiFi.status() != WL_CONNECTED) {
    debug(".");
    living::rgb_led.set_red(LED_BRIGHT);
    delay(250);
    living::rgb_led.set_red(0);
    delay(250);
  }
  debugln(" done!");

  living::rgb_led.set_red(0);
  
  debug("| | > IP: ");
  debugln(WiFi.localIP());

  debug("| | > RRSI: ");
  debugln(WiFi.RSSI());

  // Wifi connected
  living::rgb_led.set_blue(LED_BRIGHT);
  
  // MQTT
  debugln("> Setting up MQTT ... ");
  print_mqtt_info();
  
  mqtt_client.setServer(CIRCLELAB_WIFI_PLC_HOST, CIRCLELAB_WIFI_PLC_MQTT_PORT);
  mqtt_client.setCallback(mqtt_callback);
  
  debugln("| > Done!");

  delay(100);   // Wait before starting to recv msgs
}

void loop() {
  // Maintain Connections
  wifi_reconnect();
  mqtt_reconnect();

  mqtt_send_active();

  mqtt_client.loop();
}