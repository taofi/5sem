const WebSocket = require("ws");

const ws = new WebSocket('ws://localhost:4000/');

let n = 0;

ws.on('open', ()=>{
  ws.on('message', (message) => {
    console.log(`${message}`);
  })
  setTimeout(()=>{ws.close()}, 25000);
  setInterval(() => {
    ws.send(`10-01-client: ${n}`);
    n++;
  }, 3000);

});


