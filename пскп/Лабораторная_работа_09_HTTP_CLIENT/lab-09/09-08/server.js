const http = require("http");
const fs = require("fs");

const server = http
  .createServer((req, res) => {
    res.statusCode = 200;

    const readStream = fs.createReadStream(
      "./MyFile.png"
    );

    readStream.pipe(res);
  })
  .listen(5000);
