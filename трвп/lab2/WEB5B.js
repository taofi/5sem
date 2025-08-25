const http = require('http');
const port = 5000;
const fs = require("fs");

const server = http.createServer((req, res) => {
    if(req.url === '/' && req.method === 'GET'){
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/html');
        fs.readFile("index2.html", function(error,data){
            if(error) {
                res.statusCode = 500;
                res.setHeader('Content-Type', 'text/plain; charset=utf-8');
                res.end('server error');
                return console.log(error);
            }
            res.end(data);
        });
    }else if (req.method === 'POST' && req.url === '/random-numbers') {
        const n = parseInt(req.headers['x-rand-n']); // Читаем заголовок X-Rand-N

        if (isNaN(n)) {
            res.writeHead(400, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Invalid input' }));
            return;
        }

        const count = Math.floor(Math.random() * 6) + 5;

        const randomNumbers = Array.from({ length: count }, () => Math.floor(Math.random() * (2 * n + 1)) - n);

        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ numbers: randomNumbers }));
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Not Found');
    }
});

server.listen(port, () => {
    console.log(`Server running at http://127.0.0.1:${port}`);
})