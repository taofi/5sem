const http = require("http");
const fs = require("fs");

let lastModified = new Date();
let etag = 1;

const expiresDate = new Date();
expiresDate.setSeconds(expiresDate.getSeconds() + 30);

setTimeout(() => {
  etag++;
}, 10000);

const server = http
  .createServer((req, res) => {
    const method = req.method;
    const url = req.url;

    const cacheParam = url.split("?")[1]?.split("=")[1];

    switch (cacheParam) {
      case "max-age":
        res.setHeader("Cache-Control", "max-age=30");
        break;
      case "expires":
        res.setHeader("Cache-Control", "private");
        res.setHeader("Expires", expiresDate.toUTCString());
        break;
      case "no-store":
        res.setHeader("Cache-Control", "no-store");
        break;
      case "lm":
        res.setHeader("Last-Modified", lastModified.toUTCString());
        res.setHeader("Cache-Control", "no-cache");

        const lastModifiedHeader = new Date(req.headers["if-modified-since"]);
       
        if (
            Math.floor(lastModifiedHeader.getTime() / 1000) >=
            Math.floor(lastModified.getTime() / 1000)
        ){
          res.statusCode = 304;
          console.log("last modification is actual");

          res.end();
          return;
        }

        break;
      case "ETag":
        res.setHeader("ETag", etag.toString());

        let e = req.headers["if-none-match"];
        console.log(e);
        if (e == etag) {
          res.statusCode = 304;
          console.log("etag is actual");

          res.end();
          return;
        }

        res.setHeader("Cache-Control", "no-cache");

        break;
    }

    if(req.url === '/' && req.method === 'GET'){
      res.statusCode = 200;
      res.setHeader('Content-Type', 'text/html');
      fs.readFile("index.html", function(error,data){
        if(error) {
          res.statusCode = 500;
          res.setHeader('Content-Type', 'text/plain; charset=utf-8');
          res.end('server error');
          return console.log(error);
        }
        res.end(data);
      });
    }
    else if (url.startsWith("/img")) {
      res.writeHead(200, { "Content-Type": "image/png" });

      const readStream = fs.createReadStream(
        "pic.png"
      );

      readStream.pipe(res);
    }
    else if (url.startsWith("/script")) {
      res.writeHead(200, { "Content-Type": "application/javascript" });

      const readStream = fs.createReadStream(
        "hello.js"
      );

      readStream.pipe(res);
    } else if (url.startsWith("/css")) {
      res.writeHead(200, { "Content-Type": "text/css" });

      const readStream = fs.createReadStream(
        "style.css"
      );

      readStream.pipe(res);
    }
  });

const port = 3000;
server.listen(port, () => {
  console.log(`Server running at http://127.0.0.1:${port}`);
})
