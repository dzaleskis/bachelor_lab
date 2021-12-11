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