#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

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
void insertion_pass(T & container, int gap) {
    std::size_t n = container.size();

    for (int i = gap; i < n; ++i) {
        if (container[i - gap] > container[i]) {
            auto temp = container[i];
            int j = i;

            do {
                container[j] = container[j - gap];
                j -= gap;

            } while (j >= gap && container[j - gap] > temp);

            container[j] = temp;
        }
    }
}

template <typename T>
void bubble_pass(T & container, int gap) {
    std::size_t n = container.size();

    for (int i = 0; i < n - gap; ++i) {
        if (container[i] > container[i + gap]){
            std::swap(container[i], container[i + gap]);
        }
    }
}

template <typename T>
void brick_pass(T & container, int gap) {
    std::size_t n = container.size();
    int increment = gap * 2;

    for (int i = gap; i < n - gap; i+= increment) {
        if (container[i] > container[i + gap]) {
            std::swap(container[i], container[i + gap]);
        }
    }

    for (int i = 0; i < n - gap; i+= increment) {
        if (container[i] > container[i + gap]) {
            std::swap(container[i], container[i + gap]);
        }
    }
}

template <typename T>
void shake_pass(T & container,  int gap) {
    std::size_t n = container.size();

    // perform bubble pass forward
    for (int i = 0; i < n - gap; ++i) {
        if (container[i] > container[i+gap]) {
            std::swap(container[i], container[i + gap]);
        }
    }

    // perform bubble pass backward
    for (int i = n - gap - 1; i >= 0; --i) {
        if (container[i] > container[i+gap]) {
            std::swap(container[i], container[i + gap]);
        }
    }
}
