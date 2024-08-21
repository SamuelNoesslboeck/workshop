var ws_data = {
    "smartshop/temp": 0.0,
    "smartshop/humid": 0.0,

    "smartshop/light/chain_main": false,
    "smartshop/light/chain_living": false,
}; 

var ws_on_msg = { };

var smartshop = {
    send: function (path, value) {
        smartshop.write(path, value);

        var message = new Paho.MQTT.Message(JSON.stringify(value));
        message.destinationName = path;
        message.retained = true;

        mqtt.send(message);
    },
    write: function (path, value) {
        ws_data[path] = value;
        
        if (path in ws_on_msg) {
            ws_on_msg[path](value);
        }
    },
    listen: function (path, callback) {
        ws_on_msg[path] = callback;
    }
};