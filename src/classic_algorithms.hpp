#pragma once
#include <array>
#include "gaps.hpp"
#include "pass.hpp"
#include "pdqsort.hpp"
#include "pdqsort2.hpp"

template <typename T>
void insertion_sort(T & data)
{
    const std::size_t size = data.size();

    for (int i = 1; i < size; i++)
    {
        if (data[i - 1] > data[i]) {
            auto temp = data[i];
            int j = i - 1;

            do {
                data[j + 1] = data[j];
                j -= 1;

            } while (j >= 0 && data[j] > temp);

            data[j + 1] = temp;
        }
    }
}

template <typename T, std::size_t N>
static void shell_sort(T & data, const std::array<int, N> & gaps) {
    const std::size_t size = data.size();

    for (int gap: gaps) {
        for (int i = gap; i < size; i++)
        {
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

