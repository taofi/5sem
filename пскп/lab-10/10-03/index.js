const WebSocket = require("ws");

const ws = new WebSocket.Server({ port: 4001, host: "localhost" });

let i = 0;

setInterval(() => {
  ws.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(`message ${i}`);
      i++;
    }
  });
}, 2000);
