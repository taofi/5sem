const http = require('http');

const port = 4000;

const server = http.createServer((req, res) => {
    let body = '';

    req.on('data', chunk => {
        body += chunk.toString();
    });

    req.on('end', () => {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'text/html');
        const responseHTML = `
      <html>
      <head><title>Request Info</title></head>
      <body>
        <h1>Request Information</h1>
        <p><strong>Method:</strong> ${req.method}</p>
        <p><strong>URI:</strong> ${req.url}</p>
        <p><strong>Protocol Version:</strong> ${req.httpVersion}</p>
        <h2>Headers:</h2>
        <pre>${JSON.stringify(req.headers, null, 2)}</pre>
        <h2>Body:</h2>
        <pre>${body}</pre>
      </body>
      </html>
    `;

        res.end(responseHTML);
    });
});

server.listen(port, () => {
    console.log(`Server running at http://127.0.0.1:${port}/`);
});