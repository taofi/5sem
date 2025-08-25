const http = require("http");

const options = {
  host: "localhost",
  path: "/",
  port: 5000,
  method: "GET",
};

const req = http.request(options, (res) => {
  console.log(`status code: ${res.statusCode}`);
  console.log(`status msg: ${res.statusMessage}`);
  console.log(`server ip: ${res.socket.remoteAddress}`);
  console.log(`server port: ${res.socket.remotePort}`);

  let body = "";

  res.on("data", (chunk) => {
    body += chunk.toString();
  });

  res.on("end", () => {
    console.log(`body: ${body}`);
  });
});

req.end();
