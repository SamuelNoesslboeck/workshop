// Network data
const FALLBACK_HOST = "plc.local";
const PORT = 9001;

var mqtt;

// MQTT Callbacks
    function on_connect() {
        console.log("Connected!");

        for (var key in __ws_data) {
            mqtt.subscribe(key);
        }
    }

    function on_msg(msg) {
        // console.log(msg.destinationName + ": " + msg.payloadString);
        smartshop.write(msg.destinationName, JSON.parse(msg.payloadString));
    }

    function on_fail(msg) {
        console.error(msg);
    }
// 

// Connecting
var host = window.location.hostname;

if ((host == "127.0.0.1") || (host == "localhost")) {
    host = FALLBACK_HOST;
}

mqtt = new Paho.MQTT.Client(host, PORT, "circlelab_webclient-" + Math.round(Math.random() * 100000));

var options = {
    timeout: 3,
    onSuccess: on_connect,
    onFailure: on_fail
};

mqtt.onMessageArrived = on_msg;

mqtt.connect(options);