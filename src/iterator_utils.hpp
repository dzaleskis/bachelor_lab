#pragma once

#include <array>
#include <functional>
#include <algorithm>

namespace utils {
    template<class in_it>
    int count_inversions(in_it begin, in_it end) {
        in_it before_end = std::prev(end);
        int inv_count = 0;

        for (; begin != before_end; ++begin) {
            for (in_it after_begin = std::next(begin); after_begin != end; ++after_begin) {
                if ((*begin) > (*after_begin)) {
                    inv_count++;
                }
            }
        }

        return inv_count;
    }
}