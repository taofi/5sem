const http = require("http");
const fs = require("fs");
const loader = require("./m07-01.js")("static");

const server = http.createServer((req, res) => {
    if (req.url === "/") {
        fs.readFile('static/text/index.html', 'utf8', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('Ошибка при загрузке страницы');
                return;
            }
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end(data);
        });
    }
    else
    loader(req, res);
});

server.listen(3000, () => {
    console.log('Server running at http://localhost:3000/');
});