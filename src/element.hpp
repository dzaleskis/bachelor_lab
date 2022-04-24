#pragma once
#include <unordered_map>
#include <exception>
#include <functional>
#include <memory>

struct MeasureReport {
    // operations
    int assignments = 0;
    int comparisons = 0;
    // swap tracing
    int prev_access_index = -1;
    int total_accesses = 0;
    int total_distance = 0;
};

class Measure {
    public:
        void incrAssignments() {
            if (!isEnabled) {
                return;
            }

            report.assignments += 1;
        }

        void incrComparisons() {
            if (!isEnabled) {
                return;
            }

            report.comparisons += 1;
        }

        void traceAccess(int index) {
            if (!isEnabled) {
                return;
            }

            report.total_accesses += 1;

            if (report.total_accesses % 2 == 0) {
                report.total_distance += std::abs(report.prev_access_index - index);
            }

            report.prev_access_index = index;
        }

        MeasureReport withReport(const std::function<void()>& fn) {
            isEnabled = true;

            fn();

            isEnabled = false;

            auto collectedReport = report;
            report = MeasureReport();

            return collectedReport;
        }

    private:
        bool isEnabled = false;
        MeasureReport report;
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

template <typename T>
class TracedVec: public std::vector<T> {
public:
    TracedVec(const std::vector<T>& vec): std::vector<T>(vec) {}

    T& operator[](int index) {
        global_measure.traceAccess(index);
        return this->at(index);
    }
};