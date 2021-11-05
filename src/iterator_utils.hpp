#pragma once

#include <array>
#include <functional>
#include <algorithm>
#include "random_utils.hpp"

namespace iterator_utils {
    template<class forward_it>
    void fill_random(forward_it begin, forward_it end, int min = INT_MIN, int max = INT_MAX) {
        std::generate(begin, end, [&] () {
            return random_utils::get_random_int(min, max);
        });
    }

    template<class forward_it>
    void fill_sorted_ascending(forward_it begin, forward_it end, int min = INT_MIN, int max = INT_MAX) {
        fill_random(begin, end, min, max);
        std::sort(begin, end);
    }

    template<class forward_it>
    void fill_sorted_descending(forward_it begin, forward_it end, int min = INT_MIN, int max = INT_MAX) {
        fill_random(begin, end, min, max);
        std::sort(begin, end);
    }

    template<class in_it, class out_it>
	out_it copy_every_other_n( in_it b, in_it e, out_it r, size_t n) {
		for (size_t i = std::distance(b,e) / n; i--; std::advance(b,n)){
			*r = *b;
			std::advance(r,n);
		}
		return r;
	}

	template<class in_it, class out_it>
	out_it copy_every_n( in_it b, in_it e, out_it r, size_t n) {
		for (size_t i = std::distance(b,e) / n; i--; std::advance(b,n)){
			*r++ = *b;
		}
		return r;
	}

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