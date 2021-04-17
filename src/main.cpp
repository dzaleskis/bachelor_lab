#include <iostream>
#include <array>
#include <algorithm>

#include "array_utils.hpp"
#include "algorithms.hpp"
#include "openGA.hpp"

int main(int argc, char* argv[]) {
    std::array<int, 8> gaps {701, 301, 132, 57, 23, 10, 4, 1};
    std::array<int, 50> array;

    array_utils::fill_random(array);
    algorithms::shellsort_improved(array, gaps);

    if (std::is_sorted(array.begin(), array.end())) {
        std::cout << "sorted!"<< std::endl;
    }

    for (auto el: array) {
        std::cout << el  << el << std::endl;
    }
}