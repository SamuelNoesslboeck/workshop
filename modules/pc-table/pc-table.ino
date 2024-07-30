// #####################################
// #    PC-TABLE - SMARTSHOP MODULE    #
// #####################################
//
// > Version 0.1.0
// 

# define VERSION "0.1.0"

# define DEBUG true
# include "arduino_helpers.hpp"

// Libraries
# include <WiFi.h>
# include <PubSubClient.h>

// General module
# include "module.hpp"
# include "wifi_data.hpp"   // Hidden file

// Components
# include "rotary_encoder.hpp"
# include "rtrig.hpp"
# include "toff.hpp"

// MQTT Module
# define HOSTNAME "module-pc-table"

// Static
static RTrig ut, lt;
static TOff up(50), lp(50);

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
    debug("| > Message arrived on topic: '");
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
        debugln("| | > Main light chain is activated!");
        WS::Light::chain_main = true;
      } else if (messageTemp == "false") {
        debugln("| | > Main light chain is deactivated!");
        WS::Light::chain_main = false;
      } else {
        debugln("| | > Bad payload, no action could be read!");
      }
    }
  }

  void mqtt_reconnect() {
    while (!mqtt_client.connected()) {
      debug("| > Connecting to MQTT-Server ... ");

      if (mqtt_client.connect(HOSTNAME)) {
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

  debugln("# PC-Table Module");
  debug("> Initializing Pins ... ");

  // TODO: Add Buzzer

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
    // Bedside::rgb_led.set_red(UINT8_MAX);
    delay(250);
    // Bedside::rgb_led.set_red(0);
    delay(250);
  }
  debugln(" done!");

  // Bedside::rgb_led.set_red(0);
  
  debug("| | > IP: ");
  debugln(WiFi.localIP());

  debug("| | > RRSI: ");
  debugln(WiFi.RSSI());

  // Wifi connected
  // Bedside::rgb_led.set_blue(UINT8_MAX);
  
  // MQTT
  debugln("> Setting up MQTT ... ");
  print_mqtt_info();
  
  mqtt_client.setServer(CIRCLELAB_WIFI_PLC_HOST, CIRCLELAB_WIFI_PLC_MQTT_PORT);
  mqtt_client.setCallback(mqtt_callback);
  
  debugln("| > Done!");

  delay(100);   // Wait before starting to recv msgs

  debugln("> Initialization complete!");
}


void loop() {
  // Rotary-Encoders
  RotaryMove upper_val = PcTable::upper_encoder.check_rotary();
  RotaryMove lower_val = PcTable::lower_encoder.check_rotary();

  if (ut.check(up.check(PcTable::upper_encoder.check_switch()))) {
    debugln("| > Upper switch pressed!");
    mqtt_client.publish("ws/light/chain_main", "true");
  }

  if (lt.check(lp.check(PcTable::lower_encoder.check_switch()))) {
    debugln("| > Lower switch pressed!");
    mqtt_client.publish("ws/light/chain_main", "false");
  }

  if (is_movement(upper_val)) {
    debug("| > Upper rotary encoder moved to: ");
    debugln(PcTable::upper_encoder.counter); 
  }

  if (is_movement(lower_val)) {
    debug("| > Lower rotary encoder moved to: ");
    debugln(PcTable::lower_encoder.counter); 
  }

  // MQTT Connection
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }

  mqtt_client.loop();
}
