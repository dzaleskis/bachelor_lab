#pragma once

#include <functional>
#include <unordered_map>

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
        // TODO: is this implementation correct??

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
        for (int i = 0; i < n - gap; ++i) {
            if (container[i] > container[i+gap]) {
                std::swap(container[i], container[i + gap]);
            }
        }

        // perform bubble pass backward
        for (int i = n - gap - 1; i >= 0; ++i) {
            if (container[i] > container[i+gap]) {
                std::swap(container[i], container[i + gap]);
            }
        }
    }
};

template <typename T>
class PassFactory {
public:
    Pass<T>* getPass(PassType passType) {
        switch (passType) {
            case PassType::INSERTION:
                return new InsertionPass<T>();
            case PassType::INSERTION_IMPROVED:
                return new ImprovedInsertionPass<T>();
            case PassType::BRICK:
                return new BrickPass<T>();
            case PassType::BUBBLE:
                return new BubblePass<T>();
            case PassType::SHAKE:
                return new ShakePass<T>();
            default:
                throw std::runtime_error("something wrong");
        }
    }
};