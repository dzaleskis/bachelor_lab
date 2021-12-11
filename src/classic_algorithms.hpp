#pragma once

enum class ClassicAlgorithm {
    SHELLSORT,
    SHELLSORT_IMPROVED,
    INSERTION_SORT,
    BUBBLE_SORT,
    BRICK_SORT
};

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