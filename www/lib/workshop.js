var ws_data = {
    "ws/light/chain_main": false
}; 

var workshop = {
    set: function (path, value) {
        ws_data[path] = value;

        var message = new Paho.MQTT.Message(JSON.stringify(value));
        message.destinationName = path;
        message.retained = true;

        mqtt.send(message);
    }
};