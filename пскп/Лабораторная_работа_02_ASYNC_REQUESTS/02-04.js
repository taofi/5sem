const fs = require("fs");
const http = require('http');
const port = 5000;

const server = http.createServer((req, res) => {
    if(req.url === '/xmlhttprequest' && req.method === 'GET'){
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/html');
        fs.readFile("xmlhttprequest.html", function(error,data){
            if(error) {
                res.statusCode = 500;
                res.setHeader('Content-Type', 'text/plain; charset=utf-8');
                res.end('server error');
                return console.log(error);
            }
            res.end(data);
        });
    }
    else if(req.url === '/api/name' && req.method === 'GET'){
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/plain; charset=utf-8');
        res.end('Ворович Кирилл Сергеевич');
    }
    else
    {
        res.statusCode = 404;
        res.setHeader('Content-Type', 'text/html');
        res.end('not found');
    }

});

server.listen(port, () => {
    console.log(`Server running at http://127.0.0.1:${port}/xmlhttprequest`);
});