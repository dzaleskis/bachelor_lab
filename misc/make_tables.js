const fs = require('fs');
const path = require('path');

const args = process.argv.slice(2);
const [filePath, dataset] = args;
const fileName = path.parse(filePath).base;

const benchesJson = fs.readFileSync(filePath, 'utf8');
const benches = JSON.parse(benchesJson);

const benchesBySize = benches.reduce((acc, bench) => {
    if (!acc[bench.size]) {
        acc[bench.size] = [];
    }

    acc[bench.size].push(bench);

    return acc;
}, {});

const roundToTwo = (num) => +(Math.round(num + "e+2")  + "e-2");

const getErrorMargin = (stat) => {
    const amount = stat.iqr/stat.median;
    return roundToTwo(amount * 100);

}

Object.entries(benchesBySize).forEach(([size, benches]) => {
    const latexTable = 
`
\\begin{table}[H]
  \\caption{Efektyvumo tyrimo rezultatai, kai $N = ${size}$}
  \\label{bench_${dataset}_${size}}
  \\centering
  \\begin{tabular}{@{}llll@{}}
  Algoritmas & Duomenų rinkinys & Palyginimai & Priskyrimai \\\\ \\midrule
${benches.map(b => `  ${b.algorithm} & \\verb|${b.distribution}| & ${b.comparisons.median} & ${b.assignments.median} \\\\`).join('\n')}
  \\bottomrule
  \\end{tabular}
\\end{table}
`;

    console.log(latexTable);
});
