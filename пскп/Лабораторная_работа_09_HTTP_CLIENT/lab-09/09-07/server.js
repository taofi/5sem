const http = require('http');
const fs = require('fs');

const server = http.createServer((req, res) => {
    if (req.method === 'POST' && req.headers['content-type'].startsWith('multipart/form-data')) {
        const boundary = req.headers['content-type'].split('boundary=')[1];
        const boundaryBuffer = Buffer.from(`--${boundary}`);
        const buffer = [];

        req.on('data', (chunk) => {
            buffer.push(chunk);
        });
        req.on('end', () => {
            const body = Buffer.concat(buffer);
            console.log(body);

            // Разделяем части тела запроса
            const parts = [];
            let startIndex = 0;
            while (startIndex < body.length) {
                const endIndex = body.indexOf(boundaryBuffer, startIndex);
                if (endIndex === -1) break;
                parts.push(body.slice(startIndex, endIndex));
                startIndex = endIndex + boundaryBuffer.length;
            }

            // Обрабатываем каждую часть
            let fileData;
            parts.forEach(part => {
                const contentDisposition = part.indexOf('Content-Disposition: form-data; name="file"');
                if (contentDisposition !== -1) {
                    const contentStart = part.indexOf('\r\n\r\n') + 4; // Пропускаем заголовки
                    const contentEnd = part.lastIndexOf('\r\n');
                    fileData = part.slice(contentStart, contentEnd);
                }
            });

            if (fileData) {
                // Сохраняем файл на диск
                fs.writeFileSync('UploadedFile.png', fileData); // Запись бинарных данных
                console.log('Файл сохранен как UploadedFile.png');

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ status: 'success', filename: 'UploadedFile.png' }));
            } else {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ status: 'error', message: 'Файл не найден в запросе' }));
            }
        });
    } else {
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Only POST method is supported');
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен на порту 5000');
});
