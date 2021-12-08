#pragma once
#include <vector>
#include <algorithm>

const std::vector<int> CIURA_GAPS {90927,40412,17961,7983,3548,1577,701,301,132,57,23,10,4,1};
const std::vector<int> TOKUDA_GAPS {68178,30301,13467,5985,2660,1182,525,233,103,46,20,9,4,1};
const std::vector<int> SEDGEWICK_GAPS {64769,36289,16001,8929,3905,2161,929,505,209,109,41,19,5,1};

const std::vector<std::vector<int>> ALL_GAPS { CIURA_GAPS, TOKUDA_GAPS, SEDGEWICK_GAPS };