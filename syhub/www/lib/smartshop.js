var __ws_data = {
    "smartshop/temp": 0.0,
    "smartshop/humid": 0.0,

    "smartshop/light/chain_main": false,
    "smartshop/light/chain_living": false,

    // Helper topic
    "smartshop/modules/#": null
}; 

// EventHandlers
var __ws_on_msg = { };
var __ws_on_msg_all = [ ];

var modules = [ ];

var smartshop = {
    send: function (path, value) {
        smartshop.write(path, value);

        var message = new Paho.MQTT.Message(JSON.stringify(value));
        message.destinationName = path;
        message.retained = true;

        mqtt.send(message);
    },
    toggle: function (path) {
        smartshop.send(path, !__ws_data[path]);
    },
    write: function (path, value) {
        __ws_data[path] = value;
        
        if (path in __ws_on_msg) {
            __ws_on_msg[path](value);
        }

        for (var elem in __ws_on_msg_all) {
            elem(value)
        }
    },
    listen: function (path, callback) {
        __ws_on_msg[path] = callback;
    },
    listen_all: function (callback) {
        __ws_on_msg_all.push(callback)
    }
};
