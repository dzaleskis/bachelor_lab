#pragma once
#include <vector>
#include <set>

std::vector<int> get_geometric_gaps(const int size, const double q) {
    std::set<int> gaps;
    double currentGap = 1;

    while (currentGap < size) {
        gaps.insert(floor(currentGap));
        currentGap *= q;
    }

    return std::vector<int>(gaps.begin(), gaps.end());
}