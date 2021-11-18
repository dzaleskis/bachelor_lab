#pragma once

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

// would be nice if the class could just have a virtual template method
// but c++ does not allow it
template <typename T>
class Pass {
public:
    virtual void perform_pass(T & container, std::size_t n, int gap) const = 0;
};

template <typename T>
class InsertionPass: public Pass<T> {
public:
    void perform_pass(T & container, std::size_t n, int gap) const override {
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
    void perform_pass(T & container, std::size_t n, int gap) const override {
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
    void perform_pass(T & container, std::size_t n, int gap) const override {
        for (int i = 0; i < n - gap; ++i) {
            if(container[i] > container[i + gap]){
                std::swap(container[i], container[i + gap]);
            }
        }
    }
};

template <typename T>
class BrickPass: public Pass<T> {
public:
    void perform_pass(T & container, std::size_t n, int gap) const override {
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
    void perform_pass(T & container, std::size_t n, int gap) const override {
        // perform bubble pass forward
        for (std::size_t i = 0; i < n - gap; ++i) {
            if (container[i] > container[i+gap]) {
                std::swap(container[i], container[i + gap]);
            }
        }

        // perform bubble pass backward
        for (std::size_t i = n - gap - 1; i >= 0; ++i) {
            if (container[i] > container[i+gap]) {
                std::swap(container[i], container[i + gap]);
            }
        }
    }
};

template <typename T>
class PassFactory {
public:
    std::unique_ptr<Pass<T>> getPass(PassType passType) {
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
                throw std::runtime_error("something wrong");
        }
    }
};
