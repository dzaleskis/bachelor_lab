#pragma once

#include <vector>

enum class PassType {
    INSERTION,
    BRICK,
    BUBBLE,
    SHAKE
};

const std::vector<PassType> ALL_PASSES { PassType::INSERTION, PassType::BRICK, PassType::BUBBLE, PassType::SHAKE };

NLOHMANN_JSON_SERIALIZE_ENUM(PassType, {
    {PassType::INSERTION, "insertion"},
    {PassType::BRICK, "brick"},
    {PassType::SHAKE, "shake"},
    {PassType::BUBBLE, "bubble"},
})

template <typename T>
inline void insertion_pass(T & data, int gap) {
    std::size_t n = data.size();

    for (int i = gap; i < n; ++i) {
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

template <typename T>
inline void bubble_pass(T & data, int gap) {
    std::size_t n = data.size();

    for (int i = gap; i < n; ++i) {
        if (data[i - gap] > data[i]){
            std::swap(data[i - gap], data[i]);
        }
    }
}

template <typename T>
inline void brick_pass(T & data, int gap) {
    std::size_t n = data.size();
    int increment = gap * 2;

    for (int i = gap; i < n - gap; i+= increment) {
        if (data[i] > data[i + gap]) {
            std::swap(data[i], data[i + gap]);
        }
    }

    for (int i = 0; i < n - gap; i+= increment) {
        if (data[i] > data[i + gap]) {
            std::swap(data[i], data[i + gap]);
        }
    }
}

template <typename T>
inline void shake_pass(T & data,  int gap) {
    std::size_t n = data.size();

    // perform bubble pass forward
    for (int i = 0; i < n - gap; ++i) {
        if (data[i] > data[i+gap]) {
            std::swap(data[i], data[i + gap]);
        }
    }

    // perform bubble pass backward
    for (int i = n - gap - 1; i >= 0; --i) {
        if (data[i] > data[i+gap]) {
            std::swap(data[i], data[i + gap]);
        }
    }
}
