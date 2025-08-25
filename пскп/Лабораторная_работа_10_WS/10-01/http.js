const http = require("http");
const fs = require("fs");

const httpServer = http
    .createServer((req, res) => {
        if (req.method === "GET" && req.url === "/start") {
            const htmlStream = fs.createReadStream(
                "./index.html"
            );
            htmlStream.pipe(res);
            return;
        }

        res.statusCode = 400;
        res.end();
    })
    .listen(3000, () => {
        console.log('Server running at http://localhost:3000/');
    });