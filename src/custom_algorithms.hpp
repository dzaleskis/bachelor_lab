#pragma once
#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include "pass.hpp"

enum class CustomAlgorithm {
    ONE_TWO_SORT,
};

template <typename T>
void one_two_sort(T & data)
{
    shake_pass(data, 19);
    brick_pass(data, 2);
    insertion_pass(data, 1);
}

template <typename T>
void run_custom_sort(CustomAlgorithm algorithm, T & data) {
    switch(algorithm) {
        case CustomAlgorithm::ONE_TWO_SORT:
            one_two_sort(data);
            break;
        default:
            throw std::runtime_error("unsupported algorithm type");
    }
}