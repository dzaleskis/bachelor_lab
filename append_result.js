const fs = require('fs')

// this a script for transforming result output to ingestable gaps and appending to existing gaps
const args = process.argv.slice(2);
if (args.length < 1) {
    console.error("input args: <input_file> <output_file?>");
    process.exit(-1);
}

const [input_file, output_file = 'gaps.json'] = args;

const raw_result = fs.readFileSync(input_file, 'utf8');
const data = JSON.parse(raw_result);
const transformed = data.results.map(res => res.sobj_solution.gaps);

let existing_gaps = [];

try {
    const raw_existing = fs.readFileSync(output_file, 'utf8');
    existing_gaps = JSON.parse(raw_existing);
} catch {}

fs.writeFileSync(output_file, JSON.stringify(existing_gaps.concat(transformed)))
