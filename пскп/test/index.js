const http = require("http");
const url = require("url");
const querystring = require("querystring");
const xml2js = require("xml2js");
const xmlbuilder = require("xmlbuilder");
const fs = require("fs");
const formidable = require("formidable");
const path = require("path");

const server = http
  .createServer((req, res) => {
    const method = req.method;
    const urlInfo = url.parse(req.url, true);

    if (method === "GET" && urlInfo.path === "/connection") {
      res.writeHead(200, { "Content-Type": "text/plain charset=utf-8" });
      res.end(server.keepAliveTimeout.toString());
      return;
    }

    if (method === "GET" && urlInfo.path.startsWith("/connection?set=")) {
      const newTimeout = parseInt(urlInfo.path.split("=")[1], 10);
      server.keepAliveTimeout = newTimeout;
      res.writeHead(200, { "Content-Type": "text/plain charset=utf-8" });
      res.end(server.keepAliveTimeout.toString());
      return;
    }

    if (method === "GET" && urlInfo.path === "/headers") {
      res.writeHead(200, {
        "Content-Type": "application/json charset=utf-8",
        "X-Custom-Header": "MyCustomValue",
      });

      const headers = {
        requestHeaders: req.headers,
        responseHeaders: {
          "Content-Type": "application/json",
          "X-Custom-Header": "MyCustomValue",
        },
      };
      res.end(JSON.stringify(headers, null, 2));
      return;
    }

    if (method === "GET" && urlInfo.path.startsWith("/parameter?")) {
      const x = parseFloat(urlInfo.query["x"]);
      const y = parseFloat(urlInfo.query["y"]);

      if (!isNaN(x) && !isNaN(y)) {
        const sum = x + y;
        const minus = x - y;
        const div = x / y;
        const mult = x * y;
        res.write("sum = " + sum + "\n");
        res.write("minus = " + minus + "\n");
        res.write("div = " + div + "\n");
        res.end("mult = " + mult + "\n");
      } else {
        res.writeHead(400);
        res.end("error");
      }

      return;
    }

    if (method === "GET" && urlInfo.path.startsWith("/parameter")) {
      const splits = urlInfo.path.split("/");

      const x = parseFloat(splits[2]);
      const y = parseFloat(splits[3]);

      if (!isNaN(x) && !isNaN(y)) {
        const sum = x + y;
        const minus = x - y;
        const div = x / y;
        const mult = x * y;
        res.write("sum = " + sum + "\n");
        res.write("minus = " + minus + "\n");
        res.write("div = " + div + "\n");
        res.end("mult = " + mult + "\n");
      } else {
        res.end(req.url);
      }

      return;
    }

    if (method === "GET" && urlInfo.path === "/close") {
      res.end("the server will close in 10 seconds");
      setTimeout(() => server.close(), 10000);
      return;
    }

    if (method === "GET" && urlInfo.path === "/socket") {
      const clientIp = req.socket.remoteAddress;
      const clientPort = req.socket.remotePort;
      const serverIp = req.socket.localAddress;
      const serverPort = req.socket.localPort;

      const response = `
            client ip: ${clientIp}
            client port: ${clientPort}
            server ip: ${serverIp}
            server port: ${serverPort}
        `;

      res.writeHead(200, { "Content-Type": "text/plain" });
      res.end(response);
      return;
    }

    if (method === "GET" && urlInfo.path === "/req-data") {
      let chunksCount = 0;

      req.on("data", (chunk) => {
        chunksCount++;
      });

      req.on("end", () => {
        res.writeHead(200, { "Content-Type": "text/plain" });
        res.end(chunksCount.toString());
        return;
      });
    }

    if (method === "GET" && urlInfo.path.startsWith("/resp-status?")) {
      const code = parseInt(urlInfo.query["code"]);
      const mess = urlInfo.query["mess"];

      res.statusCode = code;
      res.statusMessage = mess;
      res.end();
      return;
    }

    if (method === "POST" && urlInfo.path === "/formparameter") {
      let body = "";

      req.on("data", (chunk) => {
        body += chunk.toString();
      });

      req.on("end", () => {
        const params = querystring.parse(body);

        let responseMessage = "<h1>Params:</h1><ul>";
        for (const key in params) {
          responseMessage += `<li>${key}: ${params[key]}</li>`;
        }
        responseMessage += "</ul>";

        console.log(responseMessage);

        res.writeHead(200, { "Content-Type": "text/html" });
        res.end(responseMessage);
      });
    }

    if (method === "POST" && urlInfo.path === "/json") {
      let rawBody = "";

      req.on("data", (chunk) => {
        rawBody = chunk.toString();
      });

      req.on("end", () => {
        const body = JSON.parse(rawBody);

        res.writeHead(200, { "Content-Type": "application/json" });

        res.end(
          JSON.stringify({
            __comment: "Запрос.Лабораторная работа 8/10",
            x_plus_y: body.x + body.y,
            Concatenation_s_o: `${body.s}: ${body.o.surname}, ${body.o.name}`,
            Length_m: body.m.length,
          })
        );
      });
    }

    if (method === "POST" && urlInfo.path === "/xml") {
      let rawXml = "";

      req.on("data", (chunk) => {
        rawXml += chunk.toString();
      });

      req.on("end", () => {
        xml2js.parseString(rawXml, (_, result) => {
          const request = result.request;

          const xValues = request.x.map((item) =>
            parseInt(item["$"].value, 10)
          );
          const sumX = xValues.reduce((acc, curr) => acc + curr, 0);

          const mValues = request.m.map((item) => item["$"].value);
          const concatM = mValues.reduce((accumulator, current) => {
            return accumulator + current;
          }, "");

          const xml = xmlbuilder
            .create("response")
            .att("id", "33")
            .att("request", request["$"].id)
            .ele("sum")
            .att("element", "x")
            .att("result", sumX.toString())
            .up()
            .ele("concat")
            .att("element", "m")
            .att("result", concatM)
            .end({ pretty: true });

          res.writeHead(200, { "Content-Type": "application/xml" });
          res.end(xml);
        });
      });
    }

    if (req.method === "GET" && urlInfo.path === "/files") {
      const staticDir = "/home/vlad/labs/pskp/lab-08/lab-08/static";

      fs.readdir(staticDir, (err, files) => {
        const fileCount = files.length;

        res.writeHead(200, {
          "Content-Type": "text/plain",
          "X-static-files-count": fileCount,
        });
        res.end(`file count: ${fileCount}`);
      });
    }

    if (method === "GET" && urlInfo.path.startsWith("/files/")) {
      const filename = urlInfo.path.split("/")[2];

      fs.readFile(
        `/home/vlad/labs/pskp/lab-08/lab-08/static/${filename}`,
        (err, data) => {
          if (err) {
            res.writeHead(404);
            res.end("file does not found");
            return;
          }

          res.end(data);
        }
      );
    }

    if (method === "GET" && urlInfo.path === "/upload") {
      const htmlStream = fs.createReadStream(
        "/home/vlad/labs/pskp/lab-08/lab-08/upload.html"
      );
      htmlStream.pipe(res);
    }

    if (method === "POST" && urlInfo.path === "/upload") {
      const form = new formidable.IncomingForm();

      form.uploadDir = "/home/vlad/labs/pskp/lab-08/lab-08/static";

      form.parse(req, (err, fields, files) => {
        var oldpath = files.file[0].filepath;
        var newpath =
          "/home/vlad/labs/pskp/lab-08/lab-08/static/" +
          files.file[0].originalFilename;

        fs.rename(oldpath, newpath, (err) => {
          if (err) {
            res.writeHead(500, { "Content-Type": "text/plain" });
            res.write("File uploaded and moved!");
            return;
          }

          res.writeHead(200, { "Content-Type": "text/plain" });
          res.end("ok");
        });
      });
    }
  })
  .listen(5000);
