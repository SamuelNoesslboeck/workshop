// Network data
const HOST = "plc.local";
const PORT = 9001;

var mqtt;

// MQTT Callbacks
    function on_connect() {
        console.log("Connected!");

        // Topics
        mqtt.subscribe("ws/light/chain_main");
    }

    function on_msg(msg) {
        console.log("New msg: ");
        console.log(msg.destinationName);
        console.log(msg.payloadString);
        console.log(msg);
    }

    function on_fail(msg) {
        console.error(msg);
    }
// 

// Connecting
mqtt = new Paho.MQTT.Client(HOST, PORT, "circlelab_webclient");

var options = {
    timeout: 3,
    onSuccess: on_connect,
    onFailure: on_fail
};

mqtt.onMessageArrived = on_msg;

mqtt.connect(options);