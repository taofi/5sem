const http = require('http');
const fs = require('fs');

const boundary = 'lb9t7-lb9t7';
let body = `--${boundary}\r\n`;
body += 'Content-Disposition: form-data; name="file"; filename="MyFile.png"\r\n';
body += 'Content-Type: application/octet-stream\r\n\r\n'; // Указываем тип содержимого файла

const options = {
  host: 'localhost',
  path: '/',
  port: 5000,
  method: 'POST',
  headers: {
    'Content-Type': `multipart/form-data; boundary=${boundary}`

  }
};

const req = http.request(options, (res) => {
  let data = '';
  res.on('data', (chunk) => { data += chunk; });
  res.on('end', () => {
    console.log('http.response: end: length body =', Buffer.byteLength(data));
    console.log('Response:', data);
  });
});

req.on('error', (e) => {
  console.log('http.request: error:', e.message);
});

// Отправляем начальную часть тела запроса
req.write(body)
let stream = new fs.createReadStream('./MyFile.png');
stream.on('data', (chunk) => {
  req.write(chunk); // Отправляем данные файла порциями
  console.log(Buffer.byteLength(chunk));
});
stream.on('end', () => {
  req.end(`\r\n--${boundary}--\r\n`); // Завершаем запрос
});
