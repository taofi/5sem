const WebSocket = require("ws");

const ws = new WebSocket.Server({ port: 4000, host: "localhost" });

let n;
let k = 0;

ws.on("connection", (socket) => {
    console.log("Client connected");

    socket.on("message", (data) => {
        console.log(`get: ${data.toString()}`);
        n = parseInt(data.toString().split(" ")[1]);
    });

    socket.on("close", () => {
        console.log("Client disconnected");
    });

    setInterval(() => {
        socket.send(`10-01-server: ${n}->${k}`);
        k++;
    }, 5000);
});
