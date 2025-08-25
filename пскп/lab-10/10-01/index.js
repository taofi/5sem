const http = require("http");
const fs = require("fs");
const WebSocket = require("ws");

const httpServer = http
  .createServer((req, res) => {
    if (req.method === "GET" && req.url === "/start") {
      const htmlStream = fs.createReadStream(
        "/home/vlad/labs/pskp/lab-10/10-01/index.html"
      );
      htmlStream.pipe(res);
      return;
    }

    res.statusCode = 400;
    res.end();
  })
  .listen(3001);

const ws = new WebSocket.Server({ port: 4001, host: "localhost" });

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
