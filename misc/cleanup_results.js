const fs = require('fs');
const path = require('path');

const args = process.argv.slice(2);
const [filePath, sizeToExclude] = args;

const benchesJson = fs.readFileSync(filePath, 'utf8');
const benches = JSON.parse(benchesJson);

const processedBenches = benches.filter(b => b.size != sizeToExclude);
const outputJson = JSON.stringify(processedBenches, null, 2);

fs.writeFileSync(filePath, outputJson);