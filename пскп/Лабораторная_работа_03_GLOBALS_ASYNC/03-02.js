const http = require('http');
const url = require('url');
const fs = require("fs");

function factorial(n) {
    if (n === 0 || n === 1) return 1;
    return n * factorial(n - 1);
}
const port =5000;

const server = http.createServer((req, res) => {
    if (req.method === 'GET' && req.url.startsWith('/fact')) {
        let q = url.parse(req.url, true);
        let qdata = q.query;
        let k = parseInt(qdata.k);
        if (!isNaN(k) && k >= 0)
        {
            let fact = factorial(k);
            const response = {
                k: k,
                fact: fact
            };

            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(response));
        }else {
            res.writeHead(400, { 'Content-Type': 'text/plain' });
            res.end('Invalid parameter k.');
        }
    }
    else if(req.url === '/fetch' && req.method === 'GET'){
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/html');
        fs.readFile("index.html", function(error,data){
            if(error) {  // если возникла ошибка
                return console.log(error);
            }
            res.end(data.toString());
        });
    }else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Not Found');
    }
});


server.listen(port, () => {
    console.log(`Сервер запущен на http://localhost:${port}/fetch`);
});