#pragma once
#include "gaps.hpp"

enum class ClassicAlgorithm {
    SHELLSORT,
    SHELLSORT_IMPROVED,
    INSERTION_SORT,
    INSERTION_SORT_IMPROVED,
    BUBBLE_SORT,
    BRICK_SORT
};

const std::vector<int> CIURA_GAPS {701,301,132,57,23,10,4,1};

template <typename T>
void shellsort(T & vector, const std::vector<int> & gaps) {
    const std::size_t size = vector.size();

    for (int gap: gaps) {
        for (int i = gap; i < size; i++)
        {
            auto temp = vector[i];
            int j = i;

            for (j; j >= gap && vector[j - gap] > temp; j -= gap)
            {
                vector[j] = vector[j - gap];
            }

            vector[j] = temp;
        }
    }
}

template <typename T>
void shellsort_improved(T & vector, const std::vector<int> & gaps) {
    const std::size_t size = vector.size();

    for (int gap: gaps) {
        for (int i = gap; i < size; i++)
        {
            if (vector[i - gap] > vector[i]) {
                auto temp = vector[i];
                int j = i;

                do {
                    vector[j] = vector[j - gap];
                    j -= gap;

                } while (j >= gap && vector[j - gap] > temp);

                vector[j] = temp;
            }
        }
    }
}

template <typename T>
void bubble_sort(T & vector)
{
    const std::size_t size = vector.size();

    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (vector[j] > vector[j + 1]) {
                std::swap(vector[j], vector[j + 1]);
            }
        }
    }
}

template <typename T>
void insertion_sort(T & vector)
{
    const std::size_t size = vector.size();

    for (int i = 1; i < size; i++)
    {
        auto temp = vector[i];
        int j = i - 1;

        for (; j >= 0 && vector[j] > temp; j -= 1)
        {
            vector[j + 1] = vector[j];
        }

        vector[j + 1] = temp;
    }
}

template <typename T>
void insertion_sort_improved(T & vector)
{
    const std::size_t size = vector.size();

    for (int i = 1; i < size; i++)
    {
        if (vector[i-1] > vector[i]) {
            auto temp = vector[i];
            int j = i - 1;

            do {
                vector[j + 1] = vector[j];
                j -= 1;

            } while (j >= 0 && vector[j] > temp);

            vector[j + 1] = temp;
        }
    }
}

template <typename T>
void brick_sort(T & vector)
{
    const std::size_t size = vector.size();
    bool isSorted = false;

    while (!isSorted) {
        isSorted = true;

        for (int i = 1; i < size - 1; i += 2) {
            if (vector[i] > vector[i + 1]) {
                std::swap(vector[i], vector[i + 1]);
                isSorted = false;
            }
        }

        for (int i = 0; i < size - 1; i += 2) {
            if (vector[i] > vector[i + 1]) {
                std::swap(vector[i], vector[i + 1]);
                isSorted = false;
            }
        }
    }
}

template <typename T>
void run_classic_sort(ClassicAlgorithm algorithm, T & data) {
    switch(algorithm) {
        case ClassicAlgorithm::SHELLSORT:
            shellsort(data, CIURA_GAPS);
            break;
        case ClassicAlgorithm::SHELLSORT_IMPROVED:
            shellsort_improved(data, CIURA_GAPS);
            break;
        case ClassicAlgorithm::INSERTION_SORT:
            insertion_sort(data);
            break;
        case ClassicAlgorithm::INSERTION_SORT_IMPROVED:
            insertion_sort_improved(data);
            break;
        case ClassicAlgorithm::BRICK_SORT:
            brick_sort(data);
            break;
        case ClassicAlgorithm::BUBBLE_SORT:
            bubble_sort(data);
            break;
        default:
            throw std::runtime_error("unsupported algorithm type");
    }
}