#pragma once
#include "gaps.hpp"
#include "pass.hpp"
#include "pdqsort.hpp"
#include "pdqsort2.hpp"

enum class ClassicAlgorithm {
    SHELL_SORT,
    INSERTION_SORT,
    BUBBLE_SORT,
    BRICK_SORT,
    PDQ_SORT,
    PDQ_SORT2,
    STD_SORT,
    MY_SORT,
};

NLOHMANN_JSON_SERIALIZE_ENUM(ClassicAlgorithm, {
{ClassicAlgorithm::SHELL_SORT, "shell"},
{ClassicAlgorithm::INSERTION_SORT, "insertion"},
{ClassicAlgorithm::BUBBLE_SORT, "bubble"},
{ClassicAlgorithm::BRICK_SORT, "brick"},
{ClassicAlgorithm::PDQ_SORT, "pdq"},
{ClassicAlgorithm::PDQ_SORT2, "pdq2"},
{ClassicAlgorithm::STD_SORT, "std"},
{ClassicAlgorithm::MY_SORT, "my"},
})

const std::vector<int> CIURA_GAPS {701,301,132,57,23,10,4,1};

template <typename T>
void shellsort(T & vector, const std::vector<int> & gaps) {
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
void my_sort(T & vector)
{
    bubble_pass(vector, 24);
    insertion_sort(vector);
}

template <typename T>
void run_classic_sort(ClassicAlgorithm algorithm, T & data) {
    switch(algorithm) {
        case ClassicAlgorithm::SHELL_SORT:
            shellsort(data, CIURA_GAPS);
            break;
        case ClassicAlgorithm::INSERTION_SORT:
            insertion_sort(data);
            break;
        case ClassicAlgorithm::BRICK_SORT:
            brick_sort(data);
            break;
        case ClassicAlgorithm::BUBBLE_SORT:
            bubble_sort(data);
            break;
        case ClassicAlgorithm::PDQ_SORT:
            pdqsort(data.begin(), data.end());
            break;
        case ClassicAlgorithm::PDQ_SORT2:
            pdqsort2(data.begin(), data.end());
            break;
        case ClassicAlgorithm::STD_SORT:
            std::sort(data.begin(), data.end());
            break;
        case ClassicAlgorithm::MY_SORT:
            my_sort(data);
            break;
        default:
            throw std::runtime_error("unsupported algorithm type");
    }
}