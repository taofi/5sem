const http = require("http");
const url = require("url");

let users = [
  {
    id: 1,
    name: "Alice Smith",
    email: "alice.smith@example.com",
    age: 30,
  },
  {
    id: 2,
    name: "Bob Johnson",
    email: "bob.johnson@example.com",
    age: 25,
  },
  {
    id: 3,
    name: "Charlie Brown",
    email: "charlie.brown@example.com",
    age: 28,
  },
  {
    id: 4,
    name: "Diana Prince",
    email: "diana.prince@example.com",
    age: 35,
  },
  {
    id: 5,
    name: "Evan White",
    email: "evan.white@example.com",
    age: 40,
  },
  {
    id: 6,
    name: "Fiona Green",
    email: "fiona.green@example.com",
    age: 32,
  },
  {
    id: 7,
    name: "George Black",
    email: "george.black@example.com",
    age: 29,
  },
  {
    id: 8,
    name: "Hannah Blue",
    email: "hannah.blue@example.com",
    age: 27,
  },
  {
    id: 9,
    name: "Ian Gray",
    email: "ian.gray@example.com",
    age: 26,
  },
  {
    id: 10,
    name: "Jessica Red",
    email: "jessica.red@example.com",
    age: 31,
  },
  {
    id: 11,
    name: "Kevin Yellow",
    email: "kevin.yellow@example.com",
    age: 34,
  },
  {
    id: 12,
    name: "Laura Purple",
    email: "laura.purple@example.com",
    age: 33,
  },
  {
    id: 13,
    name: "Mike Orange",
    email: "mike.orange@example.com",
    age: 36,
  },
  {
    id: 14,
    name: "Nina Pink",
    email: "nina.pink@example.com",
    age: 38,
  },
  {
    id: 15,
    name: "Oscar Brown",
    email: "oscar.brown@example.com",
    age: 39,
  },
];

const PORT = 4000;

const server = http
  .createServer((req, res) => {
    const method = req.method;
    const URL = url.parse(req.url, true);

    if (method === "GET" && URL.pathname === "/users") {
      const page = parseInt(URL.query.page) || 1;
      const limit = parseInt(URL.query.limit) || 10;

      const startIndex = (page - 1) * limit;
      const endIndex = startIndex + limit;

      const resultUsers = users.slice(startIndex, endIndex);

      res.statusCode = 200;
      res.setHeader("Content-Type", "application/json");
      res.end(JSON.stringify(resultUsers));
      return;
    } else if (method === "GET" && URL.pathname.startsWith("/users/")) {
      const id = URL.pathname.split("/")[2];

      const user = users.find((u) => u.id == id);

      if (user === undefined) {
        res.statusCode = 404;
        res.end("can't find user");
        return;
      }

      res.statusCode = 200;
      res.setHeader("Content-Type", "application/json");
      res.end(JSON.stringify(user));
      return;
    } else if (method === "POST" && URL.pathname === "/users") {
      let rawBody = "";

      req.on("data", (chunk) => {
        rawBody += chunk.toString();
      });

      req.on("end", () => {
        if (rawBody == "") {
          res.statusCode = 400;
          res.end("invalid body");
          return;
        }

        const body = JSON.parse(rawBody);

        if (
          body.age === undefined ||
          body.email === undefined ||
          body.name === undefined
        ) {
          res.statusCode = 400;
          res.end("invalid body");
          return;
        }

        if (body.age < 18 || body.age > 100) {
          res.statusCode = 400;
          res.end("invalid age");
          return;
        }

        if (!isValidEmail(body.email)) {
          res.statusCode = 400;
          res.end("invalid email");
          return;
        }

        if (body.name == "" || body.name.length > 100) {
          res.statusCode = 400;
          res.end("invalid name");
          return;
        }

        const user = {
          id: users.length + 1,
          name: body.name,
          email: body.email,
          age: body.age,
        };

        users.push(user);

        res.statusCode = 201;
        res.setHeader("Content-Type", "application/json");
        res.end(JSON.stringify(user));
        return;
      });
    } else if (method === "PUT" && URL.pathname === "/users") {
      let rawBody = "";

      req.on("data", (chunk) => {
        rawBody += chunk.toString();
      });

      req.on("end", () => {
        if (rawBody == "") {
          res.statusCode = 400;
          res.end("invalid body");
          return;
        }

        const body = JSON.parse(rawBody);

        if (
          body.age === undefined ||
          body.email === undefined ||
          body.name === undefined ||
          body.id === undefined
        ) {
          res.statusCode = 400;
          res.end("invalid body");
          return;
        }

        if (body.age < 18 || body.age > 100) {
          res.statusCode = 400;
          res.end("invalid age");
          return;
        }

        if (!isValidEmail(body.email)) {
          res.statusCode = 400;
          res.end("invalid email");
          return;
        }

        if (body.name === "" || body.name.length > 100) {
          res.statusCode = 400;
          res.end("invalid name");
          return;
        }

        const userIndex = users.findIndex((u) => u.id === body.id);

        if (userIndex === -1) {
          res.statusCode = 404;
          res.end("user is not found");
          return;
        }

        users[userIndex] = { ...users[userIndex], ...body };

        res.statusCode = 200;
        res.setHeader("Content-Type", "application/json");
        res.end(JSON.stringify(users[userIndex]));
        return;
      });
    } else if (method === "DELETE" && URL.pathname.startsWith("/users/")) {
      const id = URL.pathname.split("/")[2];

      const user = users.find((u) => u.id == id);

      if (user === undefined) {
        res.statusCode = 404;
        res.end("user is not found");
        return;
      }

      users = users.filter((u) => u.id !== user.id);

      res.statusCode = 200;
      res.setHeader("Content-Type", "application/json");
      res.end(JSON.stringify(user));
      return;
    } else if (method === "POST" && URL.pathname === "/users/secure") {
      let rawBody = "";

      req.on("data", (chunk) => {
        rawBody += chunk.toString();
      });

      req.on("end", () => {
        if (rawBody == "") {
          res.statusCode = 400;
          res.end("invalid body");
          return;
        }

        const body = JSON.parse(rawBody);

        if (body.pass !== 123) {
          res.statusCode = 401;
          res.end("invalid password");
          return;
        }

        res.statusCode = 200;
        res.end("secure data");
        return;
      });
    } else {
      res.statusCode = 404;
      res.end("invalid endpoint");
    }
  })
  .listen(PORT, () => console.log("server is listening on port " + PORT));

function isValidEmail(email) {
  return true;
}
