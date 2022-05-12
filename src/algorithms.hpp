#pragma once
#include <array>
#include "gaps.hpp"
#include "pass.hpp"
#include "pdqsort.hpp"
#include "pdqsort2.hpp"

template <typename T>
inline void insertion_sort(T & data)
{
    const std::size_t size = data.size();

    for (int i = 1; i < size; i++) {
        if (data[i - 1] > data[i]) {
            auto temp = data[i];
            int j = i;

            do {
                data[j] = data[j - 1];
                j -= 1;
            } while (j >= 1 && data[j - 1] > temp);

            data[j] = temp;
        }
    }
}

template <typename T, std::size_t N>
static inline void shell_sort(T & data, const std::array<int, N> & gaps) {
    const std::size_t size = data.size();

    for (int gap: gaps) {
        for (int i = gap; i < size; i++) {
            if (data[i - gap] > data[i]) {
                auto temp = data[i];
                int j = i;

                do {
                    data[j] = data[j - gap];
                    j -= gap;
                } while (j >= gap && data[j - gap] > temp);

                data[j] = temp;
            }
        }
    }
}

template <typename T>
inline void ciura_shell_sort(T& data) {
    const std::array<int, 8> gaps {701,301,132,57,23,10,4,1};
    shell_sort(data, gaps);
}

template <typename T>
inline void tokuda_shell_sort(T& data) {
    const std::array<int, 8> gaps {525,233,103,46,20,9,4,1};
    shell_sort(data, gaps);
}

template <typename T>
inline void sedgewick_shell_sort(T& data) {
    const std::array<int, 8> gaps {929,505,209,109,41,19,5,1};
    shell_sort(data, gaps);
}

template <typename T>
inline void incerpi_shell_sort(T& data) {
    const std::array<int, 8> gaps {861,336,112,48,21,7,3,1};
    shell_sort(data, gaps);
}

template <typename T>
inline void pratt_shell_sort(T& data) {
    const std::array<int, 8> gaps {12,9,8,6,4,3,2,1};
    shell_sort(data, gaps);
}

template <typename T>
void standard_sort(T & data) {
    std::sort(data.begin(), data.end());
}

// 128 sorts

// id: 7850
template <typename T>
inline void genetic_sort_128_1(T& data) {
    bubble_pass(data, 52);
    insertion_pass(data, 6);
    insertion_pass(data, 1);
}

// id: 7860
template <typename T>
inline void genetic_sort_128_2(T& data) {
    shake_pass(data, 92);
    insertion_pass(data, 7);
    insertion_pass(data, 1);
}

// id: 8690
template <typename T>
inline void genetic_sort_128_3(T& data) {
    bubble_pass(data, 76);
    insertion_pass(data, 10);
    insertion_pass(data, 1);
}

// 1024 sorts

// id: 8745
template <typename T>
inline void genetic_sort_1024_1(T& data) {
    const std::array<int, 5> gaps {162,40,17,4,1};
    shell_sort(data, gaps);
}

// id: 8755
template <typename T>
inline void genetic_sort_1024_2(T& data) {
    const std::array<int, 4> gaps {155,19,5,1};
    shell_sort(data, gaps);
}

// id: 8760
template <typename T>
inline void genetic_sort_1024_3(T& data) {
    const std::array<int, 4> gaps {97,26,8,1};
    shell_sort(data, gaps);
}

// 8192 sorts

// id: 9423
template <typename T>
inline void genetic_sort_8192_1(T& data) {
    const std::array<int, 8> gaps {1794,615,264,104,25,11,4,1};
    shell_sort(data, gaps);
}

// id: 9431
template <typename T>
inline void genetic_sort_8192_2(T& data) {
    insertion_pass(data, 1794);
    brick_pass(data, 1152);

    const std::array<int, 6> gaps {356,104,25,11,4,1};
    shell_sort(data, gaps);
}

// id: 9448
template <typename T>
inline void genetic_sort_8192_3(T& data) {
    const std::array<int, 6> gaps {760,187,53,19,6,1};
    shell_sort(data, gaps);
}

template <typename T>
inline void experimental_shell_sort(T & data) {
    int n = data.size();

    // perform a bubble pass with gap 32
    const int gap = 32;
    for (int i = gap; i < n; ++i) {
        if (data[i - gap] > data[i]) {
            std::swap(data[i - gap], data[i]);
        }
    }

    // keep track of min index for later
    int min_index = 0;
    int bound = std::min(n, gap);
    // the smallest element will be within [0, bound), since we sorted with gap 32 before
    for (int i = 1; i < bound; ++i) {
        if (data[min_index] > data[i]) {
            min_index = i;
        }
    }
    // swap the smallest element into place
    std::swap(data[0], data[min_index]);

    // perform insertion pass separately, avoid bounds check
    for (int i = 1; i < n; ++i) {
        if (data[i - 1] > data[i]) {
            auto temp = data[i];
            int j = i;

            do {
                data[j] = data[j - 1];
                j -= 1;
            } while (data[j - 1] > temp);

            data[j] = temp;
        }
    }
}
