const { log } = require("console");
const http = require("http");
const { parseString } = require("xml2js");
const xmlbuilder = require("xmlbuilder");

const xmlBody = xmlbuilder.create('request', { version: '1.0', encoding: 'UTF-8' })
    .att('id', '28')
    .ele('x', { value: '1' }).up()
    .ele('x', { value: '2' }).up()
    .ele('m', { value: 'a' }).up()
    .ele('m', { value: 'b' }).up()
    .ele('m', { value: 'c' }).up()
    .end({ pretty: true });

const options = {
  host: "localhost",
  path: "/",
  port: 5000,
  method: "POST",
  headers: {
    "Content-Type": "application/xml", "accept" : "text/xml"
  },
};

const req = http.request(options, (res) => {
  log(`status code: ${res.statusCode}`);

  let body = "";

  res.on("data", (chunk) => {
    body += chunk;
  });

  res.on("end", () => {
    log(body);
    parseString(body, (err, str)=>{
      if(err) log('xml parse error');
      else{
        log('str = ', str);

        const sumResult = str.response.sum[0].$?.result;
        console.log("sum result =", sumResult);
        const concatResult = str.response.concat[0].$?.result;
        console.log("concat result =", concatResult);


      }
    })
  });
});


req.end(xmlBody.toString({pretty:true}));
