const http = require("http");
const url = require("url");

const server = http
  .createServer((req, res) => {
    res.statusCode = 200;

    let rawBody = "";

    req.on("data", (chunk) => {
      rawBody = chunk.toString();
    });

    req.on("end", () => {
      const body = JSON.parse(rawBody);

      res.end(
        JSON.stringify({
          __comment: "Запрос.Лабораторная работа 9/4",
          x_plus_y: body.x + body.y,
          Concatenation_s_o: `${body.s}: ${body.o.surname}, ${body.o.name}`,
          Length_m: body.m.length,
        })
      );
    });
  })
  .listen(5000);
