const fs = require('fs')

// this a script for reversing gaps for easier integration in course work paper
const args = process.argv.slice(2);

const [input_file = 'gaps.json', output_file = 'transformed_gaps.json'] = args;

const raw_result = fs.readFileSync(input_file, 'utf8');
const data = JSON.parse(raw_result);
data.forEach(gap_seq => gap_seq.sort());

fs.writeFileSync(output_file, JSON.stringify(data));
