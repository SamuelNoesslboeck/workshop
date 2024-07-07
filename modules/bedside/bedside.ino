// ####################################
// #    BEDSIDE - SMARTSHOP MODULE    #
// ####################################
//
// > Version 0.2.0
// 

# define VERSION "0.2.0"

// Libraries
# include <WiFi.h>
# include <PubSubClient.h>

# define DEBUG true
# include "arduino_helpers.hpp"

# include "module.hpp"
# include "rgb_led.hpp"
# include "wifi_data.hpp"

// MQTT Module
# define HOSTNAME "module-bedside"

// Static
static WiFiClient esp_client;
static PubSubClient mqtt_client(esp_client);

// Helper functions
  void print_wifi_info() {
    debug("| > SSID: '");
    debug(CIRCLELAB_WIFI_SSID);
    debug("'\n| > PASSWORD: '");
    debug(CIRCLELAB_WIFI_PASSWD);
    debugln("'");
    debug("| > HOSTNAME: '");
    debug(HOSTNAME);
    debugln("'");
  }

  void print_mqtt_info() {
    debug("| > ADDR: 'mqtt://");
    debug(CIRCLELAB_WIFI_PLC_HOST); 
    debug(":");
    debug(CIRCLELAB_WIFI_PLC_MQTT_PORT);
    debugln("'");
  }
// 

// MQTT Functions
  void mqtt_callback(char* topic, byte* message, unsigned int length) {
    debug("| | > Message arrived on topic: '");
    debug(topic);
    debug("' with payload: '");

    String messageTemp;
    for (int i = 0; i < length; i++) {
      debug((char)message[i]);
      messageTemp += (char)message[i];
    }

    debugln("'");

    if (String(topic) == "ws/light/chain_main") {
      if (messageTemp == "true") {
        debugln("| | | > Activating main light chain!");
        Bedside::activate_relay();
      } else if (messageTemp == "false") {
        debugln("| | | > Deactivating main light chain!");
        Bedside::deactive_relay();
      }

      debugln("| | | > Bad payload, no action was taken!");
    }
  }

  void mqtt_reconnect() {
    while (!mqtt_client.connected()) {
      Bedside::rgb_led.set_green(0);
      Bedside::rgb_led.set_blue(UINT8_MAX);
      debug("| > Connecting to MQTT-Server ... ");

      if (mqtt_client.connect(HOSTNAME)) {
        Bedside::rgb_led.set_green(UINT8_MAX);
        Bedside::rgb_led.set_blue(0);
        debugln("done!");

        // Subscribe to topics
        mqtt_client.subscribe("ws/light/chain_main");
      } else {
        debug("failed! Error-Code (");
        debug(mqtt_client.state());
        debugln(") try again in 5 seconds ... ");

        delay(5000);
      }
    }
  }
// 

void setup() {
  init_debug(115200);

  debugln("\n# Bedside module");
  debug("> Version: ");
  debugln(VERSION);
  debug("> Initializing module ... ");

  Bedside::setup();

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
    Bedside::rgb_led.set_red(UINT8_MAX);
    delay(250);
    Bedside::rgb_led.set_red(0);
    delay(250);
  }
  debugln(" done!");

  Bedside::rgb_led.set_red(0);
  
  debug("| | > IP: ");
  debugln(WiFi.localIP());

  debug("| | > RRSI: ");
  debugln(WiFi.RSSI());

  // Wifi connected
  Bedside::rgb_led.set_blue(UINT8_MAX);
  
  // MQTT
  debugln("> Setting up MQTT ... ");
  print_mqtt_info();
  
  mqtt_client.setServer(CIRCLELAB_WIFI_PLC_HOST, CIRCLELAB_WIFI_PLC_MQTT_PORT);
  mqtt_client.setCallback(mqtt_callback);
  
  debugln("| > Done!");

  delay(100);   // Wait before starting to recv msgs
}

void loop() {
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }

  mqtt_client.loop();
}