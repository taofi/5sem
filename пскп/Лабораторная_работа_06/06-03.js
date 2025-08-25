const http = require('http');
const fs = require('fs');
const querystring = require('querystring');
//const { send } = require('./vorovichmoduleLab6/vorovichmoduleLab6');
const {send} = require('vorovichmoduleLab6');

const server = http.createServer((req, res) => {
    if (req.method === 'GET') {

            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end("<h1>Отправить сообщение</h1>\n" +
                "    <form method=\"POST\" action=\"/\">\n" +
                "        <label for=\"message\">Сообщение:</label><br>\n" +
                "        <textarea id=\"message\" name=\"message\" rows=\"4\" cols=\"50\" placeholder=\"Введите текст сообщения\"></textarea><br><br>\n" +
                "        <button type=\"submit\">Отправить</button>\n" +
                "    </form>");
    } else if (req.method === 'POST') {
        let body = '';
        req.on('data', chunk => {
            body += chunk.toString();
        });
        req.on('end', async () => {
            const postData = querystring.parse(body);
            const { message } = postData;

            if (!message) {
                res.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('Ошибка: поле "сообщение" обязательно для заполнения!');
                return;
            }

            try {
                const result = await send(message);
                console.log('Email sent:', result);
                res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                res.end('<h1>Письмо успешно отправлено!</h1>');
            } catch (error) {
                console.error('Ошибка отправки письма:', error);
                res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('Ошибка отправки письма.');
            }
        });
    }
});

server.listen(3000, () => {
    console.log('Сервер запущен на http://localhost:3000');
});
