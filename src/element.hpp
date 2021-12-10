#pragma once
#include <unordered_map>
#include <exception>
#include <functional>
#include <memory>
#include "random_utils.hpp"

struct measure_report {
    int assignments;
    int comparisons;
};

class Measure {
    public:
        void incrAssignments() {
            auto it = reports.find(current_key);
            if (it == reports.end()) {
                return;
            }

            (*it).second->assignments += 1;
        }

        void incrComparisons() {
            auto it = reports.find(current_key);
            if (it == reports.end()) {
                return;
            }

            (*it).second->comparisons += 1;
        }

        const measure_report & withReport(const std::function<void()>& fn) {
            create_report();
            fn();

            return *(reports.at(current_key));
        }

    private:
        void create_report() {
            current_key = utils::get_random_int();
            reports.insert(std::make_pair(current_key, std::make_unique<measure_report>()));
        }

        int current_key = 0;
        std::unordered_map<int, std::unique_ptr<measure_report>> reports;
};

// avoid locks
thread_local Measure global_measure;

template <typename T>
class Element {
    public:
        Element() = default;

        Element(Element<T> const &other): value(other.get_value()) {
            global_measure.incrAssignments();
        }

        Element(const T & val): value(val) {
            global_measure.incrAssignments();
        }

        inline T get_value() const {
            return this->value;
        }

        Element<T> & operator = (const Element<T> & b) {
            this->value = b.get_value();
            global_measure.incrAssignments();
            return *this;
        }

        Element<T> & operator = (const T & b) {
            this->value = b;
            global_measure.incrAssignments();
            return *this;
        }

        bool operator > (const Element<T>& b) const
        {
            global_measure.incrComparisons();
            return value > b.get_value();
        }

        bool operator >= (const Element<T>& b) const
        {
            global_measure.incrComparisons();
            return value >= b.get_value();
        }

        bool operator < (const Element<T>& b) const
        {
            global_measure.incrComparisons();
            return value < b.get_value();
        }

        bool operator <= (const Element<T>& b) const
        {
            global_measure.incrComparisons();
            return value <= b.get_value();
        }

        bool operator == (const Element<T>& b) const
        {
            global_measure.incrComparisons();
            return value == b.get_value();
        }

        bool operator != (const Element<T>& b) const
        {
            global_measure.incrComparisons();
            return value != b.get_value();
        }

    private:
        T value;
};