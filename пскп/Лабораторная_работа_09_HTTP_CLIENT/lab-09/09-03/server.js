const http = require("http");
const query = require("querystring");

const server = http
  .createServer((req, res) => {
    res.statusCode = 200;
      let body = "";
      req.on("data", (chunk) => {
          body += chunk.toString();
      });
      req.on("end", () => {
          const data = query.parse(body);
          const {x, y, s} = data;
          res.end(JSON.stringify({ x, y, s }));

      })

  })
  .listen(5000);
