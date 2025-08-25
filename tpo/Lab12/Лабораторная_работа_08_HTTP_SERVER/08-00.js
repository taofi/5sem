const http = require("http");
const fs = require("fs");
const url = require("url");
const path = require("path");
const mp = require('multiparty');

const xml2js = require('xml2js');

const xmlbuilder = require('xmlbuilder');

const staticDir = path.join( "static");

const isValidEmail = (email) => {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return re.test(email);
};




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






const GET_hendler = (req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;
    const query = parsedUrl.query;
    if (pathname === "/users") {
        const page = parseInt(query.page) || 1; // Получаем номер страницы, по умолчанию 1
        const limit = parseInt(query.limit) || 5; // Получаем лимит, по умолчанию 5
        console.log(page, limit);
        // Вычисляем начальный и конечный индексы
        const startIndex = (page - 1) * limit;
        const endIndex = startIndex + limit;

        // Получаем пользователей с учетом пагинации
        const paginatedUsers = users.slice(startIndex, endIndex);

        // Если на странице нет пользователей, возвращаем 404
        if (paginatedUsers.length === 0) {
            res.statusCode = 404;
            res.end("No users found on this page");
            return;
        }

        // Возвращаем список пользователей
        res.statusCode = 200;
        res.setHeader("Content-Type", "application/json");
        res.end(JSON.stringify(paginatedUsers));
    } else
    if (pathname === "/formparameter") {
        fs.readFile("form.html", "utf8", (err, data) => {
            if (err) {
                res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
                res.end("Ошибка при чтении файла");
                return;
            }
            res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
            res.end(data);
        });
    }
    else if (pathname === "/connection") {
        if (query.set) {
            const newValue = parseInt(query.set, 10);
            if (!isNaN(newValue)) {
                server.keepAliveTimeout = newValue;
                res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
                res.end(`Установлено новое значение параметра KeepAliveTimeout=${newValue}`);
            } else {
                res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
                res.end("Ошибка: значение set должно быть числом.");
            }
        } else {
            res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`Текущее значение KeepAliveTimeout=${server.keepAliveTimeout}`);
        }
    }
    else if (pathname === "/headers") {
        res.setHeader("X-Custom-Header", "MyValue");
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(`Request Headers:\n${JSON.stringify(req.headers, null, 2)}\nResponse Headers:\n${JSON.stringify(res.getHeaders(), null, 2)}`);
    }
    else if (pathname === "/parameter") {
        const x = parseFloat(query.x);
        const y = parseFloat(query.y);
        if (!isNaN(x) && !isNaN(y)) {
            res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`Сумма: ${x + y}\nРазность: ${x - y}\nПроизведение: ${x * y}\nЧастное: ${x / y}`);
        } else {
            res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
            res.end("Ошибка: параметры x и y должны быть числами.");
        }
    }
    else if (pathname.startsWith("/parameter/")) {
        const [, , xStr, yStr] = pathname.split("/");
        const x = parseFloat(xStr);
        const y = parseFloat(yStr);
        if (!isNaN(x) && !isNaN(y)) {
            res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`Сумма: ${x + y}\nРазность: ${x - y}\nПроизведение: ${x * y}\nЧастное: ${x / y}`);
        } else {
            res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`URI: ${req.url}`);
        }
    }
    else if (pathname === "/close") {
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("Сервер будет закрыт через 10 секунд.");
        setTimeout(() => {
            console.log("Сервер остановлен");
            server.close(() => {
                console.log("Сервер выключен");
                process.exit(0)
            })
        },  10000);
    }
    else if (pathname === "/socket") {
        const client = req.socket;
        const server = client.server;
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(`Client IP: ${client.remoteAddress}, Port: ${client.remotePort}\nServer IP: ${server.address().address}, Port: ${server.address().port}`);
    }
    else if (pathname === "/req-data") {
        let body = "";
        let chunksCount = 0;

        req.on("data", chunk => {
            body += chunk;
            chunksCount++;

        });
        req.on("end", () => {
            res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`Body: ${body}, chunksCount: ${chunksCount}`);
        });
    }
    else if (pathname === "/resp-status") {
        const code = parseInt(query.code, 10);
        const mess = query.mess || "";
        if (!isNaN(code)) {
            res.writeHead(code, { "Content-Type": "text/plain; charset=utf-8" });
            res.end(`Статус: ${code}, Сообщение: ${mess}`);
        } else {
            res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
            res.end("Ошибка: параметр code должен быть числом.");
        }
    }
    else if (pathname.startsWith("/files/")) {
        const filename = pathname.replace("/files/", ""); // Извлекаем имя файла
        const filePath = path.join(staticDir, filename);

        fs.access(filePath, fs.constants.F_OK, (err) => {
            if (err) {
                // Файл не найден
                res.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
                res.end("Ошибка 404: файл не найден.");
            } else {
                const fileStream = fs.createReadStream(filePath);
                res.writeHead(200, { "Content-Type": "application/octet-stream" });
                fileStream.pipe(res);
            }
        });
    }
    else if (pathname === "/files") {
        fs.readdir(staticDir, (err, files) => {
            if (err) {
                res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
                res.end("Ошибка сервера при чтении директории.");
                return;
            }

            const fileCount = files.length;
            res.writeHead(200, {
                "Content-Type": "text/plain; charset=utf-8",
                "X-static-files-count": fileCount
            });
            res.end(`Количество файлов в директории: ${fileCount}`);
        });
    }
    else if ( pathname === "/upload") {
        res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
        res.end(`
            <form action="/upload" method="POST" enctype="multipart/form-data">
                <input type="file" name="file" required>
                </br>
                <input type="submit" value="Загрузить">
            </form>
        `);
    }
    else {
        res.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("404 Not Found");
    }
}
const POST_hendler = (req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;

    if ( req.url.startsWith("/formparameter")) {
        let body = "";
        req.on("data", chunk => {
            body += chunk;
        });
        req.on("end", () => {

            const parsedUrl = url.parse(req.url, true); // Для анализа параметров URL (если они есть)
            const params = new URLSearchParams(body); // Для анализа тела запроса

            res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
            res.write("<h1>Полученные параметры:</h1>");
            res.write("<ul>");
            for (const [key, value] of params.entries()) {
                res.write(`<li>${key}: ${value}</li>`);
            }
            res.write("</ul>");
            res.end();
        });
    }
    else if ( pathname === "/json") {
        let rawBody = "";

        req.on("data", (chunk) => {
            rawBody = chunk.toString();
        });

        req.on("end", () => {
            const body = JSON.parse(rawBody);

            res.writeHead(200, { "Content-Type": "application/json" });

            res.end(
                JSON.stringify({
                    __comment: body.__comment,
                    x_plus_y: body.x + body.y,
                    Concatenation_s_o: `${body.s}: ${body.o.surname}, ${body.o.name}`,
                    Length_m: body.m.length,
                })
            );
        });
    }
    else if (pathname === "/xml") {
        let rawXml = "";

        req.on("data", (chunk) => {
            rawXml += chunk.toString();
        });

        req.on('end', () => {
            const parser = new xml2js.Parser();
            parser.parseString(rawXml, (err, result) => {
                if (err) {
                    res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
                    res.end("Ошибка обработки XML.");
                    return;
                }

                const request = result.request;

                const xValues = request.x.map((item) =>
                    parseInt(item["$"].value, 10)
                );
                const sumX = xValues.reduce((acc, curr) => acc + curr, 0);

                const mValues = request.m.map((item) => item["$"].value);
                const concatM = mValues.join("");

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
    else if ( pathname === "/upload") {
        const result = {};
        const form = new mp.Form({ uploadDir: './static' });

        form.on('field', (name, data) => {
            console.log(`Поле: ${name} = ${data}`);
            result[name] = data;
        });

        form.on('file', (name, file) => {
            console.log(`Файл: ${name} = ${file.originalFilename}`);
            result[name] = file.path; // Хранит путь к загруженному файлу
        });

        form.on('error', (err) => {
            console.error('Ошибка:', err);
            res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
            res.end('Ошибка обработки формы.');
        });

        form.on('close', () => {
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end('Форма успешно обработана!'); // Ответ клиенту
        });

        form.parse(req);
    }
    else {
        res.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("404 Not Found");
    }
}

const server = http.createServer();
server.keepAliveTimeout = 3000;
server.on('connection', (socket) => {
    console.log('Соединение установлено');

    socket.on('close', () => {
        console.log('Соединение закрыто');
    });
});
server.on('request', (req, res) => {
    console.log(`Запрос на: ${req.url}`);
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;
    const method = req.method;
    if (method === "GET" && pathname.startsWith("/users/")) {
        const id = parseInt(pathname.split("/")[2]);
        const user = users.find((u) => u.id === id);

        if (user) {
            res.statusCode = 200;
            res.setHeader("Content-Type", "application/json");
            res.end(JSON.stringify(user));
        } else {
            res.statusCode = 404;
            res.end("User not found");
        }
    } else if (method === "POST" && pathname === "/users") {
        let rawBody = "";

        req.on("data", (chunk) => {
            rawBody += chunk.toString();
        });

        req.on("end", () => {
            if (rawBody === "") {
                res.statusCode = 400;
                res.end("invalid body");
                return;
            }

            let body;

            try {
                body = JSON.parse(rawBody);
            } catch (error) {
                res.statusCode = 400;
                res.end("invalid body");
                return;
            }

            if (
                body.age === undefined
            ) {
                res.statusCode = 400;
                res.end("поле age обязательно");
                return;
            }
            if (
                body.email === undefined
            ) {
                res.statusCode = 400;
                res.end("поле email обязательно");
                return;
            }
            if (
                body.name === undefined
            ) {
                res.statusCode = 400;
                res.end("поле name обязательно");
                return;
            }
            if (typeof body.age !== "number" || body.age < 18 || body.age > 100) {
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

            // Добавление пользователя
            users.push(body);
            res.statusCode = 201; // Created
            res.setHeader("Content-Type", "application/json");
            res.end(JSON.stringify(body));
        });
    } else if (method === "DELETE" && pathname.startsWith("/users/")) {
        const id = parseInt(pathname.split("/")[2]);
        const index = users.findIndex((u) => u.id === id);

        if (index !== -1) {
            const deletedUser = users.splice(index, 1);
            res.statusCode = 204;
            res.setHeader("Content-Type", "application/json");
            res.end(JSON.stringify(deletedUser));
        } else {
            res.statusCode = 404;
            res.end("User not found");
        }
    }
    else if(req.method === "GET")
        GET_hendler(req, res);
    else if (req.method === "POST")
        POST_hendler(req, res);
    else {
        res.writeHead(405, { 'Content-Type': 'text/plain; charset=utf-8' });
        res.end('Метод не разрешен.');
    }
});
server.listen(3000, () => {
    console.log('Server running at http://localhost:3000/');
});