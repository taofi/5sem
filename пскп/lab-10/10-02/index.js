const WebSocket = require("ws");

const ws = new WebSocket("ws://localhost:4001");

let n = 0;

ws.on("message", (data) => {
  console.log(`get: ${data.toString()}`);
});

let interval = setInterval(() => {
  ws.send(`10-01-client: ${n}`);
  n++;
}, 3000);

setTimeout(() => {
  clearInterval(interval);
  console.log("interval stopped");
}, 25000);
