const http = require("http");
const query = require("querystring");

const params = query.stringify({ x: 1, y: 2, s: "sss" });
console.log(params);
const options = {
  host: "localhost",
  path: '/',
  port: 5000,
  method: "POST",
};

const req = http.request(options, (res) => {
  console.log(`status code: ${res.statusCode}`);

  let body = "";

  res.on("data", (chunk) => {
    body += chunk.toString();
  });

  res.on("end", () => {
    const parsedBody = JSON.parse(body);

    console.log(parsedBody);
  });
});

req.on('error', (e) => {log(e)});

req.write(params);

req.end();
