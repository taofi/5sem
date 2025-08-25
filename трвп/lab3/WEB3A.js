const http = require('http');
const url = require('url');

// Функция для чтения и парсинга куки из заголовков
function parseCookies(request) {
    const list = {};
    const cookieHeader = request.headers.cookie;

    if (!cookieHeader) return list;

    cookieHeader.split(';').forEach(cookie => {
        const parts = cookie.split('=');
        list[parts[0].trim()] = decodeURIComponent(parts[1]);
    });

    return list;
}

// Функция для создания и обновления куки
function createCookie(name, value) {
    return `${name}=${value}; HttpOnly; Path=/`;
}

// Обработка запросов
const server = http.createServer((req, res) => {
    if (req.method === 'POST') {
        let body = '';

        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', () => {
            const data = JSON.parse(body);
            const cookies = parseCookies(req);

            // Инициализация накопленных сумм или их извлечение из куки
            let sx = parseInt(cookies.sx || 0);
            let sy = parseInt(cookies.sy || 0);
            let count = parseInt(cookies.count || 0);
            sx = sx === null ? 0 : sx;
            sy = sy === null ? 0 : sy;
            count = count === null ? 0 : count;
            const { x, y } = data;
            if(x == null || y == null) {
                count = 0;
                sx = 0;
                sy = 0;
            }
            console.log(`${x} ${y} ${count}`)
            // Накапливаем значения
            sx += x;
            sy += y;

            // Если запрос кратен 5, сбрасываем накопленные суммы после ответа
            if (count !== 0 && count  % 5 === 0) {
                count += 1;
                res.writeHead(200, {
                    'Set-Cookie': [
                        createCookie('sx', 0),
                        createCookie('sy', 0),
                        createCookie('count', count)
                    ],
                    'Content-Type': 'application/json'
                });
                res.end(JSON.stringify({ sx, sy }));
                sx = 0;
                sy = 0;
            } else {
                count += 1;
                res.writeHead(200, {
                    'Set-Cookie': [
                        createCookie('sx', sx),
                        createCookie('sy', sy),
                        createCookie('count', count)
                    ],
                    'Content-Type': 'application/json'
                });
                res.end(JSON.stringify({ sx, sy }));
            }

        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Not Found');
    }
});

server.listen(3000, () => {
    console.log('Server listening on port 3000');
});
