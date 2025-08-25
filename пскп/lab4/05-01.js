const http = require('http');
const url = require('url');
const fs = require('fs');
const data = require('./DB');


const port = 3000;
const db = new data.DB();
let requestNumber = 0;
let commitNumber = 0;


// ---------------------------------------
// Слушатели событий
// ---------------------------------------
db.on('GET', (req, res) => {
    console.log('DB.GET');
    res.end(JSON.stringify(db.select()));
});
db.on('COMMIT', (req, res) => {
    console.log('DB.COMMIT');
    const result = db.commit();
    if (result.error) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: result.error }));
    } else {
        res.end(JSON.stringify(result));
    }
});

db.on('LASTCOMMIT', (req, res) => {
    console.log('DB.LASTCOMMIT');
    res.end(JSON.stringify(db.lastCommit()));
});
db.on('POST', (req, res) => {
    console.log('DB.POST');
    let data = '';
    req.on('data', chunk => {
        data += chunk;
    });
    req.on('end', () => {
        const parsedData = JSON.parse(data);
        const result = db.insert(parsedData);
        if (result.error) {
            res.writeHead(400, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: result.error }));
        } else {
            res.end(JSON.stringify(result));
        }
    });
});

db.on('PUT', (req, res) => {
    console.log('DB.PUT');
    let data = '';
    req.on('data', chunk => {
        data += chunk;
    });
    req.on('end', () => {
        const parsedData = JSON.parse(data);
        const result = db.update(parsedData);
        if (result.error) {
            res.writeHead(400, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: result.error }));
        } else {
            res.end(JSON.stringify(result));
        }
    });
});

db.on('DELETE', (req, res) => {
    console.log('DB.DELETE');
    let data = '';
    req.on('data', chunk => {
        data += chunk;
    });
    req.on('end', () => {
        const parsedData = JSON.parse(data);
        const result = db.delete(parsedData.id);
        if (result.error) {
            res.writeHead(400, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: result.error }));
        } else {
            res.end(JSON.stringify(result));
        }
    });
});

// ---------------------------------------
// Обработка команд cmd
// ---------------------------------------
let statsTimeout;
let stopServerTimeout;
let commitInterval;
let startTime = null;
let endTime = null;
function collectStats(x) {
    if (statsTimeout) clearTimeout(statsTimeout);



    let result = {
        startTime: startTime,
        endTime: null,
        requestCount: 0,
        commitCount: 0
    };

    if (x) {
        let seconds = parseInt(x, 10);
        console.log("Сбор статистики запущен");
        startTime = new Date();
        endTime = null;
        requestNumber = 0;
        commitNumber = 0;
        statsTimeout = setTimeout(() => {
            endTime = new Date();
            result.endTime = endTime;
            result.requestCount = requestNumber;
            result.commitCount = commitNumber;

            console.log("Сбор статистики завершен");
            console.log(result);

        }, seconds * 1000);
        statsTimeout.unref();
    } else {
        endTime = new Date();
        console.log("Сбор статистики остановлен");
    }
    return result;
}


let handleCommand = (command) => {
    let [cmd, param] = command.split(" ");

    switch(cmd) {
        case 'sd': {
            if (stopServerTimeout) clearTimeout(stopServerTimeout);
            if (param) {
                let seconds = parseInt(param, 10);
                stopServerTimeout = setTimeout(() => {
                    console.log("Сервер остановлен");
                    server.close(() => {
                        console.log("Сервер выключен");
                        process.exit(0)
                    })
                }, seconds * 1000);
                stopServerTimeout.unref();
                console.log(`Сервер будет остановлен через ${seconds} секунд`);
            } else {
                console.log("Остановка сервера отменена");
            }
            break;
        }

        case 'sc': {
            if (commitInterval) clearInterval(commitInterval);
            if (param) {
                let seconds = parseInt(param, 10);
                commitInterval = setInterval(() => {
                    console.log("Выполнен commit состояния БД");
                    db.commit();
                    commitNumber ++;
                }, seconds * 1000);
                commitInterval.unref();
                console.log(`Commit состояния БД каждые ${seconds} секунд`);
            } else {
                console.log("Периодический commit остановлен");
            }
            break;
        }

        case 'ss': {
            collectStats(param);
            break;
        }

        default: {
            console.log("Неизвестная команда");
        }
    }
}
process.stdin.setEncoding('utf8');

process.stdin.on('data', data => {
    let command = data.toString().trim();
    handleCommand(command);
})


// ---------------------------------------
// Создание сервера
// ---------------------------------------
let server = http.createServer((request, response) => {
    const parsedUrl = url.parse(request.url);
    if (parsedUrl.pathname === '/') {
        const html = fs.readFileSync('./index.html');
        response.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
        response.end(html);
    } else if (parsedUrl.pathname === '/api/db') {
        db.emit(request.method, request, response);
        requestNumber++;
    }
    else if (parsedUrl.pathname === '/api/db/commit') {
        db.emit('COMMIT', request, response);
        commitNumber ++;
    }
    else if (parsedUrl.pathname === "/api/ss" && request.method === 'GET'){
        let stats = {
            startT: startTime ? startTime.toISOString() : '',
            endT: endTime ? endTime.toISOString() : '',
            requestNumber,
            commitNumber
        }
        response.writeHead(200, {'Content-Type': 'application/json; charset=utf8'});
        response.end(JSON.stringify(stats));
    }

}).listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});