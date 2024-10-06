// Network data
const PORT = 1883;

var mqtt;

// Stream
    function on_open(event) {
        console.log("Connected!");
    }

    function on_msg(msg) {
        smartshop.write(msg.lastEventId, JSON.parse(msg.data));
    }

    function on_error(event) {
        console.error(event);
    }
// 

let test = new EventSource("/events");

test.onopen = on_open;
test.onmessage = on_msg;
test.on_error = on_error;