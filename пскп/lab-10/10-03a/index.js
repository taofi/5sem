const WebSocket = require("ws");

const ws = new WebSocket("ws://localhost:4001");

ws.on("message", (data) => {
  console.log(`get: ${data.toString()}`);
});
