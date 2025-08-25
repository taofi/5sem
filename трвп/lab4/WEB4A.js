const http = require('http');

// Хранилище сессий на сервере
const sessions = {};
let sessionCounter = 0;

function getSession(req, res) {
    let sessionId = req.headers.cookie && req.headers.cookie.split('=')[1];

    if (!sessionId || !sessions[sessionId]) {
        sessionId = (sessionCounter++).toString();
        sessions[sessionId] = { sx: 0, sy: 0, count: 0 };

        res.setHeader('Set-Cookie', `sessionId=${sessionId}; HttpOnly; Path=/; Max-Age=3600`);
    }

    return sessions[sessionId];
}

const server = http.createServer((req, res) => {
    if (req.method === 'POST') {
        let body = '';

        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', () => {
            const data = JSON.parse(body);
            const session = getSession(req, res);

            const { x, y } = data;

            if (x == null || y == null) {
                session.sx = 0;
                session.sy = 0;
                session.count = 0;
            } else {
                session.sx += x;
                session.sy += y;
                session.count += 1;
            }

            const response = { sx: session.sx, sy: session.sy };

            if (session.count % 5 === 0) {
                session.sx = 0;
                session.sy = 0;
            }

            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(response));
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Not Found');
    }
});

server.listen(3000, () => {
    console.log('Server listening on port 3000');
});
