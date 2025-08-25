const http = require("http");
const xml2js = require("xml2js");
const xmlbuilder = require("xmlbuilder");

const server = http
  .createServer((req, res) => {
    res.statusCode = 200;

    let rawXml = "";

    req.on("data", (chunk) => {
      rawXml += chunk.toString();
    });

    req.on("end", () => {
      xml2js.parseString(rawXml, (_, result) => {
        const request = result.request;

        const xValues = request.x.map((item) => parseInt(item["$"].value, 10));
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
  })
  .listen(5000);
