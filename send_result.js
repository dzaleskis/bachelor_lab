const fs = require('fs')
const https = require('https');

const args = process.argv.slice(2);
const [data_path = 'result.json'] = args;

const data = fs.readFileSync(data_path, 'utf8');

const options = {
    host: 'course-project-next.vercel.app',
    port: 443,
    path: '/api/results',
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        'Content-Length': data.length
    }
};

callback = function(response) {
    console.log(`responded with status: ${response.statusCode}`)

    req.on('error', (e) => {
        console.error(e);
    });

    response.on('end', function () {
        console.log('result sent successfully');
    });
}

let req = https.request(options, callback);
req.write(data);
req.end();
