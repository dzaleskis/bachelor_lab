const fs = require('fs')

// this a script for reversing gaps for easier integration in course work paper
const args = process.argv.slice(2);

const [input_file = 'gaps.json', output_file = 'transformed_gaps.json'] = args;

const raw_result = fs.readFileSync(input_file, 'utf8');
const data = JSON.parse(raw_result);
const transformed = data.map(gap_seq => gap_seq.sort((a, b) => a < b));

fs.writeFileSync(output_file, JSON.stringify(transformed));
