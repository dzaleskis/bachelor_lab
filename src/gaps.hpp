#pragma once
#include <vector>

const std::vector<int> CIURA_SMALL_GAPS {701,301,132,57,23,10,4,1};
const std::vector<int> TOKUDA_SMALL_GAPS {525,233,103,46,20,9,4,1};
const std::vector<int> SEDGEWICK_SMALL_GAPS {929,505,209,109,41,19,5,1};
const std::vector<int> INCERPI_SEDGEWICK_SMALL_GAPS {861,336,112,48,21,7,3,1};
const std::vector<int> PRATT_SMALL_GAPS {12,9,8,6,4,3,2,1};

const std::vector<int> GEOMETRIC_225_SMALL_GAPS {292, 130, 58, 26, 12, 6, 3, 1};
const std::vector<int> GEOMETRIC_235_SMALL_GAPS {396, 169, 72, 31, 13, 6, 3, 1};
const std::vector<int> GEOMETRIC_245_SMALL_GAPS {530, 217, 89, 37, 15, 7, 3, 1};
const std::vector<int> GEOMETRIC_255_SMALL_GAPS {702, 275, 108, 43, 17, 7, 3, 1};
const std::vector<int> GEOMETRIC_265_SMALL_GAPS {918, 347, 131, 50, 19, 8, 3, 1};

const std::vector<std::vector<int>> SMALL_GAPS {
    CIURA_SMALL_GAPS, TOKUDA_SMALL_GAPS, SEDGEWICK_SMALL_GAPS, INCERPI_SEDGEWICK_SMALL_GAPS, PRATT_SMALL_GAPS,
    GEOMETRIC_225_SMALL_GAPS, GEOMETRIC_235_SMALL_GAPS, GEOMETRIC_245_SMALL_GAPS, GEOMETRIC_255_SMALL_GAPS, GEOMETRIC_265_SMALL_GAPS
    };

const std::vector<int> CIURA_BIG_GAPS {90927,40412,17961,7983,3548,1577,701,301,132,57,23,10,4,1};
const std::vector<int> TOKUDA_BIG_GAPS {68178,30301,13467,5985,2660,1182,525,233,103,46,20,9,4,1};
const std::vector<int> SEDGEWICK_BIG_GAPS {64769,36289,16001,8929,3905,2161,929,505,209,109,41,19,5,1};
const std::vector<int> INCERPI_SEDGEWICK_BIG_GAPS {91511, 86961,33936,13776,4592,1968,861,336,112,48,21,7,3,1};
const std::vector<int> PRATT_BIG_GAPS {36,32,27,24,18,16,12,9,8,6,4,3,2,1};

const std::vector<int> GEOMETRIC_195_BIG_GAPS {5895, 3023, 1551, 795, 408, 210, 108, 55, 29, 15, 8, 4, 2, 1};
const std::vector<int> GEOMETRIC_205_BIG_GAPS {11293, 5509, 2688, 1311, 640, 312, 153, 75, 37, 18, 9, 5, 3, 1};
const std::vector<int> GEOMETRIC_215_BIG_GAPS {20975, 9756, 4538, 2111, 982, 457, 213, 99, 46, 22, 10, 5, 3, 1};
const std::vector<int> GEOMETRIC_225_BIG_GAPS {37877, 16835, 7482, 3326, 1478, 657, 292, 130, 58, 26, 12, 6, 3, 1};
const std::vector<int> GEOMETRIC_235_BIG_GAPS {66663, 28368, 12072, 5137, 2186, 931, 396, 169, 72, 31, 13, 6, 3, 1};

const std::vector<std::vector<int>> BIG_GAPS {
        CIURA_BIG_GAPS, TOKUDA_BIG_GAPS, SEDGEWICK_BIG_GAPS, INCERPI_SEDGEWICK_BIG_GAPS, PRATT_BIG_GAPS,
        GEOMETRIC_195_BIG_GAPS, GEOMETRIC_205_BIG_GAPS, GEOMETRIC_215_BIG_GAPS, GEOMETRIC_225_BIG_GAPS, GEOMETRIC_235_BIG_GAPS
};

const auto ALL_GAPS = BIG_GAPS;
const auto GAPS_SIZE = ALL_GAPS.front().size();

