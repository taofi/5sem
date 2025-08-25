const http = require('http');
const readline = require('readline');

const states = ['norm', 'stop', 'test', 'idle'];
let currentState = 'idle';

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
function exitApp() {
    rl.close();
    server.close(() => {
        process.exit();
    });
}

function isValidState(state) {
    return states.includes(state);
}

function updateStatePrompt() {
    rl.setPrompt(`${currentState} --> `);
    rl.prompt();
}

function handleStateChange(newState) {
    if (isValidState(newState)) {
        console.log(`reg = ${currentState} -> ${newState}`);
        currentState = newState;
    } else {
        console.log(newState);
    }
    updateStatePrompt();
}

function processInput(input) {
    const trimmedInput = input.trim();

    switch (trimmedInput) {
        case 'exit':
            exitApp();
            break;
        default:
            handleStateChange(trimmedInput);
            break;
    }
}

rl.on('line', (input) => {
    processInput(input);
});

const server = http.createServer((req, res) => {
    res.writeHead(200, { 'Content-Type': 'text/html' });
    res.end(currentState);
});

server.listen(5000, () => {
    console.log('Сервер запущен на http://localhost:5000');
    updateStatePrompt();
});