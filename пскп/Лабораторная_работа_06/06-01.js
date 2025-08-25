const sendmail = require('sendmail')({silent: true}); // тихий режим работы
const http = require('http');
const querystring = require('querystring');
const fs = require('fs');




const server = http.createServer((req, res) => {
    if (req.method === 'GET') {
        fs.readFile('index.html', 'utf8', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('Ошибка при загрузке страницы');
                return;
            }
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end(data);
        });

    } else if (req.method === 'POST') {
        let body = '';

        req.on('data', chunk => {
            body += chunk.toString();
        });

        req.on('end', () => {
            const postData = querystring.parse(body);
            const { sender, receiver, subject, message } = postData;

            if (!sender || !receiver || !subject || !message) {
                res.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('Ошибка: все поля обязательны для заполнения!');
                return;
            }
            console.log(1);
            sendmail({
                from: sender,
                to: receiver,
                subject: subject,
                html: message,
            }, (err, reply) => {
                if (err) {
                    console.error(err);
                    res.writeHead(500, { 'Content-Type': 'text/plain; charset=utf-8' });
                    res.end('Ошибка при отправке письма.');
                } else {
                    console.log('Email sent:', reply);
                    res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                    res.end('<h1>Письмо отправлено успешно!</h1>');
                }
            });
            console.log(2);
        });
    }
});

server.listen(3000, () => {
    console.log('Сервер запущен на http://localhost:3000');
});