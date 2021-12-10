#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

enum class PassType {
    INSERTION,
    INSERTION_IMPROVED,
    BRICK,
    BUBBLE,
    SHAKE
};

NLOHMANN_JSON_SERIALIZE_ENUM(PassType, {
    {PassType::INSERTION, "insertion"},
    {PassType::INSERTION_IMPROVED, "insertion_improved"},
    {PassType::BRICK, "brick"},
    {PassType::SHAKE, "shake"},
    {PassType::BUBBLE, "bubble"},
})

const std::vector<PassType> ALL_PASSES
    { PassType::INSERTION, PassType::INSERTION_IMPROVED, PassType::BRICK, PassType::BUBBLE, PassType::SHAKE };

// TODO: look into optimizations

template <typename T>
class Pass {
public:
    virtual void performPass(T & container, std::size_t n, int gap) const = 0;
};

template <typename T>
class InsertionPass: public Pass<T> {
public:
    void performPass(T & container, std::size_t n, int gap) const override {
        for (int i = gap; i < n; ++i) {
            auto temp = container[i];
            int j = i;

            for (; j >= gap && container[j - gap] > temp; j -= gap) {
                container[j] = container[j - gap];
            }

            container[j] = temp;
        }
    }
};

template <typename T>
class ImprovedInsertionPass: public Pass<T> {
public:
    void performPass(T & container, std::size_t n, int gap) const override {
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
};

template <typename T>
class BubblePass: public Pass<T> {
public:
    void performPass(T & container, std::size_t n, int gap) const override {
        for (int i = 0; i < n - gap; ++i) {
            if (container[i] > container[i + gap]){
                std::swap(container[i], container[i + gap]);
            }
        }
    }
};

template <typename T>
class BrickPass: public Pass<T> {
public:
    void performPass(T & container, std::size_t n, int gap) const override {
        int increment = gap * 2;

        for (int i = 0; i < n - gap; i+= increment) {
            if (container[i] > container[i + gap]) {
                std::swap(container[i], container[i + gap]);
            }
        }

        for (int i = gap; i < n - gap; i+= increment) {
            if (container[i] > container[i + gap]) {
                std::swap(container[i], container[i + gap]);
            }
        }
    }
};

template <typename T>
class ShakePass: public Pass<T> {
public:
    void performPass(T & container, std::size_t n, int gap) const override {
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
};

class PassFactory {
public:
    template <typename T>
    static std::unique_ptr<Pass<T>> getPass(PassType passType) {
        switch (passType) {
            case PassType::INSERTION:
                return std::make_unique<InsertionPass<T>>();
            case PassType::INSERTION_IMPROVED:
                return std::make_unique<ImprovedInsertionPass<T>>();
            case PassType::BRICK:
                return std::make_unique<BrickPass<T>>();
            case PassType::BUBBLE:
                return std::make_unique<BubblePass<T>>();
            case PassType::SHAKE:
                return std::make_unique<ShakePass<T>>();
            default:
                throw std::runtime_error("unsupported pass type");
        }
    }
};
