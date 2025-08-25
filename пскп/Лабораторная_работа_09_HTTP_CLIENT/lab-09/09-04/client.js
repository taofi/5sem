const http = require("http");

const jsonBody = JSON.stringify({
  __comment: "Запрос.Лабораторная работа 9/4",
  x: 6,
  y: 44,
  s: "Сообщение",
  m: ["a", "b", "c", "d"],
  o: {
    surname: "Иванов",
    name: "Иван",
  },
});

const options = {
  host: "localhost",
  path: "/",
  port: 5000,
  method: "POST",
  headers: {
    "Content-Type": "application/json", 'accept': 'application/json',
  },
};

const req = http.request(options, (res) => {
  console.log(`status code: ${res.statusCode}`);

  let body = "";

  res.on("data", (chunk) => {
    body += chunk.toString('utf8');
  });

  res.on("end", () => {
    const parsedBody = JSON.parse(body);

    console.log(parsedBody);
  });
});


req.end(jsonBody);
