#pragma once

namespace iterator_utils {
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
}