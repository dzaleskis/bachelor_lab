#pragma once
#include <unordered_map>
#include <exception>
#include <functional>
#include "random_utils.hpp"

struct measure_report {
    int assignments;
    int comparisons;
};

class Measure {
    public:
        void incr_assignments() {
            auto it = reports.find(current_key);
            if (it == reports.end()) {
                return;
            }

            (*it).second->assignments += 1;
        }

        void incr_comparisons() {
            auto it = reports.find(current_key);
            if (it == reports.end()) {
                return;
            }

            (*it).second->comparisons += 1;
        }

        const measure_report & with_report(const std::function<void()>& fn) {
            create_report();
            fn();

            return *(reports.at(current_key));
        }

    private:
        void create_report() {
            current_key = random_utils::get_random_int();
            reports.insert(std::make_pair(current_key, new measure_report()));
        }

        int current_key = 0;
        std::unordered_map<int, measure_report*> reports;
};

// avoid locks
thread_local Measure global_measure;

template <typename T>
class Element {
    public:
        explicit Element(T val): value(val) {}

        T get_value() const {
            return this->value;
        }

        Element<T> & operator = (const Element<T> & b) {
            this->value = b.get_value();
            global_measure.incr_assignments();
            return *this;
        }

        Element<T> & operator = (const T & b) {
            this->value = b;
            global_measure.incr_assignments();
            return *this;
        }

    private:
        T value;
};


template <typename T>
bool operator > (const Element<T> left, const Element<T> right) {
    global_measure.incr_comparisons();
    return left.get_value() > right.get_value();
}

template <typename T>
bool operator >= (const Element<T> left, const Element<T> right) {
    global_measure.incr_comparisons();
    return left.get_value() >= right.get_value();
}

template <typename T>
bool operator < (const Element<T> left, const Element<T> right) {
    global_measure.incr_comparisons();
    return left.get_value() < right.get_value();
}

template <typename T>
bool operator <= (const Element<T> left, const Element<T> right) {
    global_measure.incr_comparisons();
    return left.get_value() <= right.get_value();
}

template <typename T>
bool operator == (const Element<T> left, const Element<T> right) {
    global_measure.incr_comparisons();
    return left.get_value() == right.get_value();
}

template <typename T>
bool operator != (const Element<T> left, const Element<T> right) {
    global_measure.incr_comparisons();
    return left.get_value() != right.get_value();
}
