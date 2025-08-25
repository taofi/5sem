const sendmail = require('sendmail')();

const EMAIL = 'tojijan189@kazvi.com'

function send(message) {
    return new Promise((resolve, reject) => {
        sendmail({
            from: EMAIL,
            to: EMAIL,
            subject: 'Сообщение от vorovichmoduleLab6',
            html: `<p>${message}</p>`, // HTML-сообщение
        }, (err, reply) => {
            if (err) {
                reject(err);
            } else {
                resolve(reply);
            }
        });
    });
}

module.exports = { send };
