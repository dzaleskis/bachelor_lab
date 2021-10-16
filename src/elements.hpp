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
            auto report = reports.at(current_key);
            report.assignments += 1;
        }

        void incr_comparisons() {
            auto report = reports.at(current_key);
            report.comparisons += 1;
        }

        const measure_report & with_report(std::function<void()>& fn) {
            create_report();
            fn();
            return get_report(current_key);
        }

    private:
        void create_report() {
            current_key = random_utils::get_random();
            reports.insert(std::make_pair(current_key, new measure_report()));
        }

        const measure_report & get_report(int measure_key) const {
            return reports.at(measure_key);
        }

        int current_key;
        std::unordered_map<int, measure_report*> reports;
};

// avoid locks
thread_local Measure measure;

template <typename T>
class Element {
    public:
        static Element<T> & from(T value) {
            return new Element<T>(value);
        }

        T get_value() const {
            return this->value;
        }

        Element<T> & operator = (const Element<T> & b) {
            this->value = b.get_value();
            measure.incr_assignments();
            return *this;
        }

        Element<T> & operator = (const T & b) {
            this->value = b;
            measure.incr_assignments();
            return *this;
        }

    private:
        Element(T value): value(value) {}
        T value;
};


template <typename T>
bool operator > (const Element<T> left, const Element<T> right) {
    measure.incr_comparisons();
    return left.value() > right.value();
}

template <typename T>
bool operator >= (const Element<T> left, const Element<T> right) {
    measure.incr_comparisons();
    return left.value() >= right.value();
}

template <typename T>
bool operator < (const Element<T> left, const Element<T> right) {
    measure.incr_comparisons();
    return left.value() < right.value();
}

template <typename T>
bool operator <= (const Element<T> left, const Element<T> right) {
    measure.incr_comparisons();
    return left.value() <= right.value();
}

template <typename T>
bool operator == (const Element<T> left, const Element<T> right) {
    measure.incr_comparisons();
    return left.value() == right.value();
}

template <typename T>
bool operator != (const Element<T> left, const Element<T> right) {
    measure.incr_comparisons();
    return left.value() != right.value();
}
