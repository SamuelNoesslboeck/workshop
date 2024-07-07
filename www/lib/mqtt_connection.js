// Network data
const HOST = "plc.local";
const PORT = 9001;

var mqtt;

// MQTT Callbacks
    function on_connect() {
        console.log("Connected!");

        for (var key in ws_data) {
            mqtt.subscribe(key);
        }
    }

    function on_msg(msg) {
        console.log("New msg: ");
        console.log(msg.destinationName);
        console.log(msg.payloadString);
        console.log(msg);

        ws_data[msg.destinationName] = JSON.parse(msg.payloadString);
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