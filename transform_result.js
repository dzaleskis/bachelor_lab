const fs = require('fs')

// this a script for transforming GA result output to ingestable gaps
const args = process.argv.slice(2);
if (args.length < 1) {
    console.error("input args: <input_file> <output_file?>");
    process.exit(-1);
}

const [input_file, output_file = 'gaps.json'] = args;

const raw = fs.readFileSync(input_file, 'utf8');
const data = JSON.parse(raw);
const transformed = data.results.map(res => res.solution.gaps);
fs.writeFileSync(output_file, JSON.stringify(transformed))
