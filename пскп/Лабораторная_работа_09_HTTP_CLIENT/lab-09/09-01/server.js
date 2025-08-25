const http = require("http");

const server = http
  .createServer((req, res) => {
    res.statusCode = 200;
    res.statusMessage = "all is good";
    res.end("hi!");
  })
  .listen(5000);
